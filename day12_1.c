#include<stdio.h>
#include<stdlib.h>

#define MAXSPRINGS 25
#define MAXGROUPS 15
#define MAXDIGS 2

FILE *fptr;

void set_file(FILE *ptr)
{
  if(ptr != NULL) fptr = ptr;
}

int isdigit_(char c)
{
  if(c >= '0' && c <= '9') return 1;
  return 0;
}

int strlen_(char str[])
{
  int i = 0;
  while(str[i++] != '\0');
  return i;
}

int strlen_from(char str[], int start)
{
  int i, len;
  len = 0;
  i = start;
  while(str[i] != '\0')
  {
    i++;
    len++;
  }
  return len;
}

int str_to_num(char str[])
{
  int num = 0;
  int mul = 1;
  int is_neg = 0;
  if(str[0]  == '-') is_neg = 1;
  for(int i= strlen_(str) - 1; i >= 0 ; i--)
  {
    if(isdigit_(str[i]))
    {
      num = ((str[i] - '0') * mul) + num;
      mul *= 10;
    }
  }
  return is_neg? -num: num;
}

void print_from(char str[], int start)
{
  int i;
  i = start;
  while(str[i] != '\0')
  {
    printf("%c", str[i++]);
  }
}

void copy(char to[], char from[])
{
  for(int i = 0; (to[i] = from[i]) !='\0'; i++);
}

void copy_till(char to[], char from[], int start, int end)
{
  int i;
  for(i = 0; (i + start) <= end && (to[i] = from[start + i]) != '\0'; i++);
  to[i] = '\0';
}

int fit(char *str, int groups[], int ngroups)
{
  int n, len;
  len = strlen_(str) - 1;
  n = 0;
  for(int i = 0; str[i] != '\0';)
  { 

    if(str[i] == '?' || str[i] == '#')
    {
      if(n >= ngroups) return 0;
      if(i + groups[n] > len) return 0;
      if((i && str[i - 1] == '#') || (i + groups[n] < len && (str[i + groups[n]] == '#')))
        return 0;
      
      int k;
      for(k = i; k < i + groups[n]; k++)
      {
        if(str[k] == '.') return 0;
      }
      n++;
      i = k + 1;
    }
    else i++;
  }

  if(n < ngroups) return 0;
  return 1;
}

int f(char *str, int groups[], int ngroups)
{
  for(int i = 0; str[i] != '\0'; i++)
  {
    if(str[i] == '?')
    {
      char *dot = (char *) malloc(sizeof(char) * (MAXSPRINGS));
      char *hash = (char *) malloc(sizeof(char) * (MAXSPRINGS));
      copy(dot, str);
      copy(hash, str);
      dot[i] = '.';
      hash[i] = '#';
      free(str);
      return f(dot, groups, ngroups) + f(hash, groups, ngroups);
    }
  }
  // no ? was found;
  int val = fit(str, groups, ngroups);
  if(val)
  {
    free(str);
    return 1;
  }
  return 0;
}

void parse()
{
  char *line = NULL;
  size_t bufsize = 0;
  ssize_t linelen;

  int row_count = 0;
  int sum = 0;

  while((linelen = getline(&line, &bufsize, fptr) != -1))
  {
    char *hotsprings = (char *) malloc(sizeof(char) * (MAXSPRINGS));
    char groups[MAXGROUPS + 1];
    int order[MAXGROUPS];
    int ngroups = 0;
    int i = 0;
    while(line[i] != ' ')
    {
      i++;
    }
    copy_till(hotsprings, line, 0, i - 1);
    copy_till(groups, line, i + 1, strlen_(line) - 3);
    char buff[MAXDIGS + 1];
    int num_idx, in_num;
    num_idx = in_num = 0;
    for(int i = 0; i < strlen_(groups); i++)
    {
      if(isdigit_(groups[i]))
      {
        buff[num_idx++] = groups[i];
        buff[num_idx] = '\0';
        in_num = 1;
      }
      else
      {
        if(in_num)
        {
          order[ngroups++] = str_to_num(buff);
          num_idx = in_num = 0;
          buff[num_idx] = '\0';
        }
      }
    }
    int ways = f(hotsprings, order, ngroups);
    sum += ways;
  }

    printf("sum %d\n", sum);
}

int main()
{
  char filename[] = "day12.txt";
  FILE *input = fopen(filename, "r");
  set_file(input);

  parse();
}
