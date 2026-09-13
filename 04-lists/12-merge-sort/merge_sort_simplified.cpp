#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <iostream>

using namespace std;

long long compCount = 0;
long long moveCount = 0;

struct List {
  int data;
  List *next;
};

struct Queue {
  List *head;
  List *tail;
};

void resetCounters() {
  compCount = 0;
  moveCount = 0;
}

void initQueue(Queue *q) {
  q->head = NULL;
  q->tail = NULL;
}

void moveToQueue(List *&list, Queue *q) {
  List *p = list;
  list = list->next;
  p->next = NULL;

  if (q->head == NULL) {
    q->head = p;
  } else {
    q->tail->next = p;
  }
  q->tail = p;
  moveCount++;
}

List *push(List *head, int value) {
  List *p = new List;
  p->data = value;
  p->next = head;
  return p;
}

List *createAscendingList(int n) {
  List *head = NULL;

  for (int i = n; i >= 1; i--) {
    head = push(head, i);
  }

  return head;
}

List *createDescendingList(int n) {
  List *head = NULL;

  for (int i = 1; i <= n; i++) {
    head = push(head, i);
  }

  return head;
}

List *createRandomList(int n) {
  List *head = NULL;

  for (int i = 0; i < n; i++) {
    head = push(head, rand() % 100 + 10);
  }

  return head;
}

void printList(List *head) {
  cout << "\nСписок: ";

  while (head != NULL) {
    cout << head->data << " ";
    head = head->next;
  }

  cout << "\n";
}

int sumList(List *head) {
  int sum = 0;

  while (head != NULL) {
    sum += head->data;
    head = head->next;
  }

  return sum;
}

int countList(List *head) {
  int count = 0;

  while (head != NULL) {
    count++;
    head = head->next;
  }

  return count;
}

void deleteList(List **head) {
  while (*head != NULL) {
    List *temp = *head;
    *head = (*head)->next;
    delete temp;
  }
}

void calculateTheoretical(int n, long long &theoryC, long long &theoryM) {
  int log2n = 0;

  while ((1 << log2n) < n) {
    log2n++;
  }

  theoryC = (long long)n * log2n;
  theoryM = (long long)n * log2n + n;
}

void splitList(List *S, List *&a, List *&b) {
  if (S == NULL) {
    a = NULL;
    b = NULL;
    return;
  }

  a = S;
  b = S->next;

  List *k = a;
  List *p = b;

  while (p != NULL) {
    k->next = p->next;
    k = p;
    p = p->next;
  }
}

void mergeSeries(List *&a, int q, List *&b, int r, Queue *c) {
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

void mergeSort(List *&S, int n) {
  if (S == NULL || S->next == NULL) {
    return;
  }

  List *a;
  List *b;
  splitList(S, a, b);

  Queue c0;
  Queue c1;
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

      if (i == 0) {
        mergeSeries(a, q, b, r, &c0);
      } else {
        mergeSeries(a, q, b, r, &c1);
      }

      i = 1 - i;
    }

    a = c0.head;
    b = c1.head;
    p *= 2;
  }

  S = c0.head;
}

void printMergeSortTable() {
  long long theoryC;
  long long theoryM;

  cout << "\n\nТрудоемкость сортировки прямого слияния\n\n";
  printf("%-5s|%-15s|%-33s|\n", "N", "M+C теор", "Mфакт+Cфакт");
  printf("%-5s|%-15s|%-10s|%-10s|%-10s|\n", "", "", "Убыв.", "Случ.", "Возр.");
  printf("-------------------------------------------------------------\n");

  for (int n = 100; n <= 500; n += 100) {
    List *dec = createDescendingList(n);
    List *rnd = createRandomList(n);
    List *asc = createAscendingList(n);

    calculateTheoretical(n, theoryC, theoryM);

    resetCounters();
    mergeSort(dec, n);
    long long decFact = compCount + moveCount;

    resetCounters();
    mergeSort(rnd, n);
    long long rndFact = compCount + moveCount;

    resetCounters();
    mergeSort(asc, n);
    long long ascFact = compCount + moveCount;

    printf("%-5d|%-15lld|%-10lld|%-10lld|%-10lld|\n", n, theoryC + theoryM,
           decFact, rndFact, ascFact);

    deleteList(&dec);
    deleteList(&rnd);
    deleteList(&asc);
  }
}

int main() {
  srand(time(NULL));

  cout << "==================== Расщепление списка ====================\n";

  List *list = createRandomList(20);
  printList(list);

  List *a;
  List *b;
  splitList(list, a, b);

  cout << "\nСписок a после расщепления:";
  printList(a);

  cout << "\nСписок b после расщепления:";
  printList(b);

  cout << "\nВсего элементов: 20";
  cout << "\nВ списке a: " << countList(a);
  cout << "\nВ списке b: " << countList(b) << "\n";

  deleteList(&a);
  deleteList(&b);

  cout << "\n==================== Слияние серий ====================\n";

  a = createAscendingList(5);
  b = createAscendingList(5);

  cout << "\nСерия a:";
  printList(a);

  cout << "\nСерия b:";
  printList(b);

  cout << "\nСумма a: " << sumList(a);
  cout << "\nСумма b: " << sumList(b);

  Queue c;
  initQueue(&c);
  resetCounters();
  mergeSeries(a, 5, b, 5, &c);

  cout << "\n\nРезультат слияния в c:";
  printList(c.head);

  cout << "\nСумма c: " << sumList(c.head);
  cout << "\nCфакт: " << compCount;
  cout << "\nMфакт: " << moveCount << "\n";

  deleteList(&c.head);

  cout << "\n==================== MergeSort ====================\n";

  int n = 15;
  list = createRandomList(n);

  cout << "\nИсходный список:";
  printList(list);
  cout << "\nСумма до сортировки: " << sumList(list);

  resetCounters();
  mergeSort(list, n);

  cout << "\n\nСписок после сортировки:";
  printList(list);
  cout << "\nСумма после сортировки: " << sumList(list);
  cout << "\nCфакт: " << compCount;
  cout << "\nMфакт: " << moveCount << "\n";

  deleteList(&list);

  printMergeSortTable();

  return 0;
}
