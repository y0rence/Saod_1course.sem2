#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cstdio>

using namespace std;

void fill_inc(vector<int> &a) {
    for (int i = 0; i < (int)a.size(); i++)
        a[i] = i + 1;
}

void fill_dec(vector<int> &a) {
    int n = (int)a.size();
    for (int i = 0; i < n; i++)
        a[i] = n - i;
}

void fill_rand(vector<int> &a) {
    for (int i = 0; i < (int)a.size(); i++)
        a[i] = rand() % 100;
}

int sum_arr(const vector<int> &a) {
    int s = 0;
    for (int x : a)
        s += x;
    return s;
}

int runs_arr(const vector<int> &a) {
    if (a.empty())
        return 0;
    int r = 1;
    for (int i = 1; i < (int)a.size(); i++)
        if (a[i] < a[i - 1])
            r++;
    return r;
}

void bubble(vector<int> &a, int &c, int &m) {
    c = 0;
    m = 0;
    int n = (int)a.size();
    for (int i = 0; i < n - 1; i++) {
        for (int j = n - 1; j > i; j--) {
            c++;
            if (a[j] < a[j - 1]) {
                int t = a[j];
                a[j] = a[j - 1];
                a[j - 1] = t;
                m += 3;
            }
        }
    }
}

void select_sort(vector<int> &a, int &c, int &m) {
    c = 0;
    m = 0;
    int n = (int)a.size();
    for (int i = 0; i < n - 1; i++) {
        int k = i;
        for (int j = i + 1; j < n; j++) {
            c++;
            if (a[j] < a[k])
                k = j;
        }
        if (k != i) {
            int t = a[i];
            a[i] = a[k];
            a[k] = t;
            m += 3;
        }
    }
}

void shaker(vector<int> &a, int &c, int &m) {
    c = 0;
    m = 0;
    int l = 0;
    int r = (int)a.size() - 1;
    int k = r;

    do {
        for (int j = r; j > l; j--) {
            c++;
            if (a[j] < a[j - 1]) {
                int t = a[j];
                a[j] = a[j - 1];
                a[j - 1] = t;
                m += 3;
                k = j;
            }
        }
        l = k;

        for (int j = l; j < r; j++) {
            c++;
            if (a[j] > a[j + 1]) {
                int t = a[j];
                a[j] = a[j + 1];
                a[j + 1] = t;
                m += 3;
                k = j;
            }
        }
        r = k;
    } while (l < r);
}

void shaker_str(string &s) {
    int l = 0;
    int r = (int)s.size() - 1;
    int k = r;

    do {
        for (int j = r; j > l; j--) {
            if (s[j] < s[j - 1]) {
                char t = s[j];
                s[j] = s[j - 1];
                s[j - 1] = t;
                k = j;
            }
        }
        l = k;

        for (int j = l; j < r; j++) {
            if (s[j] > s[j + 1]) {
                char t = s[j];
                s[j] = s[j + 1];
                s[j + 1] = t;
                k = j;
            }
        }
        r = k;
    } while (l < r);
}

int max3(int a, int b, int c) {
    int x = a;
    if (b > x)
        x = b;
    if (c > x)
        x = c;
    return x;
}

