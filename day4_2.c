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

struct card {
  int id;
  int matches;
  int count;
  UT_hash_handle hh;
};

struct hash *h = NULL;  
struct card *cards = NULL;

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

void add_card(int id, int matches) {
    struct card *s;
    HASH_FIND_INT(cards, &id, s);  /* id already in the hash? */
    if (s == NULL) {
      s = (struct card *)malloc(sizeof *s);
      s->id = id;
      HASH_ADD_INT(cards, id, s);  /* id: name of key field */
    }
    s->count++;
    s->matches = matches;
}

struct card *find_card(int n) {
    struct card *s;

    HASH_FIND_INT(cards, &n, s);  /* s: output pointer */
    return s;
}

void delete_all_cards() {
  struct card *current_card, *tmp;

  HASH_ITER(hh, cards, current_card, tmp) {
    HASH_DEL(cards, current_card);  /* delete; users advances to next */
    free(current_card);             /* optional- if you want to free  */
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

char start(char c)
{
  if(!isdigit(c)) return '\0';
  state = 1;
  return c;
}

char cid(char c)
{
  if(isdigit(c)) return c;
  if(c == ':') state = 2;
  return '\0';
}

char win_space(char c)
{
  if(c == ' ') return '\0';
  if(c == '|') 
  {
    state = 4;
    return '\0';
  }
  state = 3;
  return c;
}

char win_dig(char c)
{
  if(isdigit(c)) return c;
  state = 2;
  return '\0';
}

void your(int c)
{
  if(c == ' ') state = 5;
}

char your_space(char c)
{
  if(c == ' ') return '\0';
  state = 6;
  return c;
}

char your_dig(char c)
{
  if(isdigit(c)) return c;
  if(c == '\n') 
  {
    state = 7;
  } 
  else
    state = 5;
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
  char id[MAXDIGS + 1];
  int id_ptr = 0;
  char number[MAXDIGS + 1];
  int idx = 0;
  int matches, id_int;
  id_int = matches = 0;

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
        id[id_ptr] = start(c);
        if(id[id_ptr] != '\0')
        {
          id[++id_ptr] = '\0';
        }
        break;
      case 1:
        id[id_ptr] = cid(c);
        if(id[id_ptr] != '\0')
        {
          id[++id_ptr] = '\0';
        }
        else
        {
          id_int = str_to_num(id);
          printf("id: %d ", id_int);
          id_ptr = 0;
          id[id_ptr] = '\0';
        }
       
        break;
      case 2:
        number[idx] = win_space(c);
        if(number[idx] != '\0')
        {
          number[++idx] = '\0';
        }
        break;
      case 3:
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
      case 4:
        your(c);
        printf("|");
        break;
      case 5:
        number[idx] = your_space(c);
        if(number[idx] != '\0')
        {
          number[++idx] = '\0';
        }
        break;
      case 6:
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
            matches++;
          }
          printf(" %d ", n);
          reset_num();
        }
        break;
      case 7:
        printf("\n");
        delete_all_num();
        add_card(id_int, matches);
        matches = 0;
        next();
        break;
    }
  }
  add_card(id_int, matches);
  unsigned int num_cards;
  num_cards = HASH_COUNT(cards);
  unsigned int total_cards = 0;
  struct card *ccard;
  printf("\n");
  printf("\n");
  for (ccard = cards; ccard != NULL; ccard = ccard->hh.next) {
    for(int i = ccard->count; i > 0; i--)
      for(int j = 1; j <= ccard -> matches; j++)
      {
        struct card *t;
        t = find_card(ccard->id + j);
        t->count++;
      }
    total_cards += ccard->count;
    printf("id: %d, matches: %d, count: %d\n", ccard->id, ccard->matches, ccard->count);
  }
  printf("\n");
  printf("there are %u original cards\n", num_cards);
  printf("there are %u total cards\n", total_cards);

}
