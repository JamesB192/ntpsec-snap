/*  
 *  EDIT THIS FILE WITH CAUTION  (sntp-opts.h)
 *  
 *  It has been AutoGen-ed  October 23, 2011 at 10:43:07 AM by AutoGen 5.12
 *  From the definitions    sntp-opts.def
 *  and the template file   options
 *
 * Generated from AutoOpts 35:0:10 templates.
 *
 *  AutoOpts is a copyrighted work.  This header file is not encumbered
 *  by AutoOpts licensing, but is provided under the licensing terms chosen
 *  by the sntp author or copyright holder.  AutoOpts is
 *  licensed under the terms of the LGPL.  The redistributable library
 *  (``libopts'') is licensed under the terms of either the LGPL or, at the
 *  users discretion, the BSD license.  See the AutoOpts and/or libopts sources
 *  for details.
 *
 * This source file is copyrighted and licensed under the following terms:
 *
 *  Copyright (C) 1970-2011 The University of Delaware, David L. Mills, and/or others, all rights reserved.
 *  This is free software. It is licensed for use, modification and
 *  redistribution under the terms of the NTP License, copies of which
 *  can be seen at:
 *    <http://ntp.org/license>
 *    <http://opensource.org/licenses/NTP>
 *  
PFX>Permission to use, copy, modify, and distribute this software and its
 *  documentation for any purpose with or without fee is hereby granted,
 *  provided that the above copyright notice appears in all copies and that
 *  both the copyright notice and this permission notice appear in
 *  supporting documentation, and that the name The University of Delaware, David L. Mills, and/or others not be used in
 *  advertising or publicity pertaining to distribution of the software
 *  without specific, written prior permission. The University of Delaware, David L. Mills, and/or others makes no
 *  representations about the suitability this software for any purpose. It
 *  is provided "as is" without express or implied warranty.
 */
/*
 *  This file contains the programmatic interface to the Automated
 *  Options generated for the sntp program.
 *  These macros are documented in the AutoGen info file in the
 *  "AutoOpts" chapter.  Please refer to that doc for usage help.
 */
#ifndef AUTOOPTS_SNTP_OPTS_H_GUARD
#define AUTOOPTS_SNTP_OPTS_H_GUARD 1
#include "config.h"
#include <autoopts/options.h>

/*
 *  Ensure that the library used for compiling this generated header is at
 *  least as new as the version current when the header template was released
 *  (not counting patch version increments).  Also ensure that the oldest
 *  tolerable version is at least as old as what was current when the header
 *  template was released.
 */
#define AO_TEMPLATE_VERSION 143360
#if (AO_TEMPLATE_VERSION < OPTIONS_MINIMUM_VERSION) \
 || (AO_TEMPLATE_VERSION > OPTIONS_STRUCT_VERSION)
# error option template version mismatches autoopts/options.h header
  Choke Me.
#endif

/*
 *  Enumeration of each option:
 */
typedef enum {
    INDEX_OPT_IPV4             =  0,
    INDEX_OPT_IPV6             =  1,
    INDEX_OPT_AUTHENTICATION   =  2,
    INDEX_OPT_BCTIMEOUT        =  3,
    INDEX_OPT_BROADCAST        =  4,
    INDEX_OPT_CONCURRENT       =  5,
    INDEX_OPT_DEBUG_LEVEL      =  6,
    INDEX_OPT_SET_DEBUG_LEVEL  =  7,
    INDEX_OPT_GAP              =  8,
    INDEX_OPT_KOD              =  9,
    INDEX_OPT_KEYFILE          = 10,
    INDEX_OPT_LOGFILE          = 11,
    INDEX_OPT_STEPLIMIT        = 12,
    INDEX_OPT_NTPVERSION       = 13,
    INDEX_OPT_USERESERVEDPORT  = 14,
    INDEX_OPT_STEP             = 15,
    INDEX_OPT_SLEW             = 16,
    INDEX_OPT_UCTIMEOUT        = 17,
    INDEX_OPT_WAIT             = 18,
    INDEX_OPT_VERSION          = 19,
    INDEX_OPT_HELP             = 20,
    INDEX_OPT_MORE_HELP        = 21,
    INDEX_OPT_SAVE_OPTS        = 22,
    INDEX_OPT_LOAD_OPTS        = 23
} teOptIndex;

#define OPTION_CT    24
#define SNTP_VERSION       "4.2.7p228"
#define SNTP_FULL_VERSION  "sntp 4.2.7p228"

/*
 *  Interface defines for all options.  Replace "n" with the UPPER_CASED
 *  option name (as in the teOptIndex enumeration above).
 *  e.g. HAVE_OPT(IPV4)
 */
