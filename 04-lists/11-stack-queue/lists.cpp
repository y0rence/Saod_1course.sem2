#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

using namespace std;

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

void removeFromQueueByPosition(struct Queue *q, int position) {
  if (q == NULL || q->head == NULL || position <= 0) {
    return;
  }

  struct List *prev = q->head;
  for (int i = 1; i < position - 1 && prev->next != NULL; i++) {
    prev = prev->next;
  }

  if (prev->next == NULL) {
    return;
  }

  struct List *toDelete = prev->next;
  prev->next = toDelete->next;
  if (toDelete == q->tail) {
    q->tail = prev;
  }
  delete toDelete;
}

void removeSecondAndFourthFromQueue(struct Queue *q) {
  removeFromQueueByPosition(q, 4);
  removeFromQueueByPosition(q, 2);
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

int main() {
  srand(time(NULL));

  cout << "Стек (работа с ним)" << endl << endl;
  struct List *stack1 = createStackAscending(10);
  cout << "Cтек возрастающий: ";
  printList(stack1);
  struct List *stack2 = createStackDescending(10);
  cout << endl << "Cтек убывающий: ";
  printList(stack2);
  struct List *stack3 = createStackRand(10);
  cout << endl << "Cтек рандомный: ";
  printList(stack3);
  cout << "Работа с очередью" << endl;
  struct Queue queue1 = createQueueAscending(10);

  cout << "Очередь возрастающая: ";
  printList(queue1.head);

  struct Queue queue2 = createQueueDescending(10);

  cout << endl << "Очередь убывающая: ";
  printList(queue2.head);

  struct Queue queue3 = createQueueRand(10);

  cout << endl << "Очередь рандомная: ";
  printList(queue3.head);

  cout << endl << endl << "=== Доп задание ===" << endl;
  cout << "Удалить 2-й и 4-й элементы из очереди из 10 элементов" << endl;
  struct Queue queue4 = createQueueAscending(10);
  cout << "до удаления: ";
  printList(queue4.head);
  removeSecondAndFourthFromQueue(&queue4);
  cout << "после удаления 2-го и 4-го элементов: ";
  printList(queue4.head);

  cout << endl << "Демонстрация функций для работы со списками: " << endl;

  cout << "Контрольная сумма: " << sumList(stack1) << endl;
  cout << "Подсчет серий в возрастающем стеке: " << countSeries(stack1)
       << ", в убывающем: " << countSeries(stack2) << endl;
  cout << "Подсчет серий в возрастающей очереди: " << countSeries(queue1.head)
       << ", в убывающей: " << countSeries(queue2.head);

  cout << endl << endl << "Рекурсивная печать в прямом порядке: ";
  printListForward(stack1);
  cout << endl << "Обратный порядок: ";
  printListReverse(stack1);

  deleteList(&stack1);
  deleteList(&stack2);
  deleteList(&stack3);
  deleteList(&queue1.head);
  deleteList(&queue2.head);
  deleteList(&queue3.head);
  deleteList(&queue4.head);

  cout << endl << endl << "stack1 после удаления:";
  printList(stack1);

  return 0;
}
