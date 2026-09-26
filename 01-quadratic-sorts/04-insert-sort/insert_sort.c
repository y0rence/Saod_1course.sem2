#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <CSFML/Graphics.h>
#include <math.h>

#define MAX_N 10000
#define COUNT_N 5

void copy_array(int src[], int dst[], int n) {
    int i;
    for (i = 1; i <= n; i++) {
        dst[i] = src[i];
    }
}

void fill_random(int a[], int n) {
    int i;
    for (i = 1; i <= n; i++) {
        a[i] = rand() % 1000;
    }
}

void fill_ascending(int a[], int n) {
    int i;
    for (i = 1; i <= n; i++) {
        a[i] = i;
    }
}

void fill_descending(int a[], int n) {
    int i;
    for (i = 1; i <= n; i++) {
        a[i] = n - i + 1;
    }
}

static int is_less_counted(int lhs, int rhs, long long *c) {
    (*c)++;
    return lhs < rhs;
}

void insert_sort(int a[], int n, long long *m, long long *c) {
    int i, j, t;
    *m = 0;
    *c = 0;

    for (i = 2; i <= n; i++) {
        t = a[i];
        (*m)++;
        j = i - 1;

        while (j > 0 && is_less_counted(t, a[j], c)) {
            a[j + 1] = a[j];
            (*m)++;
            j = j - 1;
        }

        a[j + 1] = t;
        (*m)++;
    }
}

void select_sort(int a[], int n, long long *m, long long *c) {
    int i, j, min, t;
    *m = 0;
    *c = 0;

    for (i = 1; i <= n - 1; i++) {
        min = i;
        for (j = i + 1; j <= n; j++) {
            (*c)++;
            if (a[j] < a[min]) {
                min = j;
            }
        }
        t = a[i];
        a[i] = a[min];
        a[min] = t;
        (*m) += 3;
    }
}

void bubble_sort(int a[], int n, long long *m, long long *c) {
    int i, j, t;
    *m = 0;
    *c = 0;

    for (i = 1; i <= n - 1; i++) {
        for (j = n; j >= i + 1; j--) {
            (*c)++;
            if (a[j] < a[j - 1]) {
                t = a[j];
                a[j] = a[j - 1];
                a[j - 1] = t;
                (*m) += 3;
            }
        }
    }
}

void shaker_sort(int a[], int n, long long *m, long long *c) {
    int left, right, k, j, t;
    *m = 0;
    *c = 0;

    left = 1;
    right = n;
    k = n;

    do {
        for (j = right; j >= left + 1; j--) {
            (*c)++;
            if (a[j] < a[j - 1]) {
                t = a[j];
                a[j] = a[j - 1];
                a[j - 1] = t;
                (*m) += 3;
                k = j;
            }
        }
        left = k;

        for (j = left; j <= right - 1; j++) {
            (*c)++;
            if (a[j] > a[j + 1]) {
                t = a[j];
                a[j] = a[j + 1];
                a[j + 1] = t;
                (*m) += 3;
                k = j;
            }
        }
        right = k;
    } while (left < right);
}

void insert_theor(int n, long long *m, long long *c) {
    *c = ((long long)n * n - n) / 2;
    *m = *c + 2LL * n - 2;
}

long long get_max_value(long long a[], int count) {
    int i;
    long long max = a[0];
    for (i = 1; i < count; i++) {
        if (a[i] > max) {
            max = a[i];
        }
    }
    return max;
}

static void draw_polyline_thick(sfRenderWindow *window, const sfVector2f *points, int count,
                                sfColor color, float thickness) {
    if (count < 2) {
        return;
    }

    sfRectangleShape *segment = sfRectangleShape_create();
    sfRectangleShape_setFillColor(segment, color);
    sfRectangleShape_setOrigin(segment, (sfVector2f){0.0f, thickness * 0.5f});

    int i;
    for (i = 0; i < count - 1; i++) {
        sfVector2f p1 = points[i];
        sfVector2f p2 = points[i + 1];
        float dx = p2.x - p1.x;
        float dy = p2.y - p1.y;
        float length = sqrtf(dx * dx + dy * dy);

        if (length <= 0.0001f) {
            continue;
        }

        sfRectangleShape_setSize(segment, (sfVector2f){length, thickness});
        sfRectangleShape_setPosition(segment, p1);
        sfRectangleShape_setRotation(segment, atan2f(dy, dx) * 57.29578f);
        sfRenderWindow_drawRectangleShape(window, segment, NULL);
    }

    sfRectangleShape_destroy(segment);
}

