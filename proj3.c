#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

#define BIG_NUMBER 1000000000
#define DIV 4000000000.
#define MILLION 1000000.
struct s
{
	float value;
	int pad[NUM];
} Array[4];

int main(){

#ifndef _OPENMP
	fprintf( stderr, "OpenMP is not available\n" );
	return 1;
#endif
	
	
	omp_set_num_threads( NUMT );

	unsigned int someBigNumber = BIG_NUMBER;	// if > 4B, use "long unsigned int"

	double time0 = omp_get_wtime();

	float sum;
	#pragma omp parallel for private(sum)
	for( int i = 0; i < 4; i++ )
	{
		for( unsigned int j = 0; j < someBigNumber; j++ )
		{
			sum = sum + 2.;
			//Array[i].value = Array[i].value + 2.;
		}
	}
	double time1 = omp_get_wtime();
	FILE *fd;
	switch(NUMT)
	{
		case 1:
			fd = fopen("results11", "a+");
			break;
		case 2:
			fd = fopen("results22", "a+");
			break;
		case 4:
			fd = fopen("results44", "a+");
			break;
		case 8:
			fd = fopen("results88", "a+");
			break;
		default:
			printf("unknown NUMT\n");
			exit(0);
	}
	fprintf(fd,"%d %f\n", NUM, (DIV)/(time1-time0)/MILLION);
	fclose(fd);

	return 0;
}

//	fprintf(fd,"Threads:%d\t\tNUM:%5d\ttime:%f\n", NUMT, NUM, time1-time0);	
