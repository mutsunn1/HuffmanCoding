#include <iostream>
#include <string>
#include <cstring>
#include <climits>
#include <fstream>
#include <limits>
#define MAX 100
using namespace std;

typedef struct HTNode
{
    char data;
    int weight;
    int parent, lchild, rchild;
} HTNode, *HuffmanTree;
string code[MAX];  // 存储哈夫曼编码
char chars[MAX];   // 字符数组
int freq[MAX];     // 频率数组
int charCount = 0; // 实际字符数量



// 转换为小写字符
char toLowerChar(char c)
{
    return (c >= 'A' && c <= 'Z') ? c + 32 : c;
}
// 初始化字符数组：0-9、a-z
void InitCharList()
{
    for (int i = 0; i < 10; ++i)
    {
        chars[i] = '0' + i;
    }
    for (int i = 10; i < 36; ++i)
    {
        chars[i] = 'a' + (i - 10);
    }
    charCount = 36;
}
// 读取文件内容
/**
 * @brief 读取文本文件内容到字符串中。
 *
 * 打开指定文件，逐行读取内容，并将每行（包括换行符）追加到输出字符串中。
 *
 * @param filename 要读取的文本文件路径。
 * @param content 用于存储文件内容的字符串引用。
 * @return 如果文件成功打开并读取返回 true，否则返回 false。
 */
bool ReadTextFromFile(const string &filename, string &content)
{
    ifstream file(filename);
    if (!file.is_open())
        return false;

    string line;
    while (getline(file, line))
    {
        content += line + "\n"; // 保留换行符
    }
    file.close();
    return true;
}
// 统计字符频率
/**
 * @brief 统计指定文件中各字符的出现频率。
 *
 * 该函数会打开指定的文件，读取其中的每个字符，并统计数字（'0'-'9'）、小写字母（'a'-'z'）以及其他字符的出现频率。
 * 统计结果会输出到标准输出流。字符频率信息存储在全局数组 freq 和 chars 中。
 *
 * @param filename 要统计字符频率的文件名。
 * @return 如果文件成功打开并统计完成，返回 true；否则返回 false。
 */
bool CountCharacterFrequency(const string &filename)
{
    ifstream file(filename);
    if (!file.is_open())
    {
        cerr << "无法打开文件: " << filename << endl;
        return false;
    }

    // 初始化字符数组
    InitCharList();

    // 使用循环初始化频率数组
    for (int i = 0; i < MAX; ++i)
    {
        freq[i] = 0;
    }

    char c;
    while (file.get(c))
    {
        c = toLowerChar(c); // 统一转为小写

        if (c >= '0' && c <= '9')
        {
            int index = c - '0';
            freq[index]++;
        }
        else if (c >= 'a' && c <= 'z')
        {
            int index = 10 + (c - 'a');
            freq[index]++;
        }
        else
        {
            bool found = false;
            for (int i = 36; i < charCount; ++i)
            {
                if (chars[i] == c)
                {
                    freq[i]++;
                    found = true;
                    break;
                }
            }
            if (!found && charCount < MAX)
            {
                chars[charCount] = c;
                freq[charCount] = 1;
                charCount++;
            }
        }
    }
    file.close();

    // 输出频率统计结果
    cout << "字符频率统计结果：\n";
    for (int i = 0; i < charCount; ++i)
    {
        if (freq[i] > 0)
        {
            cout << chars[i] << " : " << freq[i] << endl;
        }
    }
    return true;
}
// 选择两个权值最小且无父节点的节点，s1、s2 带回下标
/**
 * @brief 从哈夫曼树中选择两个权值最小且尚未被选中的节点。
 *
 * 本函数在哈夫曼树数组 HT 的前 m 个元素中，查找父节点为 0（即尚未被合并）的两个最小权值节点，
 * 并通过引用参数 s1 和 s2 返回它们的下标。
 *
 * @param HT 哈夫曼树数组，每个元素代表一个节点，至少包含 'weight' 和 'parent' 字段。
 * @param m 需要参与选择的节点数量（通常为当前树中的节点总数）。
 * @param s1 用于返回权值最小节点的下标的引用参数。
 * @param s2 用于返回权值次小节点的下标的引用参数。
 */
void Select(HuffmanTree HT, int m, int &s1, int &s2)
{
    s1 = s2 = -1;
    int min1 = INT_MAX, min2 = INT_MAX;

    for (int i = 1; i <= m; ++i)
    {
        if (HT[i].parent == 0)
        { // 未被选中的节点
            if (HT[i].weight < min1)
            {
                min2 = min1;
                s2 = s1;
                min1 = HT[i].weight;
                s1 = i;
            }
            else if (HT[i].weight < min2)
            {
                min2 = HT[i].weight;
                s2 = i;
            }
        }
    }
}

