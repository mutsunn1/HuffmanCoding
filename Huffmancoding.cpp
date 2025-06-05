#include <iostream>
#include <string>
#include <cstring>
#include <climits>
#include <fstream>
#define MAX 100
using namespace std;

typedef struct HTNode {
    int weight;
    int parent, lchild, rchild;
} HTNode, *HuffmanTree;

int ch[MAX]; // 权值数组(字符频率)

// 选择两个权值最小且无父节点的节点，s1、s2 带回下标
void Select(HuffmanTree HT, int m, int &s1, int &s2) {
    s1 = s2 = -1;
    int min1 = INT_MAX, min2 = INT_MAX;
    for (int i = 1; i <= m; ++i) {
        if (HT[i].parent == 0) {  // 只考虑无父节点的节点
            if (HT[i].weight < min1) {
                min2 = min1;
                s2 = s1;
                min1 = HT[i].weight;
                s1 = i;
            } else if (HT[i].weight < min2) { 
                min2 = HT[i].weight;
                s2 = i;
            }
        }
    }
}

// 创建哈夫曼树，n 为字符数量（权值数量）
void CreateHuffmanTree(HuffmanTree &HT, int n) {
    if (n <= 1) return;
    int m = 2 * n - 1;  // 哈夫曼树总节点数
    HT = new HTNode[m + 1];  // 下标从 1 开始

    // 初始化所有节点
    for (int i = 1; i <= m; ++i) {
        HT[i].parent = HT[i].lchild = HT[i].rchild = 0;
    }
    // 录入前 n 个叶子节点的权值（字符频率）
    for (int i = 1; i <= n; ++i) {
        HT[i].weight = ch[i - 1]; 
    }

    // 构建哈夫曼树（合并非叶子节点）
    for (int i = n + 1; i <= m; ++i) {
        int s1, s2;
        Select(HT, i - 1, s1, s2); // 选两个最小权值节点
        HT[s1].parent = HT[s2].parent = i;
        HT[i].lchild = s1;
        HT[i].rchild = s2;
        HT[i].weight = HT[s1].weight + HT[s2].weight;
    }
}

int main(){
    HuffmanTree HT;
    CreateHuffmanTree(HT, 5);
    return 0;
}