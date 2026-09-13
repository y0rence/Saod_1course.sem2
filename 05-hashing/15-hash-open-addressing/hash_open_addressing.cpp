#include <cmath>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

using namespace std;

struct List {
  char data;
  struct List *next;
};

int hashFunc(char k, int m) { return (unsigned char)k % m; }

void insert_chain(List *table[], int m, char key) {
  int ind = hashFunc(key, m);
  List *cur = table[ind];
  List *last = NULL;

  while (cur != NULL) {
    if (cur->data == key) {
      return;
    }
    last = cur;
    cur = cur->next;
  }

  List *p = new List{key, NULL};
  if (last == NULL) {
    table[ind] = p;
  } else {
    last->next = p;
  }
}

void printTable_chain(List *table[], int m) {
  printf("Таблица (метод цепочек):\n");
  for (int i = 0; i < m; i++) {
    printf("Индекс %2d: ", i);
    List *cur = table[i];
    if (cur == NULL) {
      printf("Пусто");
    } else {
      bool first = true;
      while (cur != NULL) {
        if (!first)
          printf(" -> ");
        printf("%c", cur->data);
        cur = cur->next;
        first = false;
      }
    }
    printf("\n");
  }
}

bool search_chain(List *table[], int m, char key) {
  int ind = hashFunc(key, m);
  List *cur = table[ind];
  int pos = 1;

  while (cur != NULL) {
    if (cur->data == key) {
      printf("Элемент '%c' найден. Индекс хэша: %d, позиция в цепочке: %d\n",
             key, ind, pos);
      return true;
    }
    cur = cur->next;
    pos++;
  }

  printf("Элемент '%c' не найден\n", key);
  return false;
}

int countCollisions_chain(List *table[], int m) {
  int collisions = 0;
  for (int i = 0; i < m; i++) {
    for (List *cur = table[i]; cur != NULL && cur->next != NULL;
         cur = cur->next) {
      collisions++;
    }
  }
  return collisions;
}

void clearTable_chain(List *table[], int m) {
  for (int i = 0; i < m; i++) {
    List *cur = table[i];
    while (cur != NULL) {
      List *t = cur;
      cur = cur->next;
      delete t;
    }
    table[i] = NULL;
  }
}

int insertLinear(char *table, int m, char key, int &collisions) {
  int h = hashFunc(key, m);

  for (int i = 0; i < m; i++) {
    if (table[h] == key) {
      return h;
    }
    if (table[h] == 0) {
      table[h] = key;
      return h;
    }
    collisions++;
    h++;
    if (h >= m) {
      h -= m;
    }
  }
  return -1;
}

int insertQuadratic(char *table, int m, char key, int &collisions) {
  int h = hashFunc(key, m);
  int d = 1;

  while (true) {
    if (table[h] == key) {
      return h;
    }
    if (table[h] == 0) {
      table[h] = key;
      return h;
    }
    if (d >= m) {
      return -1;
    }
    collisions++;
    h += d;
    if (h >= m) {
      h -= m;
    }
    d += 2;
  }
}

int searchLinear(char *table, int m, char key) {
  int h0 = hashFunc(key, m);
  printf("Поиск '%c' (линейные пробы): ", key);

  for (int i = 0; i < m; i++) {
    int idx = (h0 + i) % m;
    printf("%d ", idx);

    if (table[idx] == key) {
      printf("-> найдено на позиции %d\n", idx);
      return idx;
    }
    if (table[idx] == 0) {
      printf("-> пусто, элемент отсутствует\n");
      return -1;
    }
  }
  printf("-> элемент отсутствует\n");
  return -1;
}

