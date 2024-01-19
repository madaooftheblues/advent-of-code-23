#include<stdio.h>
#include<ctype.h>
#include<stdlib.h>
#include<string.h>
#define MAXLINE 1000
#define MAXDIG 3
#define RED 12
#define GREEN 13
#define BLUE 14

int state = 0;

struct charx {
  char str[MAXDIG + 1];
  int ptr;
};

struct game {
  struct charx *red;
  struct charx *green;
  struct charx *blue;
};

void copy(char to[], char from[])
{
  for(int i=0; (to[i] = from[i]) != '\0'; i++);
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

int is_possible(int r, int g, int b)
{
  printf("%d red, %d green, %d blue\n",r,g,b);
  if((RED - r >= 0) && (GREEN - g >= 0) && (BLUE - b >= 0)) return 1;
  return 0;
}

void start(char c)
{
  if(c == ' ') 
  {
    state = 1;
  }
}

char gameid(char c)
{
  if(c == ':') state = 2;
  else if(isdigit(c))
  {
    state = 1;
    return c;
  }
  return '\0';
}

void colon(char c)
{
  if(c == ' ')
  {
    state = 3;
  }
}

char num(char c)
{
  if(c == ' ')
  {
    state = 4;
  }
  else if(isdigit(c))
  {
    state = 3;
    return c;
  }
  return '\0';
}

char color(char c)
{
  if(c == 'r' || c == 'g' || c == 'b') 
  {
    state = 5;
    return c;
  }
  return '\0';
}

void semicolcom(char c)
{
  if(c == ',')
  {
    state = 2;
  }
  else if(c == ';')
  {
    state = 6;
  }
  else if(c == '\n')
  {
    state = 7;
  }
}

void next(char c)
{
  if(c == ' ')
  {
    state = 3;
  }
  else if(c == '\n')
  {
    state = 6;
  }
}

int main()
{
  char str[MAXLINE];
  FILE *input = fopen("day2.txt", "r");
  long sum_ids = 0;

  while(fgets(str, MAXLINE, input) != NULL)
  {
    state = 0;
    int valid = 1;

    struct game *g = malloc(sizeof g);
    g->red = (struct charx *)malloc(sizeof(struct charx));
    g->green = (struct charx *)malloc(sizeof(struct charx));
    g->blue = (struct charx *)malloc(sizeof(struct charx));


    struct charx *id = malloc(sizeof *id);
    struct charx *cnum = malloc(sizeof *cnum);

    id->ptr = 0;
    cnum->ptr = 0;

    for(int i=0; str[i] != '\0'; i++)
    {
      char c = str[i];
      switch(state)
      {
        case 0:
            start(c);
            break;
        case 1:
            if (gameid(c) != '\0')
            {
              id->str[id->ptr] = c;
              id->ptr++;
            }
            break;
        case 2:
            colon(c);
            break;
        case 3:
            if (num(c) != '\0')
            {
              cnum->str[cnum->ptr] = c;
              cnum->str[++cnum->ptr] = '\0';
            }
            break;
        case 4:
            if(color(c) != '\0')
            {
              if(c == 'r') copy(g->red->str, cnum->str);
              else if(c == 'g') copy(g->green->str, cnum->str);
              else if(c == 'b') copy(g->blue->str, cnum->str);
            }
            break;
        case 5:
            cnum->ptr = 0; 
            semicolcom(c);
            break;
        case 6:
            int redc = str_to_num(g->red->str);
            int greenc = str_to_num(g->green->str);
            int bluec = str_to_num(g->blue->str);

            if(!is_possible(redc, greenc, bluec))
            {
              valid = 0;
            }

            g->red->str[0] = '\0';
            g->green->str[0] = '\0';
            g->blue->str[0] = '\0';
            next(c);
            break;
        case 7:
            break;
      }
      int redc = str_to_num(g->red->str);
      int greenc = str_to_num(g->green->str);
      int bluec = str_to_num(g->blue->str);

      if(!is_possible(redc, greenc, bluec))
      {
        valid = 0;
      }

      g->red->str[0] = '\0';
      g->green->str[0] = '\0';
      g->blue->str[0] = '\0';
      if(!valid) break;
    }
    if(valid) 
    {
      printf("%d\n", str_to_num(id->str));
      sum_ids += str_to_num(id->str);
    }
    printf("\n");

  }
  printf("%ld\n", sum_ids);
}
