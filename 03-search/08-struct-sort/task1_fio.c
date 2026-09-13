#include <stdio.h>
#include <string.h>

enum { kPeopleCount = 4, kTextSize = 64 };

typedef struct {
  char last_name[kTextSize];
  char first_name[kTextSize];
  char middle_name[kTextSize];
} Person;

int ComparePersons(const Person *left, const Person *right) {
  int result = strcmp(left->last_name, right->last_name);
  if (result != 0) {
    return result;
  }
  return strcmp(left->first_name, right->first_name);
}

void SwapPersons(Person *left, Person *right) {
  Person temp = *left;
  *left = *right;
  *right = temp;
}

void PrintPeople(const Person people[], int size) {
  for (int i = 0; i < size; ++i) {
    printf("%d. %s %s %s\n", i + 1, people[i].last_name, people[i].first_name,
           people[i].middle_name);
  }
}

void ManualSort(Person people[]) {
  if (ComparePersons(&people[0], &people[1]) > 0) {
    SwapPersons(&people[0], &people[1]);
  }
  if (ComparePersons(&people[0], &people[2]) > 0) {
    SwapPersons(&people[0], &people[2]);
  }
  if (ComparePersons(&people[0], &people[3]) > 0) {
    SwapPersons(&people[0], &people[3]);
  }
  if (ComparePersons(&people[1], &people[2]) > 0) {
    SwapPersons(&people[1], &people[2]);
  }
  if (ComparePersons(&people[1], &people[3]) > 0) {
    SwapPersons(&people[1], &people[3]);
  }
  if (ComparePersons(&people[2], &people[3]) > 0) {
    SwapPersons(&people[2], &people[3]);
  }
}

int BinarySearchByLastName(const Person people[], int size,
                           const char *last_name) {
  int left = 0;
  int right = size - 1;

  if (size <= 0) {
    return -1;
  }

  while (left < right) {
    int middle = (left + right) / 2;

    if (strcmp(people[middle].last_name, last_name) < 0) {
      left = middle + 1;
    } else {
      right = middle;
    }
  }

  return strcmp(people[right].last_name, last_name) == 0 ? right : -1;
}

void FindByLastName(const Person people[], int size, const char *last_name) {
  int index = BinarySearchByLastName(people, size, last_name);

  if (index == -1) {
    printf("Люди с фамилией %s не найдены.\n", last_name);
    return;
  }

  while (index < size && strcmp(people[index].last_name, last_name) == 0) {
    printf("%s %s %s\n", people[index].last_name, people[index].first_name,
           people[index].middle_name);
    ++index;
  }
}

int main(void) {
  Person people[kPeopleCount] = {
      {"Иванов", "Дмитрий", "Сергеевич"},
      {"Петров", "Андрей", "Олегович"},
      {"Иванов", "Алексей", "Игоревич"},
      {"Сидоров", "Максим", "Павлович"},
  };
  char search_last_name[kTextSize];

  printf("Исходный набор ФИО:\n");
  PrintPeople(people, kPeopleCount);

  ManualSort(people);

  printf("\nПосле ручной сортировки по фамилии и имени:\n");
  PrintPeople(people, kPeopleCount);

  printf("\nВведите фамилию для поиска: ");
  if (scanf("%63s", search_last_name) != 1) {
    return 1;
  }

  printf("\nРезультат поиска:\n");
  FindByLastName(people, kPeopleCount, search_last_name);

  return 0;
}
