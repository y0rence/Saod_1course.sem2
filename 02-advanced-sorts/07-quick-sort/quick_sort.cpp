#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <SFML/Graphics/Image.hpp>

#define MAX_N 500
#define SIZE_COUNT 5

typedef long long Count;

typedef struct {
  Count cmp;
  Count mov;
} Stats;

void copy_array(int dst[], const int src[], int n) {
  int i;
  for (i = 1; i <= n; ++i)
    dst[i] = src[i];
}

Count checksum(const int a[], int n) {
  Count s = 0;
  int i;
  for (i = 1; i <= n; ++i)
    s += a[i];
  return s;
}

int count_series(const int a[], int n) {
  int i, k = 1;
  if (n <= 0)
    return 0;
  for (i = 2; i <= n; ++i)
    if (a[i - 1] > a[i])
      k++;
  return k;
}

void make_sorted(int a[], int n) {
  int i;
  for (i = 1; i <= n; ++i)
    a[i] = i;
}

void make_random(int a[], int n) {
  int i, j, t;
  for (i = 1; i <= n; ++i)
    a[i] = i;
  for (i = n; i > 1; --i) {
    j = 1 + rand() % i;
    t = a[i];
    a[i] = a[j];
    a[j] = t;
  }
}

void print_chars(const int a[], int n) {
  int i;
  for (i = 1; i <= n; ++i) {
    if (i > 1)
      printf(" ");
    printf("%c", (char)a[i]);
  }
  printf("\n");
}

void quick_sort_v1(int a[], int L, int R, Stats *st) {
  int x, i, j, t;

  x = a[L];
  i = L;
  j = R;

  do {
    while (st->cmp++, a[i] < x)
      i++;
    while (st->cmp++, a[j] > x)
      j--;

    if (i <= j) {
      t = a[i];
      a[i] = a[j];
      a[j] = t;
      st->mov += 3;
      i++;
      j--;
    }
  } while (i <= j);

  if (L < j)
    quick_sort_v1(a, L, j, st);
  if (i < R)
    quick_sort_v1(a, i, R, st);
}

void quick_sort_v2(int a[], int L, int R, Stats *st) {
  int x, i, j, t;

  while (L < R) {
    x = a[L];
    i = L;
    j = R;

    do {
      while (st->cmp++, a[i] < x)
        i++;
      while (st->cmp++, a[j] > x)
        j--;

      if (i <= j) {
        t = a[i];
        a[i] = a[j];
        a[j] = t;
        st->mov += 3;
        i++;
        j--;
      }
    } while (i <= j);

    if (j - L > R - i) {
      if (i < R)
        quick_sort_v2(a, i, R, st);
      R = j;
    } else {
      if (L < j)
        quick_sort_v2(a, L, j, st);
      L = i;
    }
  }
}

void shell_sort(int a[], int n, Stats *st) {
  int h[32];
  int m, s, k, i, j, t;

  m = n < 4 ? 1 : (int)floor(log2((double)n)) - 1;
  h[0] = 1;
  for (s = 1; s < m; ++s)
    h[s] = 2 * h[s - 1] + 1;

  for (s = m - 1; s >= 0; --s) {
    k = h[s];
    for (i = k + 1; i <= n; ++i) {
      t = a[i];
      st->mov++;
      j = i - k;
      while (j > 0 && (st->cmp++, t < a[j])) {
        a[j + k] = a[j];
        st->mov++;
        j -= k;
      }
      a[j + k] = t;
      st->mov++;
    }
  }
}

void sift_down(int a[], int L, int R, Stats *st) {
  int x = a[L];
  int i = L;
  int j;

  st->mov++;
  while (1) {
    j = 2 * i;
    if (j > R)
      break;

    if (j < R) {
      st->cmp++;
      if (a[j + 1] <= a[j])
        j++;
    }

    st->cmp++;
    if (x <= a[j])
      break;

    a[i] = a[j];
    st->mov++;
    i = j;
  }
  a[i] = x;
  st->mov++;
}

void heap_sort(int a[], int n, Stats *st) {
  int L, R, t;

  for (L = n / 2; L > 0; --L)
    sift_down(a, L, n, st);

  for (R = n; R > 1; --R) {
    t = a[1];
    a[1] = a[R];
    a[R] = t;
    st->mov += 3;
    sift_down(a, 1, R - 1, st);
  }
}

static int g_trace_step = 0;
static int g_trace_n = 0;

void quick_sort_trace(int a[], int L, int R) {
  int x, i, j, t;

  x = a[L];
  i = L;
  j = R;

  do {
    while (a[i] < x)
      i++;
    while (a[j] > x)
      j--;

    if (i <= j) {
      t = a[i];
      a[i] = a[j];
      a[j] = t;
      i++;
      j--;
    }
  } while (i <= j);

  printf("  Шаг %2d: [%2d..%2d] x=%c  ->  ", ++g_trace_step, L, R, (char)x);
  print_chars(a, g_trace_n);

  if (L < j)
    quick_sort_trace(a, L, j);
  if (i < R)
    quick_sort_trace(a, i, R);
}

