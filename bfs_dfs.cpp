
#include <iostream>
#include <chrono>
#include <vector>
#include <stack>
#include <queue>
#include <omp.h>

using namespace std;

void seq_dfs(vector<vector<int>>& adjList, vector<bool>& visited, int src) {
    cout << src << " ";
    visited[src] = true;
    for (int i = 0; i < adjList[src].size(); i++) {
        int adj = adjList[src][i];
        if (!visited[adj])
            seq_dfs(adjList, visited, adj);
    }
}

void parallel_dfs(vector<vector<int>>& adjList, vector<bool>& visited, int src) {
    stack<int> st;
    st.push(src);
    visited[src] = true;

    while (!st.empty()) {
        int curr;
        #pragma omp critical
        // only one thread at a time accesses the shared stack.
        {
            curr = st.top();
            st.pop();
        }
        cout << curr << " ";

        #pragma omp parallel for
        for (int i = 0; i < adjList[curr].size(); i++) {
            int adj = adjList[curr][i];
            #pragma omp critical
            {
                if (!visited[adj]) {
                    st.push(adj);
                    visited[adj] = true;
                }
            }
        }
    }
}

void seq_bfs(vector<vector<int>>& adjList, vector<bool>& visited, int src) {
    queue<int> que;
    que.push(src);
    visited[src] = true;

    while (!que.empty()) {
        int curr = que.front();
        que.pop();
        cout << curr << " ";

        for (int i = 0; i < adjList[curr].size(); i++) {
            int adj = adjList[curr][i];
            if (!visited[adj]) {
                que.push(adj);
                visited[adj] = true;
            }
        }
    }
}

void parallel_bfs(vector<vector<int>>& adjList, vector<bool>& visited, int src) {
    queue<int> que;
    que.push(src);
    visited[src] = true;

    while (!que.empty()) {
        int curr = que.front();
        que.pop();
        cout << curr << " ";

        #pragma omp parallel for
        for (int i = 0; i < adjList[curr].size(); i++) {
            int adj = adjList[curr][i];
            #pragma omp critical
            {
                if (!visited[adj]) {
                    que.push(adj);
                    visited[adj] = true;
                }
            }
        }
    }
}

void clear(vector<bool>& visited) {
    for (int i = 0; i < visited.size(); i++)
        visited[i] = false;
}

int main() {
    vector<vector<int>> adjList = {
        {1,3,4},
        {2},
        { },
        {2},
        {5},
        {3}
    };

    vector<bool> visited(adjList.size(), false);
    int src = 0;

    visited[src] = true;
    auto start = chrono::high_resolution_clock::now();
    seq_dfs(adjList, visited, src);
    auto stop = chrono::high_resolution_clock::now();
    cout << "Time taken for sequential dfs = " << chrono::duration_cast <chrono::microseconds>(stop - start).count() << " microseconds." << endl;
    clear(visited);

    visited[src] = true;
    start = chrono::high_resolution_clock::now();
    parallel_dfs(adjList, visited, src);
    stop = chrono::high_resolution_clock::now();
    cout << "Time taken for parallel dfs = " << chrono::duration_cast <chrono::microseconds>(stop - start).count() << " microseconds." << endl;
    clear(visited);

    visited[src] = true;
    start = chrono::high_resolution_clock::now();
    seq_bfs(adjList, visited, src);
    stop = chrono::high_resolution_clock::now();
    cout << "Time taken for sequential bfs = " << chrono::duration_cast <chrono::microseconds>(stop - start).count() << " microseconds." << endl;
    clear(visited);

    visited[src] = true;
    start = chrono::high_resolution_clock::now();
    parallel_bfs(adjList, visited, src);
    stop = chrono::high_resolution_clock::now();
    cout << "Time taken for parallel bfs = " << chrono::duration_cast <chrono::microseconds>(stop - start).count() << " microseconds." << endl;
    clear(visited);

    return 0;
}
