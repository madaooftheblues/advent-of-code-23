#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"uthash.h"

#define MAXCHARS 3
#define MAXAZ 10

/*
#define MAXQUE 500

struct node 
{
  char d[MAXCHARS + 1];
  struct node *l;
  struct node *r;
};

struct node *root = NULL;

struct que 
{
  int first;
  int last;
  struct node *elements[MAXQUE];
};

int is_que_empty(struct que *q)
{
  if(q -> first == q -> last) return 1;
  return 0;
}

void enque(struct node *vertex, struct que *q)
{
  if(q == NULL || vertex == NULL) return;
  if(is_que_empty(q))
    q -> first = q ->last = -1;

  q -> elements[++q -> last] = vertex;
}

struct node* deque(struct que *q)
{
  if(q == NULL || is_que_empty(q)) return NULL;

  return q -> elements[++q -> first];
}


int strcmp_(char str1[], char str2[])
{
  int len1 = strlen(str1);
  int len2 = strlen(str2);

  if(len1 != len2) return 0;

  int i, flag;
  i = 0;

  while(i < len1)
  {
    if(str1[i] != str2[i]) return 0;
  }

  return 1;
}

struct node* create_node(char data[])
{
  struct node *temp = (struct node *) malloc(sizeof(struct node));
  copy(temp -> d, data);
  temp -> l = NULL;
  temp -> r = NULL;

  return temp;
}

struct node* bfs(char data[])
{
  struct que *q = (struct que *) malloc(sizeof(struct que));
  struct node *current = (struct node *) malloc(sizeof(struct node));
  enque(root, q);

  while(q -> first != q -> last)
  {
    current = deque(q);
    if(current == NULL) continue;
    if(strcmp_(data, current -> l -> d)) return current -> l;
    else if(strcmp_(data, current -> r -> d)) return current -> r;
    else 
    {
      enque(current -> l, q);
      enque(current -> r, q);
    }
  }

  return NULL;
}

void add_node(char data[], char left[], char right[])
{
  struct node *t, *l, *r;
  l = create_node(left);
  r = create_node(right);

  if(root == NULL)
  {
    root = create_node(data);
    root -> l = l;
    root -> r = r;

    return;
  }

}

*/

struct hash {
    int id;                    /* key */
    int left;
    int right;
    
    UT_hash_handle hh;         /* makes this structure hashable */
};

struct hash *h = NULL;
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

