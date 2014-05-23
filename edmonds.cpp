#include "edmonds.hpp"

#include <iostream>
#include <list>
#include <cstring>
using namespace std;
const int MAX_LEFT = 500;
const int MAX_RIGHT = 500;
class Bipartite {
private:
    struct Edge {
        int to;
        Edge* next;
        Edge(int _to) {
            to = _to;
        }
    };
    Edge* m_adjList[MAX_LEFT];
    int m_lCnt;
    int m_rCnt;
    int m_lMatchR[MAX_RIGHT];
    int m_rMatchL[MAX_LEFT];
    int m_preL[MAX_LEFT];
    bool m_visitR[MAX_RIGHT];
    //This matrix is just used to prevent adding two repeated edges.
    bool m_matrix[MAX_LEFT][MAX_RIGHT];
    void clear() {
        for (int i = 0; i < m_lCnt; i++) {
            Edge* e = m_adjList[i];
            while (e != NULL) {
                Edge* pre = e;
                e = e->next;
                delete pre;
            }
            m_adjList[i] = NULL;
        }
        memset(m_matrix, 0, sizeof(m_matrix));
    }
    void findAugment(int start) {
        for (int i = 0; i < m_lCnt; i++) {
            m_preL[i] = -1;
        }
        memset(m_visitR, 0, sizeof(bool) * m_rCnt);
        list<int> que;
        que.push_back(start);
        bool found = false;
        while (!que.empty() && !found) {
            int from = que.front();
            que.pop_front();
            Edge* edge = m_adjList[from];
            while (edge != NULL && !found) {
                int to = edge->to;
                if (!m_visitR[to]) {
                    m_visitR[to] = true;
                    if (m_rMatchL[to] == -1) {
                        found = true;
                        reverse(from, to);
                    }
                    else {
                        que.push_back(m_rMatchL[to]);
                        m_preL[m_rMatchL[to]] = from;
                    }
                }
                edge = edge->next;
            }
        }
    }
    void reverse(int left, int right) {
        m_rMatchL[right] = left;
        while(m_preL[left] != -1) {
            int nextR = m_lMatchR[left];
            m_rMatchL[nextR] = m_preL[left];
            m_lMatchR[left] = right;
            left = m_preL[left];
            right = nextR;
        }
        m_lMatchR[left] = right;
    }
public:
    Bipartite() {
        memset(m_adjList, 0, sizeof(m_adjList));
        m_lCnt = 0;
        m_rCnt = 0;
    }
    ~Bipartite() {
        clear();
    }
    //Add an edge between vertex "left" and "right" while "left" and "right" are
    //the indices of two vertices in the left/right parts of the graph. Indices
    //in the left and right parts are separated and they both begin from 0.
    void addEdge(int left, int right) {
        if (!m_matrix[left][right]) {
            m_matrix[left][right] = true;
            Edge* newEdge = new Edge(right);
            newEdge->next = m_adjList[left]; 
            m_adjList[left] = newEdge;
        }
    }
    //Before invoking this function, "maxMatch()" must be invoked. This function
    //returns the index of the matching vertex of "left" while "left" is the
    //index of a vertex in the left part of the graphic.
    int getLMatchR(int left) const {
        return m_lMatchR[left];
    }
    //See "getLMatchR()", and this function is opposite to it.
    int getRMatchL(int right) const {
        return m_rMatchL[right];
    }
    void init(int leftCnt, int rightCnt) {
        clear();
        m_lCnt = leftCnt;
        m_rCnt = rightCnt;
        for (int i = 0; i < m_lCnt; i++) {
            m_lMatchR[i] = -1;
        }
        for (int i = 0; i < m_rCnt; i++) {
            m_rMatchL[i] = -1;
        }
    }
    int maxMatch() {
        for (int i = 0; i < m_lCnt; i++) {
            findAugment(i);
        }
        int result = 0;
        for (int i = 0; i < m_lCnt; i++) {
            if (m_lMatchR[i] != -1) {
                result++;
            }
        }
        return result;
    }
};

int test() {
    Bipartite match;
    match.init(300, 400);
    int a[] = {0, 0, 1, 1, 2, 2, 2};
    int b[] = {1, 2, 1, 3, 0, 1, 2};
    for (int i = 0; i < 7; i++) {
        match.addEdge(a[i], b[i]);
    }
    int maxMatch = match.maxMatch();
    cout << maxMatch << " ";
    for (int i = 0; i < 3; i++) {
        cout << match.getLMatchR(i) << " ";
    }
    for (int i = 0; i < 4; i++) {
        cout << match.getRMatchL(i) << " ";
    }
    cout << endl;//Correct: 3 2 3 1 -1 2 0 1
    return 0;
}