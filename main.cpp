#include <iostream>
#include <vector>
#include <cmath>
#include <chrono>
#include <fstream>
#include <cstdlib>
#include <algorithm>

using namespace std;

int n = 4;
int closint;
int opint;

bool possible(const vector<pair<int,int>> &state) {
    for (int i = 0; i < state.size(); i++) {
        for (int j = i + 1; j < state.size(); j++) {
            if (state[i].first == state[j].first ||
                state[i].second == state[j].second ||
                abs(state[i].first - state[j].first) == abs(state[i].second - state[j].second)) {
                return false;
            }
        }
    }
    return true;
}

vector<vector<pair<int,int>>> createChildren(const vector<pair<int,int>> &S) {
    vector<vector<pair<int,int>>> child;
    //chyba najlepsze co moze byc czyli generuje dzieci tylko jedna kolumne glebokosci czyli mniej dzieci
    //przepraszam, ale to konieczne bo mi plonie ram jak chce to zasymulowac
    int currentCol = S.size();
    for (int row = 0; row < n; row++) {
        vector<pair<int,int>> newState = S;
        newState.push_back({row, currentCol});
        child.push_back(newState);
    }

    return child;
}

bool search(vector<pair<int,int>> S0, vector< vector<pair<int,int>> > &solutions, bool useDFS)
{
    vector< vector<pair<int,int>> > open;
    vector< vector<pair<int,int>> > closed;
    open.push_back(S0);

    while (!open.empty()) {
        vector<pair<int,int>> s;
        if (useDFS) {
            s = open.back();
            open.pop_back();
        } else {
            s = open.front();
            open.erase(open.begin());
        }

        if (s.size() == n && possible(s)) {
            solutions.push_back(s);
            closint=closed.size();
            opint=open.size();
            return true;
        }

        vector<vector<pair<int,int>>> T = createChildren(s);
        for (int i = 0; i < T.size(); i++) {
            bool inClosed = false;
            bool inOpen   = false;

            for (int c = 0; c < closed.size(); c++) {
                if (closed[c] == T[i]) {
                    inClosed = true;
                    break;
                }
            }
            if (!inClosed) {
                for (int o = 0; o < open.size(); o++) {
                    if (open[o] == T[i]) {
                        inOpen = true;
                        break;
                    }
                }
            }

            if (!inClosed && !inOpen && possible(T[i])) {
                open.push_back(T[i]);
            }
        }

        closed.push_back(s);
    }

    return false;
}

int h1(const vector<pair<int, int>>& state) {
    int l = state.size();
    int sum = 0;
    for (int i = 0; i < state.size(); i++) {
        int row = state[i].first;
        int weight = (row + 1 <= n / 2) ? (n - row) : (row + 1);
        sum += weight;
    }
    return (n - l) * sum;
}

int count_attacks(const vector<pair<int, int>>& state) {
    int count = 0;
    for (int i = 0; i < state.size(); i++) {
        for (int j = i + 1; j < state.size(); j++) {
            if (state[i].first == state[j].first ||
                state[i].second == state[j].second ||
                abs(state[i].first - state[j].first) == abs(state[i].second - state[j].second)) {
                count++;
            }
        }
    }
    return count;
}

int h2(const vector<pair<int, int>>& state) {
    int l = state.size();
    return count_attacks(state) + (n - l);
}

int hdod(const vector<pair<int, int>>& state) {
    int sum = 0;
    for (int i = 0; i < state.size(); i++) {
        for (int j = i + 1; j < state.size(); j++) {
            int dist = abs(state[i].first - state[j].first) + abs(state[i].second - state[j].second);
            sum += abs(dist - 3);
        }
    }
    return sum;
}

bool bestFirstSearchHeuristic(
        vector<pair<int, int>> state,
        vector<vector<pair<int, int>>> &solutions,
        int (*heuristic)(const vector<pair<int, int>>&)
) {
    vector<vector<pair<int, int>>> open;
    vector<vector<pair<int, int>>> closed;
    open.push_back(state);

    while (!open.empty()) {
        sort(open.begin(), open.end(), [heuristic](const vector<pair<int, int>> &a, const vector<pair<int, int>> &b) {
            return heuristic(a) < heuristic(b);
        });

        vector<pair<int, int>> current = open.front();
        open.erase(open.begin());

        if (current.size() == n && possible(current)) {
            solutions.push_back(current);
            closint = closed.size();
            opint = open.size();
            return true;
        }

        for (int row = 0; row < n; row++) {
            for (int col = 0; col < n; col++) {
                vector<pair<int, int>> newState = current;
                newState.push_back(make_pair(row, col));
                if (possible(newState)) {
                    open.push_back(newState);
                }
            }
        }


        closed.push_back(current);
    }

    return false;
}

