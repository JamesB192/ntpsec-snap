/*
 * system.c
 */
/* Portions of this file are subject to the following copyright(s).  See
 * the Net-SNMP's COPYING file for more details and other copyrights
 * that may apply:
 */
/***********************************************************
        Copyright 1992 by Carnegie Mellon University

                      All Rights Reserved

Permission to use, copy, modify, and distribute this software and its
documentation for any purpose and without fee is hereby granted,
provided that the above copyright notice appear in all copies and that
both that copyright notice and this permission notice appear in
supporting documentation, and that the name of CMU not be
used in advertising or publicity pertaining to distribution of the
software without specific, written prior permission.

CMU DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING
ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL
CMU BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR
ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
SOFTWARE.
******************************************************************/
/*
 * Portions of this file are copyrighted by:
 * Copyright � 2003 Sun Microsystems, Inc. All rights reserved.
 * Use is subject to license terms specified in the COPYING file
 * distributed with the Net-SNMP package.
 */
/*
 * System dependent routines go here
 */
#include <net-snmp/net-snmp-config.h>
#undef PACKAGE_BUGREPORT
#undef PACKAGE_NAME
#undef PACKAGE_STRING
#undef PACKAGE_TARNAME
#undef PACKAGE_URL
#undef PACKAGE_VERSION
#include <config.h>

#ifdef NEED_NETSNMP_DAEMONIZE

#include <stdio.h>
#include <errno.h>

#include <unistd.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

/**
 * fork current process into the background.
 *
 * This function forks a process into the background, in order to
 * become a daemon process. It does a few things along the way:
 *
 * - becoming a process/session group leader, and  forking a second time so
 *   that process/session group leader can exit.
 *
 * - changing the working directory to /
 *
 * - closing stdin, stdout and stderr (unless stderr_log is set) and
 *   redirecting them to /dev/null
 *
 * @param quit_immediately : indicates if the parent process should
 *                           exit after a successful fork.
 * @param stderr_log       : indicates if stderr is being used for
 *                           logging and shouldn't be closed
 * @returns -1 : fork error
 *           0 : child process returning
 *          >0 : parent process returning. returned value is the child PID.
 */
int
netsnmp_daemonize(int quit_immediately, int stderr_log)
{
    int i = 0;
    int saved_errno;

    DEBUGMSGT(("daemonize","deamonizing...\n"));
#ifdef HAVE_WORKING_FORK
    /*
     * Fork to return control to the invoking process and to
     * guarantee that we aren't a process group leader.
     */
    i = fork();
    if (i != 0) {
        /* Parent. */
	saved_errno = errno;
        DEBUGMSGT(("daemonize","first fork returned %d.\n", i));
        if(i == -1) {
            snmp_log(LOG_ERR,"first fork failed (errno %d) in "
                     "netsnmp_daemonize()\n", saved_errno);
            return -1;
        }
        if (quit_immediately) {
            DEBUGMSGT(("daemonize","parent exiting\n"));
            exit(0);
        }
    } else {
        /* Child. */
        /* Become a process/session group leader. */
        setsid();
        /*
         * Fork to let the process/session group leader exit.
         */
        if ((i = fork()) != 0) {
	    saved_errno = errno;
            DEBUGMSGT(("daemonize","second fork returned %d.\n", i));
            if(i == -1) {
                snmp_log(LOG_ERR,"second fork failed (errno %d) in "
                         "netsnmp_daemonize()\n", saved_errno);
            }
            /* Parent. */
            exit(0);
        }
#ifndef WIN32
        else {
            /* Child. */
            
            DEBUGMSGT(("daemonize","child continuing\n"));

            /* Avoid keeping any directory in use. */
            chdir("/");
            
            if (!stderr_log) {
                /*
                 * Close inherited file descriptors to avoid
                 * keeping unnecessary references.
                 */
                close(0);
                close(1);
                close(2);
                
                /*
                 * Redirect std{in,out,err} to /dev/null, just in
                 * case.
                 */
                open("/dev/null", O_RDWR);
                dup(0);
                dup(0);
            }
        }
#endif /* !WIN32 */
    }
#endif /* HAVE_WORKING_FORK */
    return i;
}

#else /* !NEED_NETSNMP_DAEMONIZE */
int netsnp_daemonize_bs;
#endif
