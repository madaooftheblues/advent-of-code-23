#include<stdio.h>
#include<ctype.h>
#include<string.h>
#include "uthash.h"

#define MAXDIGS 3

struct hash {
    int id;                    /* key */
    int count;
    UT_hash_handle hh;         /* makes this structure hashable */
};

struct hash *h = NULL;  

void add_num(int num) {
    struct hash *s;
    HASH_FIND_INT(h, &num, s);  /* id already in the hash? */
    if (s == NULL) {
      s = (struct hash *)malloc(sizeof *s);
      s->id = num;
      HASH_ADD_INT(h, id, s);  /* id: name of key field */
    }
    s->count++;
}

struct hash *find_num(int n) {
    struct hash *s;

    HASH_FIND_INT(h, &n, s);  /* s: output pointer */
    return s;
}

void delete_all_num() {
  struct hash *current_num, *tmp;

  HASH_ITER(hh, h, current_num, tmp) {
    HASH_DEL(h, current_num);  /* delete; users advances to next */
    free(current_num);             /* optional- if you want to free  */
  }
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

int state = 0;

void start(char c)
{
  if(c == ':') state = 1;
}

char win_space(char c)
{
  if(c == ' ') return '\0';
  if(c == '|') 
  {
    state = 3;
    return '\0';
  }
  state = 2;
  return c;
}

char win_dig(char c)
{
  if(isdigit(c)) return c;
  state = 1;
  return '\0';
}

void your(int c)
{
  if(c == ' ') state = 4;
}

char your_space(char c)
{
  if(c == ' ') return '\0';
  state = 5;
  return c;
}

char your_dig(char c)
{
  if(isdigit(c)) return c;
  if(c == '\n') 
  {
    state = 6;
  } 
  else
    state = 4;
  return '\0';
}

void next()
{
  state = 0;
}

int main()
{
  char file_name[] = "day4.txt";
  FILE *fptr = fopen(file_name, "r");
  char c;
  char number[MAXDIGS + 1];
  int idx = 0;
  int points, total_points;
  points = total_points = 0;

  void reset_num()
  {
    idx = 0;
    number[idx] = '\0';
  }

  int get_num()
  {
    return str_to_num(number);
  }

  while((c = fgetc(fptr)) != EOF)
  {
    switch(state)
    {
      case 0:
        start(c);
        break;
      case 1:
        number[idx] = win_space(c);
        if(number[idx] != '\0')
        {
          number[++idx] = '\0';
        }
        break;
      case 2:
        number[idx] = win_dig(c);
        if(number[idx] != '\0')
        {
          number[++idx] = '\0';
        }
        else
        {
          int n = get_num();
          add_num(n);
          printf(" %d ", n);
          reset_num();
        }
        break;
      case 3:
        your(c);
        printf("|");
        break;
      case 4:
        number[idx] = your_space(c);
        if(number[idx] != '\0')
        {
          number[++idx] = '\0';
        }
        break;
      case 5:
        number[idx] = your_dig(c);
        if(number[idx] != '\0')
        {
          number[++idx] = '\0';
        }
        else
        {
          int n = get_num();
          struct hash *t = find_num(n);
          if(t != NULL)
          {
            points = points > 0 ? (points*2) : 1;
          }
          printf(" %d ", n);
          reset_num();
        }
        break;
      case 6:
        printf("\n");
        total_points += points;
        delete_all_num();
        points = 0;
        next();
        break;
    }
  }
  printf("\n");
  printf("%d\n", total_points);
}
