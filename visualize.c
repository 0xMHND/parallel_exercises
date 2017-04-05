#define _BSD_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

//config filename
#define CONFIG "visualization.cfg"
#define BUF_SIZE 1000

int plot_data()
{

	char buf[BUF_SIZE];
	//Open the config file that holds chart options
	FILE * cfg = fopen(CONFIG, "r");
	if (cfg == NULL) {
		fprintf(stderr,"Could not open %s: %s.\n", CONFIG, strerror(errno));
		return 1;
	}

	//Open the GNUplot program and get it ready to feed it instructions
	FILE * gnuplotPipe = popen("gnuplot", "w");
	if (gnuplotPipe == NULL) {
		fprintf(stderr,"Could not open GNUplot.\n");
		return 1;
	}

	//This loops goes through every line of the config file
	//and feeds them into GNUplot
	for (int i = 0; !feof(cfg); i++) {
		memset(buf, '\0', BUF_SIZE);    //clear buffer

		//get next line into the buffer
		if (fgets(buf, BUF_SIZE, cfg) == NULL && !feof(cfg)) {
			    fprintf(stderr, "Could not read line %d in file %s", i, CONFIG);
			    return 1;
		}

		//if end of file is reached, break out of the loop
		if (feof(cfg))
			    break;

		//send command to GNUplot
		fprintf(gnuplotPipe, "%s\n", buf);
	}

	fprintf(gnuplotPipe, "%s\n", "plot \'NON_SIMD_max\' w linesp ls 1 title \"max-NON_SIMD\", \
				           \'NON_SIMD_avg\' w linesp ls 5 title \"avg-NON_SIMD\", \
				           \'NON_SIMD_RED_max\' w linesp ls 2 title \"max-NON_SIMD_RED\", \
				           \'NON_SIMD_RED_avg\' w linesp ls 6 title \"avg-NON_SIMD_RED\", \
				           \'SIMD_max\' w linesp ls 3 title \"max-SIMD\", \
				           \'SIMD_avg\' w linesp ls 7 title \"avg-SIMD\", \
					   \'SIMD_RED_max\' w linesp ls 4 title \"max-SIMD_RED\", \
					   \'SIMD_RED_avg\' w linesp ls 8 title \"avg-SIMD_RED\" ");

	//close GNUplot pipe
	if (pclose(gnuplotPipe) == -1) {
		fprintf(stderr, "Could not close pipe to GNUplot");
	}
	return 0;
}

int main(){

	plot_data();
	return 1;
}
