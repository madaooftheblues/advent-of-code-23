#include<stdio.h>

#define MAXROWS 140
#define MAXCOLS 142
#define MAXGALAX 500
#define EMPTY_COLOR "\033[1;38;5;3m"
#define DEFAULT_COLOR "\033[0m"   


FILE *fptr;

struct point 
{
  int x;
  int y;
};

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

void print_matrix(char matrix[][MAXCOLS], int row_count)
{
  for(int i = 0; i < row_count; i++)
  {
    printf("%s", matrix[i]);
  }
}

void print_graph(int graph[][MAXCOLS - 2], int row_count, int r[], int c[])
{
  for(int i = 0; i < row_count; i++)
  {
    for(int j = 0; j < MAXCOLS - 2; j++)
    {
      if(graph[i][j] == '.')
      {
        if(r[i] || c[j])
          printf("%s%c%s", EMPTY_COLOR, graph[i][j], DEFAULT_COLOR);
        else
          printf("%c", graph[i][j]);

      }
      else
        printf("%d", graph[i][j]);
    }
    printf("\n");
  }
}

void zeros(int m[], int len)
{
  for(int i = 0; i < len; i++)
  {
    m[i] = 0;
  }
}


int parse(char matrix[][MAXCOLS])
{
  char *line = NULL;
  size_t bufsize = 0;
  ssize_t linelen;

  int row_count = 0;

  while((linelen = getline(&line, &bufsize, fptr) != -1))
  {
    copy(matrix[row_count++], line);
  }

  return row_count;
}

void generate_graph(int g[][MAXCOLS - 2], char m[][MAXCOLS], int row_count)
{
  int vertex_count = 1;
  for(int i = 0; i < row_count; i++)
  {
    for(int j = 0; j < MAXCOLS - 2; j++)
    {
      if(m[i][j] == '#') 
      {
        g[i][j] = vertex_count;
        vertex_count += 2;
      }
      else if(m[i][j] == '.') g[i][j] = m[i][j];
    }
  }
}

void space(int g[][MAXCOLS - 2], int rc, int r[], int c[])
{
  for (int i = 0; i < rc; i++)
  {
    int empty_row = 1;
    for (int j = 0; j < MAXCOLS - 2; j++)
    {
      if (g[i][j] != '.') 
      {
        empty_row = 0;
        break;
      }
    }

    r[i] = empty_row;
  }
  for (int j = 0; j < MAXCOLS - 2; j++)
  {
    int empty_col = 1;
    for (int i = 0; i < rc; i++)
    {
      if (g[i][j] != '.') 
      {
        empty_col = 0;
        break;
      }
    }

    c[j] = empty_col;
  }
}

int locate_galaxies(struct point galaxies[], int g[][MAXCOLS - 2], int rows)
{
  int galaxy_count = 0;
  for(int i = 0; i < rows; i++)
  {
    for(int j = 0; j < MAXCOLS - 2; j++)
    {
      if(g[i][j] != '.')
      {
        struct point *p = &galaxies[galaxy_count];
        p -> x = i;
        p -> y = j;
        galaxy_count++;
      }
    }
  }

  return galaxy_count;
}

int manhatt_dist(struct point *a, struct point *b)
{
  int v = a -> x - b -> x;
  int h = a -> y - b -> y;

  v = v < 0 ? -v : v;
  h = h < 0 ? -h : h;

  return v + h;
}

int get_distances(struct point galaxies[], int galaxy_count, int r[], int c[])
{
  int distance_sum = 0;
  int pair  = 0;
  for(int i = 0; i < galaxy_count; i++)
  {
    for(int j = 0; j < galaxy_count; j++)
    {
      if(j <= i) continue;
      int dist = manhatt_dist(&galaxies[i], &galaxies[j]);
      int vl, vs, hl, hs;
      if(galaxies[i].x >= galaxies[j].x)
      {
        vl = galaxies[i].x;
        vs = galaxies[j].x;
      }
      else
      {
        vl = galaxies[j].x;
        vs = galaxies[i].x;
      }
      if(galaxies[i].y >= galaxies[j].y)
      {
        hl = galaxies[i].y;
        hs = galaxies[j].y;
      }
      else
      {
        hl = galaxies[j].y;
        hs = galaxies[i].y;
      }

      for(int k = vs; k < vl;  k++)
      {
        if(r[k]) dist++;
      }
      for(int k = hs; k < hl; k++)
      {
        if(c[k]) dist++;
      }
      distance_sum += dist;
      pair++;
      printf("%d: (%d, %d) (%d, %d) --> %d\n", pair, galaxies[i].x, galaxies[i].y, galaxies[j].x, galaxies[j].y, dist);
    }
  }
  return distance_sum;
}

int main()
{
  char filename[] = "day11.txt";
  FILE *input = fopen(filename, "r");
  set_file(input);

  char matrix[MAXROWS][MAXCOLS];
  struct point galaxies[MAXGALAX];
  int distance_count;
  int row_count, galaxy_count;

  row_count = parse(matrix);
  print_matrix(matrix, row_count);
  
  int graph[row_count][MAXCOLS - 2];
  int empty_rows[row_count], empty_cols[MAXCOLS - 2];
  zeros(empty_rows, row_count);
  zeros(empty_cols, MAXCOLS - 2);

  generate_graph(graph, matrix, row_count);
  space(graph, row_count, empty_rows, empty_cols);

  galaxy_count = locate_galaxies(galaxies, graph, row_count);
  distance_count = get_distances(galaxies, galaxy_count, empty_rows, empty_cols);

  print_graph(graph, row_count, empty_rows, empty_cols);
  printf("%d\n", distance_count);
}
