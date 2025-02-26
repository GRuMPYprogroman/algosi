#include <iostream>
#include <windows.h>
#include <fstream>
#include <vector>
#include <sstream>
#include <chrono> // для измерения времени
#include <psapi.h> // для измерения памяти

//блок №1(3 балла)

SIZE_T GetMemoryUsage() {
    PROCESS_MEMORY_COUNTERS pmc;
    GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc));
    return pmc.WorkingSetSize;
}

#define MEASURE_TIME_MEMORY(func) \
    do { \
        auto start = std::chrono::high_resolution_clock::now(); \
        SIZE_T mem_before = GetMemoryUsage(); \
        func(); \
        auto end = std::chrono::high_resolution_clock::now(); \
        SIZE_T mem_after = GetMemoryUsage(); \
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start); \
        std::cout << #func << ":\n"; \
        std::cout << "Time: " << static_cast<double>(duration.count())/1000 << " seconds\n"; \
        std::cout << "Memory used: " << (mem_after - mem_before) / 1024.0 / 1024.0<< " MB\n\n"; \
        std::cout << "Memory before: " << (mem_before) / 1024.0 / 1024.0 << " MB\n\n"; \
        std::cout << "Memory after: " << (mem_after) / 1024.0 / 1024.0 << " MB\n\n"; \
    } while(0)

void sum()
{
    std::ifstream in("input.txt");
    std::ofstream out("output.txt");

    int a;
    int b;

    in >> a >> b;

    out << a+b;

    in.close();
    out.close();

}

void fib() {
    std::ifstream in("input.txt");
    std::ofstream out("output.txt");

    int a = 0;
    int b = 1;
    int n;
    in >> n;

    for (int i = 0; i < n; i++)
    {
        a = b - a;
        b = b + a;
    }

    out << a;
    in.close();
    out.close();

}

void last_fib() {
    std::ifstream in("input.txt");
    std::ofstream out("output.txt");

    int a = 0;
    int b = 1;
    int n;
    in >> n;

    for (int i = 0; i < n; i++)
    {
        int temp = (a + b) % 10;
        a = b;
        b = temp;
    }

    out << a;
    in.close();
    out.close();
}

//блок №2( баллов)


//(В худшем: O(n^2), В лучшем: O(n),если уже отсортирован).

void bubble_sort()
{
    std::ifstream in("input.txt");
    std::ofstream out("output.txt");

    int n;
    std::vector<int> arr;

    while (in >> n)
    {
        arr.push_back(n);
    }

    for (int i = 0; i < arr.size() - 1; i++)
    {
        bool swapped = false;

        for (int j = 0; j < arr.size() - i - 1; j++)
        {
            if (arr[j] > arr[j + 1])
            {
                std::swap(arr[j], arr[j + 1]);
                swapped = true;
            }
        }

        if (!swapped) break;
    }

    for (auto i : arr)
        out << i << " ";


    in.close();
    out.close();
}

//(В худшем: O(n^2), В лучшем: O(n),если уже отсортирован).

void insertion_sort() {
    std::ifstream in("input.txt");
    std::ofstream out("output.txt");

    int n;
    std::vector<int> arr;

    while (in >> n)
    {
        arr.push_back(n);
    }

    for (int i = 1; i < arr.size();i++)
    {
        int key = arr[i];
        int j = i - 1;

        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }

        arr[j + 1] = key;
 
    }

    for (auto i : arr)
        out << i << " ";


    in.close();
    out.close();
}

//блок №3( 2 баллов)

int rec_bin(int first, int last, int key, const std::vector<int>& comp) {

    if (first > last) {
        return -1;
    }

    int mid = first + (last - first) / 2;

    if (key == comp[mid])
    {
        return mid;
    }
    else if (key < comp[mid])
    {
        return rec_bin(first,mid-1,key, comp);
    }
    else {
        return rec_bin(mid+1, last,key, comp);
    }

}

void binary_search()
{
    std::ifstream in("input.txt");
    std::ofstream out("output.txt");

    std::vector<int> v1, v2, output;
    std::string line;

    if (std::getline(in, line)) {
        std::stringstream iss(line);
        int num;
        while (iss >> num) {
            v1.push_back(num);
        }
    }

    if (std::getline(in, line)) {
        std::stringstream iss(line);
        int num;
        while (iss >> num) {
            v2.push_back(num);
        }
    }

    for (auto elem2 : v2) {
        int toAdd = rec_bin(0, v1.size() - 1, elem2, v1);
        output.push_back(toAdd);
    }
    

    for (auto elem:output)
    {
        out << elem << " ";
    }


    in.close();
    out.close();
}

int main()
{
    //MEASURE_TIME_MEMORY(sum);
    //MEASURE_TIME_MEMORY(fib);
    MEASURE_TIME_MEMORY(last_fib);
    //MEASURE_TIME_MEMORY(bubble_sort);
    //MEASURE_TIME_MEMORY(insertion_sort);
    //MEASURE_TIME_MEMORY(binary_search);
}