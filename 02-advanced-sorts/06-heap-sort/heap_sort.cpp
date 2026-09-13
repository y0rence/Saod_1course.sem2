#include <algorithm>
#include <math.h>
#include <random>
#include <stdio.h>
#include <stdlib.h>

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Image.hpp>

#define MAX_N 500
#define SIZE_COUNT 5

typedef long long Count;

typedef struct Row {
  int n;
  Count theory;
  Count dec;
  Count rnd;
  Count inc;
} Row;

typedef struct Check {
  Count before;
  Count after;
  int series;
} Check;

typedef struct Point {
  int n;
  Count heap;
  Count shell;
} Point;

void copy_array(int dst[], const int src[], int n) {
  int i;

  for (i = 1; i <= n; ++i) {
    dst[i] = src[i];
  }
}

void swap_values(int* x, int* y, Count* total) {
  int t = *x;
  *x = *y;
  *y = t;
  *total += 3;
}

void build_heap_part(int a[], int L, int R, Count* total) {
  int x = a[L];
  int i = L;
  (*total)++;

  while (1) {
    int j = 2 * i;

    if (j > R) {
      break;
    }

    if (j < R) {
      (*total)++;
      if (a[j + 1] <= a[j]) {
        j++;
      }
    }

    (*total)++;
    if (x <= a[j]) {
      break;
    }

    a[i] = a[j];
    (*total)++;
    i = j;
  }

  a[i] = x;
  (*total)++;
}

void build_heap(int a[], int n, Count* total) {
  int L;

  for (L = n / 2; L > 0; --L) {
    build_heap_part(a, L, n, total);
  }
}

void heap_sort(int a[], int n, Count* total) {
  int R;

  *total = 0;
  build_heap(a, n, total);

  for (R = n; R > 1; --R) {
    swap_values(&a[1], &a[R], total);
    build_heap_part(a, 1, R - 1, total);
  }
}

void shell_sort(int a[], int n, Count* total) {
  int h[32];
  int m;
  int s;

  *total = 0;
  m = n < 4 ? 1 : (int)floor(log2((double)n)) - 1;
  h[0] = 1;
  for (s = 1; s < m; ++s) {
    h[s] = 2 * h[s - 1] + 1;
  }

  for (s = m - 1; s >= 0; --s) {
    int k = h[s];
    int i;

    for (i = k + 1; i <= n; ++i) {
      int t = a[i];
      int j = i - k;
      (*total)++;

      while (j > 0) {
        (*total)++;
        if (!(t < a[j])) {
          break;
        }

        a[j + k] = a[j];
        (*total)++;
        j -= k;
      }

      a[j + k] = t;
      (*total)++;
    }
  }
}

void make_increasing(int a[], int n) {
  int i;

  for (i = 1; i <= n; ++i) {
    a[i] = i;
  }
}

void make_decreasing(int a[], int n) {
  int i;

  for (i = 1; i <= n; ++i) {
    a[i] = n - i + 1;
  }
}

void make_random(int a[], int n, std::mt19937& gen) {
  make_increasing(a, n);
  std::shuffle(a + 1, a + n + 1, gen);
}

Count checksum(const int a[], int n) {
  Count sum = 0;
  int i;

  for (i = 1; i <= n; ++i) {
    sum += a[i];
  }

  return sum;
}

int series(const int a[], int n) {
  int i;
  int k = 1;

  if (n <= 0) {
    return 0;
  }

  for (i = 2; i <= n; ++i) {
    if (a[i - 1] < a[i]) {
      k++;
    }
  }

  return k;
}

Check make_check(const int before[], const int after[], int n) {
  Check result;

  result.before = checksum(before, n);
  result.after = checksum(after, n);
  result.series = series(after, n);
  return result;
}

int floor_log2_ratio(int R, int L) {
  return (int)floor(log2((double)R / (double)L) + 1e-12);
}

Count build_theory(int n) {
  Count total = 0;
  int L;

  for (L = n / 2; L >= 1; --L) {
    int k = floor_log2_ratio(n, L);
    total += 3LL * k + 2LL;
  }

  return total;
}

Count sort_theory(int n) {
  double x = (double)n;
  double lg = log2(x);
  Count c = (Count)ceil(2.0 * x * lg + x + 2.0 - 1e-12);
  Count m = (Count)ceil(x * lg + 6.5 * x - 4.0 - 1e-12);

  return c + m;
}

Count build_total(const int source[], int n) {
  int work[MAX_N + 1];
  Count total = 0;

  copy_array(work, source, n);
  build_heap(work, n, &total);
  return total;
}

Count heap_total(const int source[], int n, Check* check) {
  int work[MAX_N + 1];
  Count total = 0;

  copy_array(work, source, n);
  heap_sort(work, n, &total);

  if (check != NULL) {
    *check = make_check(source, work, n);
  }

  return total;
}