void task1(void) {
  int a[] = {0, 'O', 'B', 'E', 'R', 'E', 'M', 'O', 'K', 'S', 'E', 'R', 'G'};
  int n = 12;

  g_trace_step = 0;
  g_trace_n = n;

  printf("===== Задание 1: QuickSort для 12 символов ФИО =====\n");
  printf("Исходный массив:  ");
  print_chars(a, n);
  printf("Трассировка (каждая строка - после разбиения на части):\n");

  quick_sort_trace(a, 1, n);

  printf("Результат:        ");
  print_chars(a, n);
  printf("\n");
}

void task2(void) {
  int work[MAX_N + 1];
  int src[MAX_N + 1];
  int n = 20;
  Stats st;
  Count sum_before, sum_after;
  int ser_before, ser_after;
  int i;

  printf("===== Задание 2: QuickSort v1, проверка, Сф и Мф =====\n");

  make_random(src, n);
  copy_array(work, src, n);

  sum_before = checksum(src, n);
  ser_before = count_series(src, n);

  st.cmp = 0;
  st.mov = 0;
  quick_sort_v1(work, 1, n, &st);

  sum_after = checksum(work, n);
  ser_after = count_series(work, n);

  printf("n=%d, случайный массив\n", n);
  printf("  До:     ");
  for (i = 1; i <= n; ++i)
    printf("%3d", src[i]);
  printf("\n");
  printf("  После:  ");
  for (i = 1; i <= n; ++i)
    printf("%3d", work[i]);
  printf("\n");
  printf("  Контрольная сумма: %lld -> %lld  (%s)\n", sum_before, sum_after,
         sum_before == sum_after ? "OK" : "ОШИБКА!");
  printf("  Число серий:       %d -> %d  (%s)\n", ser_before, ser_after,
         ser_after == 1 ? "OK (1 серия = отсортировано)" : "ОШИБКА!");
  printf("  Сф (сравнений) = %lld\n", st.cmp);
  printf("  Мф (пересылок) = %lld\n", st.mov);
  printf("\n");
}

Count theory_c_worst(int n) { return ((Count)n * n + 5 * n + 4) / 2; }

Count theory_m_worst(int n) { return 3 * (n - 1); }

void task3(void) {
  int sizes[SIZE_COUNT] = {100, 200, 300, 400, 500};
  int work[MAX_N + 1];
  int src[MAX_N + 1];
  Stats st;
  int idx, n;

  printf("===== Задание 3: Трудоемкость QuickSort v1 =====\n");

  printf("\nУбыч - случайный массив:\n");
  printf("%-6s  %-10s  %-10s\n", "n", "Сф", "Мф");
  printf("%-6s  %-10s  %-10s\n", "------", "----------", "----------");

  for (idx = 0; idx < SIZE_COUNT; ++idx) {
    n = sizes[idx];
    make_random(src, n);
    copy_array(work, src, n);
    st.cmp = 0;
    st.mov = 0;
    quick_sort_v1(work, 1, n, &st);
    printf("%-6d  %-10lld  %-10lld\n", n, st.cmp, st.mov);
  }

  printf("\nВор - отсортированный массив (X = min, формулы слайд 13):\n");
  printf("%-6s  %-10s  %-10s  %-10s  %-10s\n", "n", "Сф", "С_теор", "Мф",
         "М_теор");
  printf("%-6s  %-10s  %-10s  %-10s  %-10s\n", "------", "----------",
         "----------", "----------", "----------");

  for (idx = 0; idx < SIZE_COUNT; ++idx) {
    n = sizes[idx];
    make_sorted(src, n);
    copy_array(work, src, n);
    st.cmp = 0;
    st.mov = 0;
    quick_sort_v1(work, 1, n, &st);
    printf("%-6d  %-10lld  %-10lld  %-10lld  %-10lld\n", n, st.cmp,
           theory_c_worst(n), st.mov, theory_m_worst(n));
  }
  printf("\n");
}

static int g_depth_cur = 0;
static int g_depth_max = 0;

void quick_sort_v1_depth(int a[], int L, int R) {
  int x, i, j, t;

  g_depth_cur++;
  if (g_depth_cur > g_depth_max)
    g_depth_max = g_depth_cur;

  x = a[L];
  i = L;
  j = R;
  do {
    while (a[i] < x)
      i++;
    while (a[j] > x)
      j--;
    if (i <= j) {
      t = a[i];
      a[i] = a[j];
      a[j] = t;
      i++;
      j--;
    }
  } while (i <= j);

  if (L < j)
    quick_sort_v1_depth(a, L, j);
  if (i < R)
    quick_sort_v1_depth(a, i, R);

  g_depth_cur--;
}

