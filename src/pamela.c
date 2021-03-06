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

static int	fork_exec_shell_script(char *script,
				       char *user, 
				       char *container ) {
  char		*argv[2] = { user, container };
  pid_t		worker;
  int		ret;

  if ( ( worker = fork() ) == 0 ) {
    if (execlp(script, script, user, container, 0 ) < 0)
      return 1;
  } else if ( worker < 0) {
    return 1;
  } else {
    waitpid( worker, &ret, 0 );
    return ret;
  }
}

static char	**get_container_list( char *user ) {
  FILE		*fp;
  char		*line;
  size_t	len = 0;
  ssize_t	read;
  int		i = 0;
  char		*fname = NULL;
  char		**tab;
  
  if ( ( fname = malloc( 40 * sizeof( char ) ) ) == NULL )
    return NULL;
  asprintf( &fname, "/home/%s/.container", user );
  if ( ( fp = fopen( fname, "r" ) ) != NULL ) {
    if ( ( tab = malloc( sizeof( *tab) ) ) == NULL )
      return NULL;
    while ( ( read = getline( &line, &len, fp ) ) != -1 ) {
      if ( ( tab = realloc( tab, ( i + 1 ) * sizeof( *tab) ) ) == NULL )
	return NULL;
      if ( ( tab[i] = malloc( strlen( line ) * sizeof( *line ) ) ) == NULL )
	return NULL;
      line[strlen(line) - 1] = '\0';
      tab[i] = strcpy( tab[i], line );
      i++;
    }
    tab[i] = NULL;
    fclose( fp );
    if ( line )
      free (line );
    free( fname );
  } else {
    free( fname );
    return NULL;
  }
  return tab;
}

static int	is_in_file( char *d_name,
			    char **cl ) {
  int		i;

  for ( i = 0; cl[i] != NULL; i++ ) {
    printf( "[%s] [%s]\n", d_name, cl[i] );
    if ( strcmp( d_name, cl[i] ) == 0 )
      return 1;
  }
  return 0;
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
  char		**container_list;

  pgu_ret = pam_get_user( pamh, &user, NULL );
  if ( pgu_ret != PAM_SUCCESS || user == NULL ) {
    return PAM_IGNORE;
  }
  if ( ( fname = malloc( 40 * sizeof( char ) ) ) == NULL ) {
    return PAM_IGNORE;
  }
  asprintf( &fname, "/home/%s/container/", user );
  if ( ( container_list = get_container_list( user ) ) == NULL )
    return PAM_IGNORE;
  if ( ( dp = opendir( fname ) ) != NULL ) {
    while ( ep = readdir( dp ) ){
      if (strcmp(ep->d_name, ".") && strcmp(ep->d_name, "..") &&
	  ep->d_type != DT_DIR 
	  && is_in_file( ep->d_name, container_list ) ) {
	if ( ( fork_exec_shell_script( 
	      "/usr/share/pamela/open_mount_container.sh", 
	      user, (char*)(ep->d_name) ) ) != 0 ) {
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
  char		*user = NULL;
  char		*tmp_user = NULL;
  char		*container = NULL;
  char		*fname = NULL;
  int	       	pgu_ret;
  DIR		*dp = NULL;
  struct dirent	*ep = NULL;

  pgu_ret = pam_get_user( pamh, &user, NULL );
  if ( pgu_ret != PAM_SUCCESS || user == NULL )
    return PAM_IGNORE;
  if ( ( fname = malloc( 40 * sizeof( char ) ) ) == NULL )
    return PAM_IGNORE;
  if ( ( dp = opendir( "/tmp/pamela" ) ) != NULL )
    {
      while ( ep = readdir( dp ) )
	{
	  if (strcmp(ep->d_name, ".") && strcmp(ep->d_name, "..") &&
	      ep->d_type != DT_DIR)
	    {
	      printf("File: %s\n", ep->d_name);
	      if (!(tmp_user = strtok((char*)(ep->d_name), "+")))
		return (PAM_IGNORE);
	      printf("User token: %s (Expected: %s)\n", tmp_user, user);
	      if (!strcmp(user, tmp_user))
		{
		  printf("Match!\n");
		  if (!(container = strtok(NULL, "+")))
		    return (PAM_IGNORE);
		  printf("Container: %s\n", container);
		  if ( ( fork_exec_shell_script(  
			"/usr/share/pamela/close_unmount_container.sh", 
			user, container ) ) != 0 )
		    return (PAM_IGNORE);
		}
	    }
	}
    }
  else
    return (PAM_IGNORE);
  return (PAM_SUCCESS);
}