Count shell_total(const int source[], int n) {
  int work[MAX_N + 1];
  Count total = 0;

  copy_array(work, source, n);
  shell_sort(work, n, &total);
  return total;
}

void print_chars(const int a[], int n) {
  int i;

  for (i = 1; i <= n; ++i) {
    if (i > 1) {
      printf(" ");
    }
    printf("%c", a[i]);
  }
}

float graph_x(int n, int min_n, int max_n, float left, float right, float width) {
  return left + (float)(n - min_n) * (width - left - right) / (float)(max_n - min_n);
}

float graph_y(Count value, Count max_value, float top, float bottom, float height) {
  return height - bottom - (float)value * (height - top - bottom) / (float)max_value;
}

void put_pixel(sf::Image& image, int x, int y, sf::Color color) {
  if (x < 0 || y < 0) {
    return;
  }

  if (x >= (int)image.getSize().x || y >= (int)image.getSize().y) {
    return;
  }

  image.setPixel({(unsigned)x, (unsigned)y}, color);
}

void draw_line(sf::Image& image, int x1, int y1, int x2, int y2, sf::Color color) {
  int dx = abs(x2 - x1);
  int dy = abs(y2 - y1);
  int sx = x1 < x2 ? 1 : -1;
  int sy = y1 < y2 ? 1 : -1;
  int err = dx - dy;

  while (1) {
    int e2 = 2 * err;

    put_pixel(image, x1, y1, color);
    if (x1 == x2 && y1 == y2) {
      break;
    }

    if (e2 > -dy) {
      err -= dy;
      x1 += sx;
    }
    if (e2 < dx) {
      err += dx;
      y1 += sy;
    }
  }
}

void draw_dot(sf::Image& image, int x, int y, int r, sf::Color color) {
  int dx;
  int dy;

  for (dy = -r; dy <= r; ++dy) {
    for (dx = -r; dx <= r; ++dx) {
      if (dx * dx + dy * dy <= r * r) {
        put_pixel(image, x + dx, y + dy, color);
      }
    }
  }
}

void save_graph(const Point points[], int count) {
  const int width = 900;
  const int height = 600;
  const float left = 90.0f;
  const float right = 40.0f;
  const float top = 50.0f;
  const float bottom = 70.0f;

  int i;
  int min_n = points[0].n;
  int max_n = points[count - 1].n;
  Count max_value = 0;
  sf::Image image({(unsigned)width, (unsigned)height}, sf::Color::White);

  for (i = 0; i < count; ++i) {
    if (points[i].heap > max_value) {
      max_value = points[i].heap;
    }
    if (points[i].shell > max_value) {
      max_value = points[i].shell;
    }
  }

  for (i = 0; i <= 5; ++i) {
    float y = graph_y(max_value * i / 5, max_value, top, bottom, (float)height);
    draw_line(image, (int)left, (int)y, (int)(width - right), (int)y, sf::Color(208, 208, 208));
  }

  for (i = 0; i < count; ++i) {
    float x = graph_x(points[i].n, min_n, max_n, left, right, (float)width);
    draw_line(image, (int)x, (int)top, (int)x, (int)(height - bottom), sf::Color(230, 230, 230));
  }

  draw_line(image, (int)left, (int)top, (int)left, (int)(height - bottom), sf::Color::Black);
  draw_line(image,
            (int)left,
            (int)(height - bottom),
            (int)(width - right),
            (int)(height - bottom),
            sf::Color::Black);

  for (i = 1; i < count; ++i) {
    draw_line(image,
              (int)graph_x(points[i - 1].n, min_n, max_n, left, right, (float)width),
              (int)graph_y(points[i - 1].heap, max_value, top, bottom, (float)height),
              (int)graph_x(points[i].n, min_n, max_n, left, right, (float)width),
              (int)graph_y(points[i].heap, max_value, top, bottom, (float)height),
              sf::Color(11, 110, 79));

    draw_line(image,
              (int)graph_x(points[i - 1].n, min_n, max_n, left, right, (float)width),
              (int)graph_y(points[i - 1].shell, max_value, top, bottom, (float)height),
              (int)graph_x(points[i].n, min_n, max_n, left, right, (float)width),
              (int)graph_y(points[i].shell, max_value, top, bottom, (float)height),
              sf::Color(200, 76, 9));
  }

  for (i = 0; i < count; ++i) {
    draw_dot(image,
             (int)graph_x(points[i].n, min_n, max_n, left, right, (float)width),
             (int)graph_y(points[i].heap, max_value, top, bottom, (float)height),
             5,
             sf::Color(11, 110, 79));
    draw_dot(image,
             (int)graph_x(points[i].n, min_n, max_n, left, right, (float)width),
             (int)graph_y(points[i].shell, max_value, top, bottom, (float)height),
             5,
             sf::Color(200, 76, 9));
  }

  if (!image.saveToFile("heap_shell_random.png")) {
  }
}

