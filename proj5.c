#include "simd.p5.h"
#include <math.h>

#define KILO 1024
#define MEGA 1024 * KILO

#ifndef NUM
#define NUM 1
#endif

#define ARRAY_SIZE NUM*KILO //from 1 Kb to 32 Mb
#define NUMTRIES         5

int main(){

        float *A = malloc(ARRAY_SIZE*sizeof(float));
        float *B = malloc(ARRAY_SIZE*sizeof(float));
        float *C = malloc(ARRAY_SIZE*sizeof(float));

	double time_elapsed[4] = {0.,0.,0.,0.};

	srand(time(NULL));
	for(int i=0; i<ARRAY_SIZE; i++)
	{
	   A[i] = rand()%10;
	   B[i] = rand()%10;
	}
	FILE *fd = fopen("MULT_RESULTS", "w+");
	for( int i = 0; i < ARRAY_SIZE; i++ )
		fprintf(fd,"%f %f\n",A[i],B[i]);
	fclose(fd);

        free(C);
        C = malloc(ARRAY_SIZE*sizeof(float));

        double maxmmults = 0.;
        double summmults = 0.;

        for( int t = 0; t < NUMTRIES; t++ )
        {
                double time0 = omp_get_wtime( );

//                #pragma omp parallel for
                for( int i = 0; i < ARRAY_SIZE; i++ )
                {
                        C[i] = A[i] * B[i];
                }
		double time1 = omp_get_wtime( );
                double mmults = (double)ARRAY_SIZE/(time1-time0)/1000000.;
                summmults += mmults;
                if( mmults > maxmmults )
                        maxmmults = mmults;
        }
        //printf( "   Peak Performance = %8.2lf MegaMults/Sec\n", maxmmults );
        //printf( "Average Performance = %8.2lf MegaMults/Sec\n", summmults/(double)NUMTRIES );
        //printf( "%8.2lf\n", maxmmults );
        //printf( "%8.2lf\n", summmults/(double)NUMTRIES );
	FILE *fd1 = fopen("NON_SIMD_max", "a+");
	FILE *fd3 = fopen("NON_SIMD_avg", "a+");
        fprintf(fd1, "%8.2lf\n", maxmmults );
        fprintf(fd3, "%8.2lf\n", summmults/(double)NUMTRIES );
	fclose(fd1);
	fclose(fd3);
	time_elapsed[0] = summmults/(double)NUMTRIES;
	//
//*************************************************************************************************************
	maxmmults = 0.;
        summmults = 0.;

        for( int t = 0; t < NUMTRIES; t++ )
        {
                double time0 = omp_get_wtime( );

		SimdMul( A, B, C, ARRAY_SIZE);

		double time1 = omp_get_wtime( );
                double mmults = (double)ARRAY_SIZE/(time1-time0)/1000000.;
                summmults += mmults;
                if( mmults > maxmmults )
                        maxmmults = mmults;
        }
        //printf( "   Peak Performance = %8.2lf MegaMults/Sec\n", maxmmults );
        //printf( "Average Performance = %8.2lf MegaMults/Sec\n", summmults/(double)NUMTRIES );
        //printf( "%8.2lf\n", maxmmults );
        //printf( "%8.2lf\n", summmults/(double)NUMTRIES );
	FILE *fd2 = fopen("SIMD_max", "a+");
	FILE *fd4 = fopen("SIMD_avg", "a+");
        fprintf(fd2, "%8.2lf\n", maxmmults );
        fprintf(fd4, "%8.2lf\n", summmults/(double)NUMTRIES );
	fclose(fd2);
	fclose(fd4);
	time_elapsed[1] = summmults/(double)NUMTRIES;

//*************************************************************************************************************
	maxmmults = 0.;
        summmults = 0.;
	double sum = 0.;

        for( int t = 0; t < NUMTRIES; t++ )
        {
		sum = 0.;
                double time0 = omp_get_wtime( );

                for( int i = 0; i < ARRAY_SIZE; i++ )
                {
		       sum = sum + (A[i] * B[i]);
                }

		double time1 = omp_get_wtime( );
                double mmults = (double)ARRAY_SIZE/(time1-time0)/1000000.;
                summmults += mmults;
                if( mmults > maxmmults )
                        maxmmults = mmults;
        }
	printf("%d NON_SIMD_Reduction sum: %f\n",ARRAY_SIZE, sum);
        //printf( "   Peak Performance = %8.2lf MegaMults/Sec\n", maxmmults );
        //printf( "Average Performance = %8.2lf MegaMults/Sec\n", summmults/(double)NUMTRIES );
        //printf( "%8.2lf\n", maxmmults );
        //printf( "%8.2lf\n", summmults/(double)NUMTRIES );
	fd2 = fopen("NON_SIMD_RED_max", "a+");
	fd4 = fopen("NON_SIMD_RED_avg", "a+");
        fprintf(fd2, "%8.2lf\n", maxmmults );
        fprintf(fd4, "%8.2lf\n", summmults/(double)NUMTRIES );
	fclose(fd2);
	fclose(fd4);
	time_elapsed[2] = summmults/(double)NUMTRIES;

//*************************************************************************************************************
	maxmmults = 0.;
        summmults = 0.;
        for( int t = 0; t < NUMTRIES; t++ )
        {
		sum = 0.;
                double time0 = omp_get_wtime( );

		sum = SimdMulSum( A, B, ARRAY_SIZE);

		double time1 = omp_get_wtime( );
                double mmults = (double)ARRAY_SIZE/(time1-time0)/1000000.;
                summmults += mmults;
                if( mmults > maxmmults )
                        maxmmults = mmults;
        }
	printf("%d SIMD_Reduction sum: %f\n",ARRAY_SIZE, sum);
        //printf( "   Peak Performance = %8.2lf MegaMults/Sec\n", maxmmults );
        //printf( "Average Performance = %8.2lf MegaMults/Sec\n", summmults/(double)NUMTRIES );
        //printf( "%8.2lf\n", maxmmults );
        //printf( "%8.2lf\n", summmults/(double)NUMTRIES );
	fd2 = fopen("SIMD_RED_max", "a+");
	fd4 = fopen("SIMD_RED_avg", "a+");
        fprintf(fd2, "%8.2lf\n",maxmmults );
        fprintf(fd4, "%8.2lf\n", summmults/(double)NUMTRIES );
	fclose(fd2);
	fclose(fd4);
	time_elapsed[3] = summmults/(double)NUMTRIES;
	
//*************************************************************************************************************

	fd2 = fopen("RED_speedup", "a+");
	fd4 = fopen("NoRed_speedup", "a+");
        fprintf(fd4, "%8.2lf\n", time_elapsed[1]/time_elapsed[0] );
        fprintf(fd2, "%8.2lf\n", time_elapsed[3]/time_elapsed[2] );
	fclose(fd2);
	fclose(fd4);

	free(A);
	free(B);
	free(C);

	return 0;
}















