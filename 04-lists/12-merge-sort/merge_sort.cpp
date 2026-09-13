#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <cstdio>
#include <string>
#include <optional>
#include <SFML/Graphics.hpp>

using namespace std;

struct Node {
    int data;
    Node* next;
};

struct Queue {
    Node* head;
    Node* tail;
};

long long Cf = 0;
long long Mf = 0;

Node* createNode(int val) {
    Node* p = new Node;
    p->data = val;
    p->next = NULL;
    return p;
}

void initQueue(Queue* q) {
    q->head = NULL;
    q->tail = NULL;
}

void enqueue(Queue* q, Node* node) {
    node->next = NULL;
    if (q->tail == NULL) {
        q->head = node;
        q->tail = node;
    } else {
        q->tail->next = node;
        q->tail = node;
    }
    Mf++;
}

Node* takeFirst(Node** list) {
    Node* tmp = *list;
    *list = (*list)->next;
    tmp->next = NULL;
    return tmp;
}

Node* arrayToList(int* arr, int n) {
    Node* head = NULL;
    Node** cur = &head;
    for (int i = 0; i < n; i++) {
        *cur = createNode(arr[i]);
        cur = &((*cur)->next);
    }
    return head;
}

int countList(Node* head) {
    int n = 0;
    while (head != NULL) { n++; head = head->next; }
    return n;
}

void printList(Node* head, const char* name) {
    cout << name << ": ";
    while (head != NULL) {
        cout << head->data;
        if (head->next != NULL) cout << " -> ";
        head = head->next;
    }
    cout << endl;
}

void freeList(Node* head) {
    while (head != NULL) {
        Node* tmp = head;
        head = head->next;
        delete tmp;
    }
}

void copyArr(int* src, int* dst, int n) {
    for (int i = 0; i < n; i++) dst[i] = src[i];
}

int splitList(Node* S, Node** a, Node** b) {
    if (S == NULL) { *a = NULL; *b = NULL; return 0; }

    *a = S;
    if (S->next == NULL) { *b = NULL; return 1; }

    *b = S->next;
    Node* k = *a;
    Node* p = *b;
    int n = 1;

    while (p != NULL) {
        n++;
        k->next = p->next;
        k = p;
        p = p->next;
    }
    k->next = NULL;

    return n;
}

void mergeSeries(Node** a, int q, Node** b, int r, Queue* c) {
    while (q != 0 && r != 0) {
        Cf++;
        if ((*a)->data <= (*b)->data) {
            enqueue(c, takeFirst(a));
            q--;
        } else {
            enqueue(c, takeFirst(b));
            r--;
        }
    }
    while (q > 0) { enqueue(c, takeFirst(a)); q--; }
    while (r > 0) { enqueue(c, takeFirst(b)); r--; }
}

Node* mergeSort(Node* S) {
    Node* a = NULL;
    Node* b = NULL;
    int n = splitList(S, &a, &b);

    if (n <= 1) return S;

    int p = 1;

    while (p < n) {
        Queue c[2];
        initQueue(&c[0]);
        initQueue(&c[1]);

        int i = 0;
        int m = n;

        while (m > 0) {
            int q = (m >= p) ? p : m;
            m = m - q;
            int r = (m >= p) ? p : m;
            m = m - r;
            mergeSeries(&a, q, &b, r, &c[i]);
            i = 1 - i;
        }

        if (c[0].tail != NULL) c[0].tail->next = NULL;
        if (c[1].tail != NULL) c[1].tail->next = NULL;

        a = c[0].head;
        b = c[1].head;
        p = 2 * p;
    }

    return a;
}

void swapEl(int* arr, int i, int j) {
    int tmp = arr[i];
    arr[i]  = arr[j];
    arr[j]  = tmp;
    Mf += 3;
}

void buildHeap(int* arr, int L, int R) {
    int x = arr[L];
    int i = L;
    Mf++;

    while (1) {
        int j = 2 * i + 1;
        if (j > R) break;
        if (j < R) {
            Cf++;
            if (arr[j + 1] <= arr[j]) j++;
        }
        Cf++;
        if (x <= arr[j]) break;
        arr[i] = arr[j];
        Mf++;
        i = j;
    }
    arr[i] = x;
    Mf++;
}

