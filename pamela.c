#define PAM_PM_SESSION
#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <unisth.h>
#include <dirent.h>
#include <sys/types.h>
#include <security/pam_appl.h>

static int	fork_exec_shell_script( char *user, 
					char *container ) {
  pid_t		worker;
  
  if ( ( worker = fork() ) < 0 ) {
    return 1;
  } else if ( worker == 0 ) {
    /* child process do stuff like chinese kid */
  } else {
    /* parent process wait for child */
  }
}

int pam_sm_open_session( pam_handle_t *pamh,
			 int flags,
			 int argc,
			 const char **argv ) {
  
  const char	*user = NULL;
  char		container[26];
  char		*fname;
  int	       	pgu_ret;
  DIR		*dp;
  struct dirent	*ep;

  pgu_ret = pam_get_user( pamh, &user, NULL );
  if ( pgu_ret != PAM_SUCCESS || user == NULL ) {
    return PAM_IGNORE;
  }

  if ( ( fname = malloc( 40 * sizeof( char ) ) ) == NULL ) {
    return PAM_IGNORE;
  }

  asprintf( &fname, "/home/%s/container/", user );
  if ( ( dp = opendir( fname ) ) != NULL ) {
    /* Open and Mount containers */
    while ( ep = readdir( dp ) ){
      memset( &container, 0, 26 );
      container = ep->d_name;
      if ( ( fork_exec_shell_script( user, container ) ) != 0 ) {
	return PAM_IGNORE;
      }
    }
  } else {
    return PAM_IGNORE;
  }
  return PAM_SUCCESS;
}

int pam_sm_close_session( pam_handle_t *pamh,
			  int flags,
			  int argc,
			  const char **argv ) {
  
}
