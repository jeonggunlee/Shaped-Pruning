#include <stdio.h>
#include <time.h>

#define CH 309
#define N 8

int main(void)
{

   int i, j, k;
   int fi, fj;

   double filter[3][3][CH];
   double vectored_filter[3*CH];
   double ifmap[N][N][CH];
   double vec_ifmap[N][N][3*CH];
   double ofmap[N-2][N-2];

   clock_t start,end;
   
   // ifmap & filter initialization
   // filter initialization
   for(i = 0; i < 3; i++)
   {
      for(j = 0; j < 3; j++)
         for(k = 0; k < CH; k++) {
            filter[i][j][k] = i*j*k;
         }
   }   
   // ifmap initialization
   for(i = 0; i < N; i++)
   {
      for(j = 0; j < N; j++)
         for(k = 0; k < CH; k++) {
            ifmap[i][j][k] = i*j*k;
         }
   }   

   // Memory Rearrangement
   // For Filter
   int boundary = 279/3;

   for(i = 0; i < 3; i++)
   {
      for(j = 0; j < 3; j++)
         for(k = boundary*j; k < boundary*(j+1); k++) {
            vectored_filter[CH*i+k] = filter[i][j][k];
            // vectored_filter shape = [3*CH]
         }
   }

   // For input feature map : Just one time for all the filters
   start=clock();
   for(i = 0; i < N-2; i++)
   {
      for(j = 0; j < N-2; j++)
         for(fi = 0; fi < 3; fi++)
            for(fj = 0; fj < 3; fj++)
               for(k = boundary*fj; k < boundary*(fj+1); k++)
                  vec_ifmap[i][j][3*fi+k] = ifmap[i+fi][j+fj][k];
                  // vec_ifmap shape = [N-2, N-2, 3*CH]
   }
   end=clock();
   double extime=(double) (end-start)/CLOCKS_PER_SEC;
   printf("\n\tExecution time for the memory rearrangement is %f seconds\n ",extime);

   // Performance of Convolution Filter Processing with Shaped Pruning Approach
   start=clock();
   for(i = 0; i < N-2; i++)
   {
      for(j = 0; j < N-2; j++)
         ofmap[i][j] = 0.0;
         for(k = 0; k < 3*CH; k++)
            ofmap[i][j] = ofmap[i][j] + vec_ifmap[i][j][k]*vectored_filter[k];
   }
   end=clock();
   extime=(double) (end-start)/CLOCKS_PER_SEC;
   printf("\n\tExecution time for the Proposed Conv is %f seconds\n ",extime);


   /////////////////////////////////////////////////////////////////////////////////////
   // Performance of Typical Convolution Filter Processing
   /////////////////////////////////////////////////////////////////////////////////////
   start=clock();
   for(i = 0; i < N-2; i++)
   {
      for(j = 0; j < N-2; j++)
         ofmap[i][j] = 0.0;
         for(fi = 0; fi < 3; fi++)
            for(fj = 0; fj < 3; fj++)
               for(k = 0; k < CH; k++)
                  ofmap[i][j] = ofmap[i][j] + ifmap[i+fi][j+fj][k]*filter[fi][fj][k];
   }
   end=clock();
   extime=(double) (end-start)/CLOCKS_PER_SEC;
   printf("\n\tExecution time for the Typical Conv is %f seconds\n ",extime);

   return 0;
}