static void draw_markers(sfRenderWindow *window, const sfVector2f *points, int count, sfColor color,
                         float radius, float x_offset) {
    if (count <= 0) {
        return;
    }

    sfCircleShape *dot = sfCircleShape_create();
    if (!dot) {
        return;
    }

    sfCircleShape_setRadius(dot, radius);
    sfCircleShape_setOrigin(dot, (sfVector2f){radius, radius});
    sfCircleShape_setFillColor(dot, color);
    sfCircleShape_setOutlineThickness(dot, 1.0f);

    sfColor outline = (color.r == 0 && color.g == 0 && color.b == 0) ? sfWhite : sfBlack;
    sfCircleShape_setOutlineColor(dot, outline);

    int i;
    for (i = 0; i < count; i++) {
        sfVector2f p = points[i];
        p.x += x_offset;
        sfCircleShape_setPosition(dot, p);
        sfRenderWindow_drawCircleShape(window, dot, NULL);
    }

    sfCircleShape_destroy(dot);
}

static sfFont *load_default_font(void) {
    const char *paths[] = {"/System/Library/Fonts/Supplemental/Arial.ttf",
                           "/System/Library/Fonts/Supplemental/Helvetica.ttf",
                           "/System/Library/Fonts/SFNS.ttf",
                           "/System/Library/Fonts/Monaco.ttf",
                           "/System/Library/Fonts/Menlo.ttc",
                           "/Library/Fonts/Arial.ttf"};
    size_t i;
    for (i = 0; i < sizeof(paths) / sizeof(paths[0]); i++) {
        sfFont *font = sfFont_createFromFile(paths[i]);
        if (font) {
            return font;
        }
    }
    return NULL;
}

