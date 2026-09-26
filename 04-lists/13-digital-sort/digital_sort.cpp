#include <SFML/Graphics.hpp>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <string.h>
#include <vector>

using namespace std;

long long Mf_global = 0;

struct tLE {
    tLE *Next;
    union {
        unsigned int Data;
        unsigned char Digit[32];
        char Name[32];
    };
};

struct Queue {
    tLE *Head, *Tail;
};

void initQueue(Queue &Q) {
    Q.Tail = (tLE *)&Q.Head;
}

void DigitalSort(tLE **S, int L, int KDI[], bool reverse) {
    Mf_global = 0;
    int m = 256;
    Queue Q[256];

    for (int j = 0; j < L; j++) {
        for (int i = 0; i < m; i++)
            initQueue(Q[i]);
        tLE *p = *S;
        int k = KDI[j];

        while (p) {
            unsigned char d = p->Digit[k];
            Q[d].Tail->Next = p;
            Q[d].Tail = p;
            p = p->Next;
            Mf_global++;
        }

        tLE *tempHead = NULL;
        tLE *lastTail = (tLE *)&tempHead;

        if (!reverse) {
            for (int i = 0; i < m; i++) {
                if (Q[i].Tail != (tLE *)&Q[i].Head) {
                    lastTail->Next = Q[i].Head;
                    lastTail = Q[i].Tail;
                }
            }
        } else {
            for (int i = m - 1; i >= 0; i--) {
                if (Q[i].Tail != (tLE *)&Q[i].Head) {
                    lastTail->Next = Q[i].Head;
                    lastTail = Q[i].Tail;
                }
            }
        }
        lastTail->Next = NULL;
        *S = tempHead;
    }
}

long long calculateControlSum(tLE *head) {
    long long sum = 0;
    while (head) {
        sum += head->Data;
        head = head->Next;
    }
    return sum;
}

int countSeries(tLE *head) {
    if (!head)
        return 0;
    int s = 1;
    while (head->Next) {
        if (head->Data > head->Next->Data)
            s++;
        head = head->Next;
    }
    return s;
}

tLE *createList(int n) {
    tLE *head = NULL;
    for (int i = 0; i < n; i++) {
        tLE *p = new tLE;
        p->Data = rand() % 10000;
        p->Next = head;
        head = p;
    }
    return head;
}

tLE *createListDescending(int n) {
    tLE *head = NULL;
    for (int i = 0; i < n; i++) {
        tLE *p = new tLE;
        p->Data = i;
        p->Next = head;
        head = p;
    }
    return head;
}

tLE *createListAscending(int n) {
    tLE *head = NULL;
    for (int i = 0; i < n; i++) {
        tLE *p = new tLE;
        p->Data = n - 1 - i;
        p->Next = head;
        head = p;
    }
    return head;
}

void printListInts(tLE *head) {
    while (head) {
        cout << head->Data << " ";
        head = head->Next;
    }
    cout << endl;
}

tLE *createNameNode(const char *name, tLE *head) {
    tLE *p = new tLE;
    memset(p->Name, 0, 32);
    strncpy(p->Name, name, 31);
    p->Next = head;
    return p;
}

void printListNames(tLE *head) {
    while (head) {
        cout << head->Name << " ";
        head = head->Next;
    }
    cout << endl;
}

