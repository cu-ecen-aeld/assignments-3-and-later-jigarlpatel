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
      // Wait for the specified time before obtaining the mutex
      
    printf("****** ENTERING to THREAD ********* \n");
     struct thread_data* data = (struct thread_data*)thread_param;
     
    data->thread_complete_success = 0;
    usleep(data->wait_to_obtain_ms * 1000);


    // Obtain the mutex
    int mutexReturn = pthread_mutex_lock(data->mutex);
	if(mutexReturn != 0)
	{
		printf("Not able to lock \n");
	}else
	{
	    // Wait for the specified time before releasing the mutex
	    usleep(data->wait_to_release_ms * 1000);

	    // Release the mutex
	    int mutexReturn = pthread_mutex_unlock(data->mutex);
	    if(0 != mutexReturn)
	    {
	    	printf("Not able to lunlock \n");
	    }else
	    {
               data->thread_complete_success = 1;
	    }
	   
	}


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
    
      pthread_t threadID;
      
    struct thread_data *thread_args = malloc(sizeof(struct thread_data));
    if (thread_args == NULL)
        return false; // Memory allocation failed
            
    thread_args->thread_complete_success = 0;
    // Set up thread_data
    thread_args->mutex = mutex;
    thread_args->wait_to_obtain_ms = wait_to_obtain_ms;
    thread_args->wait_to_release_ms = wait_to_release_ms;
    
    printf("wait_to_obtain_ms %d \n",wait_to_obtain_ms);
    printf("wait_to_release_ms %d \n",wait_to_release_ms);
    
      
      printf("Creating Thread \n");
      // Create a thread
      threadID = pthread_create(thread, NULL, threadfunc, thread_args);
    	if( threadID != 0) {
    		printf("Thread Creation Fail \n");
        	free(thread_args); // Free allocated memory
        	return false;      // Thread creation failed
    	}
      
      	// Wait for the thread to finish
      	#if 0
    	int result = pthread_join(threadID, NULL);
    	if (result != 0) {
    		printf("Failed Thread join\n");
        	perror("Thread join failed \n");
        	return 0;
    	}
    	#endif
      
      printf("Thread has terminated.\n");

      
     return 1;
}

