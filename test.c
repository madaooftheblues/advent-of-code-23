#include<stdio.h>


int binary_search(int start, int end,  int d, int *h, char c)
{
  int mid = (start + end) / 2;
  int data = h[mid];

  if(data > d)
    return binary_search(start, mid - 1, d, h, c);
  else if(data < d)
    return binary_search(mid + 1, end, d, h, c);
  else if(data == d)
  {
    if(c == 'f' || c == 'F')
    {
      int left_to_data = h[mid - 1];

      if(left_to_data == d)
        return binary_search(start, mid - 1, d, h, c);
    }
    else if(c == 'l' || c == 'L')
    {
      int right_to_data = h[mid + 1];

      if(right_to_data == d)
        return binary_search(mid + 1, end, d, h, c);

    }

    return mid;
  }

  return -1;
}

int main()
{
  int arr[8] = {0, 0, 1, 3, 3, 3, 8, 9 };
  int target = 3;
  int normal_index, first_index, last_index;

  normal_index = binary_search(0, sizeof(arr) / sizeof(int), target, arr, 'n');
  first_index = binary_search(0, sizeof(arr) / sizeof(int), target, arr, 'f');
  last_index = binary_search(0, sizeof(arr) / sizeof(int), target, arr, 'l');

  printf("normal index: %d\n", normal_index);
  printf("first index: %d\n", first_index);
  printf("last index: %d\n", last_index);

}
