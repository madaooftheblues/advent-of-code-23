#include <stdio.h>
#include <stdlib.h>
#include<ctype.h>
#include<string.h>
#define MAX_ROWS 142
#define MAX_COLS 143

int state = 0;
char symbols[] = "+-*/@#$%&=";

char start(char c)
{
  if(isdigit(c))
  {
    state = 1;
    return c;
  }
  return '\0';
}

char num(char c)
{
  if(isdigit(c))
  {
    state = 1;
    return c;
  }
  else state = 2;
  return '\0';
}

char skip_till_dig(char c)
{
  if(isdigit(c))
  {
    state = 1;
    return c;
  }
  return '\0';
}

int is_symbol(char c)
{
  if(c == '.') return 0;
  else if(c == '^') return 0;
  else if (isdigit(c)) return 0;
  else if (c == '\0') return 0;
  else if (c == '\n') return 0;
  return 1;
}

int is_adjacent(int i, int j, char mat[][MAX_COLS])
{
  char neighbours[] = {mat[i-1][j], mat[i+1][j], mat[i][j-1], mat[i][j+1], mat[i-1][j-1], mat[i-1][j+1], mat[i+1][j-1], mat[i+1][j+1]};
  printf("\n%s\n", neighbours);
  for(int k=0; neighbours[k] != '\0'; k++)
  {
    if(is_symbol(neighbours[k]))
    {
      // printf("%c\n", mat[i][j]);
      return 1;
    }
  }

  return 0;
}

int str_to_num(char str[])
{
  int num = 0;
  int mul = 1;
  for(int i= strlen(str) - 1; i >= 0 ; i--)
  {
    if(isdigit(str[i]))
    {
      num = ((str[i] - '0') * mul) + num;
      mul *= 10;
    }
  }
  return num;
}


int main() {
    FILE *file;
    char filename[] = "day3.txt"; // Replace with your file name
    char arr2D[MAX_ROWS][MAX_COLS]; // 2D array to hold file contents
    for(int i=0; i < MAX_ROWS; i++)
    {
      for(int j=0; j < MAX_COLS; j++)
      {
        arr2D[i][j] = '^';
      }
    }
    int rows = 1, cols = 1;

    file = fopen(filename, "r");
    if (file == NULL) {
        printf("Could not open file %s\n", filename);
        return 1;
    }

    // Read characters from the file
    int c;
    while ((c = fgetc(file)) != EOF) {
        if (c == '\n') {
  //          printf("\n");
            rows++;
            cols = 1;
            continue;
        }
  //         printf("%c", c);
        arr2D[rows][cols++] = c;
    }

    fclose(file);

    long sum = 0;
    char number[3];
    int idx = 0;
    int valid = 0;
    for(int i=1; i < rows; i++)
    {
      for(int j=1; arr2D[i][j] != '^'; j++)
      {
        char c = arr2D[i][j];
        switch(state)
        {
          case 0:
            number[idx] = start(c);
            if(number[idx] != '\0') 
            {
              if(is_adjacent(i, j, arr2D)) valid = 1;
              number[++idx] = '\0';
            }
            break;
          case 1:
            number[idx] = num(c);
            if(number[idx] != '\0') 
            {
              if(is_adjacent(i, j, arr2D)) valid = 1;
              number[++idx] = '\0';
            }
            break;
          case 2:
            if(valid)
            {
              int n = str_to_num(number);
              printf("%d\n", n);
              sum += n;
            }
            valid = 0;
            idx = 0;
            number[idx] = skip_till_dig(c);
            if(number[idx] != '\0') 
            {
              if(is_adjacent(i, j, arr2D)) valid = 1;
              number[++idx] = '\0';
            }
            break;
        }
      }
  
    }
    printf("%ld\n", sum);
    return 0;
}

