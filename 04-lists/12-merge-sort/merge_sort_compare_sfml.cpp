#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <string>
#include <algorithm>
#include <SFML/Graphics.hpp>

using namespace std;

long long compCount = 0;
long long moveCount = 0;

struct List {
    int data;
    List* next;
};

struct Queue {
    List* head;
    List* tail;
};

void resetCounters() {
    compCount = 0;
    moveCount = 0;
}

void initQueue(Queue* q) {
    q->head = NULL;
    q->tail = NULL;
}

void moveToQueue(List*& list, Queue* q) {
    List* p = list;
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

List* push(List* head, int value) {
    List* p = new List;
    p->data = value;
    p->next = head;
    return p;
}

List* createAscendingList(int n) {
    List* head = NULL;

    for (int i = n; i >= 1; i--) {
        head = push(head, i);
    }

    return head;
}

List* createDescendingList(int n) {
    List* head = NULL;

    for (int i = 1; i <= n; i++) {
        head = push(head, i);
    }

    return head;
}

List* createRandomList(int n) {
    List* head = NULL;

    for (int i = 0; i < n; i++) {
        head = push(head, rand() % 100 + 10);
    }

    return head;
}

void printList(List* head) {
    cout << "\nСписок: ";

    while (head != NULL) {
        cout << head->data << " ";
        head = head->next;
    }

    cout << "\n";
}

int sumList(List* head) {
    int sum = 0;

    while (head != NULL) {
        sum += head->data;
        head = head->next;
    }

    return sum;
}

int countList(List* head) {
    int count = 0;

    while (head != NULL) {
        count++;
        head = head->next;
    }

    return count;
}

void deleteList(List** head) {
    while (*head != NULL) {
        List* temp = *head;
        *head = (*head)->next;
        delete temp;
    }
}

void calculateTheoretical(int n, long long& theoryC, long long& theoryM) {
    int log2n = 0;

    while ((1 << log2n) < n) {
        log2n++;
    }

    theoryC = (long long)n * log2n;
    theoryM = (long long)n * log2n + n;
}

void splitList(List* S, List*& a, List*& b) {
    if (S == NULL) {
        a = NULL;
        b = NULL;
        return;
    }

    a = S;
    b = S->next;

    List* k = a;
    List* p = b;

    while (p != NULL) {
        k->next = p->next;
        k = p;
        p = p->next;
    }
}

void mergeSeries(List*& a, int q, List*& b, int r, Queue* c) {
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

void mergeSort(List*& S, int n) {
    if (S == NULL || S->next == NULL) {
        return;
    }

    List* a;
    List* b;
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
        List* dec = createDescendingList(n);
        List* rnd = createRandomList(n);
        List* asc = createAscendingList(n);

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

        printf("%-5d|%-15lld|%-10lld|%-10lld|%-10lld|\n",
               n, theoryC + theoryM, decFact, rndFact, ascFact);

        deleteList(&dec);
        deleteList(&rnd);
        deleteList(&asc);
    }
}

int* createRandomArray(int n) {
    int* a = new int[n];

    for (int i = 0; i < n; i++) {
        a[i] = rand() % 100 + 10;
    }

    return a;
}

void swapWithCount(int& a, int& b) {
    int temp = a;
    a = b;
    b = temp;
    moveCount += 3;
}

void buildHeap(int a[], int L, int R) {
    int x = a[L];
    int i = L;
    moveCount++;

    while (true) {
        int j = 2 * i + 1;
        if (j > R) break;

        if (j < R) {
            compCount++;
            if (a[j + 1] <= a[j]) j++;
        }

        compCount++;
        if (x <= a[j]) break;

        a[i] = a[j];
        moveCount++;
        i = j;
    }

    a[i] = x;
    moveCount++;
}

void heapSort(int a[], int n) {
    for (int L = n / 2 - 1; L >= 0; L--) {
        buildHeap(a, L, n - 1);
    }

    for (int R = n - 1; R > 0; R--) {
        swapWithCount(a[0], a[R]);
        buildHeap(a, 0, R - 1);
    }
}

void quickSort(int a[], int L, int R) {
    int x = a[L];
    int i = L;
    int j = R;

    while (i <= j) {
        while (compCount++, a[i] < x) i++;
        while (compCount++, a[j] > x) j--;

        if (i <= j) {
            swapWithCount(a[i], a[j]);
            i++;
            j--;
        }
    }

    if (L < j) quickSort(a, L, j);
    if (i < R) quickSort(a, i, R);
}

long long getHeapSortCost(int n) {
    int* a = createRandomArray(n);

    resetCounters();
    heapSort(a, n);

    long long cost = compCount + moveCount;
    delete[] a;

    return cost;
}

long long getQuickSortCost(int n) {
    int* a = createRandomArray(n);

    resetCounters();
    quickSort(a, 0, n - 1);

    long long cost = compCount + moveCount;
    delete[] a;

    return cost;
}

long long getMergeSortCost(int n) {
    List* list = createRandomList(n);

    resetCounters();
    mergeSort(list, n);

    long long cost = compCount + moveCount;
    deleteList(&list);

    return cost;
}

bool loadFont(sf::Font& font) {
    const string paths[] = {
        "/System/Library/Fonts/Supplemental/Arial.ttf",
        "/System/Library/Fonts/Supplemental/Times New Roman.ttf",
        "/System/Library/Fonts/Helvetica.ttc",
        "arial.ttf"
    };

    for (const string& path : paths) {
        if (font.openFromFile(path)) return true;
    }
    return false;
}

sf::Vector2f getPoint(int index, long long value, int count, long long maxValue,
                      float left, float top, float width, float height) {
    float x = left + index * (width / (count - 1));
    float y = top + height - ((float)value / maxValue) * height;
    return sf::Vector2f(x, y);
}

void drawPolyline(sf::RenderTarget& target, const vector<long long>& values,
                  long long maxValue, sf::Color color,
                  float left, float top, float width, float height) {
    sf::VertexArray line(sf::PrimitiveType::LineStrip, values.size());

    for (int i = 0; i < (int)values.size(); i++) {
        line[i].position = getPoint(i, values[i], (int)values.size(), maxValue, left, top, width, height);
        line[i].color = color;
    }

    target.draw(line);

    for (int i = 0; i < (int)values.size(); i++) {
        sf::CircleShape point(5);
        point.setFillColor(color);
        point.setOrigin({5, 5});
        point.setPosition(getPoint(i, values[i], (int)values.size(), maxValue, left, top, width, height));
        target.draw(point);
    }
}

void drawText(sf::RenderTarget& target, sf::Font& font, const string& text,
              float x, float y, int size, sf::Color color) {
    sf::Text t(font, text, size);
    t.setFillColor(color);
    t.setPosition({x, y});
    target.draw(t);
}

void buildTask6GraphSFML() {
    vector<int> nValues;
    vector<long long> heapValues;
    vector<long long> quickValues;
    vector<long long> mergeValues;

    cout << "\n\nЗадание 6*. Данные для графика SFML\n\n";
    printf("%-5s|%-12s|%-12s|%-12s|\n", "N", "HeapSort", "QuickSort", "MergeSort");
    printf("--------------------------------------------\n");

    for (int n = 100; n <= 500; n += 100) {
        long long heapCost = getHeapSortCost(n);
        long long quickCost = getQuickSortCost(n);
        long long mergeCost = getMergeSortCost(n);

        nValues.push_back(n);
        heapValues.push_back(heapCost);
        quickValues.push_back(quickCost);
        mergeValues.push_back(mergeCost);

        printf("%-5d|%-12lld|%-12lld|%-12lld|\n", n, heapCost, quickCost, mergeCost);
    }

    long long maxValue = 1;
    for (long long value : heapValues) maxValue = max(maxValue, value);
    for (long long value : quickValues) maxValue = max(maxValue, value);
    for (long long value : mergeValues) maxValue = max(maxValue, value);

    const unsigned int windowWidth = 1000;
    const unsigned int windowHeight = 700;
    const float left = 90;
    const float top = 80;
    const float graphWidth = 820;
    const float graphHeight = 480;

    sf::RenderTexture texture({windowWidth, windowHeight});
    texture.clear(sf::Color::White);

    sf::Font font;
    bool hasFont = loadFont(font);

    sf::RectangleShape axisX(sf::Vector2f(graphWidth, 2));
    axisX.setFillColor(sf::Color::Black);
    axisX.setPosition({left, top + graphHeight});
    texture.draw(axisX);

    sf::RectangleShape axisY(sf::Vector2f(2, graphHeight));
    axisY.setFillColor(sf::Color::Black);
    axisY.setPosition({left, top});
    texture.draw(axisY);

    for (int i = 0; i <= 5; i++) {
        float y = top + graphHeight - i * (graphHeight / 5);
        sf::RectangleShape grid(sf::Vector2f(graphWidth, 1));
        grid.setFillColor(sf::Color(220, 220, 220));
        grid.setPosition({left, y});
        texture.draw(grid);

        if (hasFont) {
            long long labelValue = maxValue * i / 5;
            drawText(texture, font, to_string(labelValue), 15, y - 10, 14, sf::Color::Black);
        }
    }

    for (int i = 0; i < (int)nValues.size(); i++) {
        float x = left + i * (graphWidth / (nValues.size() - 1));

        sf::RectangleShape tick(sf::Vector2f(2, 8));
        tick.setFillColor(sf::Color::Black);
        tick.setPosition({x, top + graphHeight});
        texture.draw(tick);

        if (hasFont) {
            drawText(texture, font, to_string(nValues[i]), x - 15, top + graphHeight + 15, 16, sf::Color::Black);
        }
    }

    sf::Color heapColor(220, 60, 60);
    sf::Color quickColor(60, 130, 220);
    sf::Color mergeColor(40, 160, 80);

    drawPolyline(texture, heapValues, maxValue, heapColor, left, top, graphWidth, graphHeight);
    drawPolyline(texture, quickValues, maxValue, quickColor, left, top, graphWidth, graphHeight);
    drawPolyline(texture, mergeValues, maxValue, mergeColor, left, top, graphWidth, graphHeight);

    if (hasFont) {
        drawText(texture, font, "Task 6*: Mfact + Cfact from N", 300, 20, 24, sf::Color::Black);
        drawText(texture, font, "N", left + graphWidth + 20, top + graphHeight - 5, 18, sf::Color::Black);
        drawText(texture, font, "M+C", 25, top - 35, 18, sf::Color::Black);
        drawText(texture, font, "HeapSort", 760, 90, 16, heapColor);
        drawText(texture, font, "QuickSort", 760, 115, 16, quickColor);
        drawText(texture, font, "MergeSort", 760, 140, 16, mergeColor);
    }

    texture.display();

    sf::Image image = texture.getTexture().copyToImage();
    if (image.saveToFile("task6_graph.png")) {
        cout << "\nГрафик сохранён в файл: task6_graph.png\n";
    } else {
        cout << "\nОшибка: не удалось сохранить task6_graph.png\n";
    }

    sf::RenderWindow window(sf::VideoMode({windowWidth, windowHeight}), "Task 6*: Sorting complexity graph");
    sf::Sprite sprite(texture.getTexture());

    while (window.isOpen()) {
        while (const auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) window.close();
        }

        window.clear(sf::Color::White);
        window.draw(sprite);
        window.display();
    }
}

int main() {
    srand(time(NULL));

    cout << "==================== Расщепление списка ====================\n";

    List* list = createRandomList(20);
    printList(list);

    List* a;
    List* b;
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

    buildTask6GraphSFML();

    return 0;
}
