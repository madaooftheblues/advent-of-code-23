#include<stdio.h>
#include<stdlib.h>

#define MAXNUMS 25
#define MAXDIGS 15
#define MAXSEQS 200

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

int isdigit_(char c)
{
  if(c >= '0' && c <= '9') return 1;
  return 0;
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

int *extract_nums(char str[])
{
  char buffer[MAXDIGS];
  int nums[MAXNUMS];
  int num_count, char_count;
  num_count = char_count = 0;

  for(int i = 0; i < strlen_(str) - 1; i++)
  {
    if(str[i] == ' ' || str[i] == '\n')
    {
      buffer[char_count] = '\0';
      nums[num_count++] = str_to_num(buffer);
      char_count = 0;
    }
    else if(str[i] == '-' || isdigit_(str[i]))
    {
      buffer[char_count++] = str[i];
    }
  }

  int *dnum = malloc(sizeof(int) * (num_count + 1));
  dnum[0] = num_count;
  for(int i = 0; i < num_count; i++)
  {
    dnum[i + 1] = nums[i];
  }
  return dnum;
}

int parse(int *sequences[])
{
  char *line = NULL;
  size_t bufsize = 0;
  ssize_t linelen;

  int seq_count = 0;

  while((linelen = getline(&line, &bufsize, fptr) != -1))
  {
    sequences[seq_count++] = extract_nums(line);
  }

  return seq_count;
}

int extrapolate_2(int seq[])
{
  int all_zeros, count;
  count = 0;
  all_zeros = 1;

  for(int i = 1; i <= seq[0]; i++)
  {
    if(seq[i] != 0)
    {
      all_zeros = 0;
      break;
    }
  }

  if(all_zeros) return 0;

  int new_seq[seq[0]];

  for(int i = 2; i <= seq[0]; i++)
  {
    new_seq[i - 1] = seq[i] - seq[i - 1];
    printf("%d ", seq[i - 1]);
    count++;
  }
    printf("\n");
  new_seq[0] = count;
  return seq[1] - extrapolate_2(new_seq);
}

int main()
{
  char filename[] = "day9.txt";
  FILE *input = fopen(filename, "r");
  set_file(input);

  int *sequences[MAXSEQS];
  int seq_count, extrap_sum;

  extrap_sum = 0;
  seq_count = parse(sequences);

  for(int i = 0; i < seq_count; i++)
  {
    int pred = 0;
    pred = extrapolate_2(sequences[i]);
    extrap_sum += pred;
    printf("pred: %d\n\n", pred);
    free(sequences[i]);
  }
  printf("extrapolated sum: %d\n", extrap_sum);
}
