#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

//#define NUMT	         1
#define ARRAYSIZE        3*1000*1000
#define NUMTRIES         10

int main( )
{
#ifndef _OPENMP
        fprintf( stderr, "OpenMP is not supported here -- sorry.\n" );
        return 1;
#endif

        float *A = malloc(ARRAYSIZE*sizeof(float));
        float *B = malloc(ARRAYSIZE*sizeof(float));
        float *C = malloc(ARRAYSIZE*sizeof(float));

	srand(time(NULL));
	for(int i=0; i<ARRAYSIZE; i++)
	{
	   A[i] = rand()%10;
	   B[i] = rand()%10;
	}
        omp_set_num_threads( NUMT );
        //fprintf( stderr, "Using %d threads\n", NUMT );
        fprintf( stderr, "%d\n", NUMT );

        double maxmmults = 0.;
        double summmults = 0.;

        for( int t = 0; t < NUMTRIES; t++ )
        {
                double time0 = omp_get_wtime( );

                #pragma omp parallel for
                for( int i = 0; i < ARRAYSIZE; i++ )
                {
                        C[i] = A[i] * B[i];
                }

                double time1 = omp_get_wtime( );
                double mmults = (double)ARRAYSIZE/(time1-time0)/1000000.;
                summmults += mmults;
                if( mmults > maxmmults )
                        maxmmults = mmults;
        }
        //printf( "   Peak Performance = %8.2lf MegaMults/Sec\n", maxmmults );
        //printf( "Average Performance = %8.2lf MegaMults/Sec\n", summmults/(double)NUMTRIES );
        printf( "%8.2lf\n", maxmmults );
        printf( "%8.2lf\n", summmults/(double)NUMTRIES );

        return 0;
}
