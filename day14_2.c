// find the repeating cycle. 
// starts from 80th iteration => 93920
// lasts for 51 cycles
// 51 * 19607841 = 999,999,891 + 80 = 999,999,971
// 80 + 29 - 1 = the number => 93742
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

void print_matrix(char m[][MAXCOLS], int rc)
{
  for(int i = 0; i < rc; i++)
  {
    printf("%s", m[i]);
  }
}

int parse(char m[][MAXCOLS])
{
  char *line = NULL;
  ssize_t linelen = 0;
  size_t buffsize = 0;
  int row_count = 0;

  while((linelen = getline(&line, &buffsize, fptr)) != -1)
  {
    int i;
    for(i = 0; i < MAXCOLS && line[i] != '\0'; i++)
    {
      m[row_count][i] = line[i];
    }
    m[row_count][i] = '\0';
    row_count++;
  }
  return row_count;
}

int north(char m[][MAXCOLS], int row_count)
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

int south(char m[][MAXCOLS], int row_count)
{
  for(int j = 0; m[0][j] != '\0'; j++)
  {
    for(int i = row_count - 1; i >= 0; i--)
    {
      if(m[i][j] == 'O')
      {
        m[i][j] = '.';
        int x = i;
        while(x < row_count && m[x][j] != 'O' && m[x][j] != '#')
        {
          x++;
        }
        m[x - 1][j] = 'O';
      }
    }
  }
}

int west(char m[][MAXCOLS], int row_count)
{
  for(int i = 0; i < row_count; i++)
  {
    for(int j = 0; m[0][j] != '\0'; j++)
    {
      if(m[i][j] == 'O')
      {
        m[i][j] = '.';
        int x = j;
        while(x >= 0 && m[i][x] != 'O' && m[i][x] != '#')
        {
          x--;
        }
        m[i][x + 1] = 'O';
      }
    }
  }
}

int east(char m[][MAXCOLS], int row_count)
{
  for(int i = 0; i < row_count; i++)
  {
    for(int j = strlen_(m[0]) - 3; j >= 0; j--)
    {
      if(m[i][j] == 'O')
      {
        m[i][j] = '.';
        int x = j;
        while(m[i][x] != '\n' && m[i][x] != 'O' && m[i][x] != '#')
        {
          x++;
        }
        m[i][x - 1] = 'O';
      }
    }
  }
}

void fist_of_the_north_star(char m[][MAXCOLS], int row_count)
{
  north(m, row_count);
  west(m, row_count);
  south(m, row_count);
  east(m, row_count);
}

int get_load(char m[][MAXCOLS], int rc)
{
  int load = 0;
  for(int i = 0; i < rc; i++)
  {
    for(int j = 0; m[i][j] != '\0'; j++)
    {
      if(m[i][j] == 'O')
      {
        if(i == rc - 1) load += 1;
        else
          load += rc - i;
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
  for(int i = 0; i < 1000; i++)
  {
    fist_of_the_north_star(m, row_count);
    load = get_load(m, row_count);
    printf("%d\t%d\n", i, load);
  }
  print_matrix(m, row_count);
}
