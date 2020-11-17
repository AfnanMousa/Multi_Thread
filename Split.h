#include <string.h>
void ToowerCase(char* Str)   //convert all string to lower case..
{
   for(int i = 0; i<strlen(Str); i++)
    {
      Str[i] = tolower(Str[i]);
    }
}
int split(FILE* In_file,char * str)
{
    char Row_Size[100];
    fscanf(In_file, "%s", & Row_Size ) ;
    //ToLowerCase(Row_Size);///
    int n=0;
    if(CheckExistElement(Row_Size))
    {
        char *row = strtok(Row_Size, "=");//printf("%s \n",row);
        if(!strcmp(row, str))
        {
            row = strtok(NULL, "=");
            if(CheckInteger(row))
            {
                //printf(row);
                n = atoi(row);

            }
            else{
                 printf("oops, the number of '%s'  not integer \n",str);
                 exit(-1);
            }

        }
        else{
            printf("oops, the string not contain word '%s' which not equivalent the form in lab\n",str);
            exit(-1);
        }
    }
    else{
        printf("oops, the string not contain '=' which not equivalent the form in lab\n");
        exit(-1);
    }
    return n;
}


void SetValueOfMatrix(FILE* In_file,int** Matrixes,int n,int m)
{
    int row =n;// sizeof(matrix) / sizeof(matrix[0]);
    int column =m;// sizeof(matrix[0])/sizeof(matrix[0][0]);

    for(int i=0;i<row;i++)
    {
        char  num[100];
        for(int j=0;j<column;j++)
        {
            fscanf(In_file, "%s", &num ) ;
            if(CheckInteger(num))
            {
                //printf("%d\n",atoi(num));
                 Matrixes[i][j] = atoi(num);
            }
            else
            {
               printf("oops, the string in index i = %d ,j = %d not integer number \n",i,j);
               exit(-1);
            }
            //fscanf(In_file, "%d", &Matrixes[i][j] ) ;

        }

    }
    //return matrix;
}
