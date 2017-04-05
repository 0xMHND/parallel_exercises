#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <omp.h>
#include <time.h>

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

float Ranf( float low, float high );
int Rani( int ilow, int ihigh );

int	NowYear;		// 2016 - 2021
int	NowMonth;		// 0 - 11

float	PrecipFactor;		// inches of rain per month
float	TempFactor;		//
float	NowPrecip;		// 
float	NowTemp;		// temperature this month
float	NowHeight;		// grain height in inches
int	NowNumDeer;		// number of deer in the current population

int     LocustsIsHere;		// Is Locusts here this month 
const float ONE_LOCUST_EATS_PER_MONTH =		0.01;

const float GRAIN_GROWS_PER_MONTH =		8.0; //inches
const float ONE_DEER_EATS_PER_MONTH =		0.5;

const float AVG_PRECIP_PER_MONTH =		6.0; //inches
const float AMP_PRECIP_PER_MONTH =		6.0;
const float RANDOM_PRECIP =			2.0;

const float AVG_TEMP =				50.0; //Fahrenheit
const float AMP_TEMP =				20.0;
const float RANDOM_TEMP =			10.0;

const float MIDTEMP =				50.0; //Fahrenheit
const float MIDPRECIP =				6.0; //inches


int GrainDeer(){

	int temp_grain;
	int temp_deer;
	int temp_temp;

while( NowYear <= 2021 )
{
	// compute a temporary next-value for this quantity based on the current state of the simulation:
	temp_grain = NowHeight;
	temp_deer = NowNumDeer;
	temp_temp = NowTemp;

	if(temp_grain > (float)(temp_deer) ) 
		temp_deer += 1;
	else 
		temp_deer -= 1;

	if(temp_deer<1)
		temp_deer=1;

/*
	if( (temp_temp < 0) || (temp_temp>42) )
		temp_deer = temp_deer/2;
	else if ( (temp_temp < 5) || (temp_temp > 30) )
		temp_deer = temp_deer / 3;
*/
	// DoneComputing barrier:
	#pragma omp barrier
	NowNumDeer = temp_deer;

	// DoneAssigning barrier:
	#pragma omp barrier

	// DonePrinting barrier:
	#pragma omp barrier
}

	return 0;
}
int Grain(){

	double percipFactor = 0;
	double tempFactor = 0;

while( NowYear <= 2021 )
{
	// compute a temporary next-value for this quantity based on the current state of the simulation:

	TempFactor = exp( (-1) * pow( ( (NowTemp-MIDTEMP)    / 10 ) , 2) );
	PrecipFactor =   exp( (-1) * pow( ( (NowPrecip-MIDPRECIP)/ 10 ) , 2) );

	// DoneComputing barrier:
	#pragma omp barrier

	NowHeight += TempFactor * PrecipFactor * GRAIN_GROWS_PER_MONTH;
	NowHeight -= (float)NowNumDeer * ONE_DEER_EATS_PER_MONTH;
	NowHeight -= (float)LocustsIsHere * ONE_LOCUST_EATS_PER_MONTH;
	if( (NowHeight < 0.) ) 
			NowHeight = 0.;


	// DoneAssigning barrier:
	#pragma omp barrier
	// DonePrinting barrier:
	#pragma omp barrier
}
	return 0;
}

// This kind of insects can eat the whole grain if it comes, usually not every year. 
// When they come, their time is between April and June. They migrate from a far land.
// They come in 100-500 Locusts waves. Each one eats 0.01 inch of grain each Month.
int Locusts(){

	int temp_grain = NowHeight;
while( NowYear <= 2021 )
{
	// compute a temporary next-value for this quantity based on the current state of the simulation:

	if( !(NowMonth>6) && !(NowMonth<4) )
		LocustsIsHere = rand()%450 + 50;
	else
		LocustsIsHere = 0; 
	// DoneComputing barrier:
	#pragma omp barrier
	// DoneAssigning barrier:
	#pragma omp barrier
	// DonePrinting barrier:
	#pragma omp barrier
}
	return 0;
}



