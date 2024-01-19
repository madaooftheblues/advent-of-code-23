#include<stdio.h>
#include<stdlib.h>

#define MAXROWS 100
#define MAXCOLS 100
#define MAXPAIRS 100

FILE *fptr;

struct pair 
{
  char m1[MAXROWS][MAXCOLS];
  char m2[MAXROWS][MAXCOLS];
  int mr1;
  int mr2;
};

struct pair *ps[MAXPAIRS];

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

void print_pairs(int pair_count)
{
  for(int i = 0; i < pair_count; i++)
  {
    for(int j = 0; j < ps[i] -> mr1; j++)
    {
      printf("%s", ps[i] -> m1[j]);
    }
    printf("\n");
    for(int j = 0; j < ps[i] -> mr2; j++)
    {
      printf("%s", ps[i] -> m2[j]);
    }
  }
}

void free_pairs(int pair_count)
{
  for(int i = 0; i < pair_count; i++)
  {
    free(ps[i]);
  }
}

int parse()
{
  char *line = NULL;
  size_t bufsize = 0;
  ssize_t linelen;
  int line_count, pair_count;
  line_count = pair_count = 0;
  ps[pair_count] = (struct pair *) malloc(sizeof(struct pair));
  while((linelen = getline(&line, &bufsize, fptr) != -1))
  {
    if(line[0] == '\n')
    {
      line_count++;
      if(line_count && line_count % 2 == 0)
      {
        pair_count++;
        ps[pair_count] = (struct pair *) malloc(sizeof(struct pair));
      }
    }
    else
    {
      struct pair *curr_p = ps[pair_count];
      if(line_count % 2 == 0)
      {
        copy(curr_p -> m1[curr_p -> mr1++], line);
      }
      else
      {
        copy(curr_p -> m2[curr_p -> mr2++], line);
      }
    }
  }
  pair_count++;
  return pair_count;
}

int row_ref(char m[][MAXCOLS], int r)
{
  int i, j, ti, tk, smudge;
  char temp;
  i = 0;
  j = 1;
  ti = tk = smudge = 0;
  temp = '\0';

  while(j < r)
  {
      int f = 1;
      for(int k = 0; m[i][k] != '\0'; k++)
      {
        if(m[i][k] != m[j][k]) 
        {
          if(!smudge)
          {
            ti = i;
            tk = k;
            temp = m[i][k];
            m[i][k] = m[j][k];
            smudge = 1;
          }
          else
          {
            if(temp != '\0')
            {
              smudge = 0;
              m[ti][tk] = temp;
              temp = '\0';
              ti = tk = 0;
            }
            f = 0;
            break;
          }
        }
      }

      if(f)
      {
        int o, p;
        o = i - 1;
        p = j + 1;
        int f2 = 1;
        char temp2 = '\0';

        while(o >= 0 && p < r)
        {

          for(int k = 0; m[o][k] != '\0'; k++)
          {
            if(m[o][k] != m[p][k]) 
            {
              if(!smudge)
              {
                ti = o;
                tk = k;
                temp2 = m[o][k];
                m[o][k] = m[p][k];
                smudge = 1;
              }
              else
              {
                if(temp2 != '\0')
                {
                  m[ti][tk] = temp2;
                  temp2 = '\0';
                  ti = tk = 0;
                  smudge = 0;
                }
                f2 = 0;
                break;
              }
            }
          }
          if(!f2) break;
          o--;
          p++;
        }
        if(f2 && smudge) return i;
        if(smudge)
        {
          m[ti][tk] = temp;
          temp = '\0';
          ti = tk = 0;
          smudge = 0;
        }
      }

    i++;
    j++;
  }
  return -1;
}

int col_ref(char m[][MAXCOLS], int r)
{
  int i, j, ti, tk, smudge;
  char temp;
  i = 0;
  j = 1;
  ti = tk = smudge = 0;
  temp = '\0';

  while(m[0][j] != '\0')
  {
      int f = 1;
      for(int k = 0; k < r; k++)
      {
        if(m[k][i] != m[k][j]) 
        {
          if(!smudge)
          {
            ti = i;
            tk = k;
            temp = m[k][i];
            m[k][i] = m[k][j];
            smudge = 1;
          }
          else
          {
            if(temp != '\0')
            {
              smudge = 0;
              m[tk][ti] = temp;
              temp = '\0';
              ti = tk = 0;
            }

            f = 0;
            break;
          }
        }
      }

      if(f)
      {
        int o, p;
        o = i - 1;
        p = j + 1;
        int f2 = 1;
        char temp2 = '\0';

        while(o >= 0 && p < strlen_(m[0]) - 2)
        {
          for(int k = 0; k < r; k++)
          {
            if(m[k][o] != m[k][p]) 
            {
              if(!smudge)
              {
                ti = o;
                tk = k;
                temp2 = m[k][o];
                m[k][o] = m[k][p];
                smudge = 1;
              }
              else
              {
                if(temp2 != '\0')
                {
                  m[tk][ti] = temp2;
                  temp2 = '\0';
                  ti = tk = 0;
                  smudge = 0;
                }

                f2 = 0;
                break;
              }
            }
          }
          if(!f2) break;
          o--;
          p++;
        }
        if(f2 && smudge) return i;
        if(smudge)
        {
          m[tk][ti] = temp;
          temp = '\0';
          ti = tk = 0;
          smudge = 0;
        }
      }
    i++;
    j++;
  }
  return -1;
}

int accum_ref(int pair_count)
{
  int summary = 0;
  for(int i = 0; i < pair_count; i++)
  {
    int r1, r2, c1, c2;
    r1 = r2 = c1 = c2 = -1;

    r1 = row_ref(ps[i] -> m1, ps[i] -> mr1);
    r2 = row_ref(ps[i] -> m2, ps[i] -> mr2);

    if(r1 != -1)
      summary += ((r1 + 1) * 100);
    else
    {
      c1 = col_ref(ps[i] -> m1, ps[i] -> mr1);
      if(c1 != -1)
        summary += c1 + 1;
    }

    if(r2 != -1)
      summary += ((r2 + 1) * 100);
    else
    {
      c2 = col_ref(ps[i] -> m2, ps[i] -> mr2);
      if(c2 != -1)
        summary += c2 + 1;
    }
    printf("r1: %d, c1: %d\n", r1, c1);
    printf("r2: %d, c2: %d\n", r2, c2);
  }
  return summary;
}

int main()
{
  char filename[] = "day13.txt";
  FILE *input = fopen(filename, "r");
  set_file(input);

  int pair_count, summary;
  pair_count = parse();
  print_pairs(pair_count);
  summary = accum_ref(pair_count);
  free_pairs(pair_count);
  printf("summary: %d\n", summary);
}
