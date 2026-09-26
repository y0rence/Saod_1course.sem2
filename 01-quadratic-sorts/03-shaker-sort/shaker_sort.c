#include <CSFML/Graphics.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_N 500
#define POINTS 5

void fill_inc(int a[], int n) {
    for (int i = 0; i < n; i++)
        a[i] = i + 1;
}

void fill_dec(int a[], int n) {
    for (int i = 0; i < n; i++)
        a[i] = n - i;
}

void fill_rand(int a[], int n) {
    for (int i = 0; i < n; i++)
        a[i] = rand() % 100;
}

void copy_arr(const int src[], int dst[], int n) {
    for (int i = 0; i < n; i++)
        dst[i] = src[i];
}

int sum_arr(const int a[], int n) {
    int s = 0;
    for (int i = 0; i < n; i++)
        s += a[i];
    return s;
}

int runs_arr(const int a[], int n) {
    if (n <= 0)
        return 0;
    int r = 1;
    for (int i = 1; i < n; i++) {
        if (a[i] < a[i - 1])
            r++;
    }
    return r;
}

void bubble(int a[], int n, int *c, int *m) {
    int t;
    *c = 0;
    *m = 0;
    for (int i = 0; i < n - 1; i++) {
        for (int j = n - 1; j > i; j--) {
            (*c)++;
            if (a[j] < a[j - 1]) {
                t = a[j - 1];
                a[j - 1] = a[j];
                a[j] = t;
                (*m) += 3;
            }
        }
    }
}

void select_sort(int a[], int n, int *c, int *m) {
    int t;
    *c = 0;
    *m = 0;
    for (int i = 0; i < n - 1; i++) {
        int k = i;
        for (int j = i + 1; j < n; j++) {
            if (a[j] < a[k])
                k = j;
            (*c)++;
        }
        t = a[k];
        a[k] = a[i];
        a[i] = t;
        (*m) += 3;
    }
}

void shaker(int a[], int n, int *c, int *m) {
    int t;
    int l = 0;
    int r = n - 1;
    int k = n - 1;

    *c = 0;
    *m = 0;

    do {
        for (int j = r; j > l; j--) {
            (*c)++;
            if (a[j] < a[j - 1]) {
                t = a[j];
                a[j] = a[j - 1];
                a[j - 1] = t;
                (*m) += 3;
                k = j;
            }
        }
        l = k;
        for (int j = l; j < r; j++) {
            (*c)++;
            if (a[j] > a[j + 1]) {
                t = a[j];
                a[j] = a[j + 1];
                a[j + 1] = t;
                (*m) += 3;
                k = j;
            }
        }
        r = k;
    } while (l < r);
}

void shaker_str(char s[], int n) {
    char t;
    int l = 0;
    int r = n - 1;
    int k = n - 1;

    do {
        for (int j = r; j > l; j--) {
            if (s[j] < s[j - 1]) {
                t = s[j];
                s[j] = s[j - 1];
                s[j - 1] = t;
                k = j;
            }
        }
        l = k;
        for (int j = l; j < r; j++) {
            if (s[j] > s[j + 1]) {
                t = s[j];
                s[j] = s[j + 1];
                s[j + 1] = t;
                k = j;
            }
        }
        r = k;
    } while (l < r);
}

int max3(int a, int b, int c) {
    int x = a;
    if (b > x)
        x = b;
    if (c > x)
        x = c;
    return x;
}

