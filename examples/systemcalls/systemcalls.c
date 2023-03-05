#include "systemcalls.h"

/**
 * @param cmd the command to execute with system()
 * @return true if the command in @param cmd was executed
 *   successfully using the system() call, false if an error occurred,
 *   either in invocation of the system() call, or if a non-zero return
 *   value was returned by the command issued in @param cmd.
*/
bool do_system(const char *cmd)
{

/*
 * TODO  add your code here
 *  Call the system() function with the command set in the cmd
 *   and return a boolean true if the system() call completed with success
 *   or false() if it returned a failure
*/
  bool isSuccessful;
  int ret;
  ret = system(cmd);
  if(ret == 0 && WEXITSTATUS (ret) == 0) {
    isSuccessful = true;
  }

  return isSuccessful;
}

/**
* @param count -The numbers of variables passed to the function. The variables are command to execute.
*   followed by arguments to pass to the command
*   Since exec() does not perform path expansion, the command to execute needs
*   to be an absolute path.
* @param ... - A list of 1 or more arguments after the @param count argument.
*   The first is always the full path to the command to execute with execv()
*   The remaining arguments are a list of arguments to pass to the command in execv()
* @return true if the command @param ... with arguments @param arguments were executed successfully
*   using the execv() call, false if an error occurred, either in invocation of the
*   fork, waitpid, or execv() command, or if a non-zero return value was returned
*   by the command issued in @param arguments with the specified arguments.
*/


bool do_exec(int count, ...)
{
    va_list args;
    va_start(args, count);
    char * command[count+1];
    int i;
    for(i=0; i<count; i++)
    {
        command[i] = va_arg(args, char *);
    }
    command[count] = NULL;

/*
 * TODO:
 *   Execute a system command by calling fork, execv(),
 *   and wait instead of system (see LSP page 161).
 *   Use the command[0] as the full path to the command to execute
 *   (first argument to execv), and use the remaining arguments
 *   as second argument to the execv() command.
 *
*/

/*
  char * command_path = command[0];
  if(command_path[0] != '/') {
    printf("Path is not absolute: %s\n", command_path);
    return false;
  }

   for(i=1; i<count-1; i++) {
     printf("Comparing  %s and -f\n", command[i]);
     if(strcmp(command[i], "-f") == 0) {
       printf("-f option was passed with command\n");
       char * file_path = command[i+1];
       if(file_path[0] != '/') {
         printf("File argument is not absolute path: %s\n", file_path);
         return false; 
       } 
     }
   }
*/
  fflush(stdout);

  int child_status;
  pid_t child_pid;
  if ((child_pid = fork()) < 0) {
    return false;
  } else if (child_pid == 0) {
      printf("Child process: %d\n", child_pid);
      int execv_res =  execv (command[0], command);
      if(execv_res == -1) {
        exit (EXIT_FAILURE);
      }
  }

  if (waitpid (child_pid, &child_status, 0) == -1) {
    return false;
  } else if (WIFEXITED (child_status)) {
      printf("WIFEXITED- child_status: %d\n", child_status);
      if(WEXITSTATUS (child_status) != 0) {
        return false;
      }
  }

    va_end(args);

    return true;
}



/**
* @param outputfile - The full path to the file to write with command output.
*   This file will be closed at completion of the function call.
* All other parameters, see do_exec above
*/
bool do_exec_redirect(const char *outputfile, int count, ...)
{
    va_list args;
    va_start(args, count);
    char * command[count+1];
    int i;
    for(i=0; i<count; i++)
    {
        command[i] = va_arg(args, char *);
    }
    command[count] = NULL;

/*
 * TODO
 *   Call execv, but first using https://stackoverflow.com/a/13784315/1446624 as a refernce,
 *   redirect standard out to a file specified by outputfile.
 *   The rest of the behaviour is same as do_exec()
 *
*/

  int fd = open(outputfile, O_WRONLY|O_TRUNC|O_CREAT, 0644);
  if (fd < 0) {
    printf("Error opening file %s", outputfile);
    exit (EXIT_FAILURE); 
  }

  int child_status;
  pid_t child_pid;
  if ((child_pid = fork()) < 0) {
    return -1;
  } else if (child_pid == 0) {
    printf("Child process: %d\n", child_pid);
   // char * command_args  = &command +1;

    if (dup2(fd, 1) < 0) { 
      printf("dup2"); 
      return 1; 
    }
    close(fd);
    execv (command[0], command);
   //printf("Unknown command: %s\n", &command);
   // printf("Unknown command: %.*s\n", (int)sizeof(command), command);
    exit (EXIT_FAILURE);
  }

  if (waitpid (child_pid, &child_status, 0) == -1) {
    return false;
  } else if (WIFEXITED (child_status)) {
      printf("WIFEXITED- child_status: %d\n", child_status);
      if(WEXITSTATUS (child_status) != 0) {
        return false;
      }
  }


    va_end(args);

    return true;
}
