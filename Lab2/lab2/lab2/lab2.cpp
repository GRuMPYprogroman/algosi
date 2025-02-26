#include <iostream>
#include <windows.h>
#include <algorithm>
#include <fstream>
#include <vector>
#include <sstream>
#include <chrono> // для измерения времени
#include <psapi.h> // для измерения памяти



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


//task1
void scarecrowTest(std::vector<int>& ar,int n, int k) {

    for (int i = 0; i < n - k; ++i) {
        int j = i;
        while (j < n - k && ar[j] >= ar[j + k])
        {
            std::swap(ar[j], ar[j + k]);
            j += k;
        }

    }

    for (int i = 0; i < n-1; ++i)
    {
        if (ar[i] > ar[i + 1]) {
            std::cout << "NO\n\n";
            return;
        }
    }
    std::cout << "YES\n\n";
}

void hirsh(std::vector<int>& ar) {
    const int max_elem = (*std::max_element(ar.begin(), ar.end()));

    std::vector<int> count(max_elem + 1,0);

    for (auto i : ar) {
        count[i] += 1;
    }

    int sum = 0;

    for (int i = count.size() - 1; i >= 0; i--) {
        sum += count[i];
        if (sum >= i) {
            std::cout << "H-index: " << i << "\n\n";
            break;
        }
    }
}

//task2

class Stack {
private:
    struct Node {
        int value;
        Node* next;
        Node(int val) : value(val), next(nullptr) {}
    };

    Node* dataTop;        // Туть все данные
    Node* maxTop;         // Туть максимумы    

public:
    
    Stack(size_t max = 1000000) : dataTop(nullptr), maxTop(nullptr){}

    
    ~Stack() {
        while (!isEmpty()) {
            pop();
        }
    }

    void push(int value) {

        Node* newDataNode = new Node(value);
        newDataNode->next = dataTop;
        dataTop = newDataNode;

        if (maxTop == nullptr || value >= maxTop->value) {
            Node* newMaxNode = new Node(value);
            newMaxNode->next = maxTop;
            maxTop = newMaxNode;
        }

    }

    int pop() {
        if (isEmpty()) {
            throw std::underflow_error("Stack underflow: cannot pop, stack is empty");
        }
        Node* temp = dataTop;
        int value = temp->value;
        dataTop = dataTop->next;
        delete temp;

        if (value == maxTop->value) {
            Node* tempMax = maxTop;
            maxTop = maxTop->next;
            delete tempMax;
        }

        return value;
    }

    int top() const {
        if (isEmpty()) {
            throw std::underflow_error("Stack is empty: cannot get top");
        }
        return dataTop->value;
    }

    int getMax() const {
        if (isEmpty()) {
            throw std::underflow_error("Stack is empty: cannot get max");
        }
        return maxTop->value;
    }

    bool isEmpty() const {
        return dataTop == nullptr;
    }

};

int main()
{
    /**std::ifstream fin("input.txt");
    if (!fin) {
        std::cerr << "Cannot open input.txt\n";
        return 1;
    }

    int n = 5;
    //int k;
    //fin >> n;
    //fin >> k;

    std::vector<int> ar(n);
    for (int i = 0; i < n; ++i) {
        fin >> ar[i];
    }
    fin.close();

    //MEASURE_TIME_MEMORY(hirsh,ar); **/

    Stack stack;

    stack.push(1);
    stack.push(10);
    std::cout << stack.pop() << "\n";
    stack.push(2);
    stack.push(1234);
    std::cout << stack.pop() << "\n";


}