void graph(const int n[], const int v[], const int b[], const int s[], int count) {
    const int W = 900;
    const int H = 600;
    const int O = 60;

    if (count <= 0)
        return;

    int mx = 1;
    for (int i = 0; i < count; i++) {
        int t = max3(v[i], b[i], s[i]);
        if (t > mx)
            mx = t;
    }

    sfVideoMode mode = {W, H, 32};
    sfRenderWindow *win =
        sfRenderWindow_create(mode, "Graph (Mf+Cf)", sfDefaultStyle, sfWindowed, NULL);
    if (!win)
        return;

    sfVertexArray *axes = sfVertexArray_create();
    sfVertexArray_setPrimitiveType(axes, sfLines);
    sfVertexArray_append(axes, (sfVertex){{O, H - O}, sfBlack, {0, 0}});
    sfVertexArray_append(axes, (sfVertex){{W - O, H - O}, sfBlack, {0, 0}});
    sfVertexArray_append(axes, (sfVertex){{O, H - O}, sfBlack, {0, 0}});
    sfVertexArray_append(axes, (sfVertex){{O, O}, sfBlack, {0, 0}});

    int nmin = n[0];
    int nmax = n[count - 1];
    float sx = (float)(W - 2 * O) / (float)(nmax - nmin);
    float sy = (float)(H - 2 * O) / (float)mx;

    float x[POINTS];
    float yv[POINTS];
    float yb[POINTS];
    float ys[POINTS];
    for (int i = 0; i < count; i++) {
        x[i] = (float)O + (float)(n[i] - nmin) * sx;
        yv[i] = (float)(H - O) - (float)v[i] * sy;
        yb[i] = (float)(H - O) - (float)b[i] * sy;
        ys[i] = (float)(H - O) - (float)s[i] * sy;
    }

    sfCircleShape *dot = sfCircleShape_create();
    sfCircleShape_setRadius(dot, 4.f);
    sfCircleShape_setOrigin(dot, (sfVector2f){4.f, 4.f});

    sfFont *font = sfFont_createFromFile("arial.ttf");
    if (!font) {
        font = sfFont_createFromFile("/System/Library/Fonts/Supplemental/Arial.ttf");
    }

    sfText *labelRed = NULL;
    sfText *labelGreen = NULL;
    sfText *labelBlue = NULL;
    if (font) {
        labelRed = sfText_create(font);
        labelGreen = sfText_create(font);
        labelBlue = sfText_create(font);
        sfText_setCharacterSize(labelRed, 16);
        sfText_setCharacterSize(labelGreen, 16);
        sfText_setCharacterSize(labelBlue, 16);
        sfText_setFillColor(labelRed, sfRed);
        sfText_setFillColor(labelGreen, sfGreen);
        sfText_setFillColor(labelBlue, sfBlue);
        sfText_setString(labelRed, "Red - SelectSort");
        sfText_setString(labelGreen, "Green - BubbleSort");
        sfText_setString(labelBlue, "Blue - ShakerSort");
    }

    sfRectangleShape *legendLine = sfRectangleShape_create();
    sfRectangleShape_setSize(legendLine, (sfVector2f){30.f, 4.f});

    int thickness = 4;

    while (sfRenderWindow_isOpen(win)) {
        sfEvent e;
        while (sfRenderWindow_pollEvent(win, &e)) {
            if (e.type == sfEvtClosed)
                sfRenderWindow_close(win);
        }

        sfRenderWindow_clear(win, sfWhite);
        sfRenderWindow_drawVertexArray(win, axes, NULL);

        for (int off = -thickness / 2; off <= thickness / 2; off++) {
            sfVertexArray *lv = sfVertexArray_create();
            sfVertexArray *lb = sfVertexArray_create();
            sfVertexArray *ls = sfVertexArray_create();
            sfVertexArray_setPrimitiveType(lv, sfLineStrip);
            sfVertexArray_setPrimitiveType(lb, sfLineStrip);
            sfVertexArray_setPrimitiveType(ls, sfLineStrip);

            for (int i = 0; i < count; i++) {
                sfVertexArray_append(lv, (sfVertex){{x[i], yv[i] + off}, sfRed, {0, 0}});
                sfVertexArray_append(lb, (sfVertex){{x[i], yb[i] + off}, sfGreen, {0, 0}});
                sfVertexArray_append(ls, (sfVertex){{x[i], ys[i] + off}, sfBlue, {0, 0}});
            }

            sfRenderWindow_drawVertexArray(win, lv, NULL);
            sfRenderWindow_drawVertexArray(win, lb, NULL);
            sfRenderWindow_drawVertexArray(win, ls, NULL);

            sfVertexArray_destroy(lv);
            sfVertexArray_destroy(lb);
            sfVertexArray_destroy(ls);
        }

        for (int i = 0; i < count; i++) {
            sfCircleShape_setFillColor(dot, sfRed);
            sfCircleShape_setPosition(dot, (sfVector2f){x[i], yv[i]});
            sfRenderWindow_drawCircleShape(win, dot, NULL);

            sfCircleShape_setFillColor(dot, sfGreen);
            sfCircleShape_setPosition(dot, (sfVector2f){x[i], yb[i]});
            sfRenderWindow_drawCircleShape(win, dot, NULL);

            sfCircleShape_setFillColor(dot, sfBlue);
            sfCircleShape_setPosition(dot, (sfVector2f){x[i], ys[i]});
            sfRenderWindow_drawCircleShape(win, dot, NULL);
        }

        if (font) {
            float legendY = (float)H - (float)O + 18.f;
            float legendX1 = (float)O + 20.f;
            float legendX2 = (float)O + 280.f;
            float legendX3 = (float)O + 560.f;

            sfRectangleShape_setFillColor(legendLine, sfRed);
            sfRectangleShape_setPosition(legendLine, (sfVector2f){legendX1, legendY});
            sfRenderWindow_drawRectangleShape(win, legendLine, NULL);
            sfText_setPosition(labelRed, (sfVector2f){legendX1 + 40.f, legendY - 8.f});
            sfRenderWindow_drawText(win, labelRed, NULL);

            sfRectangleShape_setFillColor(legendLine, sfGreen);
            sfRectangleShape_setPosition(legendLine, (sfVector2f){legendX2, legendY});
            sfRenderWindow_drawRectangleShape(win, legendLine, NULL);
            sfText_setPosition(labelGreen, (sfVector2f){legendX2 + 40.f, legendY - 8.f});
            sfRenderWindow_drawText(win, labelGreen, NULL);

            sfRectangleShape_setFillColor(legendLine, sfBlue);
            sfRectangleShape_setPosition(legendLine, (sfVector2f){legendX3, legendY});
            sfRenderWindow_drawRectangleShape(win, legendLine, NULL);
            sfText_setPosition(labelBlue, (sfVector2f){legendX3 + 40.f, legendY - 8.f});
            sfRenderWindow_drawText(win, labelBlue, NULL);
        }

        sfRenderWindow_display(win);
    }

    sfCircleShape_destroy(dot);
    if (labelRed)
        sfText_destroy(labelRed);
    if (labelGreen)
        sfText_destroy(labelGreen);
    if (labelBlue)
        sfText_destroy(labelBlue);
    if (font)
        sfFont_destroy(font);
    sfRectangleShape_destroy(legendLine);
    sfVertexArray_destroy(axes);
    sfRenderWindow_destroy(win);
}

