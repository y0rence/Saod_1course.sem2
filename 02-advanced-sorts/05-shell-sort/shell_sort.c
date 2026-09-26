#if defined(__has_include)
#if __has_include(<CSFML/Graphics.h>)
#define HAS_SFML 1
#include <CSFML/Graphics.h>
#elif __has_include(<SFML/Graphics.h>)
#define HAS_SFML 1
#include <SFML/Graphics.h>
#else
#define HAS_SFML 0
#endif
#else
#define HAS_SFML 0
#endif
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define METHOD_COUNT 5

enum {
    METHOD_SELECT = 0,
    METHOD_BUBBLE = 1,
    METHOD_SHAKER = 2,
    METHOD_INSERT = 3,
    METHOD_SHELL = 4
};

typedef struct {
    long long moves;
    long long comps;
} Stats;

static void copy_array(int *dst, const int *src, int n) {
    for (int i = 0; i < n; ++i) {
        dst[i] = src[i];
    }
}

static long long control_sum(const int *a, int n) {
    long long s = 0;
    for (int i = 0; i < n; ++i) {
        s += a[i];
    }
    return s;
}

static int series_count(const int *a, int n) {
    if (n <= 0)
        return 0;
    int count = 1;
    for (int i = 1; i < n; ++i) {
        if (a[i] < a[i - 1]) {
            count++;
        }
    }
    return count;
}

static int generate_knuth_gaps(int n, int *gaps, int max_count) {
    int m = n < 4 ? 1 : (int)floor(log2((double)n)) - 1;
    if (m > max_count)
        m = max_count;

    gaps[0] = 1;
    for (int i = 1; i < m; ++i) {
        gaps[i] = 2 * gaps[i - 1] + 1;
    }
    return m;
}

static int generate_tokuda_gaps(int n, int *gaps, int max_count) {
    int k = 0;
    while (k < max_count) {
        int h = (int)ceil((9.0 * pow(2.25, k) - 4.0) / 5.0);
        if (h >= n)
            break;
        gaps[k++] = h;
    }
    return k;
}

static void shell_sort_with_gaps(int *a, int n, const int *gaps, int m, Stats *st) {
    for (int gi = m - 1; gi >= 0; --gi) {
        int k = gaps[gi];
        for (int i = k; i < n; ++i) {
            int t = a[i];
            st->moves++;
            int j = i - k;
            while (j >= 0) {
                st->comps++;
                if (t < a[j]) {
                    a[j + k] = a[j];
                    st->moves++;
                    j -= k;
                } else {
                    break;
                }
            }
            a[j + k] = t;
            st->moves++;
        }
    }
}

static void shell_sort_halving(int *a, int n, Stats *st) {
    int gaps[32];
    int m = 0;
    for (int k = n / 2; k > 0; k /= 2) {
        gaps[m++] = k;
    }
    for (int i = 0; i < m / 2; ++i) {
        int t = gaps[i];
        gaps[i] = gaps[m - 1 - i];
        gaps[m - 1 - i] = t;
    }
    shell_sort_with_gaps(a, n, gaps, m, st);
}

static void shell_sort_knuth(int *a, int n, Stats *st) {
    int gaps[32];
    int m = generate_knuth_gaps(n, gaps, 32);
    shell_sort_with_gaps(a, n, gaps, m, st);
}

static void shell_sort_tokuda(int *a, int n, Stats *st) {
    int gaps[32];
    int m = generate_tokuda_gaps(n, gaps, 32);
    shell_sort_with_gaps(a, n, gaps, m, st);
}

static void insert_sort(int *a, int n, Stats *st) {
    for (int i = 1; i < n; ++i) {
        int t = a[i];
        st->moves++;
        int j = i - 1;
        while (j >= 0) {
            st->comps++;
            if (t < a[j]) {
                a[j + 1] = a[j];
                st->moves++;
                j--;
            } else {
                break;
            }
        }
        a[j + 1] = t;
        st->moves++;
    }
}

static void select_sort(int *a, int n, Stats *st) {
    for (int i = 0; i < n - 1; ++i) {
        int min_idx = i;
        for (int j = i + 1; j < n; ++j) {
            st->comps++;
            if (a[j] < a[min_idx]) {
                min_idx = j;
            }
        }
        if (min_idx != i) {
            int t = a[i];
            a[i] = a[min_idx];
            a[min_idx] = t;
            st->moves += 3;
        }
    }
}

