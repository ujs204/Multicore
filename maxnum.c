#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>
#include <time.h> 
#define TOTAL_CHARS 4




//gcc -fopenmp -Wall -std=c99 -o maxnum maxnum.c
//./maxnum 4 3 filename.txt


int main(int argc, char *argv[]){


    int count = 0;
    FILE * fp;
	char x;
    int THREADS;

    
    int i;
    printf("%d\n",argc);
    for(i=1;i<argc;i++)
    {
        printf("%s \n",argv[i]);
    }

    THREADS = atoi(argv[1]);

    if( (fp = fopen(argv[3],"w+t")) == NULL )
    {
	printf("Cannot create the file %s\n", argv[3]);
	exit(1);
    }

    count = atoi(argv[2]);

    srand((unsigned int)time(NULL));

    for (int i=0;i<count;i++){
		x = 97 + ((float)rand()/(float)(RAND_MAX)) * 4;
		fprintf(fp, "%c", x);
	}
    
    printf("Generated %d random characters and stored in file %s\n", count, argv[3]);
    fclose(fp);
    
    fp = fopen(argv[3],"r");
    
    if (NULL == fp) {
        printf("file can't be opened \n");
    }
    
    char c[count];
    fscanf(fp, "%[^\n]", c);
    printf("Data from the file:\n%s", c);
    
    printf("\n");
    fclose(fp);
    
    
    //printf("\n %d",remainder);
    //printf("\n");


    //Sequential Algorithm

    int _char[TOTAL_CHARS];
    //int _char[4];

    for (i = 0; i < TOTAL_CHARS; ++i) {
        _char[i] = 0;
    }

    
    char ch;

    for (size_t i = 0; i < count; i++)
    {
        
        //printf("%c \n", c[i]);
        ch = c[i];
        ch = ch - 97;
        //printf("%d \n",ch);
        _char[ch]++;

    }

    printf("\n");  

    printf("a frequency %d \n",_char['a'-97]);
    printf("b frequency %d \n",_char['b'-97]);
    printf("c frequency %d \n",_char['c'-97]);
    printf("d frequency %d \n",_char['d'-97]);

    printf("\n");  

    //4 threads

    //printf("%d \n", THREADS);

    if(THREADS == 4){
        int remainder = count % 4;
        int step;

        if (count < 4){
            step = 1;
        }
        else{
            step = count/4;
        }

        int hist[TOTAL_CHARS] = {0};

        #pragma omp parallel shared(count)
        {
            int id = omp_get_thread_num();
            int start = step * id;
            int end = start + step;
            int private_hist[TOTAL_CHARS];

            end = end > count ? count : end;

            for(int i = 0; i < TOTAL_CHARS; i++){
                private_hist[i] = 0;
            }

            for(i = start; i < end; i++){
                ch = c[i] - 97;
                private_hist[ch]++;
            }

            #pragma omp critical
            {
                for(int j = 0; j < TOTAL_CHARS; j++){
                    hist[j] += private_hist[j];
                }
            }
            
            printf("%d", step);
        }

        printf("a frequency %d \n",hist['a'-97]);
        printf("b frequency %d \n",hist['b'-97]);
        printf("c frequency %d \n",hist['c'-97]);
        printf("d frequency %d \n",hist['d'-97]);

        printf("\n");
        //printf("hello \n");
        
        /*
        int* hist_private[THREADS];

        int t0[TOTAL_CHARS] = {0,0,0,0};
        int t1[TOTAL_CHARS] = {0,0,0,0};
        int t2[TOTAL_CHARS] = {0,0,0,0};
        int t3[TOTAL_CHARS] = {0,0,0,0};

        
        hist_private[0] = t0;
        hist_private[1] = t1;
        hist_private[2] = t2;
        hist_private[3] = t3;

        
        
        //omp_set_num_threads(THREADS);
        
        
        //int id = omp_get_thread_num();
        # pragma omp parallel for num_threads(THREADS)
        for(int i = 0; i < count; i++){
            int id = omp_get_thread_num();
            ch = c[i] - 97;
            printf("value in 2d array before for character %c and thread id %d is %d \n", c[i],id,hist_private[id][ch]);
            hist_private[id][ch] += 1;
            printf("value in 2d array AFTER for character %c and thread id %d is %d \n", c[i],id,hist_private[id][ch]);
        }

        int hist[TOTAL_CHARS] = {0};

            
        for(int col = 0; col < TOTAL_CHARS; col++){
            int sum = 0;
            for(int row = 0; row < THREADS; row++){
                printf("here is the value of the private histogram for character  %d: %d \n", col, hist_private[row][col]);
                sum += hist_private[row][col];
            }
            hist[col] = sum;
            printf("%d \n", sum);
        }
        
        

        
        

        printf("\n");  

        printf("a frequency %d \n",hist['a'-97]);
        printf("b frequency %d \n",hist['b'-97]);
        printf("c frequency %d \n",hist['c'-97]);
        printf("d frequency %d \n",hist['d'-97]);

        printf("\n");
        

        

    */

    }

    
    return 0;
    
}