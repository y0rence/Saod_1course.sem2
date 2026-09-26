#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

typedef struct {
    long long moves;
    long long comps;
} Stats;

void copy_array(int *dst, int *src, int n) {
    for (int i = 0; i < n; i++) {
        dst[i] = src[i];
    }
}

long long control_sum(int *a, int n) {
    long long s = 0;
    for (int i = 0; i < n; i++) {
        s += a[i];
    }
    return s;
}

int series_count(int *a, int n) {
    if (n <= 0)
        return 0;

    int count = 1;
    for (int i = 1; i < n; i++) {
        if (a[i] < a[i - 1])
            count++;
    }
    return count;
}

void make_random_array(int *a, int n, int max_val) {
    for (int i = 0; i < n; i++) {
        a[i] = rand() % max_val;
    }
}

int generate_knuth_gaps(int n, int *gaps) {
    int m = n < 4 ? 1 : (int)floor(log2((double)n)) - 1;

    gaps[0] = 1;
    for (int i = 1; i < m; i++) {
        gaps[i] = 2 * gaps[i - 1] + 1;
    }
    return m;
}

int generate_tokuda_gaps(int n, int *gaps) {
    int k = 0;
    while (1) {
        int h = (int)ceil((9.0 * pow(2.25, k) - 4.0) / 5.0);
        if (h >= n)
            break;
        gaps[k++] = h;
    }
    return k;
}

