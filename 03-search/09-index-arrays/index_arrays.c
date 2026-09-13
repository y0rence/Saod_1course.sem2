#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define LETTER_COUNT 12
#define CONTACT_COUNT 5
#define MAX_TEXT 32
#define MAX_PHONE 24

typedef struct {
  char last_name[MAX_TEXT];
  char first_name[MAX_TEXT];
  char phone[MAX_PHONE];
  char city[MAX_TEXT];
} Contact;

typedef enum { KEY_LAST_NAME, KEY_PHONE } ContactKey;

static bool is_vowel(char ch) {
  char upper = (char)toupper((unsigned char)ch);
  return upper == 'A' || upper == 'E' || upper == 'I' || upper == 'O' ||
         upper == 'U' || upper == 'Y';
}

static bool is_consonant(char ch) {
  return isalpha((unsigned char)ch) && !is_vowel(ch);
}

static int compare_chars(char left, char right) {
  int l = toupper((unsigned char)left);
  int r = toupper((unsigned char)right);
  if (l < r) {
    return -1;
  }
  if (l > r) {
    return 1;
  }
  return 0;
}

static void fill_identity_index(int index[], int n) {
  for (int i = 0; i < n; i++) {
    index[i] = i;
  }
}

static void sort_letter_index(const char source[], int index[], int n,
                              bool ascending) {
  for (int i = 0; i < n - 1; i++) {
    for (int j = n - 1; j > i; j--) {
      int cmp = compare_chars(source[index[j]], source[index[j - 1]]);
      bool swap = ascending ? (cmp < 0) : (cmp > 0);
      if (swap) {
        int t = index[j];
        index[j] = index[j - 1];
        index[j - 1] = t;
      }
    }
  }
}

static int build_filtered_letter_index(const char source[], int n, int index[],
                                       bool (*keep)(char), bool ascending) {
  int count = 0;
  for (int i = 0; i < n; i++) {
    if (keep(source[i])) {
      index[count++] = i;
    }
  }
  sort_letter_index(source, index, count, ascending);
  return count;
}

static void print_letters_with_positions(const char source[], int n) {
  printf("Исходный массив (позиция:символ):\n");
  for (int i = 0; i < n; i++) {
    printf("%2d:%c  ", i + 1, source[i]);
  }
  printf("\n");
}

static void print_letter_index(const char *title, const char source[],
                               const int index[], int n) {
  printf("%s\n", title);
  printf("Индексы (1..12): ");
  for (int i = 0; i < n; i++) {
    printf("%d ", index[i] + 1);
  }
  printf("\nСимволы:         ");
  for (int i = 0; i < n; i++) {
    printf("%c ", source[index[i]]);
  }
  printf("\n\n");
}

static int compare_contacts(const Contact *left, const Contact *right,
                            ContactKey key) {
  int cmp = 0;
  if (key == KEY_LAST_NAME) {
    cmp = strcmp(left->last_name, right->last_name);
    if (cmp != 0) {
      return cmp;
    }
    cmp = strcmp(left->first_name, right->first_name);
    if (cmp != 0) {
      return cmp;
    }
    return strcmp(left->phone, right->phone);
  }

  cmp = strcmp(left->phone, right->phone);
  if (cmp != 0) {
    return cmp;
  }
  cmp = strcmp(left->last_name, right->last_name);
  if (cmp != 0) {
    return cmp;
  }
  return strcmp(left->first_name, right->first_name);
}

static void sort_contact_index(const Contact contacts[], int index[], int n,
                               ContactKey key) {
  for (int i = 0; i < n - 1; i++) {
    for (int j = n - 1; j > i; j--) {
      if (compare_contacts(&contacts[index[j]], &contacts[index[j - 1]], key) <
          0) {
        int t = index[j];
        index[j] = index[j - 1];
        index[j - 1] = t;
      }
    }
  }
}

static void print_contacts_by_index(const char *title, const Contact contacts[],
                                    const int index[], int n) {
  printf("%s\n", title);
  printf("--------------------------------------------------------------\n");
  printf("%-4s %-12s %-12s %-16s %-12s\n", "№", "Фамилия", "Имя", "Телефон",
         "Город");
  printf("--------------------------------------------------------------\n");
  for (int i = 0; i < n; i++) {
    const Contact *c = &contacts[index[i]];
    printf("%-4d %-12s %-12s %-16s %-12s\n", i + 1, c->last_name, c->first_name,
           c->phone, c->city);
  }
  printf("--------------------------------------------------------------\n\n");
}

