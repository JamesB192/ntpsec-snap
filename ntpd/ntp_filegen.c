/*
 *  implements file generations support for NTP
 *  logfiles and statistic files
 *
 *
 * Copyright (C) 1992, 1996 by Rainer Pruy
 * Friedrich-Alexander Universitaet Erlangen-Nuernberg, Germany
 *
 * SPDX-License-Identifier: BSD-2-clause
 */

#include "config.h"

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

#include "ntpd.h"
#include "ntp_io.h"
#include "ntp_calendar.h"
#include "ntp_filegen.h"
#include "ntp_stdlib.h"

/*
 * NTP is intended to run long periods of time without restart.
 * Thus log and statistic files generated by NTP will grow large.
 *
 * this set of routines provides a central interface 
 * to generating files using file generations
 *
 * the generation of a file is changed according to file generation type
 */


/*
 * redefine this if your system dislikes filename suffixes like
 * X.19910101 or X.1992W50 or ....
 */
#define SUFFIX_SEP '.'

static	void	filegen_open	(FILEGEN *, const time_t);
static	int	valid_fileref	(const char *, const char *);
static	void	filegen_init	(const char *, const char *, FILEGEN *);
#ifdef	DEBUG
static	void	filegen_uninit		(FILEGEN *);
#endif	/* DEBUG */


/*
 * filegen_init
 */

static void
filegen_init(
	const char *	dir,
	const char *	fname,
	FILEGEN *	fgp
	)
{
	fgp->fp = NULL;
	fgp->dir = estrdup(dir);
	fgp->fname = estrdup(fname);
	fgp->id_lo = 0;
	fgp->id_hi = 0;
	fgp->type = FILEGEN_DAY;
	fgp->flag = FGEN_FLAG_LINK; /* not yet enabled !!*/
}


/*
 * filegen_uninit - free memory allocated by filegen_init
 */
#ifdef DEBUG
static void
filegen_uninit(
	FILEGEN *fgp
	)
{
	free(fgp->dir);
	free(fgp->fname);
}
#endif


/*
 * open a file generation according to the current settings of gen
 * will also provide a link to basename if requested to do so
 */

