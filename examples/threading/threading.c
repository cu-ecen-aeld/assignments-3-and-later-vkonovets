#include "threading.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

// Optional: use these functions to add debug or error prints to your application
#define DEBUG_LOG(msg,...)
//#define DEBUG_LOG(msg,...) printf("threading: " msg "\n" , ##__VA_ARGS__)
#define ERROR_LOG(msg,...) printf("threading ERROR: " msg "\n" , ##__VA_ARGS__)

void* threadfunc(void* thread_param)
{

    // TODO: wait, obtain mutex, wait, release mutex as described by thread_data structure
    // hint: use a cast like the one below to obtain thread arguments from your parameter
    //struct thread_data* thread_func_args = (struct thread_data *) thread_param;
    struct thread_data* thread_func_args = (struct thread_data*)thread_param;

    struct timespec req = {
        .tv_sec = thread_func_args->wait_to_obtain_ms / 1000,
        .tv_nsec = (thread_func_args->wait_to_obtain_ms % 1000) * 1000000
    };
    struct timespec rem;

    int rc = nanosleep(&req, &rem);
    if (rc != 0)
    {
        printf("nanosleep call 1 failed with exit code %d.\n", rc);
        thread_func_args->thread_complete_success = false;
        return thread_param;
    }
    
    rc = pthread_mutex_lock(thread_func_args->mutex);
    if (rc != 0)
    {
        printf("pthread_mutex_lock failed with exit code %d.\n", rc);
        thread_func_args->thread_complete_success = false;
        return thread_param;
    }

    req.tv_sec = thread_func_args->wait_to_release_ms / 1000;
    req.tv_nsec = (thread_func_args->wait_to_release_ms % 1000) * 1000000;

    rc = nanosleep(&req, &rem);
    if (rc != 0)
    {
        printf("nanosleep call 2 failed with exit code %d.\n", rc);
        thread_func_args->thread_complete_success = false;
        return thread_param;
    }

    rc = pthread_mutex_unlock(thread_func_args->mutex);
    if (rc != 0)
    {
        printf("pthread_mutex_unlock failed with exit code %d.\n", rc);
        thread_func_args->thread_complete_success = false;
        return thread_param;
    }

    thread_func_args->thread_complete_success = true;
    return thread_param;
}


bool start_thread_obtaining_mutex(pthread_t *thread, pthread_mutex_t *mutex,int wait_to_obtain_ms, int wait_to_release_ms)
{
    /**
     * TODO: allocate memory for thread_data, setup mutex and wait arguments, pass thread_data to created thread
     * using threadfunc() as entry point.
     *
     * return true if successful.
     *
     * See implementation details in threading.h file comment block
     */
    
    int rc = pthread_mutex_init(mutex, NULL);
    if (rc != 0)
    {
        printf("Attempt to pthread_mutex_init failed with %d\n.", rc);
        return false;
    }
    
    // Write mutex pointer to struct only after it has been created
    struct thread_data* data = (struct thread_data*)malloc(sizeof(struct thread_data));
    data->wait_to_obtain_ms = wait_to_obtain_ms;
    data->wait_to_release_ms = wait_to_release_ms;
    data->mutex = mutex;

    return true;
}

