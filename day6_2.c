#include<stdio.h>
#include<ctype.h>
#include<string.h>
#include<stdint.h>
#include<stdlib.h>

#define MAXDIGS 15
#define MAXRACES 10

typedef unsigned long int u64;

struct race 
{
  u64 time;
  u64 record;
};

FILE *fptr;

void set_file(FILE *ptr)
{
  fptr = ptr;
}

u64 str_to_num(char str[])
{
  u64 num = 0;
  u64 mul = 1;
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


void skip_to(char c)
{
  char curr;
  while((curr = fgetc(fptr)) != EOF)
    if(curr == c) return;
}

void parse(struct race *r)
{
  int  charnum_idx;
  char charnum[MAXDIGS + 1];
  char c;
  charnum_idx = 0;

  skip_to(':');
  while((c = fgetc(fptr)) != ':')
  {
    if(isdigit(c))
    {
      charnum[charnum_idx++] = c;
    }

    charnum[charnum_idx] = '\0';
  }

  u64 n = str_to_num(charnum);
  r -> time = n;

  charnum_idx = 0;
  charnum[charnum_idx] = '\0';

  while(c = fgetc(fptr))
  {
    if(isdigit(c))
    {
      charnum[charnum_idx++] = c;
    }

    charnum[charnum_idx] = '\0';

    if(c == '\n') break;
  }

    printf("%s\n", charnum);
  n = str_to_num(charnum);
  r -> record = n;
    printf("%lu\n", n);
}

u64 cal_dist(u64 s, u64 t)
{
  return (t - s) * s;
}

u64 get_n_of_ways(struct race *r)
{
  u64 mid = r -> time / 2;
  u64 ways = 0;
  u64 cals;
  while((cals = cal_dist(mid, r -> time)) > r -> record)
  {
    mid--;
    ways++;
  }

  return r -> time % 2 == 0? (ways * 2) - 1 : ways * 2;
}

int main()
{
  char filename[] = "day6.txt";
  FILE *input = fopen(filename, "r");

  struct race *r = (struct race *) malloc(sizeof(struct race));

  set_file(input);

  parse(r);

  u64 possibilities = 1;
  u64 ways = get_n_of_ways(r);
  printf("time: %lu, record: %lu\n", r -> time, r -> record);
  printf("ways: %lu\n", ways);
  possibilities *= ways;

  printf("possibilities: %lu\n", possibilities);
}