int main() {
  int sizes[SIZE_COUNT] = {100, 200, 300, 400, 500};
  Row build_rows[SIZE_COUNT];
  Row sort_rows[SIZE_COUNT];
  Check dec_checks[SIZE_COUNT];
  Check inc_checks[SIZE_COUNT];
  Check rnd_checks[SIZE_COUNT];
  Point graph[SIZE_COUNT];

  int inc[MAX_N + 1];
  int dec[MAX_N + 1];
  int rnd[MAX_N + 1];
  int size_index;
  std::mt19937 gen(42);

  for (size_index = 0; size_index < SIZE_COUNT; ++size_index) {
    int n = sizes[size_index];

    make_increasing(inc, n);
    make_decreasing(dec, n);
    make_random(rnd, n, gen);

    build_rows[size_index].n = n;
    build_rows[size_index].theory = build_theory(n);
    build_rows[size_index].dec = build_total(dec, n);
    build_rows[size_index].rnd = build_total(rnd, n);
    build_rows[size_index].inc = build_total(inc, n);

    sort_rows[size_index].n = n;
    sort_rows[size_index].theory = sort_theory(n);
    sort_rows[size_index].dec = heap_total(dec, n, &dec_checks[size_index]);
    sort_rows[size_index].inc = heap_total(inc, n, &inc_checks[size_index]);
    sort_rows[size_index].rnd = heap_total(rnd, n, &rnd_checks[size_index]);

    graph[size_index].n = n;
    graph[size_index].heap = sort_rows[size_index].rnd;
    graph[size_index].shell = shell_total(rnd, n);
  }

  save_graph(graph, SIZE_COUNT);

  {
    int letters[] = {0, 'O', 'B', 'E', 'R', 'E', 'M', 'O', 'K', 'S', 'E', 'R', 'G'};
    int n = 12;
    int step;
    int R;
    Count total = 0;

    printf("TASK 1\n");
    printf("Start: ");
    print_chars(letters, n);
    printf("\n");

    build_heap(letters, n, &total);
    printf("Heap:  ");
    print_chars(letters, n);
    printf("\n");

    for (step = 1, R = n; R > 1; --R, ++step) {
      swap_values(&letters[1], &letters[R], &total);
      build_heap_part(letters, 1, R - 1, &total);
      printf("Step %d: ", step);
      print_chars(letters, n);
      printf("\n");
    }
    printf("\n");
  }

  printf("TASK 2\n");
  printf("%-8s%-18s%-18s%-18s%-18s\n", "N", "Theory", "Decreasing", "Random", "Increasing");
  for (size_index = 0; size_index < SIZE_COUNT; ++size_index) {
    printf("%-8d%-18lld%-18lld%-18lld%-18lld\n",
           build_rows[size_index].n,
           build_rows[size_index].theory,
           build_rows[size_index].dec,
           build_rows[size_index].rnd,
           build_rows[size_index].inc);
  }
  printf("\n");

  printf("TASK 3\n");
  for (size_index = 0; size_index < SIZE_COUNT; ++size_index) {
    printf("n=%-3d  %-10s checksum: %lld -> %lld, series(desc): %d\n",
           sizes[size_index],
           "decreasing",
           dec_checks[size_index].before,
           dec_checks[size_index].after,
           dec_checks[size_index].series);
    printf("n=%-3d  %-10s checksum: %lld -> %lld, series(desc): %d\n",
           sizes[size_index],
           "increasing",
           inc_checks[size_index].before,
           inc_checks[size_index].after,
           inc_checks[size_index].series);
    printf("n=%-3d  %-10s checksum: %lld -> %lld, series(desc): %d\n",
           sizes[size_index],
           "random",
           rnd_checks[size_index].before,
           rnd_checks[size_index].after,
           rnd_checks[size_index].series);
  }
  printf("\n");

  printf("TASK 4\n");
  printf("%-8s%-18s%-18s%-18s%-18s\n", "N", "Theory", "Decreasing", "Increasing", "Random");
  for (size_index = 0; size_index < SIZE_COUNT; ++size_index) {
    printf("%-8d%-18lld%-18lld%-18lld%-18lld\n",
           sort_rows[size_index].n,
           sort_rows[size_index].theory,
           sort_rows[size_index].dec,
           sort_rows[size_index].inc,
           sort_rows[size_index].rnd);
  }
  printf("\n");

  printf("TASK 5\n");
  printf("%-8s%-18s%-18s\n", "N", "HeapRandom", "ShellRandom");
  for (size_index = 0; size_index < SIZE_COUNT; ++size_index) {
    printf("%-8d%-18lld%-18lld\n",
           graph[size_index].n,
           graph[size_index].heap,
           graph[size_index].shell);
  }

  return 0;
}