static void
filegen_open(
	FILEGEN *	gen,
	const time_t 	stamp
	)
{
	char *savename;	/* temp store for name collision handling */
	char *fullname;	/* name with any designation extension */
	char *filename;	/* name without designation extension */
	char *suffix;	/* where to print suffix extension */
	u_int len, suflen;
	FILE *fp;
	struct tm tm;

	/* get basic filename in buffer, leave room for extensions */
	len = strlen(gen->dir) + strlen(gen->fname) + 65;
	filename = emalloc(len);
	fullname = emalloc(len);
	savename = NULL;
	snprintf(filename, len, "%s%s", gen->dir, gen->fname);

	/* where to place suffix */
	suflen = strlcpy(fullname, filename, len);
	suffix = fullname + suflen;
	suflen = len - suflen;

	/* last octet of fullname set to '\0' for truncation check */
	fullname[len - 1] = '\0';

	switch (gen->type) {

	default:
		msyslog(LOG_ERR, 
			"unsupported file generations type %d for "
			"\"%s\" - reverting to FILEGEN_NONE",
			gen->type, filename);
		gen->type = FILEGEN_NONE;
		break;

	case FILEGEN_NONE:
		/* no suffix, all set */
		break;

	case FILEGEN_PID:
		gen->id_lo = getpid();
		gen->id_hi = 0;
		snprintf(suffix, suflen, "%c#%ld",
			 SUFFIX_SEP, gen->id_lo);
		break;

	case FILEGEN_DAY:
		/*
		 * You can argue here in favor of using MJD, but I
		 * would assume it to be easier for humans to interpret
		 * dates in a format they are used to in everyday life.
		 */
		gmtime_r(&stamp, &tm);
		snprintf(suffix, suflen, "%c%04d%02d%02d",
			 SUFFIX_SEP, tm.tm_year+1900, tm.tm_mon+1, tm.tm_mday);
		gen->id_lo = stamp - (stamp % SECSPERDAY);
		gen->id_hi = gen->id_lo + SECSPERDAY;
		break;

	case FILEGEN_WEEK:
		/* week number is day-of-year mod 7 */
		gmtime_r(&stamp, &tm);
		snprintf(suffix, suflen, "%c%04dw%02d",
			 SUFFIX_SEP, tm.tm_year+1900, tm.tm_yday % 7);
		/* See comment below at MONTH */
		gen->id_lo = stamp - (stamp % SECSPERDAY);
		gen->id_hi = gen->id_lo + SECSPERDAY;
		break;

	case FILEGEN_MONTH:
		gmtime_r(&stamp, &tm);
		snprintf(suffix, suflen, "%c%04d%02d",
			 SUFFIX_SEP, tm.tm_year+1900, tm.tm_mon+1);
		/* If we had a mktime that didn't use the local time zone
		 * we could setup id_lo and id_hi to bracket the month.
		 * This will have to recalculate things each day.
		 */
		gen->id_lo = stamp - (stamp % SECSPERDAY);
		gen->id_hi = gen->id_lo + SECSPERDAY;
		break;

	case FILEGEN_YEAR:
		gmtime_r(&stamp, &tm);
		snprintf(suffix, suflen, "%c%04d",
			 SUFFIX_SEP, tm.tm_year+1900);
		/* See comment above at MONTH */
		gen->id_lo = stamp - (stamp % SECSPERDAY);
		gen->id_hi = gen->id_lo + SECSPERDAY;
		break;

	case FILEGEN_AGE:
		gen->id_lo = current_time - (current_time % SECSPERDAY);
		gen->id_hi = gen->id_lo + SECSPERDAY;
		snprintf(suffix, suflen, "%ca%08ld",
			 SUFFIX_SEP, gen->id_lo);
	}
  
	/* check possible truncation */
	if ('\0' != fullname[len - 1]) {
		fullname[len - 1] = '\0';
		msyslog(LOG_ERR, "logfile name truncated: \"%s\"",
			fullname);
	}

	if (FILEGEN_NONE != gen->type) {
		/*
		 * check for existence of a file with name 'basename'
		 * as we disallow such a file
		 * if FGEN_FLAG_LINK is set create a link
		 */
		struct stat stats;
		/*
		 * try to resolve name collisions
		 */
		static u_long conflicts = 0;

#ifndef	S_ISREG
#define	S_ISREG(mode)	(((mode) & S_IFREG) == S_IFREG)
#endif
		if (stat(filename, &stats) == 0) {
			/* Hm, file exists... */
			if (S_ISREG(stats.st_mode)) {
				if (stats.st_nlink <= 1)	{
					/*
					 * Oh, it is not linked - try to save it
					 */
					savename = emalloc(len);
					snprintf(savename, len,
						"%s%c%dC%lu",
						filename, SUFFIX_SEP,
						(int)getpid(), conflicts++);

					if (rename(filename, savename) != 0)
						msyslog(LOG_ERR,
							"couldn't save %s: %m",
							filename);
					free(savename);
				} else {
					/*
					 * there is at least a second link to
					 * this file.
					 * just remove the conflicting one
					 */
					/* coverity[toctou] */
					if (unlink(filename) != 0)
						msyslog(LOG_ERR, 
							"couldn't unlink %s: %m",
							filename);
				}
			} else {
				/*
				 * Ehh? Not a regular file ?? strange !!!!
				 */
				msyslog(LOG_ERR, 
					"expected regular file for %s "
					"(found mode 0%lo)",
					filename,
					(unsigned long)stats.st_mode);
			}
		} else {
			/*
			 * stat(..) failed, but it is absolutely correct for
			 * 'basename' not to exist
			 */
			if (ENOENT != errno)
				msyslog(LOG_ERR, "stat(%s) failed: %m",
						 filename);
		}
	}

	/*
	 * now, try to open new file generation...
	 */
	DPRINTF(4, ("opening filegen (type=%d/stamp=%lld) \"%s\"\n",
		    gen->type, (long long)stamp, fullname));

	fp = fopen(fullname, "a");
  
	if (NULL == fp)	{
		/* open failed -- keep previous state
		 *
		 * If the file was open before keep the previous generation.
		 * This will cause output to end up in the 'wrong' file,
		 * but I think this is still better than losing output
		 *
		 * ignore errors due to missing directories
		 */

		if (ENOENT != errno)
			msyslog(LOG_ERR, "can't open %s: %m", fullname);
	} else {
		if (NULL != gen->fp) {
			fclose(gen->fp);
			gen->fp = NULL;
		}
		gen->fp = fp;

		if (gen->flag & FGEN_FLAG_LINK) {
			/*
			 * need to link file to basename
			 * have to use hardlink for now as I want to allow
			 * gen->basename spanning directory levels
			 * this would make it more complex to get the correct
			 * fullname for symlink
			 *
			 * Ok, it would just mean taking the part following
			 * the last '/' in the name.... Should add it later....
			 */
			if (link(fullname, filename) != 0)
				if (EEXIST != errno)
					msyslog(LOG_ERR, 
						"can't link(%s, %s): %m",
						fullname, filename);
		}		/* flags & FGEN_FLAG_LINK */
	}			/* else fp == NULL */
	
	free(filename);
	free(fullname);
	return;
}

