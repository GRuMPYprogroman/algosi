#include <iostream>
#include <windows.h>
#include <algorithm>
#include <fstream>
#include <vector>
#include <sstream>
#include <chrono> // для измерения времени
#include <psapi.h> // для измерения памяти
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <string>
#include <unordered_set>
#include <functional>


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

void isHeap(std::vector<int>& a) {

    int n = a.size();

    for (int i = 1; i < n/2 + 1; ++i) 
    {
        if (2 * i + 1 < n) 
        {
            if (!(a[i - 1] <= a[i * 2 - 1] && a[i - 1] <= a[i * 2]))
            {
                std::cout << "NO!\n\n";
                return;
            } 
        }
        else
        {
            if (!(a[i - 1] <= a[i * 2 - 1]))
            {
                std::cout << "NO!\n\n";
                return;
            }
        }
    }

    std::cout << "YES!\n\n";
}

static std::atomic<bool> shouldStop(false);

class Queue {
private:

    struct Node {
        int P;
        Node* next;     
        Node(const int& p) : P(p), next(nullptr) {}
    };

    Node* head; 
    Node* tail;
    size_t size;

public:
    size_t maxSize;
    std::mutex q;
    std::condition_variable not_empty; 

    Queue() : head(nullptr), tail(nullptr), size(0), maxSize(0) {}

    ~Queue() {
        while (!empty()) {
            pop();
        }
    }

    void push(const int& p, const int& a) {

        if (size == maxSize) {
            std::cout << "-1\n\n";
            return;
        }
        
        std::unique_lock<std::mutex> lock(q);

        std::cout << a << "\n\n";

        Node* new_node = new Node(p);
        if (empty()) {
            head = tail = new_node; 
        }
        else {
            tail->next = new_node;  
            tail = new_node;        
        }
        ++size;

        lock.unlock();
        not_empty.notify_one();
    }

    void pop() {    
        if (empty()) {
            return;
        }
        Node* temp = head;       
        head = head->next;       
        delete temp;            
        if (empty()) {
            tail = nullptr;
        }
        --size;
    }


    int front() {
        if (empty()) {
            return 0;
        }
        return head->P;
    }

    bool empty(){
        return size == 0;
    }

    size_t get_size(){
        return size;
    }
};

class packetsHandler {
private:
    std::thread thread;
public:
    static Queue buffer;

    packetsHandler(size_t s) {
        buffer.maxSize = s;
        thread = std::thread(&packetsHandler::run, this);
    }

    ~packetsHandler() {
        shouldStop = true;
        buffer.not_empty.notify_one();
        if (this->thread.joinable()) {
            thread.join();
        }
    }

    void run() {
        while (true) {
            int P;
            {
                std::unique_lock<std::mutex> lock(buffer.q);
                buffer.not_empty.wait(lock, []() { return !buffer.empty() || shouldStop; });
                if (shouldStop && buffer.empty()) break;
                P = buffer.front();
                std::this_thread::sleep_for(std::chrono::seconds(P));
                buffer.pop();
            }
        }
    }

};

Queue packetsHandler::buffer;

class reader {
private:
    std::string filename;
    std::thread thread;
public:
    reader(std::string name):filename(name)
    {
        thread = std::thread(&reader::run, this);
    }

    ~reader() {
        if (thread.joinable())
            thread.join();
    }

    void run() {
        std::fstream in(filename);
        std::string line;
        std::stringstream ss;

        getline(in, line);

        int last_time = 0;

        while (getline(in,line))
        {
                int A, P;
                ss.str(line);
                ss.clear();
                ss >> A >> P;
                std::this_thread::sleep_for(std::chrono::seconds(A) - std::chrono::seconds(last_time));
                packetsHandler::buffer.push(P,A);
                last_time = A;
        }

        shouldStop = true;
        in.close();
    }
};

//task2

std::string stringAdd(const std::string& a, const std::string& b) {
    std::string result;
    int carry = 0; // добавочный разряд 
    int i = a.size() - 1;  
    int j = b.size() - 1; 

    while (i >= 0 || j >= 0 || carry) {
        int sum = carry;
        if (i >= 0) sum += a[i--] - '0';  
        if (j >= 0) sum += b[j--] - '0'; 
        carry = sum / 10;  
        result.push_back(sum % 10 + '0');  
    }

    reverse(result.begin(), result.end());
    return result;
}

struct customHash {
    size_t operator()(const std::string& s) const {
        std::string prefix = s.size() > 20 ? s.substr(0, 20) : s;
        return std::hash<std::string>()(prefix);
    }
};

void is_fib()
{
    std::unordered_set<std::string, customHash> fib;
    std::string a = "1";
    std::string b = "1";

    fib.emplace(a);

    while(true)
    {
        std::string next = stringAdd(a,b);
        if (next.size() > 5000)
            break;

        fib.emplace(next);

        a = b;
        b = next;
    }

    std::fstream in("input.txt");
    std::string line;

    while(getline(in,line))
    {
        fib.count(line) ? std::cout <<"YES\n" : std::cout << "NO\n";
    }

}

int main()
{
    /**std::vector<int> a;

    int input;
    while (std::cin >> input)
    {
        a.push_back(input);
    }

    MEASURE_TIME_MEMORY(isHeap, a);**/

    /**std::string filename = "input.txt";
    std::fstream in(filename);
    std::string line;
    std::stringstream ss;
    int s;

    getline(in, line);

    ss.str(line);

    ss >> s;

    std::cout << "Buffer size: " << s << "\n\n";

    in.close();


    packetsHandler hand(s);
    reader read(filename);**/

    MEASURE_TIME_MEMORY(is_fib);

}