int isalpha_(char c)
{
  if((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) return 1;
  return 0;
}

void copy(char to[], char from[])
{
  for(int i = 0; (to[i] = from[i]) !='\0'; i++);
}

void copy_till(char to[], char from[], int start, int end)
{
  int i;;
  for(i = 0; (i + start) <= end && (to[i] = from[start + i]) != '\0'; i++);
  to[i] = '\0';
}

int str_to_num(char str[])
{
  int num = 0;
  int mul = 1;
  for(int i= strlen(str) - 1; i >= 0 ; i--)
  {
    if(isdigit_(str[i]))
    {
      num = ((str[i] - '0') * mul) + num;
      mul *= 10;
    }
  }
  return num;
}

void num_to_str(int n, char str[])
{
  char buff[MAXCHARS + 1];
  int i, div, rem;
  div = n;
  buff[MAXCHARS + 1] = '\0';
  for(i = MAXCHARS - 1; i >= 0; i--)
  {
    rem = div % 10;
    div /= 10;
    buff[i] = rem + '0';
    if(div == 0) break;
  }
  copy(str, buff);
}

int alpha_to_num(char str[])
{
  int num = 0;
  int mul = 1;
  for(int i = strlen(str) - 1; i >= 0 ; i--)
  {
    if(isalpha_(str[i]))
    {
      num = ((str[i] - 'A') * mul) + num;
      mul *= 26;
    }
  }

  return num;
}

void num_to_alpha(int n, char str[])
{
  if(n == 0) 
  {
    copy(str, "AAA");
    return;
  }
  char buff[MAXCHARS + 1];
  int i, div, rem;
  div = n;
  buff[MAXCHARS + 1] = '\0';
  for(i = MAXCHARS - 1; i >= 0; i--)
  {
    rem = div % 26;
    div /= 26;
    buff[i] = rem + 'A';
    if(div == 0) break;
  }
  copy(str, buff);
}

void add_node(int id, int left, int right) {
    struct hash *s;
    HASH_FIND_INT(h, &id, s);  /* id already in the hash? */
    if (s == NULL) {
      s = (struct hash *)malloc(sizeof *s);
      s -> id = id;
      s -> left = left;
      s -> right = right;
      HASH_ADD_INT(h, id, s);  /* id: name of key field */
    }
}

struct hash *find_node(int id) {
    struct hash *s;

    HASH_FIND_INT(h, &id, s);  /* s: output pointer */
    return s;
}

void delete_all_data() {
  struct hash *current_node, *tmp;

  HASH_ITER(hh, h, current_node, tmp) {
    HASH_DEL(h, current_node);  /* delete; users advances to next */
    free(current_node);             /* optional- if you want to free  */
  }
}

void print_nodes() {
  struct hash *current_node, *tmp;
  HASH_ITER(hh, h, current_node, tmp) {
    char d[MAXCHARS + 1];
    char l[MAXCHARS + 1];
    char r[MAXCHARS + 1];
    num_to_alpha(current_node -> id, d);
    num_to_alpha(current_node -> left, l);
    num_to_alpha(current_node -> right, r);
    printf("%s, %s, %s\n", d, l, r);
  }
}

unsigned long long lcm(unsigned long long a, unsigned long long b)
{
  unsigned long long gcd;

  for(int i = 1; i <= a && i <= b; i++)
    if((a % i == 0) && (b % i == 0)) gcd = i;

  return (a * b) / gcd;
}

int parse(char ins[])
{
  char *line = NULL;
  size_t bufsize = 0;
  ssize_t linelen;

  int ins_len = 0;

  while((linelen = getline(&line, &bufsize, fptr) != -1))
  {
    if(line[0] == '\n') continue; 
    if(!ins_len)
    {
      copy(ins, line);
      ins_len = strlen(line) - 1;
      continue;
    }

    char d[MAXCHARS + 1];
    char l[MAXCHARS + 1];
    char r[MAXCHARS + 1];
    
    copy_till(d, line, 0, 2);
    copy_till(l, line, 7, 9);
    copy_till(r, line, 12, 14);

    int id = alpha_to_num(d);
    int left = alpha_to_num(l);
    int right = alpha_to_num(r);

    add_node(id, left, right);
  }

  free(line);
  fclose(fptr);

  return ins_len;
}

int get_steps(int root, int goal, char ins[], int len)
{
  struct hash *current;
  current = find_node(root);
  int i, steps;
  i = steps = 0; 
  while(current -> id != goal)
  {
    if(i >= len) i = 0;
    if(ins[i] == 'L')
      current = find_node(current -> left);
    else if(ins[i] == 'R')
      current = find_node(current -> right);

    steps++;
    i++;
  }
  return steps;
}

unsigned long long get_steps_2(struct hash *ends_with_a[], char ins[], int ins_len)
{
  struct hash *current_node, *tmp;
  int aidx;
  aidx = 0;
  HASH_ITER(hh, h, current_node, tmp) {
    char d[MAXCHARS + 1];

    num_to_alpha(current_node -> id, d);

    if(d[2] == 'A')
    {
      ends_with_a[aidx] = current_node;
      printf("A: %s\n\n", d);
      aidx++;
    }
  }

  unsigned long long steps;
  int ins_idx;
  int dist[aidx];
  steps = ins_idx = 0;

  for(int i = 0; i < aidx; i++) dist[i] = 0;

  while(1)
  {
    int valid = 1;
    for(int i = 0; i < aidx; i++)
    {
      if(dist[i] == 0)
      {
        valid = 0;
        break;
      }
    }

    if(valid) break;

    if(ins_idx > ins_len - 1) ins_idx = 0;

    for(int i = 0; i < aidx; i++)
    {
      if(ins[ins_idx] == 'L')
      {
        char buff[MAXCHARS + 1];
        num_to_alpha(ends_with_a[i] -> id, buff);
        printf("%d: %s -> ", i, buff);
        ends_with_a[i] = find_node(ends_with_a[i] -> left);

        num_to_alpha(ends_with_a[i] -> id, buff);
        printf("%s\n", buff);
      }
      else if(ins[ins_idx] == 'R')
      {
        char buff[MAXCHARS + 1];
        num_to_alpha(ends_with_a[i] -> id, buff);
        printf("%d: %s -> ", i, buff);
        ends_with_a[i] = find_node(ends_with_a[i] -> right);

        num_to_alpha(ends_with_a[i] -> id, buff);
        printf("%s\n", buff);
      }
    }

    steps++;
    ins_idx++;

    for(int i = 0; i < aidx; i++)
    {
      char buff[MAXCHARS + 1];
      num_to_alpha(ends_with_a[i] -> id, buff);
      if(buff[2] == 'Z' && dist[i] == 0)
      {
        dist[i] = steps;
      }
    }
  }

  for(int i = 0; i < aidx; i++)
  {
    printf("\n%d: %d\n", i, dist[i]);
  }

  unsigned long long l = dist[0];
  for(int i = 1; i < aidx; i++)
  {
    l = lcm(l, dist[i]) ;
  }

/* Bruteforce
  while(1)
  {
    int valid = 0;

    for(int i = 0; i < aidx; i++)
    {
      char buff[MAXCHARS + 1];
      num_to_alpha(ends_with_a[i] -> id, buff);
      if(buff[strlen(buff) - 1] != 'Z')
      {
        valid = 0;
        break;
      }

      valid = 1;
    }

    if(valid) return steps;
    if(ins_idx > ins_len - 1) ins_idx = 0;


    for(int i = 0; i < aidx; i++)
    {
      if(ins[ins_idx] == 'L')
      {
        char buff[MAXCHARS + 1];
        num_to_alpha(ends_with_a[i] -> id, buff);
        printf("%d: %s -> ", i, buff);
        ends_with_a[i] = find_node(ends_with_a[i] -> left);

        num_to_alpha(ends_with_a[i] -> id, buff);
        printf("%s\n", buff);
      }
      else if(ins[ins_idx] == 'R')
      {
        char buff[MAXCHARS + 1];
        num_to_alpha(ends_with_a[i] -> id, buff);
        printf("%d: %s -> ", i, buff);
        ends_with_a[i] = find_node(ends_with_a[i] -> right);

        num_to_alpha(ends_with_a[i] -> id, buff);
        printf("%s\n", buff);
      }
    }

    printf("\n");
    steps++;
    ins_idx++;
  }
*/
  return l;
}

int main()
{
  char *filename = "day8.txt";
  FILE *input = fopen(filename, "r");
  set_file(input);

  char instructions[300];
  unsigned long long int steps;
  int ins_len;

  struct hash *ends_with_a[MAXAZ];

  ins_len = steps = 0;

  ins_len = parse(instructions);
  steps = get_steps_2(ends_with_a, instructions, ins_len);
//  print_nodes();
  printf("\n");
  printf("%s", instructions);
  printf("\n");
  printf("instructions length: %d\n", ins_len);
  printf("steps: %Lu\n", steps);
}