void quick_sort_v2_depth(int a[], int L, int R) {
  int x, i, j, t;

  g_depth_cur++;
  if (g_depth_cur > g_depth_max)
    g_depth_max = g_depth_cur;

  while (L < R) {
    x = a[L];
    i = L;
    j = R;
    do {
      while (a[i] < x)
        i++;
      while (a[j] > x)
        j--;
      if (i <= j) {
        t = a[i];
        a[i] = a[j];
        a[j] = t;
        i++;
        j--;
      }
    } while (i <= j);

    if (j - L > R - i) {
      if (i < R)
        quick_sort_v2_depth(a, i, R);
      R = j;
    } else {
      if (L < j)
        quick_sort_v2_depth(a, L, j);
      L = i;
    }
  }

  g_depth_cur--;
}

void task4(void) {
  int sizes[SIZE_COUNT] = {100, 200, 300, 400, 500};
  int work[MAX_N + 1];
  int src[MAX_N + 1];
  Stats st;
  int idx, n;

  printf("===== Задание 4*: Сравнение v1 и v2 =====\n");

  printf("Трудоемкость, случайный массив:\n");
  printf("%-6s  %-10s  %-10s  %-10s  %-10s\n", "n", "v1 Сф", "v1 Мф", "v2 Сф",
         "v2 Мф");
  printf("%-6s  %-10s  %-10s  %-10s  %-10s\n", "------", "----------",
         "----------", "----------", "----------");

  for (idx = 0; idx < SIZE_COUNT; ++idx) {
    Stats st1, st2;

    n = sizes[idx];
    make_random(src, n);

    copy_array(work, src, n);
    st1.cmp = 0;
    st1.mov = 0;
    quick_sort_v1(work, 1, n, &st1);

    copy_array(work, src, n);
    st2.cmp = 0;
    st2.mov = 0;
    quick_sort_v2(work, 1, n, &st2);

    printf("%-6d  %-10lld  %-10lld  %-10lld  %-10lld\n", n, st1.cmp, st1.mov,
           st2.cmp, st2.mov);
  }

  printf("\n");

  printf("Глубина рекурсии (случайный массив):\n");
  printf("%-6s  %-14s  %-14s  %-14s\n", "n", "v1 случ.", "v1 отсорт.",
         "v2 случ.");
  printf("%-6s  %-14s  %-14s  %-14s\n", "------", "--------------",
         "--------------", "--------------");

  for (idx = 0; idx < SIZE_COUNT; ++idx) {
    int d_v1_rnd, d_v1_srt, d_v2_rnd;

    n = sizes[idx];

    make_random(src, n);
    copy_array(work, src, n);
    g_depth_cur = 0;
    g_depth_max = 0;
    quick_sort_v1_depth(work, 1, n);
    d_v1_rnd = g_depth_max;

    make_sorted(src, n);
    copy_array(work, src, n);
    g_depth_cur = 0;
    g_depth_max = 0;
    quick_sort_v1_depth(work, 1, n);
    d_v1_srt = g_depth_max;

    make_random(src, n);
    copy_array(work, src, n);
    g_depth_cur = 0;
    g_depth_max = 0;
    quick_sort_v2_depth(work, 1, n);
    d_v2_rnd = g_depth_max;

    printf("%-6d  %-14d  %-14d  %-14d\n", n, d_v1_rnd, d_v1_srt, d_v2_rnd);
  }
  printf("\n");
}

void put_pixel(sf::Image &img, int x, int y, sf::Color c) {
  if (x < 0 || y < 0)
    return;
  if (x >= (int)img.getSize().x || y >= (int)img.getSize().y)
    return;
  img.setPixel({(unsigned)x, (unsigned)y}, c);
}

