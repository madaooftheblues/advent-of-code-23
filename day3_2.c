#include<stdio.h>
#include<string.h>
#include<ctype.h>
#include<stdlib.h>

#define MAXCOLS 140
#define MAXROWS 140
#define MAXDIGS 3

struct coords {
  int x;
  int y;
};

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

int main()
{
  char file_name[] = "day3.txt";
  FILE *fptr = fopen(file_name, "r");
  long sum = 0;

  char matrix[MAXROWS][MAXCOLS];

  int rows, cols;
  rows = cols = 0;
  char c;

  while((c = fgetc(fptr)) != EOF && rows < MAXROWS)
  {
    if(c == '\n')
    {
      rows++;
      cols = 0;
      continue;
    }
    matrix[rows][cols++] = c;
  }

  for(int i=0; i < rows; i++)
  {
    for(int j=0; j < MAXCOLS; j++)
    {
      c = matrix[i][j];
      if(c != '*') continue;
      struct coords *p1 = (struct coords *)malloc(sizeof(struct coords));
      struct coords *p2 = (struct coords *)malloc(sizeof(struct coords));
      int adjn = 0;
      for(int x= i - 1; x <= i + 1; x++)
      {
        for(int y= j - 1; y <= j + 1; y++)
        {
          if(adjn > 1) break;
          if(x < 0 || x >= rows || y < 0 || y >= strlen(matrix[x]) || !isdigit(matrix[x][y])) continue;
          int root = y;
          while(root > 0 && isdigit(matrix[x][root - 1]))
          {
            root--;
          }
          if((p1->x == x && p1->y == root) || (p2->x == x && p2->y == root)) continue;
          if(adjn == 0)
          {
            p1->x = x;
            p1->y = root;
          }
          else if(adjn == 1)
          {
            p2->x = x;
            p2->y = root;
          }

          adjn++;
         // printf("(%d, %d):%c\n", x, root, matrix[x][root]);

        }
      }

      if(adjn != 2) continue;
      char charnum_1[4];
      int num_1;
      char charnum_2[4];
      int num_2;
      for(int r = 0; r < MAXDIGS && isdigit(matrix[p1->x][p1->y + r]); r++)
      {
        charnum_1[r] = matrix[p1->x][p1->y + r];
        charnum_1[r + 1] = '\0';
      }
      for(int r = 0; r < MAXDIGS && isdigit(matrix[p2->x][p2->y + r]); r++)
      {
        charnum_2[r] = matrix[p2->x][p2->y + r];
        charnum_2[r + 1] = '\0';
      }

      num_1 = str_to_num(charnum_1);
      num_2 = str_to_num(charnum_2);

      int ratio = num_1 * num_2;
      sum += ratio;
      // printf("c: %s, %s\n", charnum_1, charnum_2);
      // printf("n: %d, %d\n", num_1, num_2);

      printf("p1 (%d, %d):%c\n", p1->x, p1->y, matrix[p1->x][p1->y]);
      printf("p2 (%d, %d):%c\n", p2->x, p2->y, matrix[p2->x][p2->y]);
      printf("\n");
    }
  }
  printf("%ld\n", sum);
}
