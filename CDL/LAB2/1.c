#include <stdio.h>
#include <stdlib.h>
 
int main()
{
    int flag=0;
    char fileName[100],fileName1[100];
    char c;
    FILE *f1,*f2;
    //f1 = fopen("sample_file.c", "r");
  //f2 = fopen("outputl2q2.c", "w");

  printf("\nEnter filename to open for reading :\n");
  scanf("%s", fileName);
  f1 = fopen(fileName, "r");
  printf("\nEnter filename to open for output :\n");
  scanf("%s", fileName1);
  f2 = fopen(fileName1, "w");
 
  if(f1 == NULL || f2 == NULL)
  {
    perror("Either the input or the output file doesn't exist\n");
    return 1;
  }
 
  while(1)
  {
    c = fgetc(f1);
 
    if(c==EOF)
    {
      break;
    }
 
    else if(!flag && (c==' '||c=='\t'))
    {
      fputc(' ', f2);
      flag = 1;
    }
 
    else if(!(c==' '||c=='\t'))
    {
      flag = 0;
      fputc(c, f2);
    }
  }
 
  fclose(f1);
  fclose(f2);
}