#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>
#define MAXLINE 1000

int main()
{
  char str[MAXLINE];
  FILE *input = fopen("day1.txt", "r");
  long sum_cals = 0;

  while(fgets(str, MAXLINE, input) != NULL)
  {
    int l, r;
    char digit[2];
    digit[0] = digit[1] = '\0';
    l = 0;
    r = strlen(str) - 1;

    while(str[l] != '\0' && str[r] > 0)
    {
      if(isdigit(str[l]))
        digit[0] = str[l];
      else
        l++;

      if(isdigit(str[r]))
        digit[1] = str[r];
      else
        r--;
      if(digit[0] != '\0' && digit[1] != '\0') break;
    }
    int number = ((digit[0] - '0') * 10) + ((digit[1] - '0'));
    sum_cals += number;
  }

  printf("%li\n", sum_cals);
}
