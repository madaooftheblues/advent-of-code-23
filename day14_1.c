#include<stdio.h>

#define MAXROWS 100
#define MAXCOLS 102

FILE *fptr;

void set_file(FILE *ptr)
{
  fptr = ptr;
}

int strlen_(char str[])
{
  int i = 0;
  while(str[i++] != '\0');
  return i;
}

void copy(char to[], char from[])
{
  for(int i = 0; (to[i] = from[i]) !='\0'; i++);
}

void print_matrix(char m[][MAXCOLS], int rc)
{
  for(int i = 0; i < rc; i++)
  {
    printf("%s", m[i]);
  }
}

int parse(char m[][MAXCOLS])
{
  char *line;
  ssize_t linelen = 0;
  size_t buffsize = 0;
  int row_count = 0;

  while((linelen = getline(&line, &buffsize, fptr)) != -1)
  {
    copy(m[row_count++], line);
  }
  return row_count;
}

int fist_of_the_north_star(char m[][MAXCOLS], int row_count)
{
  int load = 0;
  for(int j = 0; m[0][j] != '\0'; j++)
  {
    for(int i = 0; i < row_count; i++)
    {
      if(m[i][j] == 'O')
      {
        m[i][j] = '.';
        int x = i;
        while(x >= 0 && m[x][j] != 'O' && m[x][j] != '#')
        {
          x--;
        }
        m[x + 1][j] = 'O';
        load += row_count - (x + 1);
      }
    }
  }
  return load;
}

int main()
{
  char filename[] = "day14.txt";
  FILE *input = fopen(filename, "r");
  set_file(input);
  
  char m[MAXROWS][MAXCOLS];
  int row_count, load;
  row_count = load = 0;
  row_count = parse(m);
  print_matrix(m, row_count);
  load = fist_of_the_north_star(m, row_count);
  printf("\n");
  print_matrix(m, row_count);
  printf("load: %d\n", load);
}