#define         DESC(n) (sntpOptions.pOptDesc[INDEX_OPT_## n])
#define     HAVE_OPT(n) (! UNUSED_OPT(& DESC(n)))
#define      OPT_ARG(n) (DESC(n).optArg.argString)
#define    STATE_OPT(n) (DESC(n).fOptState & OPTST_SET_MASK)
#define    COUNT_OPT(n) (DESC(n).optOccCt)
#define    ISSEL_OPT(n) (SELECTED_OPT(&DESC(n)))
#define ISUNUSED_OPT(n) (UNUSED_OPT(& DESC(n)))
#define  ENABLED_OPT(n) (! DISABLED_OPT(& DESC(n)))
#define  STACKCT_OPT(n) (((tArgList*)(DESC(n).optCookie))->useCt)
#define STACKLST_OPT(n) (((tArgList*)(DESC(n).optCookie))->apzArgs)
#define    CLEAR_OPT(n) STMTS( \
                DESC(n).fOptState &= OPTST_PERSISTENT_MASK;   \
                if ((DESC(n).fOptState & OPTST_INITENABLED) == 0) \
                    DESC(n).fOptState |= OPTST_DISABLED; \
                DESC(n).optCookie = NULL )

/* * * * * *
 *
 *  Enumeration of sntp exit codes
 */
typedef enum {
    SNTP_EXIT_SUCCESS = 0,
    SNTP_EXIT_FAILURE = 1
} sntp_exit_code_t;
/*
 *  Make sure there are no #define name conflicts with the option names
 */
#ifndef     NO_OPTION_NAME_WARNINGS
# ifdef    IPV4
#  warning undefining IPV4 due to option name conflict
#  undef   IPV4
# endif
# ifdef    IPV6
#  warning undefining IPV6 due to option name conflict
#  undef   IPV6
# endif
# ifdef    AUTHENTICATION
#  warning undefining AUTHENTICATION due to option name conflict
#  undef   AUTHENTICATION
# endif
# ifdef    BCTIMEOUT
#  warning undefining BCTIMEOUT due to option name conflict
#  undef   BCTIMEOUT
# endif
# ifdef    BROADCAST
#  warning undefining BROADCAST due to option name conflict
#  undef   BROADCAST
# endif
# ifdef    CONCURRENT
#  warning undefining CONCURRENT due to option name conflict
#  undef   CONCURRENT
# endif
# ifdef    DEBUG_LEVEL
#  warning undefining DEBUG_LEVEL due to option name conflict
#  undef   DEBUG_LEVEL
# endif
# ifdef    SET_DEBUG_LEVEL
#  warning undefining SET_DEBUG_LEVEL due to option name conflict
#  undef   SET_DEBUG_LEVEL
# endif
# ifdef    GAP
#  warning undefining GAP due to option name conflict
#  undef   GAP
# endif
# ifdef    KOD
#  warning undefining KOD due to option name conflict
#  undef   KOD
# endif
# ifdef    KEYFILE
#  warning undefining KEYFILE due to option name conflict
#  undef   KEYFILE
# endif
# ifdef    LOGFILE
#  warning undefining LOGFILE due to option name conflict
#  undef   LOGFILE
# endif
# ifdef    STEPLIMIT
#  warning undefining STEPLIMIT due to option name conflict
#  undef   STEPLIMIT
# endif
# ifdef    NTPVERSION
#  warning undefining NTPVERSION due to option name conflict
#  undef   NTPVERSION
# endif
# ifdef    USERESERVEDPORT
#  warning undefining USERESERVEDPORT due to option name conflict
#  undef   USERESERVEDPORT
# endif
# ifdef    STEP
#  warning undefining STEP due to option name conflict
#  undef   STEP
# endif
# ifdef    SLEW
#  warning undefining SLEW due to option name conflict
#  undef   SLEW
# endif
# ifdef    UCTIMEOUT
#  warning undefining UCTIMEOUT due to option name conflict
#  undef   UCTIMEOUT
# endif
# ifdef    WAIT
#  warning undefining WAIT due to option name conflict
#  undef   WAIT
# endif
#else  /* NO_OPTION_NAME_WARNINGS */
# undef IPV4
# undef IPV6
# undef AUTHENTICATION
# undef BCTIMEOUT
# undef BROADCAST
# undef CONCURRENT
# undef DEBUG_LEVEL
# undef SET_DEBUG_LEVEL
# undef GAP
# undef KOD
# undef KEYFILE
# undef LOGFILE
# undef STEPLIMIT
# undef NTPVERSION
# undef USERESERVEDPORT
# undef STEP
# undef SLEW
# undef UCTIMEOUT
# undef WAIT
#endif  /*  NO_OPTION_NAME_WARNINGS */

/* * * * * *
 *
 *  Interface defines for specific options.
 */
#define VALUE_OPT_IPV4           '4'
#define VALUE_OPT_IPV6           '6'
#define VALUE_OPT_AUTHENTICATION 'a'

#define OPT_VALUE_AUTHENTICATION (DESC(AUTHENTICATION).optArg.argInt)
#define VALUE_OPT_BCTIMEOUT      'B'