void draw_graph(int n_values[], long long select[], long long bubble[], long long shaker[],
                long long insert[], int count) {
    sfVideoMode mode = {800, 600, 32};
    sfRenderWindow *window =
        sfRenderWindow_create(mode, "Graph", sfResize | sfClose, sfWindowed, NULL);
    if (!window) {
        return;
    }
    sfRenderWindow_setVerticalSyncEnabled(window, true);
    sfRenderWindow_setFramerateLimit(window, 60);

    float left = 60.0f;
    float right = 20.0f;
    float top = 20.0f;
    float bottom = 60.0f;
    float width = 800.0f;
    float height = 600.0f;

    long long max1 = get_max_value(select, count);
    long long max2 = get_max_value(bubble, count);
    long long max3 = get_max_value(shaker, count);
    long long max4 = get_max_value(insert, count);

    long long max_y = max1;
    if (max2 > max_y)
        max_y = max2;
    if (max3 > max_y)
        max_y = max3;
    if (max4 > max_y)
        max_y = max4;

    int n_min = n_values[0];
    int n_max = n_values[count - 1];

    float scale_x = (width - left - right) / (float)(n_max - n_min);
    float scale_y = (height - top - bottom) / (float)max_y;

    sfVector2f *pts_select = (sfVector2f *)malloc(sizeof(sfVector2f) * count);
    sfVector2f *pts_bubble = (sfVector2f *)malloc(sizeof(sfVector2f) * count);
    sfVector2f *pts_shaker = (sfVector2f *)malloc(sizeof(sfVector2f) * count);
    sfVector2f *pts_insert = (sfVector2f *)malloc(sizeof(sfVector2f) * count);

    if (!pts_select || !pts_bubble || !pts_shaker || !pts_insert) {
        free(pts_select);
        free(pts_bubble);
        free(pts_shaker);
        free(pts_insert);
        sfRenderWindow_destroy(window);
        return;
    }

    int i;
    for (i = 0; i < count; i++) {
        float x = left + (n_values[i] - n_min) * scale_x;

        float y1 = height - bottom - select[i] * scale_y;
        float y2 = height - bottom - bubble[i] * scale_y;
        float y3 = height - bottom - shaker[i] * scale_y;
        float y4 = height - bottom - insert[i] * scale_y;

        pts_select[i] = (sfVector2f){x, y1};
        pts_bubble[i] = (sfVector2f){x, y2};
        pts_shaker[i] = (sfVector2f){x, y3};
        pts_insert[i] = (sfVector2f){x, y4};
    }

    sfFont *font = load_default_font();
    sfText *legend_text = NULL;
    sfRectangleShape *legend_line = NULL;
    if (font) {
        legend_text = sfText_create(font);
        sfText_setCharacterSize(legend_text, 14);
        sfText_setFillColor(legend_text, sfBlack);

        legend_line = sfRectangleShape_create();
        sfRectangleShape_setSize(legend_line, (sfVector2f){28.0f, 3.0f});
        sfRectangleShape_setOrigin(legend_line, (sfVector2f){0.0f, 1.5f});
    }

    float legend_x = width - right - 180.0f;
    float legend_y = top + 20.0f;
    float legend_gap = 18.0f;
    float line_thickness = 3.0f;
    float marker_radius = 3.5f;

    while (sfRenderWindow_isOpen(window)) {
        sfEvent event;
        while (sfRenderWindow_pollEvent(window, &event)) {
            if (event.type == sfEvtClosed) {
                sfRenderWindow_close(window);
            }
        }

        sfRenderWindow_clear(window, sfColor_fromRGB(250, 250, 250));

        sfVertex axis_x[2];
        axis_x[0].position.x = left;
        axis_x[0].position.y = height - bottom;
        axis_x[0].color = sfBlack;
        axis_x[0].texCoords.x = 0;
        axis_x[0].texCoords.y = 0;

        axis_x[1].position.x = width - right;
        axis_x[1].position.y = height - bottom;
        axis_x[1].color = sfBlack;
        axis_x[1].texCoords.x = 0;
        axis_x[1].texCoords.y = 0;

        sfVertex axis_y[2];
        axis_y[0].position.x = left;
        axis_y[0].position.y = height - bottom;
        axis_y[0].color = sfBlack;
        axis_y[0].texCoords.x = 0;
        axis_y[0].texCoords.y = 0;

        axis_y[1].position.x = left;
        axis_y[1].position.y = top;
        axis_y[1].color = sfBlack;
        axis_y[1].texCoords.x = 0;
        axis_y[1].texCoords.y = 0;

        sfRenderWindow_drawPrimitives(window, axis_x, 2, sfLines, NULL);
        sfRenderWindow_drawPrimitives(window, axis_y, 2, sfLines, NULL);

        draw_polyline_thick(window, pts_select, count, sfRed, line_thickness);
        draw_polyline_thick(window, pts_shaker, count, sfGreen, line_thickness);
        draw_polyline_thick(window, pts_insert, count, sfBlack, line_thickness);
        draw_polyline_thick(window, pts_bubble, count, sfBlue, line_thickness);

        draw_markers(window, pts_select, count, sfRed, marker_radius, -4.0f);
        draw_markers(window, pts_bubble, count, sfBlue, marker_radius, -1.5f);
        draw_markers(window, pts_shaker, count, sfGreen, marker_radius, 1.5f);
        draw_markers(window, pts_insert, count, sfBlack, marker_radius, 4.0f);

        if (legend_text && legend_line) {
            const char *labels[] = {"Selection sort", "Bubble sort", "Shaker sort",
                                    "Insertion sort"};
            sfColor colors[] = {sfRed, sfBlue, sfGreen, sfBlack};
            int k;
            for (k = 0; k < 4; k++) {
                float y = legend_y + k * legend_gap;
                sfRectangleShape_setFillColor(legend_line, colors[k]);
                sfRectangleShape_setPosition(legend_line, (sfVector2f){legend_x, y});
                sfRenderWindow_drawRectangleShape(window, legend_line, NULL);

                sfText_setString(legend_text, labels[k]);
                sfText_setPosition(legend_text, (sfVector2f){legend_x + 36.0f, y - 10.0f});
                sfRenderWindow_drawText(window, legend_text, NULL);
            }
        }

        sfRenderWindow_display(window);
    }

    if (legend_line) {
        sfRectangleShape_destroy(legend_line);
    }
    if (legend_text) {
        sfText_destroy(legend_text);
    }
    if (font) {
        sfFont_destroy(font);
    }
    free(pts_select);
    free(pts_bubble);
    free(pts_shaker);
    free(pts_insert);
    sfRenderWindow_destroy(window);
}

