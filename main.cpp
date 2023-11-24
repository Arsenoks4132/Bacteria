#include <iostream>
#include <fstream>
#include <windows.h>
#include <random>
#include <vector>

using namespace std;

int ichek(string x = "") {
    bool b, t = 0;
    if (x.length() == 0) {
        t = 1;
    }
    while (1) {
        if (t) {
            cin >> x;
        }
        b = 1;
        if ((x[0] == '-') || (x[0] >= '0' && x[0] <= '9')) {
            for (int i = 1; i < x.length(); ++i) {
                if (not((x[i] >= '0' && x[i] <= '9'))) {
                    b = 0;
                    break;
                }
            }
            try {
                if (b) {
                    return stoi(x);
                }
            }
            catch (out_of_range const &e) {
                cout << endl << "Число слишком большое" << endl;
            }
        }
        cout << "Неверный формат данных, введите целое число: " << endl;
        cin.clear();
        cin.ignore();
    }
}

int neighbor(vector<vector<int>> home, int y, int x) {
    int h = home.size();
    int w = home[0].size();
    int cnt = 0;
    int y_st = y - 1, y_ed = y + 1;
    int x_st = x - 1, x_ed = x + 1;

    if (home[y][x] > 0) {
        --cnt;
    }

    if (y == 0) {
        y_st = y;
    }
    if (x == 0) {
        x_st = x;
    }
    if (y == --h) {
        y_ed = y;
    }
    if (x == --w) {
        x_ed = x;
    }

    for (int i = y_st; i <= y_ed; ++i) {
        for (int j = x_st; j <= x_ed; ++j) {
            if (home[i][j] > 0) {
                ++cnt;
            }
        }
    }
    return cnt;
}

string show(vector<vector<int>> vect) {
    string s = "";
    int msize = vect.size();
    for (int i = 0; i < msize; ++i) {
        for (int j = 0; j < msize; ++j) {
            if (vect[i][j] < 10) {
                s += ' ';
            }
            if (vect[i][j] == 0) {
                s += "  ";
            } else {
                s += ' ' + to_string(vect[i][j]);
            }
        }
        s += "\n";
    }
    return s;
}

void life() {
    random_device rd;
    mt19937 gen(rd());

    cout << "Введите ширину поля: ";
    int msize = ichek();
    cout << "Введите количество возможных состояний клетки (микробом станет только одно): ";
    int lets = ichek();
    cout << "Введите количество поколений: ";
    int age = ichek();

    if (msize <= 0 || age <= 0 || lets <= 0) {
        cout << "Ошибка, размеры поля должны быть положительными" << endl;
        return;
    }

    vector<vector<char>> home(msize, vector<char>(msize, 'A'));

    vector<vector<int>> past(msize, vector<int>(msize, 0));
    vector<vector<int>> future = past;

    char isBact = (char) (gen() % lets + 'A');
    char cur;

    ofstream file("work.out");

    string cs = "";

    string isBactCh(1, isBact);

    cs = "В чашке петри множество разных частичек, но только \'" + isBactCh + "\' - микроб:" + "\n";
    cout << cs;
    file << cs;

    for (int i = 0; i < msize; ++i) {
        for (int j = 0; j < msize; ++j) {
            cur = (char) (gen() % lets + 'A');
            home[i][j] = cur;

            cs = cur + ' ';
            cout << cs;
            file << cs;

            if (cur == isBact) {
                past[i][j] = 1;
            }
        }
        cout << endl;
        file << '\n';
    }

    cs = "\n\nСтартовое поколение\n";
    cout << cs;
    file << cs;

    cs = show(past);
    cout << cs;
    file << cs;

    int cbact, cneigh;

    for (int generation = 1; generation <= age; ++generation) {
        for (int i = 0; i < msize; ++i) {
            for (int j = 0; j < msize; ++j) {
                cbact = past[i][j];
                cneigh = neighbor(past, i, j);
                if (cbact > 0) {
                    if (cbact == 11 || cneigh > 3 || cneigh < 2) {
                        cbact = 0;
                    } else {
                        ++cbact;
                    }
                } else {
                    if (cneigh == 3) {
                        cbact = 1;
                    }
                }
                future[i][j] = cbact;
            }
        }
        cs = "Поколение " + to_string(generation) + "\n";
        cout << cs;
        file << cs;

        cs = show(future);
        cout << cs;
        file << cs;
        past = future;
    }
    file.close();
}

int main() {
    SetConsoleOutputCP(CP_UTF8);
    life();
    return 0;
}
