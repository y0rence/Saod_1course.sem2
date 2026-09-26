#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

using namespace std;

long long compCount = 0;
long long moveCount = 0;

struct List {
    struct List *next;
    int data;
};

struct Queue {
    struct List *head;
    struct List *tail;
};

struct List *createStackAscending(int n) {
    struct List *head = NULL;

    for (int i = n; i >= 1; i--) {
        struct List *p = new List;
        p->data = i;
        p->next = head;
        head = p;
    }

    return head;
}

struct List *createStackDescending(int n) {
    struct List *head = NULL;

    for (int i = 1; i <= n; i++) {
        struct List *p = new List;
        p->data = i;
        p->next = head;
        head = p;
    }

    return head;
}

struct List *createStackRand(int n) {
    struct List *head = NULL;

    for (int i = 0; i < n; i++) {
        struct List *p = new List;
        p->data = rand() % 100 + 10;
        p->next = head;
        head = p;
    }

    return head;
}

void initQueue(struct Queue *p) {
    p->head = NULL;
    p->tail = NULL;
}

void insertInQueue(struct Queue *q, int n) {
    struct List *p = new List;
    p->data = n;
    p->next = NULL;

    if (q->head != NULL) {
        q->tail->next = p;
    } else {
        q->head = p;
    }
    q->tail = p;
}

struct Queue createQueueAscending(int n) {
    struct Queue p;

    initQueue(&p);

    for (int i = 1; i <= n; i++) {
        insertInQueue(&p, i);
    }

    return p;
}

struct Queue createQueueDescending(int n) {
    struct Queue p;

    initQueue(&p);

    for (int i = n; i >= 1; i--) {
        insertInQueue(&p, i);
    }

    return p;
}

struct Queue createQueueRand(int n) {
    struct Queue p;

    initQueue(&p);

    for (int i = 0; i < n; i++) {
        insertInQueue(&p, rand() % 100 + 10);
    }

    return p;
}

void printList(struct List *head) {
    struct List *p = head;
    printf("\nСписок: ");

    while (p != NULL) {
        cout << p->data << " ";
        p = p->next;
    }
    printf("\n");
}

int sumList(struct List *head) {
    int sum = 0;
    struct List *p = head;

    while (p != NULL) {
        sum += p->data;
        p = p->next;
    }

    return sum;
}

int countSeries(struct List *head) {
    if (head == NULL)
        return 0;
    if (head->next == NULL)
        return 1;

    int count = 1;
    struct List *p = head;

    while (p != NULL) {
        if (p->next != NULL && p->data > p->next->data) {
            count++;
        }
        p = p->next;
    }

    return count;
}

void deleteList(struct List **head) {
    struct List *p = *head;

    while (p != NULL) {
        struct List *next = p->next;
        delete p;
        p = next;
    }
    *head = NULL;
}

void printListForward(struct List *head) {
    if (head == NULL)
        return;
    cout << head->data << " ";
    printListForward(head->next);
}

void printListReverse(struct List *head) {
    if (head == NULL)
        return;
    printListReverse(head->next);
    cout << head->data << " ";
}

void change96InStack(struct List **head) {
    struct List *c1 = *head, *p1 = c1;
    struct List *c2 = *head, *p2 = c2;

    while (c1->data != 6) {
        p1 = c1;
        c1 = c1->next;
    }

    while (c2->data != 9) {
        p2 = c2;
        c2 = c2->next;
    }

    p1->next = c2;
    p2->next = c1;
    struct List *temp = c1->next;
    c1->next = c2->next;
    c2->next = temp;
}

int countList(struct List *head) {
    int count = 0;
    struct List *p = head;
    while (p != NULL) {
        count++;
        p = p->next;
    }

    return count;
}

void splitList(struct List *S, struct List *&a, struct List *&b) {
    if (S == NULL) {
        a = b = NULL;
        return;
    }

    a = S;
    b = S->next;

    struct List *k = a;
    struct List *p = b;

    while (p != NULL) {
        k->next = p->next;
        k = p;
        p = p->next;
    }
}

void moveToQueue(struct List *&list, struct Queue *c) {
    if (c->head != NULL) {
        c->tail->next = list;
    } else {
        c->head = list;
    }
    c->tail = list;
    list = list->next;
    moveCount++;
}

void mergeSeries(struct List *&a, int q, struct List *&b, int r, struct Queue *c) {
    while (q != 0 && r != 0) {
        compCount++;

        if (a->data <= b->data) {
            moveToQueue(a, c);
            q--;
        } else {
            moveToQueue(b, c);
            r--;
        }
    }

    while (q > 0) {
        moveToQueue(a, c);
        q--;
    }

    while (r > 0) {
        moveToQueue(b, c);
        r--;
    }
}