int main() {
    int n_start = 4, n_end = 8;
    vector<int> CLOSED_BFS(100), CLOSED_DFS(100), CLOSED_H1(100), CLOSED_H2(100), CLOSED_Hdod(100);
    vector<int> OPEN_BFS(100), OPEN_DFS(100), OPEN_H1(100), OPEN_H2(100), OPEN_Hdod(100);
    vector<double> TIME_BFS(100), TIME_DFS(100), TIME_H1(100), TIME_H2(100), TIME_Hdod(100);

    for (int currentN = n_start; currentN <= n_end; currentN++) {
        n = currentN;
        vector<pair<int,int>> Start;
        vector<vector<pair<int,int>>> Solutions;

        if (currentN <= 6) {
            auto t1 = chrono::high_resolution_clock::now();
            bool found = search(Start, Solutions, false);
            auto t2 = chrono::high_resolution_clock::now();
            TIME_BFS[n] = chrono::duration<double, milli>(t2 - t1).count();
            CLOSED_BFS[n] = closint;
            OPEN_BFS[n] = opint;

            cout << "--- BFS n=" << n << " ---\n";
            cout << "Czas: " << TIME_BFS[n] << " ms\n";
            cout << "OPEN: " << OPEN_BFS[n] << " | CLOSED: " << CLOSED_BFS[n] << "\n";
            if (found) {
                cout << "Rozwiazanie: ";
                for (int i = 0; i < Solutions[0].size(); i++) {
                    cout << "(" << Solutions[0][i].first << "," << Solutions[0][i].second << ") ";
                }
                cout << "\n";
            } else {
                cout << "Nie znaleziono rozwiazania.\n";
            }
            cout << "\n";
        }

        Solutions.clear();
        auto t3 = chrono::high_resolution_clock::now();
        bool found_dfs = search(Start, Solutions, true);
        auto t4 = chrono::high_resolution_clock::now();
        TIME_DFS[n] = chrono::duration<double, milli>(t4 - t3).count();
        CLOSED_DFS[n] = closint;
        OPEN_DFS[n] = opint;

        cout << "--- DFS n=" << n << " ---\n";
        cout << "Czas: " << TIME_DFS[n] << " ms\n";
        cout << "OPEN: " << OPEN_DFS[n] << " | CLOSED: " << CLOSED_DFS[n] << "\n";
        if (found_dfs) {
            cout << "Rozwiazanie: ";
            for (int i = 0; i < Solutions[0].size(); i++) {
                cout << "(" << Solutions[0][i].first << "," << Solutions[0][i].second << ") ";
            }
            cout << "\n";
        } else {
            cout << "Nie znaleziono rozwiazania.\n";
        }
        cout << "\n";

        Solutions.clear();
        auto t5 = chrono::high_resolution_clock::now();
        bool found_h1 = bestFirstSearchHeuristic(Start, Solutions, h1);
        auto t6 = chrono::high_resolution_clock::now();
        TIME_H1[n] = chrono::duration<double, milli>(t6 - t5).count();
        CLOSED_H1[n] = closint;
        OPEN_H1[n] = opint;

        cout << "--- BestFS H1 n=" << n << " ---\n";
        cout << "Czas: " << TIME_H1[n] << " ms\n";
        cout << "OPEN: " << OPEN_H1[n] << " | CLOSED: " << CLOSED_H1[n] << "\n";
        if (found_h1) {
            cout << "Rozwiazanie: ";
            for (int i = 0; i < Solutions[0].size(); i++) {
                cout << "(" << Solutions[0][i].first << "," << Solutions[0][i].second << ") ";
            }
            cout << "\n";
        } else {
            cout << "Nie znaleziono rozwiazania.\n";
        }
        cout << "\n";

        Solutions.clear();
        auto t7 = chrono::high_resolution_clock::now();
        bool found_h2 = bestFirstSearchHeuristic(Start, Solutions, h2);
        auto t8 = chrono::high_resolution_clock::now();
        TIME_H2[n] = chrono::duration<double, milli>(t8 - t7).count();
        CLOSED_H2[n] = closint;
        OPEN_H2[n] = opint;

        cout << "--- BestFS H2 n=" << n << " ---\n";
        cout << "Czas: " << TIME_H2[n] << " ms\n";
        cout << "OPEN: " << OPEN_H2[n] << " | CLOSED: " << CLOSED_H2[n] << "\n";
        if (found_h2) {
            cout << "Rozwiazanie: ";
            for (int i = 0; i < Solutions[0].size(); i++) {
                cout << "(" << Solutions[0][i].first << "," << Solutions[0][i].second << ") ";
            }
            cout << "\n";
        } else {
            cout << "Nie znaleziono rozwiazania.\n";
        }
        cout << "\n";
        if(currentN <= 5){
        Solutions.clear();
        auto t9 = chrono::high_resolution_clock::now();
        bool found_hdod = bestFirstSearchHeuristic(Start, Solutions, hdod);
        auto t10 = chrono::high_resolution_clock::now();
        TIME_Hdod[n] = chrono::duration<double, milli>(t10 - t9).count();
        CLOSED_Hdod[n] = closint;
        OPEN_Hdod[n] = opint;

        cout << "--- BestFS Hdod n=" << n << " ---\n";
        cout << "Czas: " << TIME_Hdod[n] << " ms\n";
        cout << "OPEN: " << OPEN_Hdod[n] << " | CLOSED: " << CLOSED_Hdod[n] << "\n";
        if (found_hdod) {
            cout << "Rozwiazanie: ";
            for (int i = 0; i < Solutions[0].size(); i++) {
                cout << "(" << Solutions[0][i].first << "," << Solutions[0][i].second << ") ";
            }
            cout << "\n";
        } else {
            cout << "Nie znaleziono rozwiazania.\n";
        }}
        cout << "\n";}


        ofstream out("wyniki.dat");
    out << "# n BFS_open BFS_closed DFS_open DFS_closed H1_open H1_closed H2_open H2_closed Hdod_open Hdod_closed ";
    out << "BFS_time DFS_time H1_time H2_time Hdod_time\n";

    for (int i = n_start; i <= n_end; i++) {
        out << i << " "
            << OPEN_BFS[i] << " " << CLOSED_BFS[i] << " "
            << OPEN_DFS[i] << " " << CLOSED_DFS[i] << " "
            << OPEN_H1[i] << " " << CLOSED_H1[i] << " "
            << OPEN_H2[i] << " " << CLOSED_H2[i] << " "
            << OPEN_Hdod[i] << " " << CLOSED_Hdod[i] << " "
            << TIME_BFS[i] << " " << TIME_DFS[i] << " "
            << TIME_H1[i] << " " << TIME_H2[i] << " " << TIME_Hdod[i] << "\n";
    }
    out.close();

    ofstream gp("skrypt.gnuplot");
    gp << "set datafile separator whitespace\n";
    gp << "set key outside\n";
    gp << "set grid\n";
    gp << "set term png size 1200,800\n";

    gp << "set output 'czas.png'\n";
    gp << "set title 'Czas wykonania'\n";
    gp << "set xlabel 'n'\n";
    gp << "set ylabel 'czas [ms]'\n";
    gp << "set logscale y\n";
    gp << "plot 'wyniki.dat' using 1:12 title 'BFS' with linespoints lw 2 pt 7, \\\n";
    gp << "     'wyniki.dat' using 1:13 title 'DFS' with linespoints lw 2 pt 7, \\\n";
    gp << "     'wyniki.dat' using 1:14 title 'H1' with linespoints lw 2 pt 7, \\\n";
    gp << "     'wyniki.dat' using 1:15 title 'H2' with linespoints lw 2 pt 7, \\\n";
    gp << "     'wyniki.dat' using 1:16 title 'Hdod' with linespoints lw 2 pt 7\n";

    gp << "set output 'open.png'\n";
    gp << "set title 'Rozmiar OPEN'\n";
    gp << "set xlabel 'n'\n";
    gp << "set ylabel 'rozmiar OPEN'\n";
    gp << "plot 'wyniki.dat' using 1:2 title 'BFS' with linespoints lw 2 pt 7, \\\n";
    gp << "     'wyniki.dat' using 1:4 title 'DFS' with linespoints lw 2 pt 7, \\\n";
    gp << "     'wyniki.dat' using 1:6 title 'H1' with linespoints lw 2 pt 7, \\\n";
    gp << "     'wyniki.dat' using 1:8 title 'H2' with linespoints lw 2 pt 7, \\\n";
    gp << "     'wyniki.dat' using 1:10 title 'Hdod' with linespoints lw 2 pt 7\n";

    gp << "set output 'closed.png'\n";
    gp << "set title 'Rozmiar CLOSED'\n";
    gp << "set xlabel 'n'\n";
    gp << "set ylabel 'rozmiar CLOSED'\n";
    gp << "plot 'wyniki.dat' using 1:3 title 'BFS' with linespoints lw 2 pt 7, \\\n";
    gp << "     'wyniki.dat' using 1:5 title 'DFS' with linespoints lw 2 pt 7, \\\n";
    gp << "     'wyniki.dat' using 1:7 title 'H1' with linespoints lw 2 pt 7, \\\n";
    gp << "     'wyniki.dat' using 1:9 title 'H2' with linespoints lw 2 pt 7, \\\n";
    gp << "     'wyniki.dat' using 1:11 title 'Hdod' with linespoints lw 2 pt 7\n";

    gp.close();


    return 0;
}