#define OPT_VALUE_BCTIMEOUT      (DESC(BCTIMEOUT).optArg.argInt)
#define VALUE_OPT_BROADCAST      'b'
#define VALUE_OPT_CONCURRENT     'c'
#define VALUE_OPT_DEBUG_LEVEL    'd'
#define VALUE_OPT_SET_DEBUG_LEVEL 'D'
#define VALUE_OPT_GAP            'g'

#define OPT_VALUE_GAP            (DESC(GAP).optArg.argInt)
#define VALUE_OPT_KOD            'K'
#define VALUE_OPT_KEYFILE        'k'
#define VALUE_OPT_LOGFILE        'l'
#define VALUE_OPT_STEPLIMIT      'M'

#define OPT_VALUE_STEPLIMIT      (DESC(STEPLIMIT).optArg.argInt)
#define VALUE_OPT_NTPVERSION     'o'

#define OPT_VALUE_NTPVERSION     (DESC(NTPVERSION).optArg.argInt)
#define VALUE_OPT_USERESERVEDPORT 'r'
#define VALUE_OPT_STEP           'S'
#define VALUE_OPT_SLEW           's'
#define VALUE_OPT_UCTIMEOUT      'u'

#define OPT_VALUE_UCTIMEOUT      (DESC(UCTIMEOUT).optArg.argInt)
#define VALUE_OPT_WAIT           18

#define SET_OPT_WAIT   STMTS( \
        DESC(WAIT).optActualIndex = 18; \
        DESC(WAIT).optActualValue = VALUE_OPT_WAIT; \
        DESC(WAIT).fOptState &= OPTST_PERSISTENT_MASK; \
        DESC(WAIT).fOptState |= OPTST_SET )
#define DISABLE_OPT_WAIT   STMTS( \
        DESC(WAIT).fOptState &= OPTST_PERSISTENT_MASK; \
        DESC(WAIT).fOptState |= OPTST_SET | OPTST_DISABLED; \
        DESC(WAIT).optArg.argString = NULL )
#define VALUE_OPT_HELP          '?'
#define VALUE_OPT_MORE_HELP     '!'
#define VALUE_OPT_VERSION       INDEX_OPT_VERSION
#define VALUE_OPT_SAVE_OPTS     '>'
#define VALUE_OPT_LOAD_OPTS     '<'
#define SET_OPT_SAVE_OPTS(a)   STMTS( \
        DESC(SAVE_OPTS).fOptState &= OPTST_PERSISTENT_MASK; \
        DESC(SAVE_OPTS).fOptState |= OPTST_SET; \
        DESC(SAVE_OPTS).optArg.argString = (char const*)(a) )
/*
 *  Interface defines not associated with particular options
 */
#define ERRSKIP_OPTERR  STMTS(sntpOptions.fOptSet &= ~OPTPROC_ERRSTOP)
#define ERRSTOP_OPTERR  STMTS(sntpOptions.fOptSet |= OPTPROC_ERRSTOP)
#define RESTART_OPT(n)  STMTS( \
                sntpOptions.curOptIdx = (n); \
                sntpOptions.pzCurOpt  = NULL)
#define START_OPT       RESTART_OPT(1)
#define USAGE(c)        (*sntpOptions.pUsageProc)(&sntpOptions, c)
/* extracted from opthead.tlib near line 451 */

#ifdef  __cplusplus
extern "C" {
#endif

/* * * * * *
 *
 *  Declare the sntp option descriptor.
 */
extern tOptions sntpOptions;

#if defined(ENABLE_NLS)
# ifndef _
#   include <stdio.h>
static inline char* aoGetsText(char const* pz) {
    if (pz == NULL) return NULL;
    return (char*)gettext(pz);
}
#   define _(s)  aoGetsText(s)
# endif /* _() */

# define OPT_NO_XLAT_CFG_NAMES  STMTS(sntpOptions.fOptSet |= \
                                    OPTPROC_NXLAT_OPT_CFG;)
# define OPT_NO_XLAT_OPT_NAMES  STMTS(sntpOptions.fOptSet |= \
                                    OPTPROC_NXLAT_OPT|OPTPROC_NXLAT_OPT_CFG;)

# define OPT_XLAT_CFG_NAMES     STMTS(sntpOptions.fOptSet &= \
                                  ~(OPTPROC_NXLAT_OPT|OPTPROC_NXLAT_OPT_CFG);)
# define OPT_XLAT_OPT_NAMES     STMTS(sntpOptions.fOptSet &= \
                                  ~OPTPROC_NXLAT_OPT;)

#else   /* ENABLE_NLS */
# define OPT_NO_XLAT_CFG_NAMES
# define OPT_NO_XLAT_OPT_NAMES

# define OPT_XLAT_CFG_NAMES
# define OPT_XLAT_OPT_NAMES

# ifndef _
#   define _(_s)  _s
# endif
#endif  /* ENABLE_NLS */

#ifdef  __cplusplus
}
#endif
#endif /* AUTOOPTS_SNTP_OPTS_H_GUARD */
/* sntp-opts.h ends here */