static void bubble_sort(int *a, int n, Stats *st) {
    for (int i = 0; i < n - 1; ++i) {
        for (int j = n - 1; j > i; --j) {
            st->comps++;
            if (a[j] < a[j - 1]) {
                int t = a[j];
                a[j] = a[j - 1];
                a[j - 1] = t;
                st->moves += 3;
            }
        }
    }
}

static void shaker_sort(int *a, int n, Stats *st) {
    int left = 0;
    int right = n - 1;
    int k = n - 1;

    while (left < right) {
        for (int j = right; j > left; --j) {
            st->comps++;
            if (a[j] < a[j - 1]) {
                int t = a[j];
                a[j] = a[j - 1];
                a[j - 1] = t;
                st->moves += 3;
                k = j;
            }
        }
        left = k;
        for (int j = left; j < right; ++j) {
            st->comps++;
            if (a[j] > a[j + 1]) {
                int t = a[j];
                a[j] = a[j + 1];
                a[j + 1] = t;
                st->moves += 3;
                k = j;
            }
        }
        right = k;
    }
}

static void make_random_array(int *a, int n, int max_val) {
    for (int i = 0; i < n; ++i) {
        a[i] = rand() % max_val;
    }
}

static void print_char_array(const char *a, int n) {
    for (int i = 0; i < n; ++i) {
        printf("%c", a[i]);
        if (i + 1 < n)
            printf(" ");
    }
    printf("\n");
}

static int read_first_8_chars(char *out) {
    char line[256];
    if (!fgets(line, sizeof(line), stdin))
        return 0;

    int k = 0;
    for (int i = 0; line[i] != '\0' && k < 8; ++i) {
        if (line[i] == ' ' || line[i] == '\n' || line[i] == '\t')
            continue;
        out[k++] = line[i];
    }
    return k;
}

static void manual_shell_fio(void) {
    char a[8];
    int n = 8;

    printf("\nРучная сортировка ShellSort для 8 первых символов ФИО.\n");
    printf("Введите ФИО (лучше латиницей, без пробелов): ");
    int got = read_first_8_chars(a);

    if (got < n) {
        const char sample[8] = {'K', 'U', 'R', 'A', 'P', 'O', 'V', 'A'};
        for (int i = 0; i < n; ++i)
            a[i] = sample[i];
        printf("Взята примерная последовательность: ");
        print_char_array(a, n);
    } else {
        printf("Исходная последовательность: ");
        print_char_array(a, n);
    }

    int gaps[2] = {2, 1};
    for (int gi = 0; gi < 2; ++gi) {
        int k = gaps[gi];
        printf("\nШаг h=%d:\n", k);
        for (int i = k; i < n; ++i) {
            char t = a[i];
            int j = i - k;
            while (j >= 0 && t < a[j]) {
                a[j + k] = a[j];
                j -= k;
            }
            a[j + k] = t;
            printf("i=%d: ", i + 1);
            print_char_array(a, n);
        }
    }

    printf("\nРезультат: ");
    print_char_array(a, n);
}

static void gaps_to_string_knuth(int n, char *buf, size_t size) {
    int gaps[32];
    int m = generate_knuth_gaps(n, gaps, 32);
    int used = 0;

    if (size == 0)
        return;
    used += snprintf(buf + used, size - used, "{");
    for (int i = 0; i < m && used < (int)size; ++i) {
        if (i > 0)
            used += snprintf(buf + used, size - used, ",");
        used += snprintf(buf + used, size - used, "%d", gaps[i]);
    }
    if (used < (int)size)
        snprintf(buf + used, size - used, "}");
}

static void gaps_to_string_tokuda(int n, char *buf, size_t size) {
    int gaps[32];
    int m = generate_tokuda_gaps(n, gaps, 32);
    int used = 0;

    if (size == 0)
        return;
    used += snprintf(buf + used, size - used, "{");
    for (int i = 0; i < m && used < (int)size; ++i) {
        if (i > 0)
            used += snprintf(buf + used, size - used, ",");
        used += snprintf(buf + used, size - used, "%d", gaps[i]);
    }
    if (used < (int)size)
        snprintf(buf + used, size - used, "}");
}

