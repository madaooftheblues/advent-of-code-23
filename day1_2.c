#include<stdio.h>
#include "uthash.h"
#include<string.h>
#include<stdlib.h>
#include<ctype.h>
#define MAXLINE 1000

struct my_struct {
    int id;                    /* key */
    char name[6];
    UT_hash_handle hh;         /* makes this structure hashable */
};

struct my_struct *spells = NULL;    /* important! initialize to NULL */

void add_dig(int dig_id, char *name) {
    struct my_struct *s;
    HASH_FIND_INT(spells, &dig_id, s);  /* id already in the hash? */
    if (s == NULL) {
      s = (struct my_struct *)malloc(sizeof *s);
      s->id = dig_id;
      HASH_ADD_INT(spells, id, s);  /* id: name of key field */
    }
    strcpy(s->name, name);
}

void print_digits() {
    struct my_struct *s;

    for (s = spells; s != NULL; s = s->hh.next) {
        printf("digit id %d: spellings %s\n", s->id, s->name);
    }
}

struct my_struct *find_dig(int dig_id) {
    struct my_struct *s;

    HASH_FIND_INT(spells, &dig_id, s);  /* s: output pointer */
    return s;
}

void copy(char to[], char from[], int n)
{
  int i;
  for(i=0; i<n; i++)
  {
    to[i] = from[i];
  }
  to[i] = '\0';
}

int match(char* str, int n)
{
  struct my_struct *s;

  for (s = spells; s != NULL; s = s->hh.next) {
    char spell[n];
    copy(spell, s->name, n);
    if(strcmp(str, spell) == 0) return s->id;
  }
  return 0;
}

int get_digits(char* str)
{
  int l, r, id;
  char digit[2], substr[6];
  digit[0] = digit[1] = '\0';
  int len = strlen(str);
  l = 0;
  r = len - 1;
  id = 0;

  while(l < len && r >= 0)
  {
    copy(substr, str + l, 2);
    if(digit[0] == '\0')
    {
      if(isdigit(str[l]))
        digit[0] = str[l];
      else if(id = match(substr, 2)) 
      {
        struct my_struct *s = find_dig(id);
        int dig_len = strlen(s->name);
        copy(substr, str + l, dig_len);
        printf("l: %s, %s\n", substr, s->name);
        if(strcmp(substr, s->name) == 0) digit[0] = id + '0';
      }
    }
    l++;

    copy(substr, str + r, 2);
    if(digit[1] == '\0')
    {
      if(isdigit(str[r]))
        digit[1] = str[r];
      else if(id = match(substr, 2)) 
      {
        struct my_struct *s = find_dig(id);
        int dig_len = strlen(s->name);
        copy(substr, str + r, dig_len);
        printf("r: %s, %s\n", substr, s->name);
        if(strcmp(substr, s->name) == 0) digit[1] = id + '0';
      }
    }
    r--;

    if(digit[0] != '\0' && digit[1] != '\0') break;
  }
  int number = ((digit[0] - '0') * 10) + ((digit[1] - '0'));
  return number;

}

void populate()
{
  add_dig(1,"one");
  add_dig(2,"two");
  add_dig(3,"three");
  add_dig(4,"four");
  add_dig(5,"five");
  add_dig(6,"six");
  add_dig(7,"seven");
  add_dig(8,"eight");
  add_dig(9,"nine");
}

int main()
{
  populate();

  char str[MAXLINE];
  FILE *input = fopen("day1.txt", "r");
  long sum_cals = 0;

  while(fgets(str, MAXLINE, input) != NULL)
  {
    int number;
    number = get_digits(str);
    printf("%d\n", number);
    sum_cals += number;
  }

  printf("%li\n", sum_cals);
}