static int compare_key_to_contact(const char *key, const Contact *contact,
                                  ContactKey contact_key) {
  if (contact_key == KEY_LAST_NAME) {
    return strcmp(key, contact->last_name);
  }
  return strcmp(key, contact->phone);
}

static void binary_search_contact(const Contact contacts[], const int index[],
                                  int n, ContactKey key, const char *wanted) {
  int left = 0;
  int right = n - 1;

  while (left < right) {
    int mid = (left + right) / 2;
    if (compare_key_to_contact(wanted, &contacts[index[mid]], key) > 0) {
      left = mid + 1;
    } else {
      right = mid;
    }
  }

  if (n <= 0 ||
      compare_key_to_contact(wanted, &contacts[index[right]], key) != 0) {
    printf("Запись не найдена.\n");
    return;
  }

  printf("Найденные записи:\n");
  int count = 0;
  for (int i = right; i < n; i++) {
    const Contact *current = &contacts[index[i]];
    if (compare_key_to_contact(wanted, current, key) != 0) {
      break;
    }
    printf("  %s %s, %s, %s\n", current->last_name, current->first_name,
           current->phone, current->city);
    count++;
  }
  printf("Всего найдено: %d\n", count);
}

int main(void) {
  const char fio12[LETTER_COUNT] = {
      'S', 'E', 'R', 'G', 'E', 'J', 'O', 'B', 'E', 'R', 'E', 'M',
  };

  int index_asc[LETTER_COUNT];
  int index_desc[LETTER_COUNT];
  int index_consonants[LETTER_COUNT];
  int index_vowels[LETTER_COUNT];

  fill_identity_index(index_asc, LETTER_COUNT);
  fill_identity_index(index_desc, LETTER_COUNT);
  sort_letter_index(fio12, index_asc, LETTER_COUNT, true);
  sort_letter_index(fio12, index_desc, LETTER_COUNT, false);

  int consonant_count = build_filtered_letter_index(
      fio12, LETTER_COUNT, index_consonants, is_consonant, true);
  int vowel_count = build_filtered_letter_index(fio12, LETTER_COUNT,
                                                index_vowels, is_vowel, false);

  printf("Задание 1. Индексные массивы для 12 символов\n");
  printf("==============================================================\n");
  print_letters_with_positions(fio12, LETTER_COUNT);
  print_letter_index("1) Сортировка по возрастанию:", fio12, index_asc,
                     LETTER_COUNT);
  print_letter_index("2) Сортировка по убыванию:", fio12, index_desc,
                     LETTER_COUNT);
  print_letter_index("3) Только согласные по возрастанию:", fio12,
                     index_consonants, consonant_count);
  print_letter_index("4) Только гласные по убыванию:", fio12, index_vowels,
                     vowel_count);

  Contact contacts[CONTACT_COUNT] = {
      {"Ivanov", "Petr", "+7-913-100-10-10", "Novosibirsk"},
      {"Sidorov", "Alexey", "+7-913-222-20-20", "Tomsk"},
      {"Smirnov", "Nikita", "+7-913-555-50-50", "Omsk"},
      {"Petrov", "Egor", "+7-913-333-30-30", "Barnaul"},
      {"Kuznetsov", "Ilya", "+7-913-333-30-30", "Kemerovo"},
  };

  int index_by_name[CONTACT_COUNT];
  int index_by_phone[CONTACT_COUNT];

  fill_identity_index(index_by_name, CONTACT_COUNT);
  fill_identity_index(index_by_phone, CONTACT_COUNT);
  sort_contact_index(contacts, index_by_name, CONTACT_COUNT, KEY_LAST_NAME);
  sort_contact_index(contacts, index_by_phone, CONTACT_COUNT, KEY_PHONE);

  printf("Задания 2-4. Сортировка справочника через индексные массивы\n");
  printf("==============================================================\n");
  print_contacts_by_index("Справочник, отсортированный по фамилии:", contacts,
                          index_by_name, CONTACT_COUNT);
  print_contacts_by_index("Справочник, отсортированный по телефону:", contacts,
                          index_by_phone, CONTACT_COUNT);

  const char *search_name = "Smirnov";
  const char *search_phone = "+7-913-333-30-30";

  printf("Двоичный поиск по двум ключам\n");
  printf("==============================================================\n");
  printf("По фамилии \"%s\":\n", search_name);
  binary_search_contact(contacts, index_by_name, CONTACT_COUNT, KEY_LAST_NAME,
                        search_name);

  printf("\nПо телефону \"%s\":\n", search_phone);
  binary_search_contact(contacts, index_by_phone, CONTACT_COUNT, KEY_PHONE,
                        search_phone);

  return 0;
}
