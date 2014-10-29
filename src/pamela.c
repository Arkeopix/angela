#define PAM_PM_SESSION
#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <security/pam_appl.h>

# define	 BUFF_SIZE	255

static int	fork_exec_shell_script( char *user, 
					char *container ) {
  char		*argv[2] = { user, container };
  pid_t		worker;
  int		ret;

  if ( ( worker = fork() ) == 0 ) {
    /* child process do stuff like chinese kid */
    printf("%s %s\n", user, container);
    if (execlp( "./scripts/open_mount_container.sh", user, container, 0 ) < 0)
      return 1;
  } else if ( worker < 0) {
    return 1;
  } else {
    /* parent process wait for child */
    waitpid( worker, &ret, 0 );
    return ret;
  }
}

int pam_sm_open_session( pam_handle_t *pamh,
			 int flags,
			 int argc,
			 const char **argv ) {
  
  char		*user = NULL;
  char		*container = NULL;
  char		*fname = NULL;
  int	       	pgu_ret;
  DIR		*dp = NULL;
  struct dirent	*ep = NULL;

  pgu_ret = pam_get_user( pamh, &user, NULL );
  if ( pgu_ret != PAM_SUCCESS || user == NULL ) {
    return PAM_IGNORE;
  }
  if ( ( fname = malloc( 40 * sizeof( char ) ) ) == NULL ) {
    return PAM_IGNORE;
  }
  printf("User: %s\n", user);
  asprintf( &fname, "/home/%s/container/", user );
  printf("User: %s\n", user);
  if ( ( dp = opendir( fname ) ) != NULL ) {
    /* Open and Mount containers */
    while ( ep = readdir( dp ) ){
      printf("User: %s\n", user);
      if (strcmp(ep->d_name, ".") && strcmp(ep->d_name, "..")) {
	printf("Container: %s\n", ep->d_name);
	printf("User: %s OMG\n", user);	
	/*
	if (!(container = malloc(BUFF_SIZE))) {
	  return PAM_IGNORE;
	}
	memset( &container, 0, BUFF_SIZE );
	  if ( !(container = strcpy( container, ep->d_name )) ) {
	  return PAM_IGNORE;
	  }
	*/
	printf("User: %s OMG\n", user);
	if ( ( fork_exec_shell_script( user, (char*)ep->d_name ) ) != 0 ) {
	  return PAM_IGNORE;
	}
	free(container);
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
