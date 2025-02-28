#include <iostream>
#include <windows.h>
#include <algorithm>
#include <fstream>
#include <vector>
#include <chrono>
#include <psapi.h>
#include <string>
#include <numeric>
#include <stack>
#include <sstream>
#include <queue>

SIZE_T GetMemoryUsage() {
    PROCESS_MEMORY_COUNTERS pmc;
    GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc));
    return pmc.WorkingSetSize;
}

#define MEASURE_TIME_MEMORY(func, ...) \
    do { \
        auto start = std::chrono::high_resolution_clock::now(); \
        SIZE_T mem_before = GetMemoryUsage(); \
        func(__VA_ARGS__); \
        auto end = std::chrono::high_resolution_clock::now(); \
        SIZE_T mem_after = GetMemoryUsage(); \
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start); \
        auto duration_us = std::chrono::duration_cast<std::chrono::microseconds>(end - start); \
        std::cout << #func << ":\n"; \
        std::cout << "Time: " << duration_us.count() << " microseconds\n"; \
        std::cout << "Time: " << duration.count() << " milliseconds\n"; \
        std::cout << "Time: " << static_cast<double>(duration.count()) / 1000 << " seconds\n"; \
        std::cout << "Memory used: " << (mem_after - mem_before) / 1024.0 / 1024.0 << " MB\n\n"; \
        std::cout << "Memory before: " << (mem_before) / 1024.0 / 1024.0 << " MB\n"; \
        std::cout << "Memory after: " << (mem_after) / 1024.0 / 1024.0 << " MB\n"; \
    } while(0)

//блок 1

bool dfs_go(int u, int v,const std::vector<std::vector<int>>& a,std::vector<bool>& visited)
{
    if (u == v) { return true; }
    visited[u-1] = true;
    for (int i : a[u-1]) {
        if (!visited[i-1]) {
            if (dfs_go(i, v, a, visited))
                return true;
        }
    }
    return false;
}

bool hasPath(int u, int v, const std::vector<std::vector<int>>& a) {
    std::vector<bool> visited(a.size(), false);
    return dfs_go(u, v, a, visited);
}

void maze()
{
    std::fstream in("input.txt");
    int n, m, u, v;

    in >> n >> m;
    std::vector<std::vector<int>> a(n);

    for (int i = 0; i < m; ++i) {
        int k, j;

        in >> k >> j;

        a[k - 1].push_back(j);
        a[j-1].push_back(k);
    }
    in >> u >> v;
    std::cout << (hasPath(u, v, a) ? 1 : 0) << "\n";
    in.close();
}

bool dfs_goCycle(int u, const std::vector<std::vector<int>>& a, std::vector<bool> visited)
{
    visited[u] = true;
    for (int v : a[u]) {
        if (!visited[v-1]) {
            if (dfs_goCycle(v-1, a, visited)) {
                return true;
            }
        }
        else if (visited[v-1]) {
            return true;
        }
    }
    visited[u] = false;
    return false;
}

bool hasCycle(const std::vector<std::vector<int>>& a) {
    int n = a.size();
    std::vector<bool> visited(n, false);
    for (int i = 0; i < n; ++i) {
            if (dfs_goCycle(i, a, visited)) {
                return true;
            }
        }
    return false;
}

void isCycled()
{
    std::fstream in("input.txt");
    int n, m;

    in >> n >> m;
    std::vector<std::vector<int>> a(n);

    for (int i = 0; i < m; ++i) {
        int k, j;

        in >> k >> j;

        a[k - 1].push_back(j);
    }

    for (int i = 0; i < a.size(); ++i)
    {
        if (hasCycle(a))
        {
            std::cout << "1\n";
            break;
        }
        else
        {
            std::cout << "0\n";
            break;
        }
    }
    in.close();
}

