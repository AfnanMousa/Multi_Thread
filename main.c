#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "ReadInput.h"

int FirstEnter=0;

struct thread_data{
 struct matrix matInput1;
 struct matrix matInput2;
 int x;
 int column;
};
struct Result
{
    int rows;
    int cols;
    int data [1000][1000];
}OutputMatrix;
struct timeval stop, start;
void *myThreadFunForRow(void *vargp)
{
	struct thread_data *my_data;
    my_data = (struct thread_data *) vargp;
    int tid = my_data->x;
    struct matrix A=my_data->matInput1;
    struct matrix B=my_data->matInput2;

    OutputMatrix.rows=A.rows;
    OutputMatrix.cols=B.cols;

    /* OutputMatrix.data= (int **)malloc(A.rows * sizeof(int *));
    for (int i=0; i<A.rows; i++)
         OutputMatrix.data[i] = (int *)malloc(B.cols * sizeof(int));*/


	for(int z=0;z<B.cols;z++)
    {
        long long sum=0;
        for(int j=0;j<A.cols;j++)
        {
             sum+=A.data[tid][j]*B.data[j][z];
        }

        OutputMatrix.data[tid][z]=sum;
        //printf("c[%d][%d]= %d\n",tid,z,sum);

    }
    //printf("Thread ID: %d \n",tid);
    pthread_exit(NULL);



}


void *myThreadFunForElement(void *vargp)
{
	struct thread_data *my_data;
    my_data = (struct thread_data *) vargp;
    int tid = my_data->x;
    int z = my_data->column;
    struct matrix A=my_data->matInput1;
    struct matrix B=my_data->matInput2;

	long long sum=0;
    for(int j=0;j<A.cols;j++)
    {
       sum+=A.data[tid][j]*B.data[j][z];
    }

    OutputMatrix.data[tid][z]=sum;
    //printf("c[%d][%d]= %d\n",tid,z,sum);

	//printf("Thread ID: %d \n",tid);
	pthread_exit(NULL);
}

