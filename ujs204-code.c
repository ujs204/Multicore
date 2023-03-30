#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>
#include <time.h>
#define TOTAL_CHARS 4




//gcc -fopenmp -Wall -std=c99 -o maxnum ujs204-code.c
//./maxnum 4 1000 filename.txt
//time ./maxnum 4 1000 filename.txt
//ssh crunchy1.cims.nyu.edu
//module load gcc-9.2



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
    //char c[count];
    char *c = NULL;
    c = malloc(count);
    //fscanf(fp, "%[^\n]", c);
    fread(c, sizeof(*c),count,fp);
    //printf("Data from the file:\n%s", c);
    
    printf("\n");
    fclose(fp);
    
    

    if(THREADS == 0){

        int _char[TOTAL_CHARS];
        //printf("Segmentation default is here");
        //int _char[4];

        for (i = 0; i < TOTAL_CHARS; ++i) {
            _char[i] = 0;
        }

        
        char ch;

        for (size_t i = 0; i < count; i++)
        {
            
            ch = c[i];
            ch = ch - 97;
            _char[ch]++;

        }

        printf("\n");  

        printf("a frequency %d \n",_char['a'-97]);
        printf("b frequency %d \n",_char['b'-97]);
        printf("c frequency %d \n",_char['c'-97]);
        printf("d frequency %d \n",_char['d'-97]);

        printf("\n");  

        int maxIndx = 0;

        for(int i = 1; i < TOTAL_CHARS; i++){
            if(_char[i] > _char[maxIndx]){
                maxIndx = i;
            }
        }

        char maxChar = (char) (maxIndx + 97);

        printf("%c occurred the most %d times of a total of %d characters \n", maxChar, _char[maxIndx],TOTAL_CHARS);
    }

    else if(THREADS == 4 || THREADS == 1){
        int remainder = count % THREADS;
        int step = (count / THREADS) + (remainder != 0);
        int hist[TOTAL_CHARS] = {0,0,0,0};
        
        omp_set_num_threads(THREADS);
        #pragma omp parallel shared(hist)
        {
            int id = omp_get_thread_num();
            int start = step * id;
            int end = start + step;
            int private_hist[TOTAL_CHARS];

            end = end > count ? count : end;

            for(int i = 0; i < TOTAL_CHARS; i++){
                private_hist[i] = 0;
            }

            for(int i = start; i < end; i++){
                //ch = c[i] - 97;
                private_hist[c[i]-97] += 1;
            }

            #pragma omp critical
            {
                for(int j = 0; j < TOTAL_CHARS; ++j){
                    hist[j] += private_hist[j];
                }
            }
            
            //printf("%d", step);
        }

        printf("a frequency %d \n",hist['a'-97]);
        printf("b frequency %d \n",hist['b'-97]);
        printf("c frequency %d \n",hist['c'-97]);
        printf("d frequency %d \n",hist['d'-97]);

        printf("\n");
        
        int maxIndx = 0;

        for(int i = 1; i < TOTAL_CHARS; i++){
            if(hist[i] > hist[maxIndx]){
                maxIndx = i;
            }
        }

        char maxChar = (char) (maxIndx + 97);

        printf("%c occurred the most %d times of a total of %d characters \n", maxChar, hist[maxIndx],TOTAL_CHARS);
        

    }

    free(c);

    
    return 0;
    
}