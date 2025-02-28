#include <iostream>
#include <windows.h>
#include <algorithm>
#include <fstream>
#include <vector>
#include <sstream>
#include <chrono>
#include <psapi.h>
#include <string>
#include <unordered_set>

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

void changeMoney(const std::vector<int>& coins, int toChange) {
    std::vector <int> min_coins(toChange + 1,1000);

    min_coins[0] = 0;

    for (int i = 1; i <= toChange; ++i)
    {
        for (auto& coin : coins)
        {
            if (coin <= i && min_coins[i - coin] != 1000)
            min_coins[i] = min(min_coins[i], min_coins[i - coin]+1);
        }
    }

    if (min_coins[toChange] != 1000)
        std::cout << "min number of coins: " << min_coins[toChange] << "\n\n";
    else
        std::cout << "Not possible to exchange!\n\n";
}

//За O(nlogn)
void max_subsequence(std::vector<int> a)
{
    const int n = a.size();
    std::vector<int> d(n);
    std::vector<int> pos(n); //  индекс элемента, на который заканчивается оптимальная подпоследовательность длины i
    std::vector<int> prev(n); // позиция предыдущего элемента для a[i]
    int length = 0;

    pos[0] = -1;
    d[0] = INT_MIN;

    for (int i = 1; i < n; ++i)
    {
        d[i] = INT_MAX;
    }

    for (int i = 0; i < n; ++i)
    {
        int j = std::lower_bound(d.begin(), d.end(), a[i]) - d.begin(); //бинарным поиском ищем позицию j элемента >= a[i] за O(logn)
        if (d[j - 1] < a[i] && a[i] < d[j])
        {
            d[j] = a[i];
            pos[j] = i;
            prev[i] = pos[j - 1];
            length = max(length, j);
        }
    }

    std::vector<int> answer;
    int p = pos[length];

    while (p != -1) {
        answer.push_back(a[p]);
        p = prev[p];
    }

    for (int i = answer.size() - 1; i >= 0; --i)
    {
        std::cout << answer.at(i) << " ";
    }
    std::cout << "\n\n";
}

void templates(const std::string& temp, const std::string& match) {
    int n = temp.size();
    int m = match.size();

    std::vector<std::vector<bool>> dp(n + 1, std::vector<bool>(m + 1, false));
    dp[0][0] = true;

    for (int j = 1; j <= m; j++) {
        if (temp[0] == '*') {
            dp[0][j] = true;
        }
    }

    for (int i = 1; i <= n; i++) {
        for (int j = 0; j <= m; j++) {
            char t = temp[i - 1];

            if (t == '*') {

                dp[i][j] = dp[i - 1][j];

                if (j > 0) 
                    dp[i][j] = dp[i][j] || dp[i][j - 1];
            }
            else if (t == '?' && j > 0) {

                dp[i][j] = dp[i - 1][j - 1];
            }
            else if (j > 0 && t == match[j - 1]) {

                dp[i][j] = dp[i - 1][j - 1];
            }
        }
    }

    std::cout << (dp[n][m] ? "YES\n\n" : "NO\n\n");
}

int main() {
    /**int toChange;
    int input;
    std::vector<int> coins;

    std::cout << "Enter value to exchange: ";
    std::cin >> toChange;

    while (std::cin >> input)
    {
        coins.push_back(input);
    }

    MEASURE_TIME_MEMORY(changeMoney,coins,toChange);**/

    /**
    std::vector<int> seq;
    int input;

    while (std::cin >> input)
    {
        seq.push_back(input);
    }

    MEASURE_TIME_MEMORY(max_subsequence, seq);**/

    std::string temp;
    std::string match;

    std::cin >> temp >> match;
    MEASURE_TIME_MEMORY(templates,temp,match);

}

