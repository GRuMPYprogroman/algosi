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

void seq(std::vector<int>& a)
{
    int sum = std::accumulate(a.begin(),a.end(),0);
    
    if(sum%2 != 0)
    {
        std::cout << "-1\n\n";
        return;
    }

    int n = a.size();
    int target = sum / 2;

    std::vector<std::vector<bool>> dp(n + 1, std::vector<bool>(target + 1, false));
    dp[0][0] = true;

    for (int i = 0; i < n; i++) {
        for (int s = 0; s <= target; s++) {
            dp[i + 1][s] = dp[i][s];
            if (s >= a[i]) {
                dp[i + 1][s] = dp[i + 1][s] || dp[i][s - a[i]];
            }
        }
    }

    if (!dp[n][target]) {
        std::cout << "-1\n";
        return;
    }

    std::vector<int> first_part; 
    std::vector<int> second_part; 
    int s = target;
    for (int i = n - 1; i >= 0; i--) {
        if (s >= a[i]) {
            first_part.push_back(a[i]);
            s -= a[i];
        }
        else {
            second_part.push_back(a[i]);
        }
    }

    for (int i = first_part.size() - 1; i >= 0; --i) {
        std::cout << first_part[i] << " ";
    }
    std::cout << "\n\n";
}

bool isOpening(char c) {
    return c == '(' || c == '[' || c == '{';
}


char getPair(char c) {
    if (c == '(') return ')';
    if (c == '[') return ']';
    if (c == '{') return '}';
    return c; // ƒл€ закрывающих скобок возвращаем их же
}

void brackets_delete(const std::string& s) {


    int n = s.length();
    std::stack<char> st;
    std::vector<int> remove(n, 0); // 0 Ч оставить, 1 Ч удалить

    for (int i = 0; i < n; i++) {
        if (isOpening(s[i])) {
            st.push(s[i]);
        }
        else {
            if (st.empty() || getPair(st.top()) != s[i]) {
                remove[i] = 1; 
            }
            else {
                st.pop(); 
            }
        }
    }

   
    while (!st.empty()) {
        int i = 0;
        while (i < n && s[i] != st.top()) i++;
        remove[i] = 1; 
        st.pop();
    }

    
    std::string result = "";
    for (int i = 0; i < n; i++) {
        if (!remove[i]) {
            result += s[i];
        }
    }

    std::cout << result << "\n\n";
}

//блок є2

struct Node {
    int left, right;
    int key;
    Node(int k = -1) : key(k), left(-1), right(-1) {}
};

void inOrder(int root, const std::vector<Node>& tree, std::vector<int>& result) {
    std::stack<int> st;
    int current = root;

    while (current != -1 || !st.empty()) {
        while (current != -1) {
            st.push(current);
            current = tree[current].left;
        }

        current = st.top();
        st.pop();

        result.push_back(tree[current].key);

        current = tree[current].right;
    }
}

void preOrder(int root, const std::vector<Node>& tree, std::vector<int>& result) {
    std::stack<int> st;
    st.push(root);

    while (!st.empty()) {
        int current = st.top();
        st.pop();

        if (current == -1) continue;

        result.push_back(tree[current].key);

        st.push(tree[current].right);
        st.push(tree[current].left);
    }
}

void postOrder(int root, const std::vector<Node>& tree, std::vector<int>& result) {
    std::stack<int> st1, st2;
    st1.push(root);

    while (!st1.empty()) {
        int current = st1.top();
        st1.pop();

        if (current == -1) continue;

        st2.push(current);

        st1.push(tree[current].left);
        st1.push(tree[current].right);
    }

    while (!st2.empty()) {
        result.push_back(tree[st2.top()].key);
        st2.pop();
    }
}

int insertNode(std::vector<Node>& tree, int& nextIndex, int key) {
    tree.push_back(Node(key));
    int current = nextIndex;
    nextIndex++;
    return current;
}

int findSuccessor(std::vector<Node>& tree, int root, int x) {
    int successor = 0; 
    int current = root;

    while (current != -1) {
        if (tree[current].key > x) {
            successor = tree[current].key;
            current = tree[current].left;  
        }
        else {
            current = tree[current].right; 
        }
    }

    return successor;
}

int insertBST(std::vector<Node>& tree, int& nextIndex, int key, int root) {
    if (root == -1) {
        return insertNode(tree, nextIndex, key);
    }

    if (key == tree[root].key) {
        return root;
    }

    if (key < tree[root].key) {
        tree[root].left = insertBST(tree, nextIndex, key, tree[root].left);
    }
    else {
        tree[root].right = insertBST(tree, nextIndex, key, tree[root].right);
    }
    return root;
}

bool existsBST(std::vector<Node>& tree, int root, int key) {
    int current = root;
    while (current != -1) {
        if (key == tree[current].key) {
            return true;
        }
        if (key < tree[current].key) {
            current = tree[current].left;
        }
        else {
            current = tree[current].right;
        }
    }
    return false;
}

int findNext(std::vector<Node>& tree, int root, int x) {
    int next = -1;
    int current = root;

    while (current != -1) {
        if (tree[current].key > x) {
            next = tree[current].key; 
            current = tree[current].left; 
        }
        else {
            current = tree[current].right;
        }
    }

    return next;
}