void ControlRows(struct matrix temp1,struct matrix temp2,char *FileNames)
{
    gettimeofday(&start, NULL); //start checking time
    pthread_t tid[temp1.rows];
    int i;

	// Let us create three threads
	int rc ;
	for ( i = 0; i < temp1.rows; i++)
    {
        struct thread_data *tdata = malloc(sizeof(struct thread_data));
        tdata->x=i;
        tdata->matInput1=temp1;
        tdata->matInput2=temp2;
        rc=pthread_create(&tid[i], NULL, myThreadFunForRow, (void *) tdata);
        if (rc)
        {
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }
    printf("The total number of threads = %d\n",temp1.rows);
    for(int t=0; t<temp1.rows; t++){
      pthread_join(tid[t], NULL);
      printf("Main: completed join with thread %d\n",t);
    }
    gettimeofday(&stop, NULL); //end checking time
    printf("Seconds taken %lu\n", stop.tv_sec - start.tv_sec);
    printf("Microseconds taken: %lu\n", stop.tv_usec - start.tv_usec);
    WriteTime(FileNames,stop.tv_sec - start.tv_sec,"Second");
    WriteTime(FileNames,stop.tv_usec - start.tv_usec,"Microseconds");

}
void ControlElements(struct matrix temp1,struct matrix temp2,char *FileNames)
{
    gettimeofday(&start, NULL); //start checking time
    pthread_t tid[temp1.rows*temp2.cols];
    int i;

	// Let us create three threads
	int rc ;
	int j=0;
	for ( i = 0; i < temp1.rows*temp2.cols; i++)
    {
        struct thread_data *tdata = malloc(sizeof(struct thread_data));
        tdata->x=i%temp1.rows;
        tdata->matInput1=temp1;
        tdata->matInput2=temp2;
        tdata->column=j%temp2.cols;
        j++;
        rc=pthread_create(&tid[i], NULL, myThreadFunForElement, (void *) tdata);
        if (rc)
        {
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }
    printf("The total number of threads = %d\n",temp1.rows*temp2.cols);
    for(int t=0; t<temp1.rows*temp2.cols; t++){
      pthread_join(tid[t], NULL);
      printf("Main: completed join with thread %d\n",t);
    }

    gettimeofday(&stop, NULL); //end checking time
    printf("Seconds taken %lu\n", stop.tv_sec - start.tv_sec);
    printf("Microseconds taken: %lu\n", stop.tv_usec - start.tv_usec);
    WriteTime(FileNames,stop.tv_sec - start.tv_sec,"Second");
    WriteTime(FileNames,stop.tv_usec - start.tv_usec,"Microseconds");

}

void WriteINOutputFile(char * FileName)
{
   FILE * log;
   if(FirstEnter==0)
   {
       log = fopen (FileName,"w");
       FirstEnter++;

   }
   else
   {
       log = fopen (FileName,"a+");
   }
   if (log == NULL) {
        printf("Error in the opening of file!");
        exit(1);
    }
    else
    {
       for(int i=0;i<OutputMatrix.rows;i++)
       {
           for(int j=0;j<OutputMatrix.cols;j++)
           {
               fprintf (log, "%d  ",OutputMatrix.data[i][j]);
           }
           fprintf(log,"\n");
       }
       fclose(log);
    }

}

void WriteTime(char * FileName,long tim,char *typetime)
{
   FILE * log;
   if(FirstEnter==0)
   {
       log = fopen (FileName,"w");
       FirstEnter++;

   }
   else
   {
       log = fopen (FileName,"a+");
   }
   //log = fopen (FileName,"a+");
   if (log == NULL) {
        printf("Error in the opening of file!");
        exit(1);
    }
    else
    {
        fprintf (log,"%s taken %lu\n",typetime,tim);
        fclose(log);
    }

}

int main(int args, char* argv[])
{
    char *InputOne;//"A.txt";
    char *InputSecond;//="B.txt";
    char *OutputOne;//="C.txt";
    if(args==1)
    {

        InputOne="A.txt";
        InputSecond="B.txt";
        OutputOne="C.txt";
    }
    else{
        InputOne=argv[1];
        InputSecond=argv[2];
        OutputOne=argv[3];
        /*strcpy(InputOne,argv[1]);
        strcpy(InputSecond,argv[2]);
        strcpy(OutputOne,argv[3]);*/
    }
    struct matrix temp1=ReadFromFile(InputOne);//argv[1]);//"A.txt");
    struct matrix temp2=ReadFromFile(InputSecond);//argv[2]);//"B.txt");
    /*for(int r=0;r<temp1.rows;r++)
    {
        for(int e=0;e<temp1.cols;e++)
        {
            printf("%d ",temp1.data[r][e]);
        }
        printf("\n");
    }
    for(int r=0;r<temp2.rows;r++)
    {
        for(int e=0;e<temp2.cols;e++)
        {
            printf("%d ",temp2.data[r][e]);
        }
        printf("\n");
    }
    printf("%d ,%d ",temp1.rows,temp2.cols);*/
    if(temp1.cols!=temp2.rows)
    {
      printf("oops, We can't multiply because the number of columns in the first matrix not equal the number of rows in the second matrix\n");
        exit(-1);
    }
	ControlRows(temp1,temp2,OutputOne);

    	for(int r=0;r<temp1.rows;r++)
    {
        for(int e=0;e<temp2.cols;e++)
        {
            printf("%d ",OutputMatrix.data[r][e]);
        }
        printf("\n");
    }
    WriteINOutputFile(OutputOne);

    ControlElements(temp1,temp2,OutputOne);
    for(int r=0;r<temp1.rows;r++)
    {
        for(int e=0;e<temp2.cols;e++)
        {
            printf("%d ",OutputMatrix.data[r][e]);
        }
        printf("\n");
    }
    WriteINOutputFile(OutputOne);
	return 0;
}