void numberOfTransfers()
{
    std::fstream in("input.txt");
    int n, m, u, v;

    in >> n >> m;
    std::vector<std::vector<int>> a(n);

    for (int i = 0; i < m; ++i) {
        int k, j;

        in >> k >> j;

        a[k - 1].push_back(j);
        a[j - 1].push_back(k);
    }

    in >> u >> v;
    in.close();

    std::vector<int> distance(n, -1);
    std::vector<bool> visited(n, false);

    std::queue<int> q;

    distance[u-1] = 0;
    visited[u-1] = true;
    q.push(u);

    while (!q.empty()) {
        int current = q.front();
        q.pop();

        for (int neighbor : a[current-1]) {
            if (!visited[neighbor-1]) {
                visited[neighbor-1] = true;
                distance[neighbor-1] = distance[current-1] + 1;
                q.push(neighbor);

                if (neighbor == v) {
                    std::cout << distance[neighbor-1] <<"\n";
                    return;
                }
            }
        }
    }

    std::cout << -1 << "\n";
}

//блок №2

void rabinSearch(std::string& p, std::string& t)
{
    const int d = 256;  // размер алфавита (ASCII)
    const int q = 10000;  // простое число для модуля

    int m = p.length();  // длина шаблона
    int n = t.length();  // длина текста

    if (m > n || m == 0) {
        std::cout << -1 << std::endl;
        return;
    }

    std::vector<int> occurrences;  // позиции вхождений

    // Вычисляем коэффициент h = d^(m-1) % q
    long long h = 1;
    for (int i = 0; i < m - 1; i++) {
        h = (h * d) % q;
    }

    // Начальные хэши шаблона и первого окна текста
    long long p_hash = 0;
    long long t_hash = 0;
    for (int i = 0; i < m; i++) {
        p_hash = (d * p_hash + p[i]) % q;
        t_hash = (d * t_hash + t[i]) % q;
    }

    for (int i = 0; i <= n - m; i++) {
        if (p_hash == t_hash) {
            bool match = true;
            for (int j = 0; j < m; j++) {
                if (p[j] != t[i + j]) {
                    match = false;
                    break;
                }
            }
            if (match) {
                occurrences.push_back(i);
            }
        }

        // Обновляем хэш для следующего окна
        if (i < n - m) {
            t_hash = (d * (t_hash - t[i] * h) + t[i + m]) % q;
            if (t_hash < 0) { 
                t_hash += q;
            }
        }
    }

    std::cout << occurrences.size() << std::endl;
    for (int i = 0; i < occurrences.size(); i++) {
        std::cout << occurrences[i] << " ";
    }
    std::cout << std::endl;
}
struct Term {
    int count;
    std::string substr;
};

void decomposeString(std::string& s) {
    int len = s.length();
    std::vector<Term> result;
    int pos = 0;

    while (pos < len) {
        int max_repeat = 0;
        std::string best_substr;

        // Перебираем возможные длины подстрок
        for (int sub_len = 1; sub_len <= (len - pos) / 2; sub_len++) {
            std::string substr = s.substr(pos, sub_len);
            int repeat = 1;

            // Считаем, сколько раз подстрока повторяется подряд
            while (pos + (repeat + 1) * sub_len <= len) {
                if (s.substr(pos + repeat * sub_len, sub_len) == substr) {
                    repeat++;
                }
                else {
                    break;
                }
            }

            // Если нашли более выгодное разбиение
            if (repeat > 1 && repeat * sub_len > max_repeat) {
                max_repeat = repeat * sub_len;
                best_substr = substr;
            }
        }

        // Если нашли повторяющуюся подстроку
        if (max_repeat > 0) {
            int count = max_repeat / best_substr.length();
            result.push_back({ count, best_substr });
            pos += max_repeat;
        }
        else {
            // Если нет повторений, берем один символ
            result.push_back({ 1, std::string(1, s[pos]) });
            pos++;
        }
    }

    for (int i = 0; i < result.size(); i++) {
        if (i > 0) std::cout << " + ";
        if (result[i].count > 1) {
            std::cout << result[i].count << "*" << result[i].substr;
        }
        else {
            std::cout << result[i].substr;
        }
    }
}
int main()
{
    //MEASURE_TIME_MEMORY(maze);
    
    //MEASURE_TIME_MEMORY(isCycled);

    //MEASURE_TIME_MEMORY(numberOfTransfers);

    /**std::string p, t;
    std::cin >> p >> t;
    MEASURE_TIME_MEMORY(rabinSearch,p,t);**/

    std::string s;
    std::cin >> s;
    MEASURE_TIME_MEMORY(decomposeString,s);
}