int findPrev(std::vector<Node>& tree, int root, int x) {
    int prev = -1; 
    int current = root;

    while (current != -1) {
        if (tree[current].key < x) {
            prev = tree[current].key; 
            current = tree[current].right; 
        }
        else {
            current = tree[current].left; 
        }
    }

    return prev;
}

int findMin(std::vector<Node>& tree, int root) {
    if (root == -1) return -1;
    int current = root;
    while (tree[current].left != -1) {
        current = tree[current].left;
    }
    return tree[current].key;
}

int deleteBST(std::vector<Node>& tree, int& nextIndex, int key, int root) {
    if (root == -1) {
        return -1; 
    }

    if (key < tree[root].key) {
        tree[root].left = deleteBST(tree, nextIndex, key, tree[root].left);
    }
    else if (key > tree[root].key) {
        tree[root].right = deleteBST(tree, nextIndex, key, tree[root].right);
    }
    else {
        if (tree[root].left == -1) { 
            int rightChild = tree[root].right;
            if (rightChild != -1) {
                tree[root] = tree[rightChild];
            }
            else {
                tree[root] = Node();
            }
            return rightChild;
        }
        else if (tree[root].right == -1) { 
            int leftChild = tree[root].left;
            if (leftChild != -1) {
                tree[root] = tree[leftChild]; 
            }
            else {
                tree[root] = Node();
            }
            return leftChild;
        }
        else {
            int minRight = findMin(tree, tree[root].right);
            tree[root].key = minRight;
            tree[root].right = deleteBST(tree, nextIndex, minRight, tree[root].right);
        }
    }
    return root;
}

void process_BTS()
{

    std::ifstream fin("input.txt");
    std::vector<Node> tree;
    int nextIndex = 0;
    int root = -1;

    std::string query;
    while (getline(fin, query))
    {
        if (query.empty()) continue;

        if (query[0] == '+')
        {
            int x = stoi(query.substr(1));
            root = insertBST(tree, nextIndex, x, root);
        }
        else
        {
            int x = stoi(query.substr(1));
            int successor = findSuccessor(tree, root, x);
            std::cout << successor << std::endl;
        }
    }
}

void process_BTS_two()
{
    std::ifstream fin("input.txt");
    std::vector<Node> tree; 
    int nextIndex = 0;
    int root = -1;

    std::string query;
    while (getline(fin, query)) {
        if (query.empty()) continue; 
            if (query.substr(0, 6) == "insert") {
                int x = stoi(query.substr(7)); 
                root = insertBST(tree, nextIndex, x, root);
                std::cout << "inserted: " << x << std::endl;
            }
            else if (query.substr(0, 6) == "exists") {
                int x = stoi(query.substr(7));
                bool exists = existsBST(tree, root, x);
                std::cout << (exists ? "true" : "false") << std::endl;
            }
            else if (query.substr(0, 4) == "next") {
                int x = stoi(query.substr(5));
                int next = findNext(tree, root, x);
                std::cout << (next != -1 ? std::to_string(next) : "none") << std::endl;
            }
            else if (query.substr(0, 4) == "prev") {
                int x = stoi(query.substr(5));
                int prev = findPrev(tree, root, x);
                std::cout << (prev != -1 ? std::to_string(prev) : "none") << std::endl;
            }
            else if (query.substr(0, 6) == "delete") {
                int x = stoi(query.substr(7));
                root = deleteBST(tree, nextIndex, x, root);
                std::cout << "deleted: " << x << std::endl;
            }
        }
    fin.close();
}

int main()
{
    /**
    std::vector<int> a;

    int input;
    while (std::cin >> input)
        a.push_back(input);

    MEASURE_TIME_MEMORY(seq, a);**/

    /**std::string a;

    std::cin >> a;

    MEASURE_TIME_MEMORY(brackets_delete,a);**/

    /**std::ifstream fin("input.txt");
    if (!fin.is_open()) {
        std::cout << "popopo." << std::endl;
        return 1;
    }

    int n; 
    fin >> n;

    std::vector<Node> tree(n);

    for (int i = 0; i < n; i++) {
        fin >> tree[i].key >> tree[i].left >> tree[i].right;
    }
    fin.close();

    std::vector<int> inOrderResult, preOrderResult, postOrderResult;

    MEASURE_TIME_MEMORY(inOrder, 0, tree, inOrderResult);
    MEASURE_TIME_MEMORY(preOrder, 0, tree, preOrderResult);
    MEASURE_TIME_MEMORY(postOrder, 0, tree, postOrderResult);

    for (size_t i = 0; i < inOrderResult.size(); i++) {
        std::cout << inOrderResult[i];
        if (i < inOrderResult.size() - 1) std::cout << " ";
    }
    std::cout << std::endl;

    for (size_t i = 0; i < preOrderResult.size(); i++) {
        std::cout << preOrderResult[i];
        if (i < preOrderResult.size() - 1) std::cout << " ";
    }
    std::cout << std::endl;

    for (size_t i = 0; i < postOrderResult.size(); i++) {
        std::cout << postOrderResult[i];
        if (i < postOrderResult.size() - 1) std::cout << " ";
    }
    std::cout << std::endl;**/
    
    //MEASURE_TIME_MEMORY(process_BTS);
    MEASURE_TIME_MEMORY(process_BTS_two);

}