void mergeSort(struct List *&S, int n) {
    if (S == NULL || S->next == NULL)
        return;

    struct List *a, *b;

    splitList(S, a, b);

    struct Queue c0, c1;

    int p = 1;

    while (p < n) {
        initQueue(&c0);
        initQueue(&c1);

        int i = 0;
        int m = n;

        while (m > 0) {
            int q = (m >= p) ? p : m;
            m -= q;
            int r = (m >= p) ? p : m;
            m -= r;

            if (i == 0)
                mergeSeries(a, q, b, r, &c0);
            else
                mergeSeries(a, q, b, r, &c1);

            i = 1 - i;
        }

        a = c0.head;
        b = c1.head;
        p *= 2;
    }

    if (c0.tail != NULL) {
        c0.tail->next = NULL;
    }
    S = c0.head;
}

void resetCounters() {
    compCount = 0;
    moveCount = 0;
}

void calculateTheoretical(int n, long long &theoryC, long long &theoryM) {
    int log2n = 0;
    while ((1 << log2n) < n) {
        log2n++;
    }

    theoryC = (long long)n * log2n;
    theoryM = (long long)n * log2n + n;
}

int main() {
    cout << "======================расщепление "
            "списка=========================\n\n";

    struct List *list = createStackRand(20);
    printList(list);

    struct List *a, *b;

    splitList(list, a, b);

    cout << "\nСписок а после разделения:";
    printList(a);

    cout << "\nСписок b после разделения:";
    printList(b);

    cout << "\n\nВсего элементов 20, в списке а: " << countList(a)
         << ", в списке b: " << countList(b);

    deleteList(&a);
    deleteList(&b);

    cout << "\n\n===========================================слияние "
            "серий=====================================\n\n";

    a = createStackAscending(5);
    b = createStackAscending(5);

    cout << "серия а:";
    printList(a);

    cout << "\nсерия b:";
    printList(b);

    cout << "\nСумма значений в а: " << sumList(a);
    cout << "\nСумма значений в b: " << sumList(b);

    struct Queue C;
    initQueue(&C);

    mergeSeries(a, 5, b, 5, &C);

    cout << "\n\nРезультат слияния в С:";
    printList(C.head);
    cout << "\nСумма значений в C: " << sumList(C.head);
    cout << "\nФактические сравнения: " << compCount;
    cout << "\nФактические перемещения: " << moveCount;

    long long theoryC, theoryM;

    calculateTheoretical(10, theoryC, theoryM);

    cout << "\n\nТеоретические сравнения: " << theoryC;
    cout << "\nТеоретические пересменки:  " << theoryM;

    deleteList(&C.head);

    cout << "\n\n==============================================MergeSort========="
            "=====================\n\n";

    int n = 15;
    list = createStackRand(n);

    cout << "Изначальный стек: ";
    printList(list);

    cout << "\nСумма до сортировки: " << sumList(list);

    resetCounters();

    mergeSort(list, n);

    cout << "\n\nСписок после сортировки: ";
    printList(list);

    cout << "\nСумма элементов после сортировки: " << sumList(list);
    cout << "\nС фактическое: " << compCount << ", М фактическое: " << moveCount;

    calculateTheoretical(n, theoryC, theoryM);

    cout << "\nС теоретическое: " << theoryC << ", М  теоретическое: " << theoryM;

    cout << "\n\nТаблица:\n\n";

    printf("%-5s|%-15s|%-33s|\n", "N", "M+C теор", "М+С факт");
    printf("%-5s|%-11s|%-12s|%-12s|%-13s|\n", " ", " ", "убыв", "случ", "возр");
    printf("----------------------------------------------\n");

    for (int i = 1; i < 6; i++) {
        int n = i * 100;

        struct List *dec = createStackDescending(n);
        struct List *ran = createStackRand(n);
        struct List *asc = createStackAscending(n);

        resetCounters();
        mergeSort(dec, n);
        calculateTheoretical(n, theoryC, theoryM);

        printf("%-5d|%-11lld|%-8lld", n, theoryC + theoryM, compCount + moveCount);

        resetCounters();
        mergeSort(ran, n);

        printf("|%-8lld|", compCount + moveCount);

        resetCounters();
        mergeSort(asc, n);

        printf("%-9lld|\n", compCount + moveCount);

        deleteList(&dec);
        deleteList(&ran);
        deleteList(&asc);

        printf("----------------------------------------------\n");
    }

    return 0;
}