/*
 * this function sets up gen->fp to point to the correct
 * generation of the file for the time specified by 'now'
 */

void
filegen_setup(
	FILEGEN * gen,
	time_t now
	)
{
	bool	current;

	if (!(gen->flag & FGEN_FLAG_ENABLED)) {
		if (NULL != gen->fp) {
			fclose(gen->fp);
			gen->fp = NULL;
		}
		return;
	}

	switch (gen->type) {

	default:
	case FILEGEN_NONE:
		current = true;
		break;

	case FILEGEN_PID:
		current = ((int)gen->id_lo == getpid());
		break;

	case FILEGEN_AGE:
		current = (gen->id_lo <= (long)current_time) &&
			  (gen->id_hi > (long)current_time);
		break;

	case FILEGEN_DAY:
	case FILEGEN_WEEK:
	case FILEGEN_MONTH:
	case FILEGEN_YEAR:
		current = (gen->id_lo <= now) &&
			  (gen->id_hi > now);
		break;
	}
	/*
	 * try to open file if not yet open
	 * reopen new file generation file on change of generation id
	 */
	if (NULL == gen->fp || !current) {
		DPRINTF(1, ("filegen  %0x %lld\n", gen->type, (long long)now));
		filegen_open(gen, now);
	}
}


/*
 * change settings for filegen files
 */
void
filegen_config(
	FILEGEN *	gen,
	const char *	dir,
	const char *	fname,
	u_int		type,
	u_int		flag
	)
{
	bool file_existed;


	/*
	 * if nothing would be changed...
	 */
	if (strcmp(dir, gen->dir) == 0 && strcmp(fname, gen->fname) == 0
	    && type == gen->type && flag == gen->flag)
		return;

	/*
	 * validate parameters
	 */
	if (!valid_fileref(dir, fname))
		return;
  
	if (NULL != gen->fp) {
		fclose(gen->fp);
		gen->fp = NULL;
		file_existed = true;
	} else {
		file_existed = false;
	}

	DPRINTF(3, ("configuring filegen:\n"
		    "\tdir:\t%s -> %s\n"
		    "\tfname:\t%s -> %s\n"
		    "\ttype:\t%d -> %d\n"
		    "\tflag: %x -> %x\n",
		    gen->dir, dir,
		    gen->fname, fname,
		    gen->type, type,
		    gen->flag, flag));

	if (strcmp(gen->dir, dir) != 0) {
		free(gen->dir);
		gen->dir = estrdup(dir);
	}

	if (strcmp(gen->fname, fname) != 0) {
		free(gen->fname);
		gen->fname = estrdup(fname);
	}
	gen->type = (uint8_t)type;
	gen->flag = (uint8_t)flag;

	/*
	 * make filegen use the new settings
	 * special action is only required when a generation file
	 * is currently open
	 * otherwise the new settings will be used anyway at the next open
	 */
	if (file_existed) {
		filegen_setup(gen, time(NULL));
	}
}