void
SimdMul( float *a, float *b,   float *c,   int len )
{
	int limit = ( len/SSE_WIDTH ) * SSE_WIDTH;
	__asm
	(
		".att_syntax\n\t"
		"movq    -24(%rbp), %rbx\n\t"		// a
		"movq    -32(%rbp), %rcx\n\t"		// b
		"movq    -40(%rbp), %rdx\n\t"		// c
	);

	for( int i = 0; i < limit; i += SSE_WIDTH )
	{
		__asm
		(
			".att_syntax\n\t"
			"movups	(%rbx), %xmm0\n\t"	// load the first sse register
			"movups	(%rcx), %xmm1\n\t"	// load the second sse register
			"mulps	%xmm1, %xmm0\n\t"	// do the multiply
			"movups	%xmm0, (%rdx)\n\t"	// store the result
			"addq $16, %rbx\n\t"
			"addq $16, %rcx\n\t"
			"addq $16, %rdx\n\t"
		);
	}

	for( int i = limit; i < len; i++ )
	{
		printf("whats going on!?\n");
		c[i] = a[i] * b[i];
	}
}



float
SimdMulSum( float *a, float *b, int len )
{
	float sum[4] = { 0., 0., 0., 0. };
	int limit = ( len/SSE_WIDTH ) * SSE_WIDTH;

	__asm
	(
		".att_syntax\n\t"
		"movq    -40(%rbp), %rbx\n\t"		// a
		"movq    -48(%rbp), %rcx\n\t"		// b
		"leaq    -32(%rbp), %rdx\n\t"		// &sum[0]
		"movups	 (%rdx), %xmm2\n\t"		// 4 copies of 0. in xmm2
	);

	for( int i = 0; i < limit; i += SSE_WIDTH )
	{
		__asm
		(
			".att_syntax\n\t"
			"movups	(%rbx), %xmm0\n\t"	// load the first sse register
			"movups	(%rcx), %xmm1\n\t"	// load the second sse register
			"mulps	%xmm1, %xmm0\n\t"	// do the multiply
			"addps	%xmm0, %xmm2\n\t"	// do the add
			"addq $16, %rbx\n\t"
			"addq $16, %rcx\n\t"
		);
	}

	__asm
	(
		".att_syntax\n\t"
		"movups	 %xmm2, (%rdx)\n\t"	// copy the sums back to sum[ ]
	);

	for( int i = limit; i < len; i++ )
	{
		sum[i-limit] += a[i] * b[i];
	}

	return sum[0] + sum[1] + sum[2] + sum[3];
}
