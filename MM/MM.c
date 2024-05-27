#include <stdio.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX_SIZE 5000

struct RowResult{
	int *result;
	int index;
};

struct shared_data{
	int **a;
	int **b;
	struct RowResult *results;
	int row;
	int colB;
};

void *populateRows(void *arg)
{
	struct shared_data *data = (struct shared_data *)arg;
	int *row = (int *)malloc(data->colB * sizeof(int));

	//iterate through the columns of the second matrix
	for(int i=0; i<data->colB; i++)
	{
		int sum = 0;
		//for every column
		for(int j=0; j<data->colB; j++)
		{
			sum=sum+(data->a[data->row][j] * data->b[j][i]);
		}
		row[i]=sum;
	}

	data->results[data->row].result=row;
	data->results[data->row].index=data->row;

	pthread_exit(NULL);
}

int **concurrentMatrixAlgorithm(int **a, int **b, int lenA, int colB)
{
	int **finalResult=(int **)malloc(lenA * sizeof(int *));
	struct RowResult *rowResults = (struct RowResult *)malloc(lenA * sizeof(struct RowResult));
	pthread_t threads[MAX_SIZE];

	for(int i=0; i<lenA; i++)
	{
		struct shared_data *data=(struct shared_data *)malloc(sizeof(struct shared_data));
		data->a=a;
		data->b=b;
		data->results=rowResults;
		data->row=i;
		data->colB=colB;
		pthread_create(&threads[i], NULL, populateRows, (void *)data);
	}

	for(int i=0; i<lenA; i++)
	{
		pthread_join(threads[i], NULL);
		finalResult[rowResults[i].index]=rowResults[i].result;
	}
	return finalResult;
}


int main(int argc, char *argv[])
{
	struct timespec start_time, end_time;
	if(argc!=3)
	{
		printf("Incorrect number of arguments");
	}

	int size=atoi(argv[1]);
	int cutoff=atoi(argv[2]);

	int **matrixA=(int **)malloc(size * sizeof(int *));
	for(int i=0; i<size; i++)
	{
		matrixA[i]=(int *)malloc(size * sizeof(int));
	}

	int **matrixB=(int **)malloc(size * sizeof(int *));
	for(int i=0; i<size; i++)
	{
		matrixB[i]=(int *)malloc(size * sizeof(int));
	}

	for(int i=0; i<size; i++)
	{
		for(int j=0; j<size; j++)
		{
			matrixA[i][j]=rand()%cutoff;
		}
	}

	for(int i=0; i<size; i++)
	{
		for(int j=0; j<size; j++)
		{
			matrixB[i][j]=rand()%cutoff;
		}
	}

	clock_gettime(CLOCK_MONOTONIC, &start_time);

	int **result=concurrentMatrixAlgorithm(matrixA, matrixB, size, size);

/*	printf("Results of matrix multiplicaiton:\n");
	for(int i=0;i<size;i++)
	{
		for(int j=0;j<size;j++)
		{
			printf("%d ", result[i][j]);
		}
		printf("\n");
	}*/

	clock_gettime(CLOCK_MONOTONIC, &end_time);
	double elapsed = (end_time.tv_sec - start_time.tv_sec);
	elapsed += (end_time.tv_nsec - start_time.tv_nsec) / 1000000000.0;

	printf("%f", elapsed);

	for(int i=0; i<size; i++)
	{
		free(matrixA[i]);
	}
	free(matrixA);

	for(int i=0; i<size; i++)
	{
		free(matrixB[i]);
	}
	free(matrixB);

	for(int i=0; i<size; i++)
	{
		free(result[i]);
	}
	free(result);

	return 0;

}
