static void print_table_header(void) {
    printf("\nТрудоемкость метода Шелла (случайные числа)\n");
    printf("+------+----------------------+---------------+---------------+\n");
    printf("| %-4s | %-20s | %-13s | %-13s |\n", "n", "h1..hm (Knuth)", "Insert M+C", "Shell M+C");
    printf("+------+----------------------+---------------+---------------+\n");
}

static void run_table(void) {
    int sizes[] = {100, 200, 300};
    int count = (int)(sizeof(sizes) / sizeof(sizes[0]));
    long long sums_before[3];
    long long sums_after[3];
    int series_before_arr[3];
    int series_after_arr[3];

    print_table_header();

    for (int idx = 0; idx < count; ++idx) {
        int n = sizes[idx];
        int *base = (int *)malloc(sizeof(int) * n);
        int *a_shell = (int *)malloc(sizeof(int) * n);
        int *a_insert = (int *)malloc(sizeof(int) * n);

        make_random_array(base, n, 1000);
        copy_array(a_shell, base, n);
        copy_array(a_insert, base, n);

        long long sum_before = control_sum(base, n);
        int series_before = series_count(base, n);

        Stats st_shell = {0, 0};
        Stats st_insert = {0, 0};

        shell_sort_knuth(a_shell, n, &st_shell);
        insert_sort(a_insert, n, &st_insert);

        long long sum_after = control_sum(a_shell, n);
        int series_after = series_count(a_shell, n);

        sums_before[idx] = sum_before;
        sums_after[idx] = sum_after;
        series_before_arr[idx] = series_before;
        series_after_arr[idx] = series_after;

        char gaps_buf[64];
        gaps_to_string_knuth(n, gaps_buf, sizeof(gaps_buf));
        printf("| %-4d | %-20s | %-13lld | %-13lld |\n", n, gaps_buf,
               st_insert.moves + st_insert.comps, st_shell.moves + st_shell.comps);

        free(base);
        free(a_shell);
        free(a_insert);
    }

    printf("+------+----------------------+---------------+---------------+\n");
    printf("\nПроверка (контрольная сумма и серии):\n");
    for (int idx = 0; idx < count; ++idx) {
        printf("n=%d: сумма %lld -> %lld, серии %d -> %d\n", sizes[idx], sums_before[idx],
               sums_after[idx], series_before_arr[idx], series_after_arr[idx]);
    }
}

static void run_shell_gap_table(void) {
    int sizes[] = {100, 200, 300, 400, 500};
    int count = (int)(sizeof(sizes) / sizeof(sizes[0]));
    long long total_knuth = 0;
    long long total_tokuda = 0;

    printf("\nЗадание 4*: Исследование трудоемкости метода Шелла (разные шаги)\n");
    printf("+------+----------------------+---------------+----------------------------+-----------"
           "----+\n");
    printf("| %-4s | %-20s | %-13s | %-26s | %-13s |\n", "n", "Knuth h1..hm", "Shell M+C",
           "Tokuda h1..hm", "Shell M+C");
    printf("+------+----------------------+---------------+----------------------------+-----------"
           "----+\n");

    for (int idx = 0; idx < count; ++idx) {
        int n = sizes[idx];
        int *base = (int *)malloc(sizeof(int) * n);
        int *a_knuth = (int *)malloc(sizeof(int) * n);
        int *a_tokuda = (int *)malloc(sizeof(int) * n);

        make_random_array(base, n, 1000);
        copy_array(a_knuth, base, n);
        copy_array(a_tokuda, base, n);

        Stats st_knuth = {0, 0};
        Stats st_tokuda = {0, 0};

        shell_sort_knuth(a_knuth, n, &st_knuth);
        shell_sort_tokuda(a_tokuda, n, &st_tokuda);

        char knuth_buf[64];
        char tokuda_buf[64];
        gaps_to_string_knuth(n, knuth_buf, sizeof(knuth_buf));
        gaps_to_string_tokuda(n, tokuda_buf, sizeof(tokuda_buf));

        long long knuth_mc = st_knuth.moves + st_knuth.comps;
        long long tokuda_mc = st_tokuda.moves + st_tokuda.comps;
        total_knuth += knuth_mc;
        total_tokuda += tokuda_mc;

        printf("| %-4d | %-20s | %-13lld | %-26s | %-13lld |\n", n, knuth_buf, knuth_mc, tokuda_buf,
               tokuda_mc);

        free(base);
        free(a_knuth);
        free(a_tokuda);
    }
    printf("+------+----------------------+---------------+----------------------------+-----------"
           "----+\n");
    if (total_knuth < total_tokuda) {
        printf("Вывод: по сумме Mf+Cf лучше последовательность Кнута.\n");
    } else if (total_tokuda < total_knuth) {
        printf("Вывод: по сумме Mf+Cf лучше последовательность Токуды.\n");
    } else {
        printf("Вывод: по сумме Mf+Cf обе последовательности примерно одинаковы.\n");
    }
}

