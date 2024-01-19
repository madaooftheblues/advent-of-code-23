#include<stdio.h>
#include<stdlib.h>

#define ASCII 256
#define MAXSTR 100
#define MAXLABEL 15

struct box
{
  char label[15];
  int lens;
  struct box *next;
};

struct box *hashmap[ASCII];

FILE *fptr;

void set_file(FILE *ptr)
{
  fptr = ptr;
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

int strcmp_(char a[], char b[])
{
  if(strlen_(a) != strlen_(b)) return 0;
  for(int i = 0; a[i] != '\0'; i++)
  {
    if(a[i] != b[i]) return 0;
  }
  return 1;
}

void copy(char to[], char from[])
{
  for(int i = 0; (to[i] = from[i]) !='\0'; i++);
}

int hash(char str[])
{
  int h = 0;
  for(int i = 0; str[i] != '\0' && str[i] != '\n'; i++)
  {
    h += (int) str[i];
    h *= 17;
    h %= 256;
  }
  return h;
}

void print_map()
{
  for(int i = 0; i < ASCII; i++)
  {
    struct box *h = hashmap[i];
    if(h == NULL) continue;
    printf("Box %d: ", i);
    while(h != NULL)
    {
      printf("[%s %d] ", h -> label, h -> lens);
      h = h -> next;
    }
    printf("\n");
  }
}

struct box *find_lens(char label[])
{
  int box_no = hash(label);
  struct box *h = hashmap[box_no];
  if(hashmap[box_no] == NULL) return NULL;
  while(h != NULL)
  {
    if(strcmp_(label, h -> label)) return h;
    h = h -> next;
  }
  return NULL;
}

void add_lens(char label[], int lens)
{
  struct box *h = find_lens(label);
  if(h == NULL)
  {
    h = (struct box *) malloc(sizeof(struct box));
    copy(h -> label, label);
    h -> next = NULL;

    struct box **a = &hashmap[hash(label)];
    struct box *t = *a;
    if(t == NULL)
    {
      *a = h;
    }
    else
    {
      while(t -> next != NULL)
      {
        t = t -> next;
      }
      t -> next = h;
    }
  }
  h -> lens = lens;
}

void remove_lens(char label[])
{
  struct box **a = &hashmap[hash(label)];
  struct box *h = *a;
  if(h == NULL) return;
  if(strcmp_(label, h -> label))
  {
    if(h -> next == NULL)
    {
      free(*a);
      *a = NULL;
      return;
    }
    *a = h -> next;
    free(h);
    h = NULL;
    return;
  }

  struct box *t = h;
  while(h != NULL)
  {
    if(strcmp_(label, h -> label))
    {
      t -> next = h -> next;
      free(h);
      h = NULL;
      break;
    }
    t = h;
    h = h -> next;
  }
}

int parse()
{
  int h, i;
  char buffer[MAXSTR];
  char c;
  i = 0;
  buffer[i] = '\0';

  while((c = fgetc(fptr)) != EOF)
  {
    if(c == '=' || c == '\0' || c == '\n') continue;

    if(c == ',')
    {
      i = 0;
      buffer[i] = '\0';
    }
    else if(c == '-')
    {
      remove_lens(buffer);
    }
    else if(isdigit_(c))
    {
      add_lens(buffer, c - '0');
    }
    else 
    {
      buffer[i++] = c;
      buffer[i] = '\0';
    }
  }
}

int focusing_powers()
{
  int fps = 0;

  for(int i = 0; i < ASCII; i++)
  {
    if(hashmap[i] != NULL)
    {
      int slot = 1;
      struct box *h = hashmap[i];
      while(h != NULL)
      {
        fps += (i + 1) * slot * h -> lens;
        slot++;
        h = h -> next;
      }
    }
  }
  return fps;
}

int main()
{
  char filename[] = "day15.txt";
  FILE *input = fopen(filename, "r");
  set_file(input);
  parse();
  print_map();
  int fps = focusing_powers();
  printf("focusing powers: %d\n", fps);
}