int main(void) {
    int n_values[COUNT_N] = {2000, 4000, 6000, 8000, 10000};
    int base[MAX_N + 1];
    int work[MAX_N + 1];

    long long insert_theory_sum[COUNT_N];
    long long insert_desc_sum[COUNT_N];
    long long insert_rand_sum[COUNT_N];
    long long insert_asc_sum[COUNT_N];

    long long insert_rand_m[COUNT_N];
    long long insert_rand_c[COUNT_N];
    long long insert_theor_m[COUNT_N];
    long long insert_theor_c[COUNT_N];

    long long select_sum[COUNT_N];
    long long bubble_sum[COUNT_N];
    long long shaker_sum[COUNT_N];

    int i;
    long long m, c;

    srand((unsigned)time(NULL));

    printf("Zadanie 1: vvedite 8 simvolov bez probelov:\n");
    char s[32];
    if (scanf("%8s", s) == 1) {
        int a8[9];
        for (i = 1; i <= 8; i++) {
            a8[i] = (int)s[i - 1];
        }
        insert_sort(a8, 8, &m, &c);
        printf("Otsortirovannye 8 simvolov: ");
        for (i = 1; i <= 8; i++) {
            printf("%c", (char)a8[i]);
        }
        printf("\n\n");
    }

    for (i = 0; i < COUNT_N; i++) {
        int n = n_values[i];

        insert_theor(n, &m, &c);
        insert_theor_m[i] = m;
        insert_theor_c[i] = c;
        insert_theory_sum[i] = m + c;

        fill_descending(base, n);
        copy_array(base, work, n);
        insert_sort(work, n, &m, &c);
        insert_desc_sum[i] = m + c;

        fill_random(base, n);
        copy_array(base, work, n);
        insert_sort(work, n, &m, &c);
        insert_rand_m[i] = m;
        insert_rand_c[i] = c;
        insert_rand_sum[i] = m + c;

        copy_array(base, work, n);
        select_sort(work, n, &m, &c);
        select_sum[i] = m + c;

        copy_array(base, work, n);
        bubble_sort(work, n, &m, &c);
        bubble_sum[i] = m + c;

        copy_array(base, work, n);
        shaker_sort(work, n, &m, &c);
        shaker_sum[i] = m + c;

        fill_ascending(base, n);
        copy_array(base, work, n);
        insert_sort(work, n, &m, &c);
        insert_asc_sum[i] = m + c;
    }

    printf("Zadanie 2: sravnenie Mf i Sf s teoriei (sluchainye massivy)\n");
    printf("N\tM_teor\tC_teor\tM_f\tC_f\n");
    for (i = 0; i < COUNT_N; i++) {
        printf("%d\t%lld\t%lld\t%lld\t%lld\n", n_values[i], insert_theor_m[i], insert_theor_c[i],
               insert_rand_m[i], insert_rand_c[i]);
    }
    printf("\n");

    printf("Zadanie 3: Trudoemkost InsertSort\n");
    printf("N\tM+C teor\tUbyv\tSluch\tVozr\n");
    for (i = 0; i < COUNT_N; i++) {
        printf("%d\t%lld\t\t%lld\t%lld\t%lld\n", n_values[i], insert_theory_sum[i],
               insert_desc_sum[i], insert_rand_sum[i], insert_asc_sum[i]);
    }
    printf("\n");
    printf("Vyvod: InsertSort zavisit ot uporyadochennosti. Vozrastayushchii massiv samyi legkii, "
           "ubyvayushchii samyi tyazhelyi.\n\n");

    printf("Zadanie 4: Trudoemkost kvadratichnyh metodov (sluchainye massivy)\n");
    printf("N\tSelect\tBubble\tShaker\tInsert\n");
    for (i = 0; i < COUNT_N; i++) {
        printf("%d\t%lld\t%lld\t%lld\t%lld\n", n_values[i], select_sum[i], bubble_sum[i],
               shaker_sum[i], insert_rand_sum[i]);
    }
    printf("\n");
    printf("Vyvod: pri roste n znacheniya M_f+C_f bystro rastut, eto kvadratichnaya "
           "trudojomkost.\n\n");

    printf("Zadanie 5: stroimsya grafik. Zakroyte okno dlya zaversheniya.\n");
    draw_graph(n_values, select_sum, bubble_sum, shaker_sum, insert_rand_sum, COUNT_N);

    return 0;
}