/*
 * check whether concatenating prefix and basename
 * yields a legal filename
 */
static int
valid_fileref(
	const char *	dir,
	const char *	fname
	)
{
	/*
	 * dir cannot be changed dynamically
	 * (within the context of filegen)
	 * so just reject basenames containing '..'
	 *
	 * ASSUMPTION:
	 *		file system parts 'below' dir may be
	 *		specified without infringement of security
	 *
	 *		restricting dir to legal values
	 *		has to be ensured by other means
	 * (however, it would be possible to perform some checks here...)
	 */
	const char *p;

	/*
	 * Just to catch, dumb errors opening up the world...
	 */
	if (NULL == dir || '\0' == dir[0])
		return false;

	if (NULL == fname)
		return false;

	for (p = fname; p != NULL; p = strchr(p, DIR_SEP)) {
		if ('.' == p[0] && '.' == p[1] 
		    && ('\0' == p[2] || DIR_SEP == p[2]))
			return false;
	}

	return true;
}


/*
 * filegen registry
 */

static struct filegen_entry {
	char *			name;
	FILEGEN *		filegen;
	struct filegen_entry *	next;
} *filegen_registry = NULL;


FILEGEN *
filegen_get(
	const char *	name
	)
{
	struct filegen_entry *f = filegen_registry;

	while (f) {
		if (f->name == name || strcmp(name, f->name) == 0) {
			DPRINTF(4, ("filegen_get(%s) = %p\n",
				    name, f->filegen));
			return f->filegen;
		}
		f = f->next;
	}
	DPRINTF(4, ("filegen_get(%s) = NULL\n", name));
	return NULL;
}


void
filegen_register(
	const char *	dir,
	const char *	name,
	FILEGEN *	filegen
	)
{
	struct filegen_entry **ppfe;

	DPRINTF(4, ("filegen_register(%s, %p)\n", name, filegen));

	filegen_init(dir, name, filegen);

	ppfe = &filegen_registry;
	while (NULL != *ppfe) {
		if ((*ppfe)->name == name 
		    || !strcmp((*ppfe)->name, name)) {

			DPRINTF(5, ("replacing filegen %p\n",
				    (*ppfe)->filegen));

			(*ppfe)->filegen = filegen;
			return;
		}
		ppfe = &((*ppfe)->next);
	}

	*ppfe = emalloc(sizeof **ppfe);

	(*ppfe)->next = NULL;
	(*ppfe)->name = estrdup(name);
	(*ppfe)->filegen = filegen;

	DPRINTF(6, ("adding new filegen\n"));
	
	return;
}


/*
 * filegen_statsdir() - reset each filegen entry's dir to statsdir.
 */
void
filegen_statsdir(void)
{
	struct filegen_entry *f;

	for (f = filegen_registry; f != NULL; f = f->next)
		filegen_config(f->filegen, statsdir, f->filegen->fname,
			       f->filegen->type, f->filegen->flag);
}


/*
 * filegen_unregister frees memory allocated by filegen_register for
 * name.
 */
#ifdef DEBUG
void
filegen_unregister(
	const char *name
	)
{
	struct filegen_entry **	ppfe;
	struct filegen_entry *	pfe;
	FILEGEN *		fg;
			
	DPRINTF(4, ("filegen_unregister(%s)\n", name));

	ppfe = &filegen_registry;

	while (NULL != *ppfe) {
		if ((*ppfe)->name == name
		    || !strcmp((*ppfe)->name, name)) {
			pfe = *ppfe;
			*ppfe = (*ppfe)->next;
			fg = pfe->filegen;
			free(pfe->name);
			free(pfe);
			filegen_uninit(fg);
			break;
		}
		ppfe = &((*ppfe)->next);
	}
}	
#endif	/* DEBUG */