int main() {
    srand(time(0));
    setlocale(LC_ALL, "ru_RU.UTF-8");

    int KDI_int[] = {0, 1, 2, 3};

    cout << "Задание 2: Базовый алгоритм, Контрольные суммы, Серии\n";
    tLE *testList = createList(15);
    cout << "[ ДО СОРТИРОВКИ ]\n";
    cout << "Список: ";
    printListInts(testList);
    cout << "Сумма:  " << calculateControlSum(testList) << " | Серий: " << countSeries(testList)
         << "\n\n";

    DigitalSort(&testList, 4, KDI_int, false);
    cout << "[ ПОСЛЕ СОРТИРОВКИ ]\n";
    cout << "Список: ";
    printListInts(testList);
    cout << "Сумма:  " << calculateControlSum(testList) << " | Серий: " << countSeries(testList)
         << "\n";

    cout << "Задание 4: Таблица зависимости трудоемкости\n";
    cout << setw(5) << "N" << " | " << setw(10) << "Теоретич." << " | " << setw(8) << "Убыв."
         << " | " << setw(8) << "Случ." << " | " << setw(8) << "Возр."
         << "\n";
    cout << "-----------------------------------------------------------------\n";

    vector<pair<float, float>> dsPoints, qsPoints;

    for (int n = 100; n <= 1000; n += 100) {
        int m_theor = 4 * n;

        tLE *listDesc = createListDescending(n);
        DigitalSort(&listDesc, 4, KDI_int, false);
        long long mf_desc = Mf_global;

        tLE *listRand = createList(n);
        DigitalSort(&listRand, 4, KDI_int, false);
        long long mf_rand = Mf_global;

        tLE *listAsc = createListAscending(n);
        DigitalSort(&listAsc, 4, KDI_int, false);
        long long mf_asc = Mf_global;

        if (n <= 500) {
            cout << setw(5) << n << " | " << setw(10) << m_theor << " | " << setw(8) << mf_desc
                 << " | " << setw(8) << mf_rand << " | " << setw(8) << mf_asc << "\n";
        }

        dsPoints.push_back({(float)n, (float)mf_rand});
        qsPoints.push_back({(float)n, (float)(n * log2(n))});
    }

    cout << "\nЗадание 3: Сортировка по убыванию (Реверс) 4 байта \n";
    tLE *reverseList = createList(10);
    DigitalSort(&reverseList, 4, KDI_int, true);

    cout << "Элементы по убыванию: ";
    printListInts(reverseList);
    cout << "\n";

    cout << "\n Сортировка по убыванию (Реверс) 2 байта\n";
    reverseList = createList(10);
    DigitalSort(&reverseList, 2, KDI_int, true);

    cout << " Элементы по убыванию: ";
    printListInts(reverseList);
    cout << "\n";

    cout << "Задание 6*: Сортировка списка русских фамилий\n";
    tLE *namesList = NULL;
    namesList = createNameNode("Смирнов", namesList);
    namesList = createNameNode("Иванов", namesList);
    namesList = createNameNode("Петров", namesList);
    namesList = createNameNode("Яковлев", namesList);
    namesList = createNameNode("Алексеев", namesList);

    cout << "Исходный список: ";
    printListNames(namesList);

    int KDI_str[32];
    for (int i = 0; i < 32; i++)
        KDI_str[i] = 31 - i;

    DigitalSort(&namesList, 32, KDI_str, false);
    cout << "По возрастанию:  ";
    printListNames(namesList);

    DigitalSort(&namesList, 32, KDI_str, true);
    cout << "По убыванию:     ";
    printListNames(namesList);

    sf::RenderWindow window(sf::VideoMode({1200, 900}), "DigitalSort Analysis");

    window.setFramerateLimit(60);

    sf::Font font;
    bool fontLoaded = font.openFromFile("/System/Library/Fonts/Supplemental/Arial.ttf");
    if (!fontLoaded)
        fontLoaded = font.openFromFile("/Library/Fonts/Arial.ttf");

    while (window.isOpen()) {
        while (const auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        window.clear(sf::Color(15, 20, 30));
        float ox = 120.f, oy = 750.f;

        sf::Vertex axes[] = {{{ox, oy}, sf::Color::White},
                             {{1150.f, oy}, sf::Color::White},
                             {{ox, oy}, sf::Color::White},
                             {{ox, 50.f}, sf::Color::White}};
        window.draw(axes, 4, sf::PrimitiveType::Lines);

        for (int i = 0; i <= 1000; i += 200) {
            float x = ox + i;
            sf::Vertex tick[] = {{{x, oy}, sf::Color::White}, {{x, oy + 10.f}, sf::Color::White}};
            window.draw(tick, 2, sf::PrimitiveType::Lines);
            if (fontLoaded) {
                sf::Text text(font, std::to_string(i), 14);
                text.setPosition({x - 10.f, oy + 15.f});
                window.draw(text);
            }
        }

        for (int i = 0; i <= 10000; i += 2000) {
            float y = oy - (i / 15.f);
            sf::Vertex tick[] = {{{ox, y}, sf::Color::White}, {{ox - 10.f, y}, sf::Color::White}};
            window.draw(tick, 2, sf::PrimitiveType::Lines);
            if (fontLoaded && i > 0) {
                sf::Text text(font, std::to_string(i), 14);
                text.setPosition({ox - 50.f, y - 8.f});
                window.draw(text);
            }
        }

        if (fontLoaded) {
            sf::Text leg1(font, "Cyan: DigitalSort (Mf = L*N)", 18);
            leg1.setFillColor(sf::Color::Cyan);
            leg1.setPosition({900, 100});
            window.draw(leg1);

            sf::Text leg2(font, "Red: QuickSort (N log N)", 18);
            leg2.setFillColor(sf::Color::Red);
            leg2.setPosition({900, 130});
            window.draw(leg2);

            sf::Text labelX(font, "Size (N)", 16);
            labelX.setPosition({1100, oy + 20});
            window.draw(labelX);

            sf::Text labelY(font, "Operations (Mf)", 16);
            labelY.setRotation(sf::degrees(-90));
            labelY.setPosition({40, 150});
            window.draw(labelY);
        }

        sf::VertexArray dsL(sf::PrimitiveType::LineStrip, dsPoints.size());
        sf::VertexArray qsL(sf::PrimitiveType::LineStrip, qsPoints.size());

        for (size_t i = 0; i < dsPoints.size(); ++i) {
            dsL[i].position = {ox + dsPoints[i].first, oy - dsPoints[i].second / 15.f};
            dsL[i].color = sf::Color::Cyan;

            qsL[i].position = {ox + qsPoints[i].first, oy - qsPoints[i].second / 15.f};
            qsL[i].color = sf::Color::Red;
        }

        window.draw(dsL);
        window.draw(qsL);
        window.display();
    }

    return 0;
}
