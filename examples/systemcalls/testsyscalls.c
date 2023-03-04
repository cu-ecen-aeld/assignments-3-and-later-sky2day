#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "systemcalls.h"



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

  int child_status;
  pid_t child_pid;
  if ((child_pid = fork()) < 0) {
    return -1;
  } else if (child_pid == 0) {
    printf("Child process: %d\n", child_pid);
    //char * command_args  = command +1;
   // execv (command[0], command_args);
    execv (command[0], command);
   // printf("Unknown command: %s\n", &command);
    //printf("Unknown command: %.*s\n", (int)sizeof(command), command);
    exit (EXIT_FAILURE);
  }

  if (waitpid (child_pid, &child_status, 0) == -1) {
    return -1;
  } else if (WIFEXITED (child_status)) {
    return WEXITSTATUS (child_status);
  }

  //return -1;


    va_end(args);

    return true;
}


/*
void test_exec_calls()
{
    printf("Running tests at %s : function %s\n",__FILE__,__func__);
    TEST_ASSERT_FALSE_MESSAGE(do_exec(2, "echo", "Testing execv implementation with echo"),
             "The exec() function should have returned false since echo was not specified"
             " with absolute path as a command and PATH expansion is not performed.");
    TEST_ASSERT_FALSE_MESSAGE(do_exec(3, "/usr/bin/test","-f","echo"),
             "The exec() function should have returned false since echo was not specified"
             " with absolute path in argument to the test executable.");
    TEST_ASSERT_TRUE_MESSAGE(do_exec(3, "/usr/bin/test","-f","/bin/echo"),
             "The function should return true since /bin/echo represents the echo command"
             " and test -f verifies this is a valid file");
}
*/

int main(void) {
  do_exec(2, "echo", "Testing execv implementation with echo");
  do_exec(3, "/usr/bin/test","-f","echo");
 // do_exec(3, "/usr/bin/test","-f","/bin/echo"); 
 return 1;
}