void shell_sort_with_gaps(int *a, int n, int *gaps, int m, Stats *st) {
    for (int g = m - 1; g >= 0; g--) {
        int k = gaps[g];

        for (int i = k; i < n; i++) {
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

void shell_sort_halving(int *a, int n, Stats *st) {
    int gaps[32];
    int m = 0;

    for (int k = n / 2; k > 0; k /= 2) {
        gaps[m++] = k;
    }
    for (int i = 0; i < m / 2; i++) {
        int t = gaps[i];
        gaps[i] = gaps[m - 1 - i];
        gaps[m - 1 - i] = t;
    }
    shell_sort_with_gaps(a, n, gaps, m, st);
}

void shell_sort_knuth(int *a, int n, Stats *st) {
    int gaps[32];
    int m = generate_knuth_gaps(n, gaps);
    shell_sort_with_gaps(a, n, gaps, m, st);
}

void shell_sort_tokuda(int *a, int n, Stats *st) {
    int gaps[32];
    int m = generate_tokuda_gaps(n, gaps);
    shell_sort_with_gaps(a, n, gaps, m, st);
}

void insert_sort(int *a, int n, Stats *st) {
    for (int i = 1; i < n; i++) {
        int temp = a[i];
        st->moves++;
        int j = i - 1;

        while (j >= 0) {
            st->comps++;

            if (a[j] > temp) {
                a[j + 1] = a[j];
                st->moves++;
                j--;
            } else {
                break;
            }
        }

        a[j + 1] = temp;
        st->moves++;
    }
}

void select_sort(int *a, int n, Stats *st) {
    for (int i = 0; i < n - 1; i++) {
        int min = i;

        for (int j = i + 1; j < n; j++) {
            st->comps++;
            if (a[j] < a[min])
                min = j;
        }

        if (min != i) {
            int t = a[i];
            a[i] = a[min];
            a[min] = t;
            st->moves += 3;
        }
    }
}

void bubble_sort(int *a, int n, Stats *st) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = n - 1; j > i; j--) {
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

void shaker_sort(int *a, int n, Stats *st) {
    int left = 0;
    int right = n - 1;
    int k = n - 1;

    while (left < right) {
        for (int j = right; j > left; j--) {
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

        for (int j = left; j < right; j++) {
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

void print_char_array(char *a, int n) {
    for (int i = 0; i < n; i++) {
        printf("%c ", a[i]);
    }
    printf("\n");
}

int read_first_8_chars(char *out) {
    char line[256];

    if (!fgets(line, sizeof(line), stdin))
        return 0;

    int k = 0;
    for (int i = 0; line[i] != '\0' && k < 8; i++) {
        if (line[i] == ' ' || line[i] == '\n' || line[i] == '\t')
            continue;
        out[k++] = line[i];
    }

    return k;
}

void manual_shell_fio() {
    char a[8];
    int n = 8;

    printf("\nРучная сортировка ShellSort для 8 символов.\n");
    printf("Введите ФИО или слово: ");

    int got = read_first_8_chars(a);

    if (got < 8) {
        char sample[8] = {'K', 'U', 'R', 'A', 'P', 'O', 'V', 'A'};
        for (int i = 0; i < 8; i++)
            a[i] = sample[i];

        printf("Взята примерная последовательность: ");
        print_char_array(a, n);
    } else {
        printf("Исходная последовательность: ");
        print_char_array(a, n);
    }

    int gaps[2] = {2, 1};

    for (int g = 0; g < 2; g++) {
        int gap = gaps[g];
        printf("\nШаг h=%d:\n", gap);

        for (int i = gap; i < n; i++) {
            char temp = a[i];
            int j = i - gap;

            while (j >= 0 && a[j] > temp) {
                a[j + gap] = a[j];
                j -= gap;
            }

            a[j + gap] = temp;

            printf("i=%d: ", i + 1);
            print_char_array(a, n);
        }
    }

    printf("\nРезультат: ");
    print_char_array(a, n);
}

void print_gaps(int *gaps, int m) {
    printf("{");
    for (int i = 0; i < m; i++) {
        printf("%d", gaps[i]);
        if (i < m - 1)
            printf(",");
    }
    printf("}");
}

void run_table() {
    int sizes[] = {100, 200, 300};
    int count = 3;

    printf("\nТрудоемкость метода Шелла (случайные числа)\n");
    printf("+------+----------------------+---------------+---------------+\n");
    printf("| n    | h1..hm (Knuth)       | Insert M+C    | Shell M+C     |\n");
    printf("+------+----------------------+---------------+---------------+\n");

    for (int idx = 0; idx < count; idx++) {
        int n = sizes[idx];

        int *base = malloc(n * sizeof(int));
        int *a_shell = malloc(n * sizeof(int));
        int *a_insert = malloc(n * sizeof(int));

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

        int gaps[32];
        int m = generate_knuth_gaps(n, gaps);

        printf("| %-4d | ", n);
        print_gaps(gaps, m);
        int len = 20 - 2 * m;
        if (len < 1)
            len = 1;
        for (int i = 0; i < len; i++)
            printf(" ");
        printf("| %-13lld | %-13lld |\n", st_insert.moves + st_insert.comps,
               st_shell.moves + st_shell.comps);

        printf("Проверка n=%d: сумма %lld -> %lld, серии %d -> %d\n", n, sum_before, sum_after,
               series_before, series_after);

        free(base);
        free(a_shell);
        free(a_insert);
    }

    printf("+------+----------------------+---------------+---------------+\n");
}

void run_shell_gap_table() {
    int sizes[] = {100, 200, 300, 400, 500};
    int count = 5;
    long long total_knuth = 0;
    long long total_tokuda = 0;

    printf("\nСравнение шагов Кнута и Токуды\n");
    printf(
        "+------+----------------------+---------------+----------------------+---------------+\n");
    printf(
        "| n    | Knuth                | Shell M+C     | Tokuda               | Shell M+C     |\n");
    printf(
        "+------+----------------------+---------------+----------------------+---------------+\n");

    for (int idx = 0; idx < count; idx++) {
        int n = sizes[idx];

        int *base = malloc(n * sizeof(int));
        int *a_knuth = malloc(n * sizeof(int));
        int *a_tokuda = malloc(n * sizeof(int));

        make_random_array(base, n, 1000);
        copy_array(a_knuth, base, n);
        copy_array(a_tokuda, base, n);

        Stats st_knuth = {0, 0};
        Stats st_tokuda = {0, 0};

        shell_sort_knuth(a_knuth, n, &st_knuth);
        shell_sort_tokuda(a_tokuda, n, &st_tokuda);

        int gaps1[32], gaps2[32];
        int m1 = generate_knuth_gaps(n, gaps1);
        int m2 = generate_tokuda_gaps(n, gaps2);

        long long knuth_mc = st_knuth.moves + st_knuth.comps;
        long long tokuda_mc = st_tokuda.moves + st_tokuda.comps;

        total_knuth += knuth_mc;
        total_tokuda += tokuda_mc;

        printf("| %-4d | ", n);
        print_gaps(gaps1, m1);
        printf(" | %-13lld | ", knuth_mc);
        print_gaps(gaps2, m2);
        printf(" | %-13lld |\n", tokuda_mc);

        free(base);
        free(a_knuth);
        free(a_tokuda);
    }

    printf(
        "+------+----------------------+---------------+----------------------+---------------+\n");

    if (total_knuth < total_tokuda)
        printf("Вывод: лучше последовательность Кнута.\n");
    else if (total_tokuda < total_knuth)
        printf("Вывод: лучше последовательность Токуды.\n");
    else
        printf("Вывод: обе последовательности примерно одинаковы.\n");
}

void run_quadratic_table() {
    int sizes[] = {2000, 4000, 6000, 8000, 10000};
    int count = 5;

    printf("\nТрудоемкость квадратичных методов\n");
    printf("%-6s %-10s %-10s %-10s %-10s %-10s\n", "N", "Select", "Bubble", "Shaker", "Insert",
           "Shell");

    for (int idx = 0; idx < count; idx++) {
        int n = sizes[idx];

        int *base = malloc(n * sizeof(int));
        int *tmp = malloc(n * sizeof(int));
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

        st.moves = st.comps = 0;
        copy_array(tmp, base, n);
        shell_sort_halving(tmp, n, &st);
        long long shel = st.moves + st.comps;

        printf("%-6d %-10lld %-10lld %-10lld %-10lld %-10lld\n", n, sel, bub, shak, ins, shel);

        free(base);
        free(tmp);
    }

    printf("\nВывод: при увеличении n трудоемкость квадратичных методов быстро растет.\n");
}

int main() {
    srand(time(NULL));

    printf("ShellSort и сравнение методов сортировки\n");

    manual_shell_fio();
    run_table();
    run_shell_gap_table();
    run_quadratic_table();

    return 0;
}
