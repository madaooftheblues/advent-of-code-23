#include<stdio.h>
#include<stdint.h>
#include<ctype.h>
#include<string.h>
#include<stdlib.h>
#include<limits.h>

#define MAXDIGS 10
#define MAXSEEDS 20
#define MAXCONVS 50

typedef  uint64_t u64;

struct seed_r 
{
  u64 start;
  u64 range;
};

struct dsr 
{
  u64 dst;
  u64 src;
  u64 rng;
};

struct map
{
  struct dsr convs[MAXCONVS];
  int n;
};


FILE *fptr;

void set_file(FILE *ptr)
{
  fptr = ptr;
}

void skip_to(char c)
{
  if(fptr == NULL)
  {
    printf("file not opened\n");
    return;
  }
  char cur;
  while((cur = fgetc(fptr)) != EOF)
  {
    if(cur == c) return;
  }
  printf("character not found\n");
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

int parse_seeds(u64 seeds[])
{
  char charnum[MAXDIGS + 1];
  int charnum_idx, seeds_idx;
  int in_num = 0;
  char c;

  charnum_idx = seeds_idx = 0;

  while((c = fgetc(fptr)))
  {
    if(isdigit(c))
    {
      in_num = 1;
      charnum[charnum_idx++] = c;
      charnum[charnum_idx] = '\0';
    }
    else
    {
      if(in_num)
      {
        seeds[seeds_idx++] = str_to_num(charnum);
        charnum_idx = 0;
        charnum[charnum_idx] = '\0';
        in_num = 0;
      }
    }

    if(c == '\n') break;
  }

  return seeds_idx;
}

int is_even(int n)
{
  if(n % 2 == 0) return 1;
  return 0;
}

int parse_seeds_2(struct seed_r seeds[])
{
  char charnum[MAXDIGS + 1];
  int charnum_idx, n, seeds_idx;
  int in_num = 0;
  char c;

  charnum_idx = seeds_idx = n = 0;

  while((c = fgetc(fptr)))
  {
    if(isdigit(c))
    {
      in_num = 1;
      charnum[charnum_idx++] = c;
      charnum[charnum_idx] = '\0';
    }
    else
    {
      if(in_num)
      {
        if(is_even(n))
        {
          seeds[seeds_idx].start = str_to_num(charnum);
        }
        else
        {
          seeds[seeds_idx].range = str_to_num(charnum);
          seeds_idx++;
        }

        charnum_idx = 0;
        charnum[charnum_idx] = '\0';
        in_num = 0;
        n++;
      }
    }

    if(c == '\n') break;
  }

  return seeds_idx;
}

void parse_map(struct map **am)
{
  struct map *m = (struct map *) malloc(sizeof(struct map));
  // m -> convs = (struct dsr *)  malloc(MAXCONVS * sizeof(struct dsr));
  
  int state = 0;
  char charnum[MAXDIGS + 1];
  int charnum_idx, convs_idx;
  char c;

  charnum_idx = convs_idx = 0;

  char wait_for_dst(char c)
  {
    if(!isdigit(c)) return '0';
    state = 1;
    return c;
  }

  char s_dst(char c)
  {
    if(isdigit(c)) return c;
    state = 2;
    return '\0';
  }

  char wait_for_src(char c)
  {
    if(!isdigit(c)) return '\0';
    state = 3;
    return c;
  }

  char s_src(char c)
  {
    if(isdigit(c)) return c;
    state = 4;
    return '\0';
  }

  char wait_for_rng(char c)
  {
    if(!isdigit(c)) return '\0';
    state = 5;
    return c;
  }

  char s_rng(char c)
  {
    if(isdigit(c)) return c;
    state = 6;
    return '\0';
  }

  char next(char c)
  {
    if(!isdigit(c)) return '\0';
    state = 1;
    return c;
  }

  printf("dest\t\tsource\t\trange\n");
  int parsed = 0;

  while((c = fgetc(fptr)))
  {
    if(parsed) break;
    switch(state)
    {
      case 0:
        charnum[charnum_idx] = wait_for_dst(c);
        if(charnum[charnum_idx] != '\0')
          charnum[++charnum_idx] = '\0';
        break;
      case 1:
        charnum[charnum_idx] = s_dst(c);
        if(charnum[charnum_idx] != '\0')
          charnum[++charnum_idx] = '\0';
        else 
        {
          u64 num = str_to_num(charnum);
          charnum_idx = 0;
          charnum[charnum_idx] = '\0';

          m -> convs[convs_idx].dst = num;
        }
        break;
      case 2:
        charnum[charnum_idx] = wait_for_src(c);
        if(charnum[charnum_idx] != '\0')
          charnum[++charnum_idx] = '\0';
        break;
      case 3:
        charnum[charnum_idx] = s_src(c);
        if(charnum[charnum_idx] != '\0')
          charnum[++charnum_idx] = '\0';
        else 
        {
          u64 num = str_to_num(charnum);
          charnum_idx = 0;
          charnum[charnum_idx] = '\0';

          m -> convs[convs_idx].src = num;
        }
        break;
      case 4:
        charnum[charnum_idx] = wait_for_rng(c);
        if(charnum[charnum_idx] != '\0')
          charnum[++charnum_idx] = '\0';
        break;
      case 5:
        charnum[charnum_idx] = s_rng(c);
        if(charnum[charnum_idx] != '\0')
          charnum[++charnum_idx] = '\0';
        else 
        {
          u64 num = str_to_num(charnum);
          charnum_idx = 0;
          charnum[charnum_idx] = '\0';

          m -> convs[convs_idx].rng = num;
        }
        break;
      case 6:
        u64 d,s,r;
        d = m -> convs[convs_idx].dst;
        s = m -> convs[convs_idx].src;
        r = m -> convs[convs_idx].rng;
        printf("%lu\t%lu\t%lu\n",d,s,r);
        charnum[charnum_idx] = next(c);
        if(charnum[charnum_idx] == '\0')
        {
          parsed = 1;
          break;
        }
        charnum[++charnum_idx] = '\0';
        convs_idx++;
        break;
    }
  }

  m -> n = convs_idx;
  *am = m;

}

u64 convert(u64 seed, struct map *m)
{
  for(int i = 0; i <= m -> n; i++)
  {
    u64 d,s,r;
    d = m -> convs[i].dst;
    s = m -> convs[i].src;
    r = m -> convs[i].rng;

    if(seed >= s && seed <= s + r - 1)
    {
      u64 diff = d - s;
      diff = diff < 0? -(diff) : diff;
      return diff + seed;
    }
  }

  return seed;
}

int main()
{
  char filename[] = "day5.txt";
  FILE *input = fopen(filename, "r");

  struct seed_r seeds[MAXSEEDS];
  int n_seeds = 0;

  struct map *soil, *fertilizer, *water, *light, *temperature, *humidity, *location;
  soil = fertilizer = water = light = temperature = humidity = location = NULL;

  set_file(input);
  skip_to(':');

  n_seeds = parse_seeds_2(seeds);
  printf("n of seeds: %d\n", n_seeds);
  printf("seeds:\n");
  for(int i = 0; i < n_seeds; i++)
  {
    printf("start: %lu range: %lu\n", seeds[i].start, seeds[i].range);
  }
  printf("\n");
  printf("\n");

  skip_to(':');
  printf("seed to soil:\n");
  parse_map(&soil);
  printf("\n");

  skip_to(':');
  printf("soil to fertilizer:\n");
  parse_map(&fertilizer);
  printf("\n");

  skip_to(':');
  printf("fertilizer to water:\n");
  parse_map(&water);
  printf("\n");

  skip_to(':');
  printf("water to light:\n");
  parse_map(&light);
  printf("\n");

  skip_to(':');
  printf("light to temperature:\n");
  parse_map(&temperature);
  printf("\n");

  skip_to(':');
  printf("temperature to humidity:\n");
  parse_map(&humidity);
  printf("\n");

  skip_to(':');
  printf("humidity to location:\n");
  parse_map(&location);
  printf("\n");

  u64 lowest = ULONG_MAX;
  
  for(int i = 0; i < n_seeds; i++)
  {
    u64 start, range;
    start = seeds[i].start;
    range = seeds[i].range;
    u64 mapping;
    for(u64 j = start; j < start + range - 1; j++)
    {
      mapping = convert(convert(convert(convert(convert(convert(convert( j ,
      soil), fertilizer), water), light), temperature), humidity), location);

      if(mapping < lowest) lowest = mapping;
    }
  }
 
  printf("lowest: %lu\n", lowest);
}