void heapSort(int* arr, int n) {
    for (int L = n / 2 - 1; L >= 0; L--) {
        buildHeap(arr, L, n - 1);
    }
    for (int R = n - 1; R > 0; R--) {
        swapEl(arr, 0, R);
        buildHeap(arr, 0, R - 1);
    }
}

void quickSort(int* arr, int L, int R) {
    int x = arr[L];
    int i = L;
    int j = R;

    while (i <= j) {
        while (Cf++, arr[i] < x) i++;
        while (Cf++, arr[j] > x) j--;
        if (i <= j) {
            swapEl(arr, i, j);
            i++;
            j--;
        }
    }

    if (L < j) quickSort(arr, L, j);
    if (i < R) quickSort(arr, i, R);
}

long long theorMC(int n) {
    double K = ceil(log2((double)n));
    return (long long)(2.0 * n * K);
}

void genDescending(int* arr, int n) {
    for (int i = 0; i < n; i++) arr[i] = n - i;
}

void genAscending(int* arr, int n) {
    for (int i = 0; i < n; i++) arr[i] = i + 1;
}

void genRandom(int* arr, int n) {
    for (int i = 0; i < n; i++) arr[i] = rand() % (n * 10) + 1;
}

bool loadGraphFont(sf::Font& font) {
    const char* candidates[] = {
        "/System/Library/Fonts/Supplemental/Arial.ttf",
        "/System/Library/Fonts/Supplemental/Arial Unicode.ttf",
        "/Library/Fonts/Arial.ttf",
        "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf"
    };

    for (const char* path : candidates) {
        if (font.openFromFile(path)) return true;
    }
    return false;
}

sf::Vertex makeVertex(float x, float y, sf::Color color) {
    sf::Vertex v;
    v.position = {x, y};
    v.color = color;
    return v;
}

