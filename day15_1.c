#include<stdio.h>

#define MAXSTR 100

FILE *fptr;

void set_file(FILE *ptr)
{
  fptr = ptr;
}

int hash(char str[])
{
  int h = 0;
  for(int i = 0; str[i] != '\0' && str[i] != '\n'; i++)
  {
    h += (int) str[i];
    h *= 17;
    h %= 256;
  }
  return h;
}

int parse()
{
  int hash_sum = 0;
  int h, i;
  char buffer[MAXSTR];
  char c;
  i = 0;
  buffer[i] = '\0';

  while((c = fgetc(fptr)) != EOF)
  {
    if(c == '\0') continue;

    if(c == ',' || c == '\n')
    {
      h = hash(buffer);
      hash_sum += h;
      i = 0;
      buffer[i] = '\0';
    }
    else
    {
      buffer[i++] = c;
      buffer[i] = '\0';
    }
  }

  return hash_sum;
}

int main()
{
  char filename[] = "day15.txt";
  FILE *input = fopen(filename, "r");
  set_file(input);

  int hash_sum = parse();
  printf("hash sum: %d\n", hash_sum);
}
