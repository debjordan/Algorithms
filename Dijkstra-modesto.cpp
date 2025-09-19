#include <iostream>
#include <vector>
#include <queue>
#include <climits>
using namespace std;

/**
 * Algoritmo de Dijkstra
 *
 * Objetivo:
 *   Encontrar a menor distância de um vértice de origem
 *   até todos os outros em um grafo ponderado.
 *
 * Complexidade:
 *   - Tempo: O(E log V), usando fila de prioridade
 *   - Espaço: O(V) para distâncias
 */

typedef pair<int, int> pii; // (distância, vértice)

void dijkstra(int V, vector<vector<pii>>& adj, int src) {
    vector<int> dist(V, INT_MAX);
    dist[src] = 0;

    priority_queue<pii, vector<pii>, greater<pii>> pq;
    pq.push({0, src});

    while (!pq.empty()) {
        int d = pq.top().first;
        int u = pq.top().second;
        pq.pop();

        if (d > dist[u]) continue;

        for (auto& edge : adj[u]) {
            int v = edge.first;
            int weight = edge.second;

            if (dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                pq.push({dist[v], v});
            }
        }
    }

    cout << "Distâncias mínimas a partir do vértice " << src << ":\n";
    for (int i = 0; i < V; i++)
        cout << "Para " << i << " -> " << dist[i] << endl;
}

int main() {
    int V = 5;
    vector<vector<pii>> adj(V);

    // Grafo 
    adj[0].push_back({1, 9});
    adj[0].push_back({2, 6});
    adj[0].push_back({3, 5});
    adj[0].push_back({4, 3});
    adj[2].push_back({1, 2});
    adj[2].push_back({3, 4});

    dijkstra(V, adj, 0);

    return 0;
}