// 创建哈夫曼树，n 为字符数量（权值数量）
/**
 * @brief 构建哈夫曼树（Huffman Tree）。
 *
 * 根据给定的字符频率数组，初始化并构建哈夫曼树。哈夫曼树用于最优前缀编码。
 * 
 * @param HT 引用类型，指向哈夫曼树节点数组的指针，函数内部分配空间并初始化。
 * @param n  叶子节点的数量（即字符的数量）。
 *
 * @note
 * - HT 的下标从 1 开始。
 * - chars 和 freq 应为全局或外部可访问的数组，分别存储字符和对应频率。
 * - 构建过程中，先初始化所有节点，再根据频率填充叶子节点，最后合并生成非叶子节点。
 * - 需要辅助函数 Select 用于选择权值最小的两个节点。
 */
void CreateHuffmanTree(HuffmanTree &HT, int n)
{
    if (n <= 1)
        return;
    int m = 2 * n - 1;      // 哈夫曼树总节点数
    HT = new HTNode[m + 1]; // 下标从 1 开始

    // 初始化所有节点
    for (int i = 1; i <= m; ++i)
    {
        HT[i].parent = HT[i].lchild = HT[i].rchild = 0;
        HT[i].weight = 0;
        HT[i].data = '\0';
    }
    // 使用传入的频率数据
    for (int i = 1; i <= n; ++i)
    {
        HT[i].data = chars[i - 1];  // 添加字符数据
        HT[i].weight = freq[i - 1]; // 使用传入的频率
    }

    // 构建哈夫曼树（合并非叶子节点）
    for (int i = n + 1; i <= m; ++i)
    {
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
    cout << "节点\t字符\t权值\t父节点\t左孩子\t右孩子" << endl;
    for (int i = 1; i <= n; ++i)
    {
        cout << i << "\t";
        if (HT[i].data == '\0')
            cout << "N/A";
        else
            cout << HT[i].data;
        cout << "\t" << HT[i].weight << "\t"
             << HT[i].parent << "\t"
             << HT[i].lchild << "\t"
             << HT[i].rchild << endl;
    }
}
// 生成哈夫曼编码
/**
 * @brief 为哈夫曼树的每个叶子节点生成哈夫曼编码。
 *
 * 本函数从每个叶子节点向上遍历到根节点，根据当前节点是左孩子还是右孩子，
 * 分别在编码前添加'0'或'1'，最终生成每个字符对应的哈夫曼编码，并存储到全局数组 code 中。
 *
 * @param HT 哈夫曼树节点数组，每个节点包含至少 parent 和 lchild 字段。
 * @param n  哈夫曼树的叶子节点数量（即字符数量）。
 */
void GenerateHuffmanCode(HuffmanTree HT, int n)
{
    for (int i = 1; i <= n; ++i)
    {
        int current = i;
        string temp = "";
        while (HT[current].parent != 0)
        {
            temp = (HT[HT[current].parent].lchild == current ? "0" : "1") + temp;
            current = HT[current].parent;
        }
        code[i - 1] = temp;
    }
}

/**
 * @brief 使用给定的哈夫曼树对输入报文进行编码，并将编码结果写入文件。
 *
 * 本函数利用已生成的哈夫曼编码（存储于全局 code 数组），对输入字符串 input 进行编码。
 * 编码后的比特串存储在 encodedMessage 中，并写入指定的输出文件 outputFilename。
 * 如果输入中有字符未在哈夫曼树中出现，则会给出警告并忽略该字符。
 *
 * @param HT             哈夫曼树数组，每个节点包含字符数据及其对应编码。
 * @param n              哈夫曼树的叶子节点数量（唯一字符数）。
 * @param input          待编码的输入报文。
 * @param encodedMessage 用于存储编码结果的字符串引用。
 * @param outputFilename 编码结果输出文件名。
 *
 * @note 本函数假设哈夫曼编码已生成并存储在全局 code 数组中，否则会提示用户先生成编码。
 * @note 输入中未出现在哈夫曼树的字符会被忽略并给出警告。
 */
void EncodeMessage(HuffmanTree HT, int n, const string &input, string &encodedMessage, const string &outputFilename)
{
    if (code[0].empty())
    {
        cout << "请先生成哈夫曼编码（选项 3）。\n";
        return;
    }

    encodedMessage.clear();

    for (char c : input)
    {
        bool found = false;
        char lowerC = tolower(c); // 统一转为小写，与频率统计一致
        for (int i = 0; i < n; ++i)
        {
            if (HT[i + 1].data == lowerC)
            {
                encodedMessage += code[i];
                found = true;
                break;
            }
        }
        if (!found)
        {
            cout << "警告：字符 '" << c << "' 不在哈夫曼编码表中，将被忽略。\n";
        }
    }

    // 输出到控制台
    cout << "编码后的报文: " << encodedMessage << endl;

    // 输出到文件
    ofstream outFile(outputFilename);
    if (!outFile.is_open())
    {
        cerr << "无法打开输出文件: " << outputFilename << endl;
        return;
    }
    outFile << encodedMessage;
    outFile.close();
    cout << "编码结果已写入文件: " << outputFilename << endl;
}
/**
 * @brief 解码哈夫曼编码的消息并将结果写入指定文件。
 *
 * 此函数根据给定的哈夫曼树和编码字符串，对编码内容进行解码，
 * 并将解码后的明文写入指定的输出文件。如果哈夫曼树未构造或编码内容为空，
 * 会输出相应的提示信息。解码过程中遇到无效编码字符会进行警告。
 *
 * @param HT 哈夫曼树数组指针，表示已构造的哈夫曼树。
 * @param n 叶子节点的数量（原始字符的数量）。
 * @param encodedMessage 需要解码的哈夫曼编码字符串（由'0'和'1'组成）。
 * @param outputFilename 解码结果输出的文件名。
 *
 * @note
 * - 若哈夫曼树未构造或编码内容为空，函数将直接返回。
 * - 若输出文件无法打开，将输出错误信息并返回。
 * - 若编码内容不完整或无效，解码结果可能不完整，并会输出警告。
 * - 解码结果既会写入文件，也会在控制台输出。
 */
void DecodeHuffman(HuffmanTree HT, int n, const string &encodedMessage, const string &outputFilename)
{
    if (HT == nullptr)
    {
        cout << "哈夫曼树未构造！请先执行选项2。\n";
        return;
    }
    if (encodedMessage.empty())
    {
        cout << "编码内容为空，无法解码。\n";
        return;
    }

    ofstream outFile(outputFilename);
    if (!outFile.is_open())
    {
        cerr << "无法打开输出文件: " << outputFilename << endl;
        return;
    }

    // 创建过滤后的编码字符串
    string filteredCode;
    for (char bit : encodedMessage)
    {
        if (bit == '0' || bit == '1')
        {
            filteredCode += bit;
        }
    }

    // 从根节点开始解码
    int root = 2 * n - 1;
    int current = root;
    string decodedMessage;

    for (char bit : filteredCode)
    {
        // 如果当前节点是叶子节点，记录字符并重置到根节点
        if (HT[current].lchild == 0 && HT[current].rchild == 0)
        {
            decodedMessage += HT[current].data;
            current = root; // 重置到根节点
        }

        // 根据编码位移动到子节点
        if (bit == '0')
        {
            current = HT[current].lchild;
        }
        else if (bit == '1')
        {
            current = HT[current].rchild;
        }
        else
        {
            cout << "无效的编码字符: " << bit << endl;
            continue;
        }
    }

    // 处理最后一个字符
    if (HT[current].lchild == 0 && HT[current].rchild == 0)
    {
        decodedMessage += HT[current].data;
    }
    else
    {
        cout << "警告：编码可能不完整或无效，解码结果可能不完整。\n";
    }

    outFile << decodedMessage;
    outFile.close();

    cout << "解码结果已写入文件: " << outputFilename << endl;
    cout << "解码后的报文: " << decodedMessage << endl;
}
int main()
{
    HuffmanTree HT;
    // 初始化字符列表
    InitCharList();
    string filename;
    string fileContent;

    while (true)
    {

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
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // 清除输入缓冲区
        switch (choice)
        {
        case 1:
        {
            cout << "请输入要读取的文件名（如 test.txt）：";
            cin >> filename;

            // 先读取文件内容
            if (!ReadTextFromFile(filename, fileContent))
            {
                cout << "文件读取失败。\n";
                continue;
            }

            // 再统计字符频率
            if (!CountCharacterFrequency(filename))
            {
                cout << "字符频率统计失败。\n";
            }
        }
            continue;
        case 2:
        {
            CreateHuffmanTree(HT, charCount);
            cout << "哈夫曼树构造完成！树结构如下：\n";
            PrintHuffmanTree(HT, charCount);
        }
            continue;
        case 3:
        {
            {
                cout << "哈夫曼编码如下：\n";
                GenerateHuffmanCode(HT, charCount);

                // 输出哈夫曼编码
                for (int i = 0; i < charCount; ++i)
                {
                    if (freq[i] > 0)
                    {
                        cout << chars[i] << " : " << code[i] << endl;
                    }
                }
            }
            GenerateHuffmanCode(HT, charCount);
        }
            continue;
        case 4:
        {
            string encodedMessage;
            string outputFilename = "encoded_output.txt";

            if (fileContent.empty())
            {
                cout << "请先读取文件（选项 1）。\n";
                continue;
            }
            if (code[0].empty())
            {
                cout << "请先生成哈夫曼编码（选项 3）。\n";
                continue;
            }

            EncodeMessage(HT, charCount, fileContent, encodedMessage, outputFilename);
        }
            continue;
        case 5:
        {
            string encodedFile;
            cout << "请输入要解码的文件名（如 encoded_output.txt）：";
            cin >> encodedFile;

            string encodedContent;
            if (!ReadTextFromFile(encodedFile, encodedContent))
            {
                cerr << "无法读取编码文件。\n";
                break;
            }

            string decodedOutputFile = "decoded_output.txt";
            DecodeHuffman(HT, charCount, encodedContent, decodedOutputFile);
        }
            continue;
        case 6:
        {
            cout << "退出程序。\n";
            return 0;
        }
        default:
        {
            cout << "无效的选择，请重新输入！\n";
            continue;
        }
        }
    }
    return 0;
}