void draw_line(sf::Image &img, int x1, int y1, int x2, int y2, sf::Color c) {
  int dx = abs(x2 - x1), dy = abs(y2 - y1);
  int sx = x1 < x2 ? 1 : -1, sy = y1 < y2 ? 1 : -1;
  int err = dx - dy;

  while (1) {
    put_pixel(img, x1, y1, c);
    if (x1 == x2 && y1 == y2)
      break;
    int e2 = 2 * err;
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

void draw_dot(sf::Image &img, int x, int y, int r, sf::Color c) {
  int dx, dy;
  for (dy = -r; dy <= r; ++dy)
    for (dx = -r; dx <= r; ++dx)
      if (dx * dx + dy * dy <= r * r)
        put_pixel(img, x + dx, y + dy, c);
}

void task5(void) {
  int sizes[SIZE_COUNT] = {100, 200, 300, 400, 500};
  Count qs[SIZE_COUNT], sh[SIZE_COUNT], hs[SIZE_COUNT];
  int src[MAX_N + 1], work[MAX_N + 1];
  Stats st;
  int idx, n;

  printf("===== Задание 5*: трудоемкость QS/Shell/Heap (случайный массив) "
         "=====\n");
  printf("%-6s  %-14s  %-14s  %-14s\n", "n", "QuickSort", "ShellSort",
         "HeapSort");
  printf("%-6s  %-14s  %-14s  %-14s\n", "------", "--------------",
         "--------------", "--------------");

  for (idx = 0; idx < SIZE_COUNT; ++idx) {
    n = sizes[idx];
    make_random(src, n);

    copy_array(work, src, n);
    st.cmp = 0;
    st.mov = 0;
    quick_sort_v1(work, 1, n, &st);
    qs[idx] = st.cmp + st.mov;

    copy_array(work, src, n);
    st.cmp = 0;
    st.mov = 0;
    shell_sort(work, n, &st);
    sh[idx] = st.cmp + st.mov;

    copy_array(work, src, n);
    st.cmp = 0;
    st.mov = 0;
    heap_sort(work, n, &st);
    hs[idx] = st.cmp + st.mov;

    printf("%-6d  %-14lld  %-14lld  %-14lld\n", n, qs[idx], sh[idx], hs[idx]);
  }
  printf("\n");

  const int W = 800, H = 520;
  const float left = 70, right = 30, top = 30, bot = 60;

  sf::Image img({(unsigned)W, (unsigned)H}, sf::Color::White);

  draw_line(img, (int)left, (int)top, (int)left, H - (int)bot,
            sf::Color::Black);
  draw_line(img, (int)left, H - (int)bot, W - (int)right, H - (int)bot,
            sf::Color::Black);

  Count max_val = 0;
  for (idx = 0; idx < SIZE_COUNT; ++idx) {
    if (qs[idx] > max_val)
      max_val = qs[idx];
    if (sh[idx] > max_val)
      max_val = sh[idx];
    if (hs[idx] > max_val)
      max_val = hs[idx];
  }

  float xw = W - left - right;
  float yw = H - top - bot;

  sf::Color col_qs = sf::Color(30, 120, 200);
  sf::Color col_sh = sf::Color(200, 80, 30);
  sf::Color col_hs = sf::Color(40, 160, 60);

  for (idx = 1; idx < SIZE_COUNT; ++idx) {
    float x0 = left + (idx - 1) * xw / (SIZE_COUNT - 1);
    float x1 = left + idx * xw / (SIZE_COUNT - 1);

    float yqs0 = H - bot - yw * qs[idx - 1] / max_val;
    float yqs1 = H - bot - yw * qs[idx] / max_val;
    float ysh0 = H - bot - yw * sh[idx - 1] / max_val;
    float ysh1 = H - bot - yw * sh[idx] / max_val;
    float yhs0 = H - bot - yw * hs[idx - 1] / max_val;
    float yhs1 = H - bot - yw * hs[idx] / max_val;

    draw_line(img, (int)x0, (int)yqs0, (int)x1, (int)yqs1, col_qs);
    draw_line(img, (int)x0, (int)ysh0, (int)x1, (int)ysh1, col_sh);
    draw_line(img, (int)x0, (int)yhs0, (int)x1, (int)yhs1, col_hs);
  }

  for (idx = 0; idx < SIZE_COUNT; ++idx) {
    float x0 = left + idx * xw / (SIZE_COUNT - 1);
    draw_dot(img, (int)x0, (int)(H - bot - yw * qs[idx] / max_val), 4, col_qs);
    draw_dot(img, (int)x0, (int)(H - bot - yw * sh[idx] / max_val), 4, col_sh);
    draw_dot(img, (int)x0, (int)(H - bot - yw * hs[idx] / max_val), 4, col_hs);
  }

  draw_dot(img, W - (int)right - 160, (int)top + 12, 4, col_qs);
  draw_dot(img, W - (int)right - 160, (int)top + 28, 4, col_sh);
  draw_dot(img, W - (int)right - 160, (int)top + 44, 4, col_hs);

  if (!img.saveToFile("qs_shell_heap.png")) {
    printf("Ошибка сохранения графика.\n");
  } else {
    printf("График сохранён в qs_shell_heap.png\n");
    printf("  Синий  = QuickSort\n");
    printf("  Оранж. = ShellSort\n");
    printf("  Зелен. = HeapSort\n");
  }
  printf("\n");
}

int main(void) {
  srand((unsigned)time(NULL));

  task1();
  task2();
  task3();
  task4();
  task5();

  return 0;
}