void drawGraph(long long* merge, long long* heap, long long* quick,
               int* sizes, int cnt) {
    if (cnt <= 0) return;

    long long maxVal = 1;
    for (int i = 0; i < cnt; i++) {
        if (merge[i] > maxVal) maxVal = merge[i];
        if (heap[i]  > maxVal) maxVal = heap[i];
        if (quick[i] > maxVal) maxVal = quick[i];
    }

    const unsigned int windowW = 1200;
    const unsigned int windowH = 760;
    const float left   = 90.0f;
    const float right  = 70.0f;
    const float top    = 70.0f;
    const float bottom = 120.0f;
    const float plotW  = static_cast<float>(windowW) - left - right;
    const float plotH  = static_cast<float>(windowH) - top - bottom;

    sf::RenderWindow window(
        sf::VideoMode({windowW, windowH}),
        "Task 6: MergeSort vs HeapSort vs QuickSort"
    );
    window.setFramerateLimit(60);

    auto mapX = [&](int i) -> float {
        if (cnt == 1) return left + plotW * 0.5f;
        return left + (plotW * static_cast<float>(i)) / static_cast<float>(cnt - 1);
    };

    auto mapY = [&](long long value) -> float {
        return top + plotH - (plotH * static_cast<float>(value)) / static_cast<float>(maxVal);
    };

    auto buildSeries = [&](long long* data, sf::Color color) -> sf::VertexArray {
        sf::VertexArray line(sf::PrimitiveType::LineStrip, static_cast<std::size_t>(cnt));
        for (int i = 0; i < cnt; i++) {
            line[static_cast<std::size_t>(i)].position = {mapX(i), mapY(data[i])};
            line[static_cast<std::size_t>(i)].color = color;
        }
        return line;
    };

    sf::VertexArray mergeLine = buildSeries(merge, sf::Color(45, 156, 219));
    sf::VertexArray heapLine  = buildSeries(heap,  sf::Color(231, 76, 60));
    sf::VertexArray quickLine = buildSeries(quick, sf::Color(46, 204, 113));

    sf::Font font;
    const bool hasFont = loadGraphFont(font);

    auto drawText = [&](const std::string& str, float x, float y,
                        unsigned int size, sf::Color color) {
        if (!hasFont) return;
        sf::Text text(font, str, size);
        text.setFillColor(color);
        text.setPosition({x, y});
        window.draw(text);
    };

    auto drawSeriesPoints = [&](long long* data, sf::Color color) {
        for (int i = 0; i < cnt; i++) {
            sf::CircleShape dot(4.0f);
            dot.setOrigin({4.0f, 4.0f});
            dot.setPosition({mapX(i), mapY(data[i])});
            dot.setFillColor(color);
            window.draw(dot);
        }
    };

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) window.close();
            if (const auto* key = event->getIf<sf::Event::KeyPressed>()) {
                if (key->code == sf::Keyboard::Key::Escape) window.close();
            }
        }

        window.clear(sf::Color(248, 249, 251));

        sf::Vertex axes[] = {
            makeVertex(left, top + plotH, sf::Color::Black),
            makeVertex(left + plotW, top + plotH, sf::Color::Black),
            makeVertex(left, top + plotH, sf::Color::Black),
            makeVertex(left, top, sf::Color::Black)
        };
        window.draw(axes, 4, sf::PrimitiveType::Lines);

        const int yTicks = 5;
        for (int t = 0; t <= yTicks; t++) {
            float ratio = static_cast<float>(t) / static_cast<float>(yTicks);
            float y = top + plotH - ratio * plotH;
            long long value = static_cast<long long>(ratio * maxVal);

            sf::Vertex gridLine[] = {
                makeVertex(left, y, sf::Color(210, 214, 220)),
                makeVertex(left + plotW, y, sf::Color(210, 214, 220))
            };
            window.draw(gridLine, 2, sf::PrimitiveType::Lines);

            drawText(std::to_string(value), 14.0f, y - 10.0f, 16, sf::Color(50, 50, 50));
        }

        for (int i = 0; i < cnt; i++) {
            float x = mapX(i);
            sf::Vertex tick[] = {
                makeVertex(x, top + plotH, sf::Color::Black),
                makeVertex(x, top + plotH + 8.0f, sf::Color::Black)
            };
            window.draw(tick, 2, sf::PrimitiveType::Lines);

            drawText(std::to_string(sizes[i]), x - 14.0f, top + plotH + 14.0f, 16, sf::Color(30, 30, 30));
        }

        window.draw(mergeLine);
        window.draw(heapLine);
        window.draw(quickLine);
        drawSeriesPoints(merge, sf::Color(45, 156, 219));
        drawSeriesPoints(heap,  sf::Color(231, 76, 60));
        drawSeriesPoints(quick, sf::Color(46, 204, 113));

        drawText("Task 6: Laboriousness (M + C)", left, 18.0f, 26, sf::Color(20, 20, 20));
        drawText("n", left + plotW + 18.0f, top + plotH + 6.0f, 20, sf::Color::Black);
        drawText("M + C", 12.0f, top - 30.0f, 20, sf::Color::Black);

        float legendY = top - 8.0f;
        sf::Vertex legendM[] = {
            makeVertex(left + 360.0f, legendY, sf::Color(45, 156, 219)),
            makeVertex(left + 410.0f, legendY, sf::Color(45, 156, 219))
        };
        sf::Vertex legendH[] = {
            makeVertex(left + 560.0f, legendY, sf::Color(231, 76, 60)),
            makeVertex(left + 610.0f, legendY, sf::Color(231, 76, 60))
        };
        sf::Vertex legendQ[] = {
            makeVertex(left + 740.0f, legendY, sf::Color(46, 204, 113)),
            makeVertex(left + 790.0f, legendY, sf::Color(46, 204, 113))
        };
        window.draw(legendM, 2, sf::PrimitiveType::Lines);
        window.draw(legendH, 2, sf::PrimitiveType::Lines);
        window.draw(legendQ, 2, sf::PrimitiveType::Lines);

        drawText("MergeSort", left + 418.0f, legendY - 12.0f, 16, sf::Color(45, 156, 219));
        drawText("HeapSort",  left + 618.0f, legendY - 12.0f, 16, sf::Color(231, 76, 60));
        drawText("QuickSort", left + 798.0f, legendY - 12.0f, 16, sf::Color(46, 204, 113));

        if (!hasFont) {
            sf::RectangleShape warnBg({540.0f, 34.0f});
            warnBg.setPosition({left, static_cast<float>(windowH) - 44.0f});
            warnBg.setFillColor(sf::Color(255, 243, 205));
            warnBg.setOutlineThickness(1.0f);
            warnBg.setOutlineColor(sf::Color(255, 193, 7));
            window.draw(warnBg);
        }

        window.display();
    }
}