static void run_quadratic_table(void) {
    int sizes[] = {2000, 4000, 6000, 8000, 10000};
    int count = (int)(sizeof(sizes) / sizeof(sizes[0]));

    printf("\nВывод: InsertSort зависит от упорядоченности. Возрастающий массив самый легкий,\n");
    printf("убывающий самый тяжелый.\n");
    printf("\nЗадание 4: Трудоемкость квадратичных методов (случайные массивы)\n");
    printf("%-6s %-10s %-10s %-10s %-10s\n", "N", "Select", "Bubble", "Shaker", "Insert");

    for (int idx = 0; idx < count; ++idx) {
        int n = sizes[idx];
        int *base = (int *)malloc(sizeof(int) * n);
        int *tmp = (int *)malloc(sizeof(int) * n);
        Stats st;

        make_random_array(base, n, 1000);

        st.moves = st.comps = 0;
        copy_array(tmp, base, n);
        select_sort(tmp, n, &st);
        long long sel = st.moves + st.comps;

        st.moves = st.comps = 0;
        copy_array(tmp, base, n);
        bubble_sort(tmp, n, &st);
        long long bub = st.moves + st.comps;

        st.moves = st.comps = 0;
        copy_array(tmp, base, n);
        shaker_sort(tmp, n, &st);
        long long shak = st.moves + st.comps;

        st.moves = st.comps = 0;
        copy_array(tmp, base, n);
        insert_sort(tmp, n, &st);
        long long ins = st.moves + st.comps;

        printf("%-6d %-10lld %-10lld %-10lld %-10lld\n", n, sel, bub, shak, ins);

        free(base);
        free(tmp);
    }

    printf("\nВывод: при росте n значения Mф+Cф быстро растут, это квадратичная трудоемкость.\n");
}

#if HAS_SFML
static sfVertexArray *build_line(const int *ns, const long long *ys, int count, float x0, float y0,
                                 float x_scale, float y_scale, sfColor color) {
    sfVertexArray *line = sfVertexArray_create();
    sfVertexArray_setPrimitiveType(line, sfLineStrip);
    for (int i = 0; i < count; ++i) {
        sfVertex v;
        v.position.x = x0 + (ns[i] - ns[0]) * x_scale;
        v.position.y = y0 - (float)ys[i] * y_scale;
        v.color = color;
        sfVertexArray_append(line, v);
    }
    return line;
}

