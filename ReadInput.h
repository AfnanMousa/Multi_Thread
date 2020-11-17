#include "Split.h"
struct matrix {
    int rows, cols;
    int **data; // packed representation, or int **data;
};
int CheckExistElement(char *str)  ///if contain '='
{
  int exclamationCheck = 0;
  if(strchr(str, '=') != NULL)
  {
    exclamationCheck = 1;
  }
  return exclamationCheck;
}

int CheckInteger(char *str) //if number
{
   int number =1 ;
   int i =0;
  if(str[0]=='-')
  {
     // printf("lkjhg");
      i=1;
  }
  for (i; str[i] != '\0'; i++)
    {
        // check for decimal digits
        if (isdigit(str[i]) == 0)
            number=0;
    }
  return number;
}


struct matrix ReadFromFile(char *NameFile)
{


    FILE* In_file = fopen(NameFile, "r"); // read only

      if (In_file==NULL )
      {
        printf("oops, file can't be read\n");
        exit(-1);
      }
      else{
        int n=split(In_file,"row");
        int m=split(In_file,"col");

        int ** TempMatrix = (int**)malloc(sizeof(int*)*n);   //https://stackoverflow.com/questions/3414827/how-to-pass-a-2d-dynamically-allocated-array-to-a-function
        for(int i=0; i<n; i++)
        *(TempMatrix+i) = (int*)malloc(sizeof(int)*m);

        SetValueOfMatrix(In_file,TempMatrix,n,m);
        struct matrix matInput={n,m,TempMatrix};

        /*for (int i=0; i<n; i++) {
            free(TempMatrix[i]);
        }
        free(TempMatrix);*/

        return matInput;
      }
}

