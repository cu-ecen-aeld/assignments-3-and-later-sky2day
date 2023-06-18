#include "threading.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

// Optional: use these functions to add debug or error prints to your application
#define DEBUG_LOG(msg,...)
//#define DEBUG_LOG(msg,...) printf("threading: " msg "\n" , ##__VA_ARGS__)
#define ERROR_LOG(msg,...) printf("threading ERROR: " msg "\n" , ##__VA_ARGS__)

void* threadfunc(void* thread_param)
{

    // TODO: wait, obtain mutex, wait, release mutex as described by thread_data structure
    // hint: use a cast like the one below to obtain thread arguments from your parameter
    //struct thread_data* thread_func_args = (struct thread_data *) thread_param;
    struct thread_data* args = (struct thread_data *) thread_param;

    int sleep_before_lock = (args -> wait_to_obtain_ms);
    usleep(sleep_before_lock * 1000);
    printf("Thread slept before lock: %d millis\n", sleep_before_lock);
    pthread_mutex_lock (args->mutex);

    printf("\nLOCKED\n");
    int sleep_before_unlock = (args -> wait_to_release_ms);
    usleep(sleep_before_unlock * 1000);
    printf("Thread slept before unlock: %d millis\n", sleep_before_unlock);
    pthread_mutex_unlock (args->mutex);
    printf("\nMutex unlocked\n");

    args -> thread_complete_success = true;
    pthread_exit(args);

    // return thread_param;
}



/**
* Start a thread which sleeps @param wait_to_obtain_ms number of milliseconds, then obtains the
* mutex in @param mutex, then holds for @param wait_to_release_ms milliseconds, then releases.
* The start_thread_obtaining_mutex function should only start the thread and should not block
* for the thread to complete.
* The start_thread_obtaining_mutex function should use dynamic memory allocation for thread_data
* structure passed into the thread.  The number of threads active should be limited only by the
* amount of available memory.
* The thread started should return a pointer to the thread_data structure when it exits, which can be used
* to free memory as well as to check thread_complete_success for successful exit.
* If a thread was started succesfully @param thread should be filled with the pthread_create thread ID
* coresponding to the thread which was started.
* @return true if the thread could be started, false if a failure occurred.
*/

bool start_thread_obtaining_mutex(pthread_t *thread, pthread_mutex_t *p_mutex,int wait_to_obtain_ms, int wait_to_release_ms)
{
    /**
     * TODO: allocate memory for thread_data, setup mutex and wait arguments, pass thread_data to created thread
     * using threadfunc() as entry point.
     *
     * return true if successful.
     *
     * See implementation details in threading.h file comment block
     */

     //setup mutex and wait arguments
     // pthread_mutex_init(mutex, NULL);
     
     //allocate memory for thread_data thread_data
     struct thread_data *data = malloc(sizeof (struct thread_data));
     data -> mutex = p_mutex;
     data -> wait_to_obtain_ms = wait_to_obtain_ms;
     data -> wait_to_release_ms = wait_to_release_ms;


     
     //pass thread_data to created thread using threadfunc() as entry point.
     int t = pthread_create(thread, NULL, threadfunc, (void*) data);
     printf("Thread started: %ld\n", *thread);
     //pthread_join(*thread, NULL);
     // sleep(30);
     return t == 0;
}

