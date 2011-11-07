#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "fourier.h"
#include "../../common/allow.h"

int fft_main(int argc, char *argv[]) {
	unsigned MAXSIZE;
	unsigned MAXWAVES;
	unsigned i,j;
	float *RealIn;
	float *ImagIn;
	float *RealOut;
	float *ImagOut;
	float *coeff;
	float *amp;
	int invfft=0;

	if (argc<3)
	{
		printf("Usage: fft <waves> <length> -i\n");
		printf("-i performs an inverse fft\n");
		printf("make <waves> random sinusoids");
		printf("<length> is the number of samples\n");
		exit(-1);
	}
	else if (argc==4)
		invfft = !strncmp(argv[3],"-i",2);
	MAXSIZE=atoi(argv[2]);
	MAXWAVES=atoi(argv[1]);
		
 srand(1);

 RealIn=(float*)mymalloc(sizeof(float)*MAXSIZE);
 ImagIn=(float*)mymalloc(sizeof(float)*MAXSIZE);
 RealOut=(float*)mymalloc(sizeof(float)*MAXSIZE);
 ImagOut=(float*)mymalloc(sizeof(float)*MAXSIZE);
 coeff=(float*)mymalloc(sizeof(float)*MAXWAVES);
 amp=(float*)mymalloc(sizeof(float)*MAXWAVES);

 /* Makes MAXWAVES waves of random amplitude and period */
	for(i=0;i<MAXWAVES;i++) 
	{
		coeff[i] = rand()%1000;
		amp[i] = rand()%1000;
	}
 for(i=0;i<MAXSIZE;i++) 
 {
   /*   RealIn[i]=rand();*/
	 RealIn[i]=0;
	 for(j=0;j<MAXWAVES;j++) 
	 {
		 /* randomly select sin or cos */
		 if (rand()%2)
		 {
		 		RealIn[i]+=coeff[j]*cos(amp[j]*i);
			}
		 else
		 {
		 	RealIn[i]+=coeff[j]*sin(amp[j]*i);
		 }
  	 ImagIn[i]=0;
	 }
 }

 /* regular*/
 ALLOW(RealIn,sizeof(float)*MAXSIZE,3LL);
 ALLOW(ImagIn,sizeof(float)*MAXSIZE,3LL);
 ALLOW(RealOut,sizeof(float)*MAXSIZE,3LL);
 ALLOW(ImagOut,sizeof(float)*MAXSIZE,3LL);
 ALLOW(coeff,sizeof(float)*MAXSIZE,3LL);
 ALLOW(amp,sizeof(float)*MAXSIZE,3LL);
 fft_float (MAXSIZE,invfft,RealIn,ImagIn,RealOut,ImagOut);

 #ifdef GICADEBUG
 printf("RealOut:\n");
 for (i=0;i<MAXSIZE;i++)
   printf("%f \t", RealOut[i]);
 printf("\n");

printf("ImagOut:\n");
 for (i=0;i<MAXSIZE;i++)
   printf("%f \t", ImagOut[i]);
   printf("\n");
 #endif

 free(RealIn);
 free(ImagIn);
 free(RealOut);
 free(ImagOut);
 free(coeff);
 free(amp);

 return 0;
}
