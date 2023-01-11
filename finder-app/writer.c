// Accepts the following arguments: the first argument is a full path to a file (including filename) on the filesystem, 
// referred to below as writefile; 
// the second argument is a text string which will be written within this file, referred to below as writestr
// Exits with value 1 error and print statements if any of the arguments above were not specified
// Creates a new file with name and path writefile with content writestr, overwriting any existing file and creating the path if it doesn’t exist.
// Exits with value 1 and error print statement if the file could not be created.

// One difference from the write.sh instructions in Assignment 1:  You do not need to make your "writer" utility create directories which do not exist.
// You can assume the directory is created by the caller.

// Setup syslog logging for your utility using the LOG_USER facility.
// Use the syslog capability to write a message “Writing <string> to <file>” where <string> is the text string written to file (second argument) 
// and <file> is the file created by the script.  This should be written with LOG_DEBUG level.
// Use the syslog capability to log any unexpected errors with LOG_ERR level.



#include <stdio.h>
#include <syslog.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char **argv) {
    openlog(NULL, 0, LOG_USER);

    if(argc < 3) {
	if(argc < 2){
	    printf("No arguments provided\n"); 
	    syslog(LOG_ERR, "No arguments provided");
	    return 1;
	}
	printf("Only one argument provided while two is required\n");
	syslog(LOG_ERR, "Only one argument provided while two is required");
	return 1;
    }

    const char *filePath = argv[1];
    const char *text = argv[2];

    // int open(const char *pathname, int flags);
    // int open(const char *pathname, int flags, mode_t mode);
    int fd = open(filePath, O_CREAT | O_WRONLY);
    if (fd == -1) {
    	printf("Error opening file %s", filePath);
    	syslog(LOG_ERR, "Error opening file %s", filePath);

    	return 1;
    }
    syslog(LOG_DEBUG, "Writing %s to %s", text, filePath);

    //ssize_t write(int fd, const void *buf, size_t count);
    ssize_t bytes =  write(fd, text, strlen(text));
    close(fd);
    closelog();

    return 0;
}
