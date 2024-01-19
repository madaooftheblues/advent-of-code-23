#include<stdio.h>
#include<string.h>
#include<ctype.h>
#include<stdlib.h>

#define MAXCARDS 5
#define MAXDIGS 3
#define MAXHANDS 1000
#define MAXBUFF 5
#define MAXOCCUR 5

// Implement binary search instead of linear search

/*
5 types

five -> all cards same
four -> 4 cards same, 1 diff
full -> 3 cards same, rest 2 same
three -> 2 cards same, rest 2 diff
two -> 2 cards same, rest 2 same, 1 diff
one -> 2 cards same, rest 3 diff
high -> all cards diff
*/

struct occur 
{
  int card;
  int count;
};

struct hand 
{
  char cards[MAXCARDS + 1];
  int bid;
  int type;
  struct occur occurences[MAXOCCUR];
  int dis;
};

enum type 
{
  HIGH = 1,
  ONEP,
  TWOP,
  THREEK,
  FULL,
  FOURK,
  FIVEK
};

int convert_alpha(char c)
{
  int n = 0;
  if(c == 'T') n = 10;
  else if(c == 'J') n = 1;
  else if(c == 'Q') n = 12;
  else if(c == 'K') n = 13;
  else if(c == 'A') n = 14;

  return n;
}

int convert(char c)
{
  int n = 0;
  if(isdigit(c))
  {
    n = c - '0';
  }
  else
  {
    n = convert_alpha(c);
  }
  return n;
}

struct char_buff 
{
  char str[MAXBUFF + 1];
  int ptr;
};

FILE *fptr;

void set_file(FILE *ptr)
{
  fptr = ptr;
}

void copy(char to[], char from[])
{
  for(int i=0; (to[i] = from[i]) != '\0'; i++);
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

int binary_search(int start, int end,  int d, struct hand *h, char c)
{
  int mid = (start + end) / 2;
  int data = h[mid].type;

  if(data > d)
    return binary_search(start, mid - 1, d, h, c);
  else if(data < d)
    return binary_search(mid + 1, end, d, h, c);
  else if(data == d)
  {
    if(c == 'f' || c == 'F')
    {
      int left_to_data = h[mid - 1].type;

      if(left_to_data == d)
        return binary_search(start, mid - 1, d, h, c);
    }
    else if(c == 'l' || c == 'L')
    {
      int right_to_data = h[mid + 1].type;

      if(right_to_data == d)
        return binary_search(mid + 1, end, d, h, c);

    }

    return mid;
  }

  return -1;
}

int parse(struct hand h[])
{
  struct char_buff *cards = (struct char_buff *) malloc(sizeof(struct char_buff));
  struct char_buff *bid = (struct char_buff *) malloc(sizeof(struct char_buff));
  int in_cards = 1;
  int idx = 0;
  char c;

  while((c = fgetc(fptr)) != EOF)
  {
    if(c != ' ')
    {
      if(in_cards)
      {
        cards -> str[cards -> ptr++] = c;
        cards -> str[cards -> ptr] = '\0';
      }
      else if(c != '\n')
      {
        bid -> str[bid -> ptr++] = c;
        bid -> str[bid -> ptr] = '\0';
      }
      else
      {
        h[idx].bid = str_to_num(bid -> str);
        bid -> ptr = 0;
        bid -> str[bid -> ptr] = '\0';
        idx++;
        in_cards = 1;
      }

    }
    else
    {
      copy(h[idx].cards, cards -> str);
      cards -> ptr = 0;
      cards -> str[cards -> ptr] = '\0';
      in_cards = 0;
    }
  }

  return idx;
}

void gen_type(struct hand *h)
{
  int counter[15] = {0};
  int dis = 0;
  int n = 0;

  for(int i = 0; i < MAXCARDS; i++)
  {
    char c = h -> cards[i];
    n = convert(c);
    counter[n]++;
  }

  int jack = 0;
  if(counter[1] != 0)
  {
    jack = counter[1];
    counter[1] = 0;
  }

  int max, m_idx;
  max = m_idx = 0;
  for(int i = 0; i < 15; i++)
  {
    if(counter[i] != 0)
    {
      if(counter[i] > max)
      {
        max = counter[i];
        m_idx = i;
      }
    }
  }

  if(jack)
  {
    counter[m_idx] += jack;
  }

  for(int i = 0; i < 15; i++)
  {
    if(counter[i] != 0)
    {
      h -> occurences[dis].card = i;
      h -> occurences[dis].count = counter[i];
      dis++;
    }
  }

  h -> dis = dis;

  int type = 0;
  if(dis == 5) type = HIGH;
  else if(dis == 4) type = ONEP;
  else if(dis == 3)
  {
    int max = 0;
    for(int i = 0; i < h -> dis; i++)
    {
      int count = h -> occurences[i].count; 
      if(count > max) max = count;
    }

    if(max == 3) type = THREEK;
    else type = TWOP;
  }
  else if(dis == 2)
  {
    int max = 0;
    for(int i = 0; i < h -> dis; i++)
    {
      int count = h -> occurences[i].count; 
      if(count > max) max = count;
    }

    if(max == 4) type = FOURK;
    else type = FULL;
  }
  else if(dis == 1) type = FIVEK;
  
  h -> type = type;
}

int comp(const void *a, const void *b)
{
  struct hand *f = (struct hand *) a;
  struct hand *s = (struct hand *) b;
  return f -> type - s -> type;
}

int comp_c(const void *a, const void *b)
{
  struct hand *f = (struct hand *) a;
  struct hand *s = (struct hand *) b;
  int nf, ns;
  char cf, cs;

  nf = ns = 0;

  for(int i = 0; i < MAXCARDS; i++)
  {
    cf = f -> cards[i];
    cs = s -> cards[i];
    if(cf == cs) continue;
    nf = convert(cf);
    ns = convert(cs);

    return nf - ns;
  }
}


int comp_cards(struct hand *a, struct hand *b)
{
  int na, nb;
  char ca, cb;

  na = nb = 0;

  for(int i = 0; i < MAXCARDS; i++)
  {
    ca = a -> cards[i];
    cb = b -> cards[i];
    if(ca == cb) continue;
    na = convert(ca);
    nb = convert(cb);

    return na - nb;
  }
}

int main()
{
  char filename[] = "day7.txt";
  FILE *input = fopen(filename, "r");
  struct hand hands[MAXHANDS];
  int n_hands = 0;
  set_file(input);
  n_hands = parse(hands);

  for(int i = 0; i < n_hands; i++)
  {
    gen_type(&hands[i]);
  }
  
  qsort(hands , n_hands, sizeof(struct hand), comp);

  int i, j, val;
  val = 0;
  i = 0;
  j = 1;

  while(j <= n_hands)
  {
    if(hands[i].type == hands[j].type)
    {
      int last_idx = -1;
      last_idx = binary_search(j, n_hands, hands[j].type, hands, 'l');
      j = last_idx;
      qsort(&hands[i], j - i, sizeof(struct hand), comp_c);
      j = i + 1;
    }

    i++;
    j++;
  }

  unsigned long winnings = 0;
  for(int i = 0; i < n_hands; i++)
  {
    winnings += (i + 1) * hands[i].bid;
    printf("%d: cards: %s, bid: %d, type: %d, wiinnings: %lu\n", i + 1, hands[i].cards, hands[i].bid, hands[i].type, winnings);
  }
  printf("winnings: %lu\n", winnings);
}