void graph(const vector<int> &n, const vector<int> &v, const vector<int> &b, const vector<int> &s) {
    const int W = 900, H = 600, O = 60;

    int k = (int)n.size();
    if (k == 0)
        return;
    int mx = 1;
    for (int i = 0; i < k; i++)
        mx = max(mx, max3(v[i], b[i], s[i]));

    sf::RenderWindow win(sf::VideoMode(sf::Vector2u((unsigned)W, (unsigned)H)), "График (Mf+Cf)");
    win.setFramerateLimit(60);

    sf::Vertex axes[] = {
        sf::Vertex{{(float)O, (float)(H - O)}, sf::Color::Black},
        sf::Vertex{{(float)(W - O), (float)(H - O)}, sf::Color::Black},
        sf::Vertex{{(float)O, (float)(H - O)}, sf::Color::Black},
        sf::Vertex{{(float)O, (float)O}, sf::Color::Black},
    };

    int nmin = n.front();
    int nmax = n.back();
    float sx = (float)(W - 2 * O) / (float)(nmax - nmin);
    float sy = (float)(H - 2 * O) / (float)mx;

    vector<float> x(k), yv(k), yb(k), ys(k);
    for (int i = 0; i < k; i++) {
        x[i] = (float)O + (float)(n[i] - nmin) * sx;
        yv[i] = (float)(H - O) - (float)v[i] * sy;
        yb[i] = (float)(H - O) - (float)b[i] * sy;
        ys[i] = (float)(H - O) - (float)s[i] * sy;
    }

    sf::CircleShape dot(4.f);
    dot.setOrigin(sf::Vector2f(4.f, 4.f));

    sf::Font font;
    bool hasFont = false;
    if (font.openFromFile("arial.ttf")) {
        hasFont = true;
    } else if (font.openFromFile("/System/Library/Fonts/Supplemental/Arial.ttf")) {
        hasFont = true;
    }

    sf::Text labelRed(font, "", 16);
    sf::Text labelGreen(font, "", 16);
    sf::Text labelBlue(font, "", 16);
    if (hasFont) {
        labelRed.setFont(font);
        labelGreen.setFont(font);
        labelBlue.setFont(font);
        labelRed.setCharacterSize(16);
        labelGreen.setCharacterSize(16);
        labelBlue.setCharacterSize(16);
        labelRed.setFillColor(sf::Color::Red);
        labelGreen.setFillColor(sf::Color::Green);
        labelBlue.setFillColor(sf::Color::Blue);
        labelRed.setString("Red - SelectSort");
        labelGreen.setString("Green - BubbleSort");
        labelBlue.setString("Blue - ShakerSort");
    }

    sf::RectangleShape legendLine(sf::Vector2f(30.f, 4.f));
    int thickness = 4;

    while (win.isOpen()) {
        while (const auto event = win.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                win.close();
        }

        win.clear(sf::Color::White);
        win.draw(axes, 4, sf::PrimitiveType::Lines);

        for (int offset = -thickness / 2; offset <= thickness / 2; offset++) {
            sf::VertexArray lv(sf::PrimitiveType::LineStrip);
            sf::VertexArray lb(sf::PrimitiveType::LineStrip);
            sf::VertexArray ls(sf::PrimitiveType::LineStrip);
            for (int i = 0; i < k; i++) {
                lv.append(sf::Vertex{{x[i], yv[i] + offset}, sf::Color::Red});
                lb.append(sf::Vertex{{x[i], yb[i] + offset}, sf::Color::Green});
                ls.append(sf::Vertex{{x[i], ys[i] + offset}, sf::Color::Blue});
            }
            win.draw(lv);
            win.draw(lb);
            win.draw(ls);
        }

        for (int i = 0; i < k; i++) {
            dot.setFillColor(sf::Color::Red);
            dot.setPosition(sf::Vector2f(x[i], yv[i]));
            win.draw(dot);

            dot.setFillColor(sf::Color::Green);
            dot.setPosition(sf::Vector2f(x[i], yb[i]));
            win.draw(dot);

            dot.setFillColor(sf::Color::Blue);
            dot.setPosition(sf::Vector2f(x[i], ys[i]));
            win.draw(dot);
        }

        if (hasFont) {
            float legendY = (float)H - (float)O + 18.f;
            float legendX1 = (float)O + 20.f;
            float legendX2 = (float)O + 280.f;
            float legendX3 = (float)O + 560.f;

            legendLine.setFillColor(sf::Color::Red);
            legendLine.setPosition(sf::Vector2f(legendX1, legendY));
            win.draw(legendLine);
            labelRed.setPosition(sf::Vector2f(legendX1 + 40.f, legendY - 8.f));
            win.draw(labelRed);

            legendLine.setFillColor(sf::Color::Green);
            legendLine.setPosition(sf::Vector2f(legendX2, legendY));
            win.draw(legendLine);
            labelGreen.setPosition(sf::Vector2f(legendX2 + 40.f, legendY - 8.f));
            win.draw(labelGreen);

            legendLine.setFillColor(sf::Color::Blue);
            legendLine.setPosition(sf::Vector2f(legendX3, legendY));
            win.draw(legendLine);
            labelBlue.setPosition(sf::Vector2f(legendX3 + 40.f, legendY - 8.f));
            win.draw(labelBlue);
        }

        win.display();
    }
}

int main() {
    srand((unsigned)time(nullptr));

    cout << "Задание 1. Введи 8 символов: ";
    string s;
    cin >> s;
    if ((int)s.size() > 8)
        s = s.substr(0, 8);

    cout << "Было:  ";
    for (char ch : s)
        cout << ch << ' ';
    shaker_str(s);
    cout << "\nСтало: ";
    for (char ch : s)
        cout << ch << ' ';
    cout << "\n\n";

    cout << "Задание 2. Проверка массива (10 чисел):\n";
    vector<int> a(10);
    fill_rand(a);

    cout << "Было:  ";
    for (int x : a)
        cout << x << ' ';
    int s1 = sum_arr(a);
    int r1 = runs_arr(a);

    int c = 0, m = 0;
    shaker(a, c, m);

    cout << "\nСтало: ";
    for (int x : a)
        cout << x << ' ';
    int s2 = sum_arr(a);
    int r2 = runs_arr(a);

    cout << "\nСумма до/после: " << s1 << " / " << s2;
    cout << "\nСерии до/после: " << r1 << " / " << r2;
    cout << "\nMf=" << m << " Cf=" << c << " Mf+Cf=" << (m + c) << "\n\n";

    cout << "Трудоемкость пузырьковой и шейкерной сортировок\n";
    cout << "+-----+---------------------------+---------------------------+\n";
    cout << "|  n  |    Mf+Cf пузырьковой      |     Mf+Cf шейкерной        |\n";
    cout << "|     |  Убыв.   Случ.   Возр.     |  Убыв.   Случ.   Возр.     |\n";
    cout << "+-----+---------------------------+---------------------------+\n";

    vector<int> nlist = {100, 200, 300, 400, 500};
    vector<int> gv(5), gb(5), gs(5);
    int idx = 0;

    for (int n : nlist) {
        vector<int> incv(n), decv(n), rndv(n), w;

        fill_inc(incv);
        fill_dec(decv);
        fill_rand(rndv);

        w = decv;
        bubble(w, c, m);
        int bdec = c + m;
        w = rndv;
        bubble(w, c, m);
        int brnd = c + m;
        w = incv;
        bubble(w, c, m);
        int binc = c + m;

        w = decv;
        shaker(w, c, m);
        int sdec = c + m;
        w = rndv;
        shaker(w, c, m);
        int srnd = c + m;
        w = incv;
        shaker(w, c, m);
        int sinc = c + m;

        printf("| %3d | %7d %7d %7d | %7d %7d %7d |\n", n, bdec, brnd, binc, sdec, srnd, sinc);

        w = rndv;
        select_sort(w, c, m);
        int v = c + m;
        gv[idx] = v;
        gb[idx] = brnd;
        gs[idx] = srnd;
        idx++;
    }
    cout << "+-----+---------------------------+---------------------------+\n";

    cout << "\nГрафик: красный — выбор, зелёный — пузырёк, синий — шейкер.\n";
    graph(nlist, gv, gb, gs);

    return 0;
}
