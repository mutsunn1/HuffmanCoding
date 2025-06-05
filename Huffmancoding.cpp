#include <iostream>
#include <string>
#include <cstring>
#include <climits>
#include <fstream>
using namespace std;

typedef struct HTNode {
    int data;
    int weight;
    int parent, lchild, rchild;
} HTNode, *HuffmanTree;

void Select(HuffmanTree HT, int m, int &s1, int &s2) // 选择两个权值最小的节点
{
    s1 = s2 = -1;
    int min1 = MAX, min2 = MAX;
    for (int i = 1; i <= m; ++i)
    {
        if (HT[i].parent == 0)
        {
            if (HT[i].weight < min1)
            {
                min2 = min1;
                s2 = s1;
                min1 = HT[i].weight;
                s1 = i;
            }
            else if (HT[i].weight < min2 && HT[i].weight != min1)
            {
                min2 = HT[i].weight;
                s2 = i;
            }
        }
    }
}
void CreateHuffmanTree(HuffmanTree &HT, int n) // 创建哈夫曼树
{
    if (n <= 1) return;
    int m = 2 * n - 1;
    int s1, s2;
    HT = new HTNode[m + 1]; // 动态分配数组空间
    for (int i = 1; i <= m; ++i)
    {
        HT[i].data = i;
        HT[i].parent = HT[i].lchild = HT[i].rchild = 0;
    }
    for (int i = 1; i <= n; ++i)
    {
        HT[i].weight = ch[i - 1];
    }
    cout << "初始树结构为：" << endl;
    cout << "数据" << "权值" << "父结点" << "左孩子" << "右孩子" << endl;
    for (int i = 1; i <= n; ++i)
    {
        cout << HT[i].data << " " << HT[i].weight << " " << HT[i].parent << " " << HT[i].lchild << " " << HT[i].rchild << endl;
    }
    for (int i = n + 1; i <= m; ++i)
    {
        Select(HT, i - 1, s1, s2);
        HT[s1].parent = HT[s2].parent = i;
        HT[i].lchild = s1;
        HT[i].rchild = s2;
        HT[i].weight = HT[s1].weight + HT[s2].weight;
        HT[i].data = i;
    }
}