int Watcher(){

	int year = NowYear;
	int cnt =0;
	double celcius = 0.;
	double celcius_arr[72];
	int Locusts_arr[72];
	double Precip_arr[72];
	int Deer_arr[72];
	double Height_arr[72];

while( NowYear <= 2021 )
{
	// compute a temporary next-value for this quantity based on the current state of the simulation:

	// DoneComputing barrier:
	#pragma omp barrier

	// DoneAssigning barrier:
	#pragma omp barrier

	if(year != NowYear){
		printf("Year: %d:\n", NowYear);
		year = NowYear;
	}
	celcius = (NowTemp-32)/1.8; 
	printf("\tMonth: %d:\n", NowMonth);
	printf("\t\tNowHeight: %f\n", NowHeight);
	printf("\t\tNowDeer: %d\n", NowNumDeer);
	printf("\t\tNowPrecip: %f\n", NowPrecip);
	printf("\t\tNowTemp: %f \n", celcius);
	printf("\t\tLocusts Attacked?: %s \n", LocustsIsHere? "Yes":"No");


	celcius_arr[cnt] = celcius;
	Locusts_arr[cnt] =  LocustsIsHere/10;
	Precip_arr[cnt] = NowPrecip*2.54;
	Height_arr[cnt] = NowHeight*2.54;
	Deer_arr[cnt] = NowNumDeer;

	NowMonth++;
	if( (NowMonth%12) == 1){
		NowMonth = 1;
		NowYear++;
	}

	float ang = (  30.*(float)NowMonth + 15.  ) * ( M_PI / 180. );

	float temp = AVG_TEMP - AMP_TEMP * cos( ang );
	NowTemp = temp + Ranf( -RANDOM_TEMP, RANDOM_TEMP );

	float precip = AVG_PRECIP_PER_MONTH + AMP_PRECIP_PER_MONTH * sin( ang );
	NowPrecip = precip + Ranf( -RANDOM_PRECIP, RANDOM_PRECIP );
	if( NowPrecip < 0. )
			NowPrecip = 0.;

	cnt++;
	// DonePrinting barrier:
	#pragma omp barrier
}
	FILE *H = fopen("Height", "a+");
	FILE *D = fopen("Deer",   "a+");
	FILE *P = fopen("Precip", "a+");
	FILE *T = fopen("Temp",   "a+");
	FILE *L = fopen("Locusts","a+");
	for(int i=0;i<cnt;i++){
		fprintf(H,"%f\n", Height_arr[i]);
		fprintf(D,"%d\n", Deer_arr[i]);
		fprintf(P,"%f\n", Precip_arr[i]);
		fprintf(T,"%f\n", celcius_arr[i]);
		fprintf(L,"%d\n", Locusts_arr[i]);
	}
	fclose(H);
	fclose(D);
	fclose(P);
	fclose(T);
	return 0;
}
int main(){
	
	// starting date and time:
	NowMonth =    1;
	NowYear  = 2016;
	
	// starting state (feel free to change this if you want):
	NowNumDeer = 1;
	NowHeight =  1.;
	NowTemp =  50;
	LocustsIsHere =  0;

	srand(time(NULL));

	omp_set_num_threads( 4 );
	#pragma omp parallel sections
	{
		#pragma omp section
		{
			GrainDeer( );
		}

		#pragma omp section
		{
			Grain( );
		}

		#pragma omp section
		{
			Watcher( );
		}

		#pragma omp section
		{
			Locusts( );
		}

	}       // implied barrier -- all functions must return in order to allow any of them to get past here


	return 0;
}










float
Ranf( float low, float high )
{
        float r = (float) rand();               // 0 - RAND_MAX

        return(   low  +  r * ( high - low ) / (float)RAND_MAX   );
}

int
Rani( int ilow, int ihigh )
{
        float low = (float)ilow;
        float high = (float)ihigh + 0.9999f;

        return (int)(  Ranf(low,high) );
}

