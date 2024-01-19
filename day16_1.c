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

void beamfall(char m[][MAXCOLS], int t[][MAXCOLS], int rc)
{
  struct beam *b = (struct beam *) malloc(sizeof(struct beam));
  b -> x = b -> y = 0;
  b -> dir = 'R';
  shine(b, m, t, rc);
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

  for(int i = 0; i < rc; i++)
    for(int j = 0; j < MAXCOLS; j++)
      t[i][j] = 0;

  print_matrix(m, rc);
  beamfall(m, t, rc);
  int excites = 0;
  for(int i = 0; i < rc; i++)
  {
    for(int j = 0; j < MAXCOLS; j++)
    {
      if(t[i][j]) excites++;
      printf("%d", t[i][j]);
    }
    printf("\n");
  }

 printf("excites: %d\n", excites);
}


