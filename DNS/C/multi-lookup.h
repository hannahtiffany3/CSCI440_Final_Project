#ifndef MULTI_LOOKUP_H 
#define MULTI_LOOKUP_H 

#include"util.h"
#include"queue.h"
#include<pthread.h>
#include<string.h>
#include<time.h>
#include<sys/time.h>


#define SBUFSIZE 1025
#define INPUTFS "%1024s"


pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t file_mutex = PTHREAD_MUTEX_INITIALIZER;


struct bounded_buffer {
		int c;
		char** v;

                queue q;
		char firstipstr[INET6_ADDRSTRLEN];

		FILE* inputfp;
		FILE* outputfp;

		int complete;

};


//int* payload_in[SBUFSIZE];
//int* payload_out[SBUFSIZE];
//char ip[SBUFSIZE];

// Function for resolver thread
void* resolver(void* arg);

// Function for requester thread
void* requester(void* arg);


#endif
