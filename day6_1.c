#include<stdio.h>
#include<ctype.h>
#include<string.h>

#define MAXDIGS 4
#define MAXRACES 10

struct race 
{
  int time;
  int record;
};

FILE *fptr;

void set_file(FILE *ptr)
{
  fptr = ptr;
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


void skip_to(char c)
{
  char curr;
  while((curr = fgetc(fptr)) != EOF)
    if(curr == c) return;
}

int parse(struct race races[])
{
  int idx, charnum_idx, in_num;
  char charnum[MAXDIGS + 1];
  char c;
  idx = charnum_idx = in_num = 0;

  skip_to(':');
  while((c = fgetc(fptr)) != ':')
  {
    if(isdigit(c))
    {
      in_num = 1;
      charnum[charnum_idx++] = c;
    }
    else if(in_num)
    {
      int n = str_to_num(charnum);
      races[idx++].time = n;
      charnum_idx = 0;
      in_num = 0;
    }

    charnum[charnum_idx] = '\0';
  }

  idx = 0;
  while(c = fgetc(fptr))
  {
    if(isdigit(c))
    {
      in_num = 1;
      charnum[charnum_idx++] = c;
    }
    else if(in_num)
    {
      int n = str_to_num(charnum);
      races[idx++].record = n;
      charnum_idx = 0;
      in_num = 0;
    }

    charnum[charnum_idx] = '\0';

    if(c == '\n') break;
  }
  return idx;
}

int cal_dist(int s, int t)
{
  return (t - s) * s;
}

int get_n_of_ways(struct race *r)
{
  int mid = r -> time / 2;
  int ways = 0;
  int cals;
  while((cals = cal_dist(mid, r -> time)) > r -> record)
  {
    mid--;
    ways++;
    printf("%d\n", cals);
  }

  return r -> time % 2 == 0? (ways * 2) - 1 : ways * 2;
}

int main()
{
  char filename[] = "day6.txt";
  FILE *input = fopen(filename, "r");

  struct race r[MAXRACES];
  int r_idx;

  set_file(input);

  r_idx = parse(r);

  int possibilities = 1;
  for(int i = 0; i < r_idx; i++)
  {
    int ways = get_n_of_ways(&r[i]);
    printf("time: %d, record: %d\n", r[i].time, r[i].record);
    printf("ways: %d\n", ways);
    possibilities *= ways;
  }

  printf("possibilities: %d\n", possibilities);
}