static void draw_graph(void) {
    enum { GRAPH_POINTS = 5 };
    const int ns[GRAPH_POINTS] = {2000, 4000, 6000, 8000, 10000};

    long long data[GRAPH_POINTS][METHOD_COUNT];

    for (int i = 0; i < GRAPH_POINTS; ++i) {
        int n = ns[i];
        int *base = (int *)malloc(sizeof(int) * n);
        int *tmp = (int *)malloc(sizeof(int) * n);

        make_random_array(base, n, 1000);

        Stats st;

        st.moves = st.comps = 0;
        copy_array(tmp, base, n);
        select_sort(tmp, n, &st);
        data[i][METHOD_SELECT] = st.moves + st.comps;

        st.moves = st.comps = 0;
        copy_array(tmp, base, n);
        bubble_sort(tmp, n, &st);
        data[i][METHOD_BUBBLE] = st.moves + st.comps;

        st.moves = st.comps = 0;
        copy_array(tmp, base, n);
        shaker_sort(tmp, n, &st);
        data[i][METHOD_SHAKER] = st.moves + st.comps;

        st.moves = st.comps = 0;
        copy_array(tmp, base, n);
        insert_sort(tmp, n, &st);
        data[i][METHOD_INSERT] = st.moves + st.comps;

        st.moves = st.comps = 0;
        copy_array(tmp, base, n);
        shell_sort_halving(tmp, n, &st);
        data[i][METHOD_SHELL] = st.moves + st.comps;

        free(base);
        free(tmp);
    }

    long long max_y = 0;
    for (int i = 0; i < GRAPH_POINTS; ++i) {
        for (int m = 0; m < METHOD_COUNT; ++m) {
            if (data[i][m] > max_y)
                max_y = data[i][m];
        }
    }

    const int width = 1000;
    const int height = 700;
    const float originX = 80.0f;
    const float originY = height - 80.0f;
    const float workWidth = width - 140.0f;
    const float workHeight = height - 140.0f;

    float x_scale = workWidth / (float)(ns[GRAPH_POINTS - 1] - ns[0]);
    float y_scale = workHeight / (float)max_y;

    sfRenderWindow *window =
        sfRenderWindow_create((sfVideoMode){width, height, 32}, "ShellSort graph (M+C)",
                              sfResize | sfClose, sfWindowed, NULL);

    if (!window)
        return;
    sfRenderWindow_setFramerateLimit(window, 60);

    sfRectangleShape *axisX = sfRectangleShape_create();
    sfRectangleShape_setSize(axisX, (sfVector2f){workWidth, 2.0f});
    sfRectangleShape_setPosition(axisX, (sfVector2f){originX, originY});
    sfRectangleShape_setFillColor(axisX, sfWhite);

    sfRectangleShape *axisY = sfRectangleShape_create();
    sfRectangleShape_setSize(axisY, (sfVector2f){2.0f, workHeight});
    sfRectangleShape_setPosition(axisY, (sfVector2f){originX, originY - workHeight});
    sfRectangleShape_setFillColor(axisY, sfWhite);

    const int grid_lines = 10;
    const int grid_count = (grid_lines + 1) * 2;
    sfRectangleShape **grid = (sfRectangleShape **)malloc(sizeof(sfRectangleShape *) * grid_count);
    int g = 0;
    for (int i = 0; i <= grid_lines; ++i) {
        float x = originX + i * workWidth / grid_lines;
        sfRectangleShape *line = sfRectangleShape_create();
        sfRectangleShape_setSize(line, (sfVector2f){1.0f, workHeight});
        sfRectangleShape_setPosition(line, (sfVector2f){x, originY - workHeight});
        sfRectangleShape_setFillColor(line, sfColor_fromRGB(80, 80, 80));
        grid[g++] = line;
    }
    for (int i = 0; i <= grid_lines; ++i) {
        float y = originY - i * workHeight / grid_lines;
        sfRectangleShape *line = sfRectangleShape_create();
        sfRectangleShape_setSize(line, (sfVector2f){workWidth, 1.0f});
        sfRectangleShape_setPosition(line, (sfVector2f){originX, y});
        sfRectangleShape_setFillColor(line, sfColor_fromRGB(80, 80, 80));
        grid[g++] = line;
    }

    sfColor colors[METHOD_COUNT] = {sfRed, sfBlue, sfGreen, sfColor_fromRGB(255, 140, 0),
                                    sfMagenta};

    const char *labels[METHOD_COUNT] = {"Select", "Bubble", "Shaker", "Insert", "Shell"};

    sfFont *font = NULL;
    const char *font_paths[] = {
        "./font.ttf", "/System/Library/Fonts/Supplemental/Arial Unicode.ttf",
        "/System/Library/Fonts/Supplemental/Arial.ttf", "/System/Library/Fonts/SFNS.ttf"};
    for (size_t i = 0; i < sizeof(font_paths) / sizeof(font_paths[0]); ++i) {
        font = sfFont_createFromFile(font_paths[i]);
        if (font)
            break;
    }

    sfRectangleShape *legend_boxes[METHOD_COUNT];
    sfText *legend_texts[METHOD_COUNT];
    if (font) {
        float legend_x = originX + workWidth - 170.0f;
        float legend_y = originY - workHeight + 20.0f;
        float line_h = 20.0f;
        for (int m = 0; m < METHOD_COUNT; ++m) {
            legend_boxes[m] = sfRectangleShape_create();
            sfRectangleShape_setSize(legend_boxes[m], (sfVector2f){12.0f, 12.0f});
            sfRectangleShape_setPosition(legend_boxes[m],
                                         (sfVector2f){legend_x, legend_y + m * line_h});
            sfRectangleShape_setFillColor(legend_boxes[m], colors[m]);

            legend_texts[m] = sfText_create(font);
            sfText_setCharacterSize(legend_texts[m], 14);
            sfText_setFillColor(legend_texts[m], sfWhite);
            sfText_setString(legend_texts[m], labels[m]);
            sfText_setPosition(legend_texts[m],
                               (sfVector2f){legend_x + 18.0f, legend_y + m * line_h - 3.0f});
        }
    } else {
        printf("Не найден шрифт для подписей графика. Положите font.ttf рядом с программой.\n");
    }

    sfVertexArray *lines[METHOD_COUNT];
    for (int m = 0; m < METHOD_COUNT; ++m) {
        long long ys[GRAPH_POINTS];
        for (int i = 0; i < GRAPH_POINTS; ++i)
            ys[i] = data[i][m];
        lines[m] = build_line(ns, ys, GRAPH_POINTS, originX, originY, x_scale, y_scale, colors[m]);
    }

    while (sfRenderWindow_isOpen(window)) {
        sfEvent event;
        while (sfRenderWindow_pollEvent(window, &event)) {
            if (event.type == sfEvtClosed) {
                sfRenderWindow_close(window);
            }
        }

        sfRenderWindow_clear(window, sfBlack);
        for (int i = 0; i < grid_count; ++i) {
            sfRenderWindow_drawRectangleShape(window, grid[i], NULL);
        }
        sfRenderWindow_drawRectangleShape(window, axisX, NULL);
        sfRenderWindow_drawRectangleShape(window, axisY, NULL);
        for (int m = 0; m < METHOD_COUNT; ++m) {
            sfRenderWindow_drawVertexArray(window, lines[m], NULL);
        }
        if (font) {
            for (int m = 0; m < METHOD_COUNT; ++m) {
                sfRenderWindow_drawRectangleShape(window, legend_boxes[m], NULL);
                sfRenderWindow_drawText(window, legend_texts[m], NULL);
            }
        }
        sfRenderWindow_display(window);
    }

    for (int m = 0; m < METHOD_COUNT; ++m) {
        sfVertexArray_destroy(lines[m]);
    }
    if (font) {
        for (int m = 0; m < METHOD_COUNT; ++m) {
            sfRectangleShape_destroy(legend_boxes[m]);
            sfText_destroy(legend_texts[m]);
        }
        sfFont_destroy(font);
    }
    for (int i = 0; i < grid_count; ++i) {
        sfRectangleShape_destroy(grid[i]);
    }
    free(grid);
    sfRectangleShape_destroy(axisX);
    sfRectangleShape_destroy(axisY);
    sfRenderWindow_destroy(window);

    printf("\nЦвета линий: красный-Select, синий-Bubble, зеленый-Shaker, оранжевый-Insert, "
           "розовый-Shell\n");
}
#else
static void draw_graph(void) {
    printf("\nSFML/CSFML не найден. График не построен.\n");
    printf("Установите CSFML и пересоберите программу с ключами: ");
    printf("-lcsfml-graphics -lcsfml-window -lcsfml-system\n");
}
#endif

int main(void) {
    srand((unsigned)time(NULL));

    printf("ShellSort по псевдокоду из презентации.\n");
    printf("Сравнение по фактическим операциям (M+C).\n");

    manual_shell_fio();
    run_table();
    run_shell_gap_table();
    run_quadratic_table();
    printf("\nЗадание 5: строится график. Закройте окно для завершения.\n");
    draw_graph();

    return 0;
}
