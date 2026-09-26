#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

int C = 0;

int binarySearch1(int *a, int n, int X) {
    C = 0;
    int L = 0;
    int R = n - 1;

    while (L <= R) {
        int m = (L + R) / 2;

        C++;
        if (a[m] == X) {
            return m;
        }

        C++;
        if (a[m] < X) {
            L = m + 1;
        } else {
            R = m - 1;
        }
    }
    return -1;
}

int binarySearch2(int *a, int n, int X) {
    C = 0;
    int L = 0;
    int R = n - 1;

    if (n <= 0) {
        return -1;
    }

    while (L < R) {
        int m = (L + R) / 2;

        C++;
        if (a[m] < X) {
            L = m + 1;
        } else {
            R = m;
        }
    }

    C++;
    if (a[R] == X) {
        return R;
    }
    return -1;
}

void testCorrectness() {
    int arr[10] = {2, 5, 5, 5, 7, 8, 10, 10, 15, 20};
    int n = 10;

    printf("\nУпорядоченный массив: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
    printf("Индексы:              ");
    for (int i = 0; i < n; i++) {
        printf("%d ", i);
    }
    printf("\n\n");

    int X = arr[0];
    printf("1. Поиск первого элемента (X = %d):\n", X);
    int pos1 = binarySearch1(arr, n, X);
    printf("   Версия 1: позиция = %d, C = %d\n", pos1, C);
    int pos2 = binarySearch2(arr, n, X);
    printf("   Версия 2: позиция = %d, C = %d\n", pos2, C);
    printf("   Теоретическая оценка: log2(10)+1 = %.2f\n\n", log2(10) + 1);

    X = arr[9];
    printf("2. Поиск последнего элемента (X = %d):\n", X);
    pos1 = binarySearch1(arr, n, X);
    printf("   Версия 1: позиция = %d, C = %d\n", pos1, C);
    pos2 = binarySearch2(arr, n, X);
    printf("   Версия 2: позиция = %d, C = %d\n", pos2, C);
    printf("   Теоретическая оценка: log2(10)+1 = %.2f\n\n", log2(10) + 1);

    X = 12;
    printf("3. Поиск отсутствующего элемента (X = %d):\n", X);
    pos1 = binarySearch1(arr, n, X);
    printf("   Версия 1: позиция = %d, C = %d\n", pos1, C);
    pos2 = binarySearch2(arr, n, X);
    printf("   Версия 2: позиция = %d, C = %d\n", pos2, C);
    printf("   Теоретическая оценка: log2(10)+1 = %.2f\n\n", log2(10) + 1);
}

void tableComplexity() {
    int sizes[] = {100, 200, 300, 400, 500, 600, 700, 800, 900, 1000};
    int num_sizes = 10;

    printf("\n========================================\n");
    printf("ТАБЛИЦА: Трудоемкость двоичного поиска элемента\n");
    printf("========================================\n");
    printf("   N    |  Теоретическая  |   Сф I версия  |   Сф II версия\n");
    printf("        |   log2(N)       |                |\n");
    printf("--------------------------------------------------------------------\n");

    for (int i = 0; i < num_sizes; i++) {
        int n = sizes[i];
        int *a = malloc(n * sizeof(int));

        for (int i = 0; i < n; i++) {
            a[i] = i * 2;
        }

        double theoretical = log2(n);

        int key = a[n / 2];

        binarySearch1(a, n, key);
        int c1 = C;

        binarySearch2(a, n, key);
        int c2 = C;

        printf("   %4d  |     %.2f        |       %3d        |       %3d\n", n, theoretical, c1,
               c2);

        free(a);
    }
    printf("--------------------------------------------------------------------\n");
}

int main() {
    srand(time(NULL));
    testCorrectness();
    tableComplexity();

    return 0;
}
