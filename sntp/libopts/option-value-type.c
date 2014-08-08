/*   -*- buffer-read-only: t -*- vi: set ro:
 *
 *  DO NOT EDIT THIS FILE   (stdin.c)
 *
 *  It has been AutoGen-ed  August  3, 2014 at 10:44:42 AM by AutoGen 5.18.4pre11
 *  From the definitions    stdin
 *  and the template file   str2enum
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name ``Bruce Korb'' nor the name of any other
 *    contributor may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * str2enum IS PROVIDED BY Bruce Korb ``AS IS'' AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL Bruce Korb OR ANY OTHER CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include "option-value-type.h"
/* ANSI-C code produced by gperf version 3.0.3 */
/* Command-line: gperf option-value-type.gp  */
/* Computed positions: -k'1' */



# if 0 /* gperf build options: */
// %struct-type
// %language=ANSI-C
// %includes
// %global-table
// %omit-struct-type
// %readonly-tables
// %compare-strncmp
//
// %define slot-name               vtp_name
// %define hash-function-name      option_value_type_hash
// %define lookup-function-name    find_option_value_type_name
// %define word-array-name         option_value_type_table
// %define initializer-suffix      ,VTP_COUNT_CMD
//
# endif

#include "option-value-type.h"
typedef struct {
    char const *    vtp_name;
    option_value_type_enum_t vtp_id;
} option_value_type_map_t;
#include <string.h>

/* maximum key range = 15, duplicates = 0 */

static unsigned int
option_value_type_hash (register const char *str, register unsigned int len)
{
  static const unsigned char asso_values[] =
    {
      18, 18, 18, 18, 18, 18, 18, 18, 18, 18,
      18, 18, 18, 18, 18, 18, 18, 18, 18, 18,
      18, 18, 18, 18, 18, 18, 18, 18, 18, 18,
      18, 18, 18, 18, 18, 18, 18, 18, 18, 18,
      18, 18, 18, 18, 18, 18, 18, 18, 18, 18,
      18, 18, 18, 18, 18, 18, 18, 18, 18, 18,
      18, 18, 18, 18, 18, 18, 18, 18, 18, 18,
      18, 18, 18, 18, 18, 18, 18, 18, 18, 18,
      18, 18, 18, 18, 18, 18, 18, 18, 18, 18,
      18, 18, 18, 18, 18, 18, 18, 18,  0, 18,
      18, 18, 18, 18,  0, 10, 18,  5, 18, 18,
       5, 18, 18, 18, 18,  0, 18, 18, 18, 18,
      18, 18, 18, 18, 18, 18, 18, 18, 18, 18,
      18, 18, 18, 18, 18, 18, 18, 18, 18, 18,
      18, 18, 18, 18, 18, 18, 18, 18, 18, 18,
      18, 18, 18, 18, 18, 18, 18, 18, 18, 18,
      18, 18, 18, 18, 18, 18, 18, 18, 18, 18,
      18, 18, 18, 18, 18, 18, 18, 18, 18, 18,
      18, 18, 18, 18, 18, 18, 18, 18, 18, 18,
      18, 18, 18, 18, 18, 18, 18, 18, 18, 18,
      18, 18, 18, 18, 18, 18, 18, 18, 18, 18,
      18, 18, 18, 18, 18, 18, 18, 18, 18, 18,
      18, 18, 18, 18, 18, 18, 18, 18, 18, 18,
      18, 18, 18, 18, 18, 18, 18, 18, 18, 18,
      18, 18, 18, 18, 18, 18, 18, 18, 18, 18,
      18, 18, 18, 18, 18, 18
    };
  return len + asso_values[(unsigned char)str[0]];
}

static const option_value_type_map_t option_value_type_table[] =
  {
    {"",VTP_COUNT_CMD}, {"",VTP_COUNT_CMD},
    {"",VTP_COUNT_CMD},
    {"set",            VTP_CMD_SET},
    {"bool",           VTP_CMD_BOOL},
    {"",VTP_COUNT_CMD},
    {"string",         VTP_CMD_STRING},
    {"boolean",        VTP_CMD_BOOLEAN},
    {"",VTP_COUNT_CMD},
    {"hierarchy",      VTP_CMD_HIERARCHY},
    {"",VTP_COUNT_CMD},
    {"nested",         VTP_CMD_NESTED},
    {"keyword",        VTP_CMD_KEYWORD},
    {"",VTP_COUNT_CMD},
    {"set-membership", VTP_CMD_SET_MEMBERSHIP},
    {"",VTP_COUNT_CMD}, {"",VTP_COUNT_CMD},
    {"integer",        VTP_CMD_INTEGER}
  };

static inline const option_value_type_map_t *
find_option_value_type_name (register const char *str, register unsigned int len)
{
  if (len <= 14 && len >= 3)
    {
      register int key = (int)option_value_type_hash (str, len);

      if (key <= 17 && key >= 0)
        {
          register const char *s = option_value_type_table[key].vtp_name;

          if (*str == *s && !strncmp (str + 1, s + 1, len - 1) && s[len] == '\0')
            return &option_value_type_table[key];
        }
    }
  return 0;
}

/**
 * Convert a command (keyword) to a option_value_type_enum_t enumeration value.
 *
 * @param[in] str   a string that should start with a known key word.
 * @param[in] len   the provided length of the keyword at \a str.
 * @returns the enumeration value.
 * If not found, that value is VTP_INVALID_CMD.
 */
option_value_type_enum_t
find_option_value_type_cmd(char const * str, size_t len)
{
    option_value_type_map_t const * map;

    map = find_option_value_type_name(str, (unsigned int)len);
    return (map == NULL) ? VTP_INVALID_CMD : map->vtp_id;
}

/* end of option-value-type.c */
