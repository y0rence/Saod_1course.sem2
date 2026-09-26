#include <stdio.h>
#include <string.h>

enum { kSubscribersCount = 5, kTextSize = 64 };

typedef struct {
    char last_name[kTextSize];
    char first_name[kTextSize];
    char street[kTextSize];
    char phone[kTextSize];
} Subscriber;

int CompareSubscribers(const Subscriber *left, const Subscriber *right) {
    int result = strcmp(left->last_name, right->last_name);
    if (result != 0) {
        return result;
    }
    return strcmp(left->first_name, right->first_name);
}

void PrintDirectory(const Subscriber directory[], int size) {
    for (int i = 0; i < size; ++i) {
        printf("%d. %s %s, %s, %s\n", i + 1, directory[i].last_name, directory[i].first_name,
               directory[i].street, directory[i].phone);
    }
}

void InsertionSort(Subscriber directory[], int size) {
    for (int i = 1; i < size; ++i) {
        Subscriber current = directory[i];
        int j = i - 1;

        while (j >= 0 && CompareSubscribers(&directory[j], &current) > 0) {
            directory[j + 1] = directory[j];
            --j;
        }

        directory[j + 1] = current;
    }
}

int BinarySearchByLastName(const Subscriber directory[], int size, const char *last_name) {
    int left = 0;
    int right = size - 1;

    if (size <= 0) {
        return -1;
    }

    while (left < right) {
        int middle = (left + right) / 2;

        if (strcmp(directory[middle].last_name, last_name) < 0) {
            left = middle + 1;
        } else {
            right = middle;
        }
    }

    return strcmp(directory[right].last_name, last_name) == 0 ? right : -1;
}

void PrintFoundSubscribers(const Subscriber directory[], int size, const char *last_name) {
    int index = BinarySearchByLastName(directory, size, last_name);

    if (index == -1) {
        printf("\nАбоненты с фамилией %s не найдены.\n", last_name);
        return;
    }

    printf("\nРезультат быстрого двоичного поиска по фамилии %s:\n", last_name);
    while (index < size && strcmp(directory[index].last_name, last_name) == 0) {
        printf("%s %s, %s, %s\n", directory[index].last_name, directory[index].first_name,
               directory[index].street, directory[index].phone);
        ++index;
    }
}

int main(void) {
    Subscriber directory[kSubscribersCount] = {
        {"Иванов", "Андрей", "ул. Ленина, 10", "111-11-11"},
        {"Петров", "Борис", "ул. Мира, 5", "222-22-22"},
        {"Сидоров", "Илья", "ул. Центральная, 7", "333-33-33"},
        {"Иванов", "Алексей", "ул. Советская, 12", "444-44-44"},
        {"Кузнецов", "Олег", "ул. Школьная, 3", "555-55-55"},
    };

    printf("Исходный телефонный справочник:\n");
    PrintDirectory(directory, kSubscribersCount);

    InsertionSort(directory, kSubscribersCount);

    printf("\nТелефонный справочник после сортировки:\n");
    PrintDirectory(directory, kSubscribersCount);

    PrintFoundSubscribers(directory, kSubscribersCount, "Иванов");

    return 0;
}
