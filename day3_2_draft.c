#include <stdio.h>
#include <stdlib.h>
#include<ctype.h>
#include<string.h>
#define MAX_ROWS 142
#define MAX_COLS 143
#define MAX_DIG 3

enum Pos {
  TL,
  T,
  TR,
  BL,
  B,
  BR,
  N
};

struct location {
  enum Pos above;
  enum Pos below;
};

enum Pos above_pos_all[] = {TL, T, TR};
enum Pos below_pos_all[] = {BL, B, BR};

struct coords {
  int x;
  int y;
};

char symbols[] = "+-*/@#$%&=";

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


struct location* is_gear(int i, int j, char mat[][MAX_COLS])
{
  char above[] = {mat[i-1][j-1], mat[i-1][j], mat[i-1][j+1], '\0'};
  char below[] = {mat[i+1][j-1], mat[i+1][j], mat[i+1][j+1], '\0'};

  enum Pos above_pos = N;
  enum Pos below_pos = N;

  struct location *l = NULL;

  for(int k=0; above[k] != '\0'; k++)
  {
    char c1 = above[k];
    char c2 = below[k];

    if(above_pos == N && isdigit(c1))
      above_pos = above_pos_all[k];

    if(below_pos == N && isdigit(c2))
      below_pos = below_pos_all[k];

    if(above_pos != N && below_pos != N)
    {
      l = (struct location *) malloc(sizeof (struct location));
      l->above = above_pos;
      l->below = below_pos;
      return l;
    }
  }

  return l;
}

int slide(int i, int j, char arr2D[][MAX_COLS])
{
  char str[4];
  int l, r;
  l = r = 0;
  str[0] = '\0';
  int count = 0;
  while(count < MAX_DIG)
  {
    if(isdigit(arr2D[i][j + l])) l--;
    if(isdigit(arr2D[i][j + r])) r++;
    count++;
  }
  int k = l;
  count = 0;
  while(k < r)
  {
    str[count] = arr2D[i][j + k]; 
    str[++count] = '\0';
    k++;
  }

  int number = str_to_num(str);
  return number;
}



int main() {
    FILE *file;
    char filename[] = "day3.txt"; // Replace with your file name
    char arr2D[MAX_ROWS][MAX_COLS]; // 2D array to hold file contents
    long sum = 0;

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


    for(int i=1; i < rows; i++)
    {
      for(int j=1; arr2D[i][j] != '^'; j++)
      {
        char c = arr2D[i][j];

        if(c != '*') continue;

        struct location *l = NULL;
        l = is_gear(i, j, arr2D);

        if(l == NULL) continue;

        enum Pos above_pos;
        enum Pos below_pos;

        above_pos = l -> above;
        below_pos = l -> below;
        printf("pos: %d, %d\n", above_pos, below_pos);

        struct coords *ac = (struct coords *) malloc(sizeof(struct coords));
        struct coords *bc = (struct coords *) malloc(sizeof(struct coords));

        ac -> x = i - 1;
        bc -> x = i + 1;

        switch(above_pos)
        {
          case T:
            ac -> y = j;
            break;
          case TL:
            ac -> y = j - 1;
            break;
          case TR:
            ac -> y = j + 1;
            break;
        }
        switch(below_pos)
        {
          case B:
            bc -> y = j;
            break;
          case BL:
            bc -> y = j - 1;
            break;
          case BR:
            bc -> y = j + 1;
            break;
        }

        int num1 = slide(ac -> x, ac -> y, arr2D);
        int num2 = slide(bc -> x, bc -> y, arr2D);
        printf("numbers: %d, %d\n", num1, num2);
        long ratio = num1 * num2;
        printf("ratio: %ld\n", ratio);
        sum += ratio;
        printf("sum: %ld\n\n", sum);
      }
  
    }
    printf("%ld\n", sum);
    return 0;
}

