#include<stdio.h>
#include<stdlib.h>

#define MAXCOLS 112
#define MAXROWS 110

struct beam 
{
  char dir;
  int x;
  int y;
};

FILE *fptr;

void set_file(FILE *ptr)
{
  fptr = ptr;
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
  ssize_t  linelen;
  size_t buffer_size;
  char *line;
  int i = 0;
  while((linelen = getline(&line, &buffer_size, fptr)) != -1)
  {
    copy(m[i++], line);
  }
  return i;
}

int count_excites(int t[][MAXCOLS], int rc)
{
  int excites = 0;
  for(int i = 0; i < rc; i++)
  {
    for(int j = 0; j < MAXCOLS; j++)
    {
      if(t[i][j]) excites++;
    }
  }
  return excites;
}

void reset_t(int t[][MAXCOLS], int rc)
{
  for(int i = 0; i < rc; i++)
    for(int j = 0; j < MAXCOLS; j++)
      t[i][j] = 0;
}

void travel(struct beam *b)
{
  char c = b -> dir;
  switch(c)
  {
    case 'L':
      b -> y--;
      break;
    case 'R':
      b -> y++;
      break;
    case 'U':
      b -> x--;
      break;
    case 'D':
      b -> x++;
      break;
  }
}

void shine(struct beam *b, char m[][MAXCOLS], int t[][MAXCOLS], int rc)
{
  while(b != NULL)
  {
    int x, y;
    char dir = b -> dir;
    x = b -> x;
    y = b -> y;
    if(x < 0 || x > rc || x > MAXROWS || y < 0 || y > MAXCOLS || m[x][y] == '\n' || m[x][y] == '\0')
    {
      free(b);
      b = NULL;
      return;
    }

    switch(m[x][y])
    {
      case '|':
        if(!t[x][y] && dir == 'R' || dir == 'L')
        {
          t[x][y] = 1;
          struct beam *q = (struct beam *) malloc(sizeof(struct beam));
          struct beam *w = (struct beam *) malloc(sizeof(struct beam));
          q -> x = b -> x - 1;
          w -> x = b -> x + 1;
          q -> y = w -> y = b -> y;
          q -> dir = 'U';
          w -> dir = 'D';
          free(b);
          b = NULL;
          shine(q, m, t, rc);
          shine(w, m, t, rc);
          return;
        }
        if(t[x][y]) return;
        break;
      case '-':
        if(!t[x][y] && dir == 'U' || dir == 'D')
        {
          t[x][y] = 1;
          struct beam *q = (struct beam *) malloc(sizeof(struct beam));
          struct beam *w = (struct beam *) malloc(sizeof(struct beam));
          q -> x = w -> x = b -> x;
          q -> y = b -> y + 1;
          w -> y = b -> y - 1;
          q -> dir = 'R';
          w -> dir = 'L';
          free(b);
          b = NULL;
          shine(q, m, t, rc);
          shine(w, m, t, rc);
          return;
        }
        if(t[x][y]) return;
        break;
      case '/':
        if(dir == 'R') b -> dir = 'U';
        else if(dir == 'L') b -> dir = 'D';
        else if(dir == 'U') b -> dir = 'R';
        else if(dir == 'D') b -> dir = 'L';
        break;
      case '\\':
        if(dir == 'R') b -> dir = 'D';
        else if(dir == 'L') b -> dir = 'U';
        else if(dir == 'U') b -> dir = 'L';
        else if(dir == 'D') b -> dir = 'R';
        break;
    }
    t[x][y] = 1;
    travel(b);
  }
}

int crazy_diamond(int x, int y, char dir, char m[][MAXCOLS], int t[][MAXCOLS], int rc)
{
  int excites = 0;
  struct beam *b = (struct beam *) malloc(sizeof(struct beam));
  b -> x = x;
  b -> y = y;
  b -> dir = dir;
  shine(b, m, t, rc);
  excites = count_excites(t, rc);
  reset_t(t, rc);
  return excites;
}

int beamfall(char m[][MAXCOLS], int t[][MAXCOLS], int rc)
{
  int max_excites = 0;

  // top row
  for(int j = 0; j <= MAXCOLS - 3; j++)
  {
    int ex = 0;
    ex = crazy_diamond(0, j, 'D', m, t, rc);
    if(ex > max_excites) max_excites = ex;
  }
  // bottom row
  for(int j = 0; j <= MAXCOLS - 3; j++)
  {
    int ex = 0;
    ex = crazy_diamond(rc - 1, j, 'U', m, t, rc);
    if(ex > max_excites) max_excites = ex;
  }
  // left col
  for(int i = 0; i < rc; i++)
  {
    int ex = 0;
    ex = crazy_diamond(i, 0, 'R', m, t, rc);
    if(ex > max_excites) max_excites = ex;
  }
  // right col
  for(int i = 0; i < rc; i++)
  {
    int ex = 0;
    ex = crazy_diamond(i, MAXCOLS - 3, 'L', m, t, rc);
    if(ex > max_excites) max_excites = ex;
  }
  return max_excites;
}

int main()
{
  char filename[] = "day16.txt";
  FILE *input = fopen(filename, "r");
  set_file(input);
  char m[MAXROWS][MAXCOLS];
  int t[MAXROWS][MAXCOLS];
  int rc = 0;
  rc = parse(m);
  reset_t(t, rc);
  print_matrix(m, rc);
  int excites = beamfall(m, t, rc);
 printf("excites: %d\n", excites);
}