int main() {
    srand((unsigned int)time(NULL));

    cout << "============================================" << endl;
    cout << "  Сортировка прямого слияния (MergeSort)" << endl;
    cout << "============================================" << endl << endl;

    cout << "=== Задание 1: Ручная трассировка (12 символов) ===" << endl;
    cout << "Символы: К У Р О П А В О В А Е Л" << endl;
    int chars12[] = {12, 21, 18, 15, 16, 1, 3, 15, 3, 1, 6, 13};

    cout << "Исходная: ";
    for (int i = 0; i < 12; i++) cout << chars12[i] << " ";
    cout << endl;

    Cf = 0; Mf = 0;
    Node* list1 = arrayToList(chars12, 12);
    Node* sorted1 = mergeSort(list1);

    cout << "Отсортированная: ";
    Node* t = sorted1;
    while (t != NULL) { cout << t->data << " "; t = t->next; }
    cout << endl;

    double K12 = ceil(log2(12.0));
    cout << "Сф=" << Cf << ", Мф=" << Mf
         << " | Теор. М=n*K=" << (int)(12*K12)
         << ", C: от " << (int)(12*K12/2) << " до " << (int)(12*K12) << endl;
    freeList(sorted1);
    cout << endl;

    cout << "=== Задание 2: Расщепление списка (n=20) ===" << endl;
    int arr20[20];
    genRandom(arr20, 20);

    cout << "S: ";
    for (int i = 0; i < 20; i++) cout << arr20[i] << " ";
    cout << endl;

    Node* listS = arrayToList(arr20, 20);
    Node* listA = NULL, *listB = NULL;
    int nTotal = splitList(listS, &listA, &listB);

    printList(listA, "a");
    cout << "  Кол-во в a: " << countList(listA) << endl;
    printList(listB, "b");
    cout << "  Кол-во в b: " << countList(listB) << ",  итого n=" << nTotal << endl;

    freeList(listA);
    freeList(listB);
    cout << endl;

    cout << "=== Задание 3: Слияние серий ===" << endl;
    int exA[] = {1, 4, 5, 6};
    int exB[] = {2, 3, 6, 7, 8};
    int q3 = 4, r3 = 5;

    Node* mergeA = arrayToList(exA, q3);
    Node* mergeB = arrayToList(exB, r3);
    Queue queueC; initQueue(&queueC);

    Cf = 0; Mf = 0;
    cout << "a (q=4): 1 4 5 6" << endl;
    cout << "b (r=5): 2 3 6 7 8" << endl;
    mergeSeries(&mergeA, q3, &mergeB, r3, &queueC);
    cout << "c: "; t = queueC.head; while(t){cout<<t->data<<" ";t=t->next;} cout<<endl;
    cout << "Сф=" << Cf << " (теор: 4<=C<=8),  Мф=" << Mf << " (теор: M=9)" << endl;

    freeList(queueC.head);
    cout << endl;

    cout << "=== Задание 4: Полная сортировка MergeSort (n=20) ===" << endl;
    int arr4[20];
    genRandom(arr4, 20);

    cout << "Исходный: ";
    for (int i = 0; i < 20; i++) cout << arr4[i] << " ";
    cout << endl;

    Cf = 0; Mf = 0;
    Node* list4 = arrayToList(arr4, 20);
    Node* sorted4 = mergeSort(list4);

    cout << "Отсортированный: ";
    t = sorted4; while(t){cout<<t->data<<" ";t=t->next;} cout<<endl;

    double K4 = ceil(log2(20.0));
    cout << "Сф=" << Cf << ", Мф=" << Mf << ", Сф+Мф=" << (Cf+Mf) << endl;
    cout << "Теор. М=n*K=20*" << (int)K4 << "=" << (int)(20*K4)
         << ", C: от " << (int)(20*K4/2) << " до " << (int)(20*K4) << endl;

    freeList(sorted4);
    cout << endl;

    cout << "=== Задание 5: Таблица трудоёмкости MergeSort ===" << endl;
    cout << " N   | M+C теор. | M+C убыв. | M+C случ. | M+C возр." << endl;
    cout << "-----|-----------|-----------|-----------|----------" << endl;

    int sizes[]   = {100, 200, 300, 400, 500};
    int numSizes  = 5;

    for (int s = 0; s < numSizes; s++) {
        int n = sizes[s];
        int* arr = new int[n];
        long long mcTheor = theorMC(n);
        long long mcDesc, mcRand, mcAsc;

        genDescending(arr, n);
        Cf = 0; Mf = 0;
        { Node* lst = arrayToList(arr, n); freeList(mergeSort(lst)); }
        mcDesc = Cf + Mf;

        genRandom(arr, n);
        Cf = 0; Mf = 0;
        { Node* lst = arrayToList(arr, n); freeList(mergeSort(lst)); }
        mcRand = Cf + Mf;

        genAscending(arr, n);
        Cf = 0; Mf = 0;
        { Node* lst = arrayToList(arr, n); freeList(mergeSort(lst)); }
        mcAsc = Cf + Mf;

        cout << " " << n << " | " << mcTheor
             << "      | " << mcDesc
             << "      | " << mcRand
             << "      | " << mcAsc << endl;

        delete[] arr;
    }

    cout << endl;
    cout << "Вывод: MergeSort НЕ зависит от упорядоченности." << endl;
    cout << "M = n*K всегда одинаково, C немного меняется." << endl;
    cout << endl;

    cout << "=== Задание 6: Сравнение MergeSort, HeapSort, QuickSort ===" << endl;
    cout << "(случайные массивы)" << endl;

    long long mergeVals[5], heapVals[5], quickVals[5];

    cout << endl;
    cout << " N   | MergeSort | HeapSort  | QuickSort" << endl;
    cout << "-----|-----------|-----------|----------" << endl;

    for (int s = 0; s < numSizes; s++) {
        int n = sizes[s];
        int* arr     = new int[n];
        int* arrCopy = new int[n];

        genRandom(arr, n);
        Cf = 0; Mf = 0;
        { Node* lst = arrayToList(arr, n); freeList(mergeSort(lst)); }
        mergeVals[s] = Cf + Mf;

        genRandom(arr, n);
        copyArr(arr, arrCopy, n);
        Cf = 0; Mf = 0;
        heapSort(arrCopy, n);
        heapVals[s] = Cf + Mf;

        genRandom(arr, n);
        copyArr(arr, arrCopy, n);
        Cf = 0; Mf = 0;
        quickSort(arrCopy, 0, n - 1);
        quickVals[s] = Cf + Mf;

        cout << " " << n << " | "
             << mergeVals[s] << "\t    | "
             << heapVals[s]  << "\t    | "
             << quickVals[s] << endl;

        delete[] arr;
        delete[] arrCopy;
    }

    drawGraph(mergeVals, heapVals, quickVals, sizes, numSizes);

    printf("Вывод по заданию 6:\n");
    printf("1) MergeSort, HeapSort, QuickSort: O(n*log2(n)).\n");
    printf("2) MergeSort: M = n*K (строго), C меняется в пределах теории.\n");
    printf("3) HeapSort: зависимость от начальной упорядоченности слабая.\n");
    printf("4) QuickSort: чувствителен к данным; на случайных часто самый быстрый.\n");

    return 0;
}