int searchQuadratic(char *table, int m, char key) {
  int h = hashFunc(key, m);
  int d = 1;
  printf("Поиск '%c' (квадратичные пробы): ", key);

  while (true) {
    printf("%d ", h);
    if (table[h] == key) {
      printf("-> найдено на позиции %d\n", h);
      return h;
    }
    if (table[h] == 0) {
      printf("-> пусто, элемент отсутствует\n");
      return -1;
    }
    if (d >= m) {
      printf("-> элемент отсутствует\n");
      return -1;
    }
    h += d;
    if (h >= m) {
      h -= m;
    }
    d += 2;
  }
}

void printTableOA(char *table, int m) {
  printf("Хэш-таблица (открытая адресация):\n");
  for (int i = 0; i < m; i++) {
    printf("Индекс %2d: ", i);
    if (table[i] == 0) {
      printf("Пусто");
    } else {
      printf("%c", table[i]);
    }
    printf("\n");
  }
}

void task3_compare(string data) {
  int n = (int)data.length();
  int primes[] = {11, 13, 17, 19, 23, 29, 31, 37, 41, 101};

  printf("\n%-15s | %-15s | %-20s | %-20s | %-20s\n", "Размер (m)",
         "Символов (n)", "Цепочки", "Линейные пробы", "Квадратичные пр.");
  printf("---------------------------------------------------------------------"
         "--------------------\n");

  for (int i = 0; i < 10; i++) {
    int m = primes[i];

    List **tableChain = new List *[m];
    for (int j = 0; j < m; j++)
      tableChain[j] = NULL;
    for (char c : data)
      insert_chain(tableChain, m, c);
    int col_chain = countCollisions_chain(tableChain, m);
    clearTable_chain(tableChain, m);
    delete[] tableChain;

    char *tableLinear = new char[m]();
    char *tableQuad = new char[m]();
    int col_lin = 0;
    int col_quad = 0;
    for (char c : data) {
      insertLinear(tableLinear, m, c, col_lin);
      insertQuadratic(tableQuad, m, c, col_quad);
    }
    delete[] tableLinear;
    delete[] tableQuad;

    printf("%-15d %-15d %-20d %-20d %-20d\n", m, n, col_chain, col_lin,
           col_quad);
  }
}

int main() {
  string fio = "OBEREMOKSERGEY";
  int n = (int)fio.length();
  int m = 14;

  printf("=== ЗАДАНИЕ 2: Построение хэш-таблиц ===\n\n");
  printf("Строка: %s  |  n = %d  |  m = %d\n\n", fio.c_str(), n, m);

  List **tableChain = new List *[m];
  for (int i = 0; i < m; i++)
    tableChain[i] = NULL;
  for (char c : fio)
    insert_chain(tableChain, m, c);
  printTable_chain(tableChain, m);

  char *tableLinear = new char[m]();
  int col_lin = 0;
  for (char c : fio)
    insertLinear(tableLinear, m, c, col_lin);
  printf("\nЛинейные пробы:\n");
  printTableOA(tableLinear, m);
  printf("Количество коллизий: %d\n", col_lin);

  char *tableQuad = new char[m]();
  int col_quad = 0;
  for (char c : fio)
    insertQuadratic(tableQuad, m, c, col_quad);
  printf("\nКвадратичные пробы:\n");
  printTableOA(tableQuad, m);
  printf("Количество коллизий: %d\n", col_quad);

  printf("\n=== ЗАДАНИЕ 3: Сравнение коллизий ===\n");
  string test_data = "ANTBEARCATDOGFOXGOATLIONWOLF";
  task3_compare(test_data);

  printf("\n=== ЗАДАНИЕ 4*: Поиск элемента ===\n\n");
  char search_key = 'E';

  printf("Ключ для поиска: '%c'\n\n", search_key);

  printf("[Цепочки]       ");
  search_chain(tableChain, m, search_key);

  searchLinear(tableLinear, m, search_key);
  searchQuadratic(tableQuad, m, search_key);

  clearTable_chain(tableChain, m);
  delete[] tableChain;
  delete[] tableLinear;
  delete[] tableQuad;

  return 0;
}
