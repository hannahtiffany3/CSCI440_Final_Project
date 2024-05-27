#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#include "multi-lookup.h"
#include "util.h"

#define MINARGS 3
#define USAGE "<inputFilePath> <outputFilePath>"


void* resolver(void* arg)
{
	char* hostname;
	char firstipstr[INET6_ADDRSTRLEN];
	struct bounded_buffer *buffer=arg;
	while(1)
	{
		//while the queue is not empty
		pthread_mutex_lock(&mutex);
		while(queue_is_empty(&buffer->q))
		{
			pthread_mutex_unlock(&mutex);
			unsigned int sleep_time = rand() % 100 +1;
			usleep(sleep_time);
			pthread_mutex_lock(&mutex);
		}
		hostname=queue_pop(&buffer->q);
		pthread_mutex_unlock(&mutex);

		if(dnslookup(hostname, firstipstr, sizeof(firstipstr)) == UTIL_FAILURE){
			fprintf(stderr, "dnslookup error: %s\n", hostname);
			strncpy(firstipstr, "", sizeof(firstipstr));
		}
		pthread_mutex_lock(&file_mutex);
		fflush(buffer->outputfp);
		fflush(buffer->outputfp);
		fprintf(buffer->outputfp, "%s,%s\n", hostname, firstipstr);
		free(hostname);
		pthread_mutex_unlock(&file_mutex);
		pthread_mutex_lock(&mutex);
		if(queue_is_empty(&buffer->q)&&buffer->complete==1)
		{
			pthread_mutex_unlock(&mutex);
			break;
		}
		pthread_mutex_unlock(&mutex);
	}

	return NULL;
}


// Function for requester thread
void* requester(void* arg) {
	static char* errorstr;
	char* hostname;

	struct bounded_buffer *buffer=arg;
	// Loop Through Input Files 
	for(int i=1; i<(buffer->c-1);i++){

		// Open Input File //
		buffer->inputfp = fopen(buffer->v[i], "r");
		if(!buffer->inputfp){
			sprintf(errorstr, "Error Opening Input File: %s", buffer->v[i]);
			perror(errorstr);
			break;
		}



		//for each line in the file
		hostname=malloc(sizeof(char) * (INET6_ADDRSTRLEN+1));
		while(fscanf(buffer->inputfp, INPUTFS, hostname) > 0){
			//if queue if full, wait
			pthread_mutex_lock(&mutex);
			while(queue_is_full(&buffer->q))
			{
				pthread_mutex_unlock(&mutex);
				//random number from 0 to 100
				unsigned int sleep_time = rand() % 100 +1;
				usleep(sleep_time);
				pthread_mutex_lock(&mutex);
			}
			queue_push(&buffer->q, (char *)hostname);
			hostname=malloc(sizeof(char) * (INET6_ADDRSTRLEN+1));
			pthread_mutex_unlock(&mutex);
		}
	}
	return NULL;
}


int main(int argc, char* argv[]){
	struct timespec start_time, end_time;
	long RESOLVER_NUM_THREADS=2;
	long REQUESTER_NUM_THREADS=1;
	pthread_t resolver_threads[RESOLVER_NUM_THREADS];
	pthread_t requester_threads[RESOLVER_NUM_THREADS];
	int rc;
	int rs;
	long t;
	//long cpyt[NUM_THREADS];//stores thread id info
	struct bounded_buffer *buffer=(struct bounded_buffer*)malloc(sizeof(struct bounded_buffer));
	buffer->outputfp=NULL;
	buffer->inputfp=NULL;
	buffer->c=argc;
	buffer->v=argv;
	buffer->complete=0;
	queue_init(&buffer->q, 10);

	/* Check Arguments */
	if(argc < MINARGS){
		fprintf(stderr, "Not enough arguments: %d\n", (argc - 1));
		fprintf(stderr, "Usage:\n %s %s\n", argv[0], USAGE);
		return EXIT_FAILURE;
	}

	/* Open Output File */
	buffer->outputfp = fopen(argv[(argc-1)], "w");
	if(!buffer->outputfp){
		perror("Error Opening Output File");
		return EXIT_FAILURE;
	}

	clock_gettime(CLOCK_REALTIME, &start_time);
	/* Spawn a requester and resolver thread */
	for(t=0; t<REQUESTER_NUM_THREADS; t++)
	{
		rc = pthread_create(&(requester_threads[t]), NULL, requester, (void *)buffer);
		printf("In main: creating requester thread %ld\n", t);
		if (rc){
			printf("ERROR; return code from pthread_create() is %d\n", rc);
			exit(EXIT_FAILURE);
		}
	}
	for(t=0; t<RESOLVER_NUM_THREADS; t++)
	{
		rs = pthread_create(&(resolver_threads[t]), NULL, resolver, (void *)buffer);
		printf("In main: creating resolver thread %ld\n", t);
		if (rs){
		printf("ERROR; return code from pthread_create() is %d\n", rc);
			exit(EXIT_FAILURE);
		}
	}


	/* Wait for All Theads to Finish */
	for(t=0;t<REQUESTER_NUM_THREADS;t++){
		pthread_join(requester_threads[t],NULL);
	}
	buffer->complete=1;

	for(t=0;t<RESOLVER_NUM_THREADS;t++){
		pthread_join(resolver_threads[t],NULL);
	}

	clock_gettime(CLOCK_REALTIME, &end_time);
	printf("All of the threads were completed!\n");
	double elapsed = (end_time.tv_sec-start_time.tv_sec);
	elapsed += (end_time.tv_nsec - start_time.tv_nsec)/1000000000.0;
	printf("%f", elapsed);
	queue_cleanup(&buffer->q);
	free(buffer);
	return 0;

}
