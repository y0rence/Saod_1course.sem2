#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node_char {
  char data[100];
  struct Node_char *next;
} Node_char;

typedef struct Stack {
  Node_char *front;
} Stack;

typedef struct hash_table {
  Stack **buckets;
  int size;
  int collisions;
  int count;
} HashTable;

unsigned long long hash_string(const char *s, int m) {
  unsigned long long h = 0;
  for (int i = 0; s[i] != '\0'; i++) {
    h = (h * 256 + (unsigned char)s[i]) % m;
  }
  return (unsigned long long)h;
}

HashTable *create_table(int size) {
  HashTable *table = (HashTable *)malloc(sizeof(HashTable));
  table->size = size;
  table->collisions = 0;
  table->count = 0;

  table->buckets = (Stack **)malloc(size * sizeof(Stack *));
  for (int i = 0; i < size; i++) {
    table->buckets[i] = (Stack *)malloc(sizeof(Stack));
    table->buckets[i]->front = NULL;
  }
  return table;
}

void insert(HashTable *table, const char *key, int m) {
  int index = hash_string(key, m);
  Node_char *current = table->buckets[index]->front;
  Node_char *last = NULL;

  while (current != NULL) {
    if (strcmp(current->data, key) == 0) {
      return;
    }
    last = current;
    current = current->next;
  }

  Node_char *new_node = (Node_char *)malloc(sizeof(Node_char));
  strcpy(new_node->data, key);
  new_node->next = NULL;

  if (last == NULL) {
    table->buckets[index]->front = new_node;
  } else {
    table->collisions++;
    last->next = new_node;
  }

  table->count++;
}

void print_table(HashTable *table) {
  if (!table)
    return;

  for (int i = 0; i < table->size; i++) {
    printf("[%2d]: ", i);
    Node_char *current = table->buckets[i]->front;
    while (current) {
      printf("\"%s\" -> ", current->data);
      current = current->next;
    }
    printf("NULL\n");
  }
  printf("Фактическое количество коллизий (Кф): %d | Всего элементов: %d\n\n",
         table->collisions, table->count);
}

void free_table(HashTable *table) {
  if (!table)
    return;
  for (int i = 0; i < table->size; i++) {
    Node_char *current = table->buckets[i]->front;
    while (current) {
      Node_char *temp = current;
      current = current->next;
      free(temp);
    }
    free(table->buckets[i]);
  }
  free(table->buckets);
  free(table);
}

int main() {
    const char *keys[] = {
        "ant",        "bear",      "cat",      "dog",
        "elephant",   "fox",       "goat",     "horse",
        "iguana",     "jaguar",    "koala",    "lion",
        "monkey",     "newt",      "octopus",  "penguin",
        "quail",      "rabbit",    "snake",    "tiger",
        "urchin",     "vulture",   "wolf",     "zebra"};
    int num_keys = sizeof(keys) / sizeof(keys[0]);
    printf("--- ВЫПОЛНЕНИЕ ЗАДАНИЯ 2 (Списки при m = 7) ---\n");
    int demo_m = 7;
    HashTable *demo_table = create_table(demo_m);
    for (int j = 0; j < 12; j++) {
        insert(demo_table, keys[j], demo_m);
    }
    print_table(demo_table);

    printf("--- ВЫПОЛНЕНИЕ ЗАДАНИЯ 3 (Таблица коллизий) ---\n");
    printf(
      "| Размер хеш-таблицы (m) | Количество ключей | Количество коллизий |\n");
    printf(
      "|------------------------|-------------------|---------------------|\n");

    int primes[10] = {11, 19, 29, 37, 47, 59, 67, 79, 89, 101};

    for (int i = 0; i < 10; i++) {
        int m = primes[i];
        HashTable *table = create_table(m);

        for (int j = 0; j < num_keys; j++) {
            insert(table, keys[j], m);
        }

        printf("| %22d | %17d | %19d |\n", m, num_keys, table->collisions);
        free_table(table);
    }

    return 0;
}