int main() {
    srand((unsigned)time(NULL));

    printf("Задание 1. Введи 8 символов: ");
    char s[9];
    if (scanf("%8s", s) == 1) {
        int len = 0;
        while (len < 8 && s[len] != '\0')
            len++;
        printf("Было:  ");
        for (int i = 0; i < len; i++)
            printf("%c ", s[i]);
        shaker_str(s, len);
        printf("\nСтало: ");
        for (int i = 0; i < len; i++)
            printf("%c ", s[i]);
        printf("\n\n");
    }

    printf("Задание 2. Проверка массива (10 чисел):\n");
    int a[10];
    fill_rand(a, 10);
    printf("Было:  ");
    for (int i = 0; i < 10; i++)
        printf("%d ", a[i]);
    int s1 = sum_arr(a, 10);
    int r1 = runs_arr(a, 10);

    int c = 0, m = 0;
    shaker(a, 10, &c, &m);

    printf("\nСтало: ");
    for (int i = 0; i < 10; i++)
        printf("%d ", a[i]);
    int s2 = sum_arr(a, 10);
    int r2 = runs_arr(a, 10);

    printf("\nСумма до/после: %d / %d", s1, s2);
    printf("\nСерии до/после: %d / %d", r1, r2);
    printf("\nMf=%d Cf=%d Mf+Cf=%d\n\n", m, c, m + c);

    printf("Трудоемкость пузырьковой и шейкерной сортировок\n");
    printf("+-----+---------------------------+---------------------------+\n");
    printf("|  n  |    Mf+Cf пузырьковой      |     Mf+Cf шейкерной        |\n");
    printf("|     |  Убыв.   Случ.   Возр.     |  Убыв.   Случ.   Возр.     |\n");
    printf("+-----+---------------------------+---------------------------+\n");

    int nlist[POINTS] = {100, 200, 300, 400, 500};
    int gv[POINTS], gb[POINTS], gs[POINTS];

    for (int i = 0; i < POINTS; i++) {
        int n = nlist[i];
        int incv[MAX_N], decv[MAX_N], rndv[MAX_N], w[MAX_N];

        fill_inc(incv, n);
        fill_dec(decv, n);
        fill_rand(rndv, n);

        copy_arr(decv, w, n);
        bubble(w, n, &c, &m);
        int bdec = c + m;
        copy_arr(rndv, w, n);
        bubble(w, n, &c, &m);
        int brnd = c + m;
        copy_arr(incv, w, n);
        bubble(w, n, &c, &m);
        int binc = c + m;

        copy_arr(decv, w, n);
        shaker(w, n, &c, &m);
        int sdec = c + m;
        copy_arr(rndv, w, n);
        shaker(w, n, &c, &m);
        int srnd = c + m;
        copy_arr(incv, w, n);
        shaker(w, n, &c, &m);
        int sinc = c + m;

        printf("| %3d | %7d %7d %7d | %7d %7d %7d |\n", n, bdec, brnd, binc, sdec, srnd, sinc);

        copy_arr(rndv, w, n);
        select_sort(w, n, &c, &m);
        gv[i] = c + m;
        gb[i] = brnd;
        gs[i] = srnd;
    }
    printf("+-----+---------------------------+---------------------------+\n");

    printf("\nГрафик: красный — выбор, зелёный — пузырёк, синий — шейкер.\n");
    graph(nlist, gv, gb, gs, POINTS);

    return 0;
}
