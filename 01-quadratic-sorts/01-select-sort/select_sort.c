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
    srand(time(NULL));
    for (int i = 0; i < n; i++) {
        a[i] = rand() % 100;
    }
}

int CheckSum(int a[], int n) {
    int sum = 0;
    for (int i = 0; i < n; i++) {
        sum += a[i];
    }
    return sum;
}

int RunNumber(int a[], int n) {
    int runs = 1;
    for (int i = 1; i < n; i++) {
        if (a[i] < a[i - 1]) {
            runs++;
        }
    }
    return runs;
}

int RunNumberAv(int a[], int n) {
    int runs = 1;
    for (int i = 1; i < n; i++) {
        if (a[i] < a[i - 1]) {
            runs++;
        }
    }
    return n / runs;
}

void SelectSort(int a[], int n, int *countC, int *countM) {
    int temp = 0;
    *countM = 0;
    *countC = 0;
    for (int i = 0; i < n - 1; i++) {
        int k = i;
        for (int j = i + 1; j < n; j++) {
            if (a[j] < a[k]) {
                k = j;
            }
            (*countC)++;
        }
        temp = a[k];
        a[k] = a[i];
        a[i] = temp;
        (*countM) += 3;
    }
}

void SelectSortEnhaced(int a[], int n, int *countC, int *countM) {
    int temp = 0;
    *countM = 0;
    *countC = 0;
    for (int i = 0; i < n - 1; i++) {
        int k = i;
        for (int j = i + 1; j < n; j++) {
            if (a[j] < a[k]) {
                k = j;
            }
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

int main() {
    printf("\nДемонстрация FillInc:\n");
    int a[10];
    int a1[10];
    FillInc(a, 10, 5);
    for (int i = 0; i < 10; i++) {
        printf("%d ", a[i]);
        a1[i] = a[i];
    }
    printf("\nСумма элементов массива a: %d", CheckSum(a, 10));

    printf("\n\nДемонстрация FillDec:\n");
    int b[10];
    int b1[10];
    FillDec(b, 10, 5);
    for (int i = 0; i < 10; i++) {
        printf("%d ", b[i]);
        b1[i] = b[i];
    }
    printf("\nСумма элементов массива b: %d", CheckSum(b, 10));

    printf("\n\nДемонстрация FillRand:\n");
    int c[100];
    FillRand(c, 100);
    int c1[100];
    for (int i = 0; i < 100; i++) {
        printf("%d ", c[i]);
        c1[i] = c[i];
    }

    printf("\nСумма элементов массива c: %d", CheckSum(c, 100));

    printf("\n\nДемонстрация RunNumber:\n");
    printf("Количество серий в массиве с: %d", RunNumber(c, 100));

    printf("\n\nКоличество серий в возрастающем массиве: %d", RunNumber(a, 10));
    printf("\n\nКоличество серий в убывающем массиве: %d", RunNumber(b, 10));

    printf("\n\nСредняя длина серии: %d", RunNumberAv(c, 100));

    printf("\n\nдемонстрация SelectSort на убыв массиве:");
    int comparisonsB = 0, movesB = 0;
    SelectSort(b, 10, &comparisonsB, &movesB);
    printf("\nотсортированный массив b:\n");

    for (int i = 0; i < 10; i++) {
        printf("%d ", b[i]);
    }

    printf("\n\nКоличество сравнений: %d", comparisonsB);
    printf("\nКоличество пересылок: %d", movesB);
    printf("\nСумма после сортировки: %d", CheckSum(b, 10));
    printf("\nКоличество серий после сортировки: %d", RunNumber(b, 10));

    printf("\n\nдемонстрация SelectSort на возр массиве:");
    int comparisonsA = 0, movesA = 0;
    SelectSort(a, 10, &comparisonsA, &movesA);
    printf("\nотсортированный массив a:\n");

    for (int i = 0; i < 10; i++) {
        printf("%d ", a[i]);
    }

    printf("\n\nКоличество сравнений: %d", comparisonsA);
    printf("\nКоличество пересылок: %d", movesA);
    printf("\nСумма после сортировки: %d", CheckSum(a, 10));
    printf("\nКоличество серий после сортировки: %d", RunNumber(a, 10));

    printf("\n\nдемонстрация SelectSort на случ массиве:");
    int comparisonsC = 0, movesC = 0;
    SelectSort(c, 100, &comparisonsC, &movesC);
    printf("\nотсортированный массив c:\n");

    for (int i = 0; i < 100; i++) {
        printf("%d ", c[i]);
    }

    printf("\n\nКоличество сравнений: %d", comparisonsC);
    printf("\nКоличество пересылок: %d", movesC);
    printf("\nСумма после сортировки: %d", CheckSum(c, 100));
    printf("\nКоличество серий после сортировки: %d", RunNumber(c, 100));

    printf("\n\nУлчшенный SS на убыв массиве:\n");
    int comparisonsEnhB = 0, movesEnhB = 0;
    SelectSortEnhaced(b1, 10, &comparisonsEnhB, &movesEnhB);

    printf("\nотсортированный массив b:\n");

    for (int i = 0; i < 10; i++) {
        printf("%d ", b1[i]);
    }

    printf("\n\nКоличество сравнений: %d", comparisonsEnhB);
    printf("\nКоличество пересылок: %d", movesEnhB);
    printf("\nСумма после сортировки: %d", CheckSum(b1, 10));
    printf("\nКоличество серий после сортировки: %d", RunNumber(b1, 10));

    printf("\n\nУлчшенный SS на возрастающем массиве:\n");
    int comparisonsEnhA = 0, movesEnhA = 0;
    SelectSortEnhaced(a1, 10, &comparisonsEnhA, &movesEnhA);

    printf("\nотсортированный массив a:\n");

    for (int i = 0; i < 10; i++) {
        printf("%d ", a1[i]);
    }

    printf("\n\nКоличество сравнений: %d", comparisonsEnhA);
    printf("\nКоличество пересылок: %d", movesEnhA);
    printf("\nСумма после сортировки: %d", CheckSum(a1, 10));
    printf("\nКоличество серий после сортировки: %d", RunNumber(a1, 10));

    printf("\n\nУлчшенный SS на случ массиве:\n");
    int comparisonsEnhC = 0, movesEnhC = 0;
    SelectSortEnhaced(c1, 100, &comparisonsEnhC, &movesEnhC);

    printf("\nотсортированный массив c:\n");

    for (int i = 0; i < 100; i++) {
        printf("%d ", c1[i]);
    }

    printf("\n\nКоличество сравнений: %d", comparisonsEnhC);
    printf("\nКоличество пересылок: %d", movesEnhC);
    printf("\nСумма после сортировки: %d", CheckSum(c1, 100));
    printf("\nКоличество серий после сортировки: %d", RunNumber(c1, 100));

    int movesBTheor = 3 * (10 - 1);
    int movesCTheor = 3 * (100 - 1);

    int compsBTheor = (10 * 10 - 10) / 2;
    int compsCTheor = (100 * 100 - 100) / 2;

    printf("\n\n  N  |  M+C  | исходный М+С факт  |  улучшенный М+С факт  |");
    printf("\n     | теор  | убыв | случ | возр |  убыв |  случ |  возр |");
    printf("\n-----------------------------------------------------------");
    printf("\n 10  | %d    | %d   |      | %d   |  %d   |       |  %d   |",
           movesBTheor + compsBTheor, comparisonsB + movesB, comparisonsA + movesA,
           comparisonsEnhB + movesEnhB, comparisonsEnhA + movesEnhA);
    printf("\n-----------------------------------------------------------");
    printf("\n 100 | %d  |      | %d |      |       |  %d |       |", movesCTheor + compsCTheor,
           comparisonsC + movesC, comparisonsEnhC + movesEnhC);

    return 1;
}
