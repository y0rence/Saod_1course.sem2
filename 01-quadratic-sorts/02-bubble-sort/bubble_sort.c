#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void FillInc(int a[], int n, int x) {
    for (int i = 0; i < n; i++, x++) {
        a[i] = x;
    }
}

void FillDec(int a[], int n, int x) {
    for (int i = 0; i < n; i++, x--) {
        a[i] = x;
    }
}

void FillRand(int a[], int n) {
    for (int i = 0; i < n; i++) {
        a[i] = rand() % 100;
    }
}

int CheckSum(const int a[], int n) {
    int sum = 0;
    for (int i = 0; i < n; i++) {
        sum += a[i];
    }
    return sum;
}

int RunNumber(const int a[], int n) {
    if (n <= 0)
        return 0;
    int runs = 1;
    for (int i = 1; i < n; i++) {
        if (a[i] < a[i - 1]) {
            runs++;
        }
    }
    return runs;
}

int RunNumberAv(const int a[], int n) {
    if (n <= 0)
        return 0;
    int runs = RunNumber(a, n);
    return n / runs;
}

void SelectSort(int a[], int n, int *countC, int *countM) {
    int temp = 0;
    *countM = 0;
    *countC = 0;

    for (int i = 0; i < n - 1; i++) {
        int k = i;
        for (int j = i + 1; j < n; j++) {
            if (a[j] < a[k])
                k = j;
            (*countC)++;
        }
        temp = a[k];
        a[k] = a[i];
        a[i] = temp;
        (*countM) += 3;
    }
}

void SelectSortEnhanced(int a[], int n, int *countC, int *countM) {
    int temp = 0;
    *countM = 0;
    *countC = 0;

    for (int i = 0; i < n - 1; i++) {
        int k = i;
        for (int j = i + 1; j < n; j++) {
            if (a[j] < a[k])
                k = j;
            (*countC)++;
        }
        if (k != i) {
            temp = a[k];
            a[k] = a[i];
            a[i] = temp;
            (*countM) += 3;
        }
    }
}

void BubbleSort(int a[], int n, int *countC, int *countM) {
    int temp = 0;
    *countM = 0;
    *countC = 0;

    for (int i = 0; i < n - 1; i++) {
        for (int j = n - 1; j > i; j--) {
            (*countC)++;
            if (a[j] < a[j - 1]) {
                temp = a[j - 1];
                a[j - 1] = a[j];
                a[j] = temp;
                (*countM) += 3;
            }
        }
    }
}

int main() {
    srand((unsigned)time(NULL));

    int b[10];
    FillDec(b, 10, 10);
    printf("\nисходный массив b:\n");
    for (int i = 0; i < 10; i++) {
        printf("%d ", b[i]);
    }
    printf("\nсумма элементов изнач: %d", CheckSum(b, 10));

    int countCB = 0;
    int countMB = 0;

    BubbleSort(b, 10, &countCB, &countMB);
    printf("\n\nотсортированный с помощью BubbleSort b:\n");
    for (int i = 0; i < 10; i++) {
        printf("%d ", b[i]);
    }

    printf("\nсумма элементов после сортировки: %d", CheckSum(b, 10));
    printf("\nколичество серий после сортировки: %d", RunNumber(b, 10));
    printf("\nколичество пересылок: %d", countMB);
    printf("\nколичество сравнений: %d\n", countCB);

    int a10[10];
    FillInc(a10, 10, 10);
    printf("\nисходный массив a:\n");
    for (int i = 0; i < 10; i++) {
        printf("%d ", a10[i]);
    }
    printf("\nсумма элементов изнач: %d", CheckSum(a10, 10));

    int countCA = 0;
    int countMA = 0;

    BubbleSort(a10, 10, &countCA, &countMA);
    printf("\n\nотсортированный с помощью BubbleSort a:\n");
    for (int i = 0; i < 10; i++) {
        printf("%d ", a10[i]);
    }

    printf("\nсумма элементов после сортировки: %d", CheckSum(a10, 10));
    printf("\nколичество серий после сортировки: %d", RunNumber(a10, 10));
    printf("\nколичество пересылок: %d", countMA);
    printf("\nколичество сравнений: %d\n", countCA);

    int c100[100];
    FillRand(c100, 100);
    printf("\nисходный массив c:\n");
    for (int i = 0; i < 100; i++) {
        printf("%d ", c100[i]);
    }
    printf("\nсумма элементов изнач: %d", CheckSum(c100, 100));

    int countCC = 0;
    int countMC = 0;

    BubbleSort(c100, 100, &countCC, &countMC);
    printf("\n\nотсортированный с помощью BubbleSort c:\n");
    for (int i = 0; i < 100; i++) {
        printf("%d ", c100[i]);
    }

    printf("\nсумма элементов после сортировки: %d", CheckSum(c100, 100));
    printf("\nколичество серий после сортировки: %d", RunNumber(c100, 100));
    printf("\nколичество пересылок: %d", countMC);
    printf("\nколичество сравнений: %d\n", countCC);

    printf("\n\n  N   |  M+C теор  | убыв факт | случ факт | возр факт |\n");
    printf("------------------------------------------------------\n");

    for (int i = 1; i <= 5; i++) {
        int n = 100 * i;
        int tTheoretical = 2 * n * (n - 1);

        int arrInc[n];
        int arrDec[n];
        int arrRnd[n];

        FillInc(arrInc, n, n);
        FillDec(arrDec, n, n);
        FillRand(arrRnd, n);

        int countC = 0;
        int countM = 0;

        BubbleSort(arrDec, n, &countC, &countM);
        int factDec = countC + countM;

        BubbleSort(arrRnd, n, &countC, &countM);
        int factRnd = countC + countM;

        BubbleSort(arrInc, n, &countC, &countM);
        int factInc = countC + countM;

        printf("%4d | %9d | %8d | %8d | %8d |\n", n, tTheoretical, factDec, factRnd, factInc);
    }

    return 0;
}
