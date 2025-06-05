#include <iostream>
#include <string>
#include <cstring>
#include <climits>
#include <fstream>
using namespace std;

typedef struct HTNode {
    int weight;
    int parent, lchild, rchild;
} HTNode, *HuffmanTree;