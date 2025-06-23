#include <iostream>
#include <string>
#include <cstring>
#include <climits>
#include <fstream>
#include <limits>
#define MAX 100
using namespace std;

typedef struct HTNode {
	char data;
    int weight;
    int parent, lchild, rchild;
} HTNode, *HuffmanTree;
string code[MAX]; // 存储哈夫曼编码
char chars[MAX];       // 字符数组
int freq[MAX];         // 频率数组
int charCount = 0;     // 实际字符数量



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
     // 使用传入的频率数据
    for (int i = 1; i <= n; ++i) {
        HT[i].data = chars[i-1];  // 添加字符数据
        HT[i].weight = ch[i-1];   // 使用传入的频率
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
void PrintHuffmanTree(HuffmanTree HT, int n) // 打印哈夫曼树
{
    for (int i = 1; i <= n; ++i)
    {
        cout << HT[i].data << " " << HT[i].weight << " " << HT[i].parent << " " << HT[i].lchild << " " << HT[i].rchild << endl;
    }
}
// 生成哈夫曼编码
void GenerateHuffmanCode(HuffmanTree HT, int n) {
  
    for (int i = 1; i <= n; ++i) {
        int current = i;
        string temp = "";
        while (HT[current].parent != 0) {
            if (HT[HT[current].parent].lchild == current) {
                temp = "0" + temp; // 左子树为0
            } else {
                temp = "1" + temp; // 右子树为1
            }
            current = HT[current].parent; // 回溯到父节点
        }
        code[i - 1] = temp; // 存储编码
    }

    // 输出哈夫曼编码
    for (int i = 0; i < n; ++i) {
        cout << "字符: " << HT[i + 1].data << " 编码: " << code[i] << endl;
    }
}
int main(){
    HuffmanTree HT;

    int choice;
    cout << "\n===== 哈夫曼编码与译码系统 =====\n";
    cout << "1. 读取文件并统计字符频率\n";
    cout << "2. 构造哈夫曼树并输出\n";
    cout << "3. 生成哈夫曼编码并输出\n";
    cout << "4. 对报文进行编码\n";
    cout << "5. 接收报文并译码\n";
    cout << "6. 退出\n";
    cout << "请选择操作(1-6): ";
    cin >> choice;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');  // 清除输入缓冲区
    switch (choice)
    {
    case 1:

        cout << "文件读取完成！字符频率统计如下：\n";

        break;
    case 2:
          CreateHuffmanTree(HT, 5);
        cout << "哈夫曼树构造完成！树结构如下：\n";
		cout << "数据 权值 父结点 左孩子 右孩子\n";
		  PrintHuffmanTree(HT, 5);
        break;
      
    case 3:
    
        cout << "哈夫曼编码如下：\n";
     
        break;
    case 4:
        cout << "请输入要编码的报文: ";
        break;
    case 5:
        break;
    case 6:
        cout << "退出程序。\n";
        break;
    default:
        cout << "无效的选择，请重新输入！\n";
    }
    return 0;
}
