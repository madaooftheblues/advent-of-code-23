#include<stdio.h>
#include<stdlib.h>

#define MAXROWS 140
#define MAXCOLS 142
#define MAXPATHS 4

// | n s
// - e w
// L n e
// J n w
// 7 s w
// F s e
// . no pipe
// S animal start

/*
            N

       W         E

            S

L: |_  J: _|  7: -| F: |-

*/

struct coords 
{
  int x;
  int y;
};

struct dir
{
  struct coords p;
  char d;
};

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

void print_matrix(char matrix[][MAXCOLS], int row_count)
{
  for(int i = 0; i < row_count; i++)
  {
    printf("%s", matrix[i]);
  }
}

int parse(char matrix[][MAXCOLS])
{
  char *line = NULL;
  size_t bufsize = 0;
  ssize_t linelen;

  int row_count = 0;

  while((linelen = getline(&line, &bufsize, fptr) != -1))
  {
    copy(matrix[row_count++], line);
  }

  return row_count;
}

struct dir *find_s(char matrix[][MAXCOLS], int row_count)
{
  for(int i = 0; i < row_count; i++)
  {
    for(int j = 0; matrix[i][j] != '\0'; j++)
    {
      if(matrix[i][j] == 'S')
      {
        struct dir *d = (struct dir *) malloc(sizeof(struct dir));
        d -> p.x = i;
        d -> p.y = j;
        return d;
      }
    }
  }
  return NULL;
}

int select_paths(struct dir *d, struct dir paths[], char mat[][MAXCOLS])
{
  int i, j, path_count;
  char c;
  i = d -> p.x;
  j = d -> p.y;

  path_count = 0;

  // Check left
  if(j > 0)
  {
    c = mat[i][j - 1];
    if(c == 'F' || c == 'L' || c == '-')
    {
      paths[path_count].p.x = i;
      paths[path_count].p.y = j - 1;
      paths[path_count].d = 'E';
      path_count++;
    }
  }

  // Check right
  if(j < MAXCOLS)
  {
    c = mat[i][j + 1];
    if(c  == '7' || c == 'J' || c == '-')
    {
      paths[path_count].p.x = i;
      paths[path_count].p.y = j + 1;
      paths[path_count].d = 'W';
      path_count++;
    }
  }
  
  // Check top
  if(i > 0)
  {
    c = mat[i - 1][j];
    if(c == 'F' || c == '7' || c == '|')
    {
      paths[path_count].p.x = i - 1;
      paths[path_count].p.y = j;
      paths[path_count].d = 'S';
      path_count++;
    }
  }
  
  // Check bottom

  if(i < MAXROWS)
  {
    c = mat[i + 1][j];
    if(c == 'L' || c == 'J' || c == '|')
    {
      paths[path_count].p.x = i + 1;
      paths[path_count].p.y = j;
      paths[path_count].d = 'N';
      path_count++;
    }
  }

  return path_count;
}

char traverse(struct dir *d, char mat[][MAXCOLS])
{
  int i, j;
  char dir;
  i = d -> p.x;
  j = d -> p.y;
  dir = d -> d;

  switch(mat[i][j])
  {
    case 'F':
        if(dir == 'E')
        {
          d ->p.x++;
          d -> d = 'N';
        }
        else if(dir == 'S')
        {
          d -> p.y++;
          d -> d = 'W';
        }
        break;

    case 'L':
        if(dir == 'E')
        {
          d -> p.x--;
          d -> d = 'S';
        }
        else if(dir == 'N')
        {
          d -> p.y++;
          d -> d = 'W';
        }
        break;

    case '7':
        if(dir == 'W')
        {
          d -> p.x++;
          d -> d = 'N';
        }
        else if(dir == 'S')
        {
          d -> p.y--;
          d -> d = 'E';
        }
        break;
    case 'J':
        if(dir == 'W')
        {
          d -> p.x--;
          d -> d = 'S';
        }
        else if(dir == 'N')
        {
          d -> p.y--;
          d -> d = 'E';
        }
        break;
    case '|':
        if(dir == 'N')
        {
          d -> p.x++;
          d -> d = 'N';
        }
        else if(dir == 'S')
        {
          d -> p.x--;
          d -> d = 'S';
        }
        break;
    case '-':
        if(dir == 'E')
        {
          d -> p.y--;
          d -> d = 'E';
        }
        else if(dir == 'W')
        {
          d -> p.y++;
          d -> d = 'W';
        }
        break;
  }

  if(mat[i][j] == 'S') return 'S';
  return '\0';
}

int main()
{
  char filename[] = "day10.txt";
  FILE *input = fopen(filename, "r");
  set_file(input);
  
  char matrix[MAXROWS][MAXCOLS];
  struct dir paths[MAXPATHS];
  int row_count, path_count;

  row_count = path_count = 0;

  row_count = parse(matrix);
  struct dir *d = find_s(matrix, row_count);

  path_count = select_paths(d, paths, matrix);

  int steps = 1;
  while(1)
  {
    int intersect = 1;
    for(int i = 0; i < path_count; i ++)
    {
      if((paths[0].p.x != paths[i].p.x) || (paths[0].p.y != paths[i].p.y))
      {
        intersect = 0;
        break;
      }
    }

    if(intersect) break;

    for(int i = 0; i < path_count; i++)
    {
      traverse(&paths[i], matrix);
    }
    steps ++;
  }

  printf("S: (%d, %d)\n", d -> p.x, d -> p.y);
  printf("path_count: %d\n", path_count);
  printf("steps: %d\n", steps);
  free(d);
}
