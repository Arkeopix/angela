#define PAM_PM_SESSION
#define _GNU_SOURCE
#include <unisth.h>
#include <stdio.h>
#include <security/pam_appl.h>

int pam_sm_open_session( pam_handle_t *pamh,
			 int flags,
			 int argc,
			 const char **argv ) {
  
  const char	*user = NULL;
  char		*fname;
  int	       	pgu_ret;

  pgu_ret = pam_get_user( pamh, &user, NULL );
  if ( pgu_ret != PAM_SUCCESS || user == NULL ) {
    return PAM_IGNORE;
  }

  if ( ( fname = malloc( 40 * sizeof( char ) ) ) == NULL ) {
    return PAM_IGNORE;
  } else {
    asprintf( &fname, "/home/%s/container/test", user );
  }

  if ( access ( fname, F_OK ) != -1 ) {
    /* Open and Mount container */
  } else {
    return PAM_IGNORE;
  }
}

int pam_sm_close_session( pam_handle_t *pamh,
			  int flags,
			  int argc,
			  const char **argv ) {
  
}
