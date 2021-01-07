#include "MyHuffman.h"

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <queue>
#include <string>
#include <vector>
using namespace std;

MyHuffman::MyHuffman(string filepath) { this->filepath = filepath; }

MyHuffman::~MyHuffman() {}

void MyHuffman::count_byte_frequency() {
  ifstream in(filepath, ios::in | ios::binary);
  if (!in) {
    cout << "File is not exist!!!" << endl;
  } else {
    unsigned char temp;  // 临时变量 读入每个字节
    while (in.peek() != EOF) {
      in.read((char *)&temp, sizeof(unsigned char));
      byte_value_frequency[temp]++;
    }
    /* for (int i = 0; i < 256; i++) {
      if (byte_value_frequency[i] != 0) {
        cout << i << ":" << byte_value_frequency[i] << endl;
      }
    } */
  }
  in.close();
}

struct cmp {  // priority_queue比较级是用结构体, 而不是函数
  bool operator()(const HuffmanNode &a,
                  const HuffmanNode &b) {  // 重载操作, 记住就行
    return a.frequency > b.frequency;  // 小的优先在前面, 因为是用"堆"去实现的,
  }
};

void MyHuffman::create_huffman_tree() {  // 建造哈夫曼树
  priority_queue<HuffmanNode, vector<HuffmanNode>, cmp> que;  // 优先队列
  for (int i = 0; i < 256; i++) {
    if (byte_value_frequency[i] > 0) {
      HuffmanNode *temp = new HuffmanNode;
      temp->value = i;
      temp->frequency = byte_value_frequency[i];
      // temp->left = temp->right = NULL;
      que.push(*temp);
    }
  }

  HuffmanNode *temp_left, *temp_right, *temp_root;
  while (que.size() >= 2) {  // 不只有一棵树
    temp_left = new HuffmanNode;
    temp_right = new HuffmanNode;
    temp_root = new HuffmanNode;

    *temp_left = que.top();
    que.pop();
    *temp_right = que.top();
    que.pop();
    // 父节点的频率为左右孩子节点之和
    temp_root->frequency = temp_left->frequency + temp_right->frequency;
    temp_root->left = temp_left;
    temp_root->right = temp_right;
    que.push(*temp_root);
  }
  root = temp_root;
}

void MyHuffman::huffman_tree_to_huffman_code(HuffmanNode *root, string code) {
  // 初始化root 仅在第一层执行
  if (root == NULL && code == "") root = this->root;
  if (root == NULL) return;
  if (root->left == NULL && root->right == NULL) {
    huffman_code[root->value] = code;
  }
  huffman_tree_to_huffman_code(root->left, code + '0');   // 左分支为'0'
  huffman_tree_to_huffman_code(root->right, code + '1');  // 右分支为'1'
}

void MyHuffman::compress() {
  ifstream in(filepath, ios::in | ios::binary);
  ofstream out(filepath + ".compress", ios::out | ios::binary);
  if (!(in && out)) {
    cout << "File is not exist or can't create!!!" << endl;
  } else {
    unsigned char temp_read = 0;   // 临时变量 读入每个字节
    unsigned char temp_write = 0;  // 临时变量 写入每个字节
    string temp_str;               // 临时字符串 存储哈夫曼编码
    long long temp_write_bit_size = 0;  // 临时变量temp_write所写的二进制位数
    while (in.peek() != EOF) {
      in.read((char *)&temp_read, sizeof(unsigned char));
      temp_str = huffman_code[temp_read];
      for (int i = 0; i < temp_str.size(); i++) {
        // 位运算的预先级低
        temp_write = (temp_write << 1) + (temp_str[i] - '0');
        temp_write_bit_size++;
        bit_size++;
        if (temp_write_bit_size == 8) {  // 8bit为1字节
          out.write((char *)&temp_write, sizeof(unsigned char));
          temp_write_bit_size = 0;
        }
      }
    }
    // 还有bit没写进去
    if (temp_write_bit_size != 0) {
      while (temp_write_bit_size != 8) {
        temp_write = temp_write << 1;
        temp_write_bit_size++;
      }
      out.write((char *)&temp_write, sizeof(unsigned char));
    }
    in.close();
    out.close();

    // 写入哈夫曼编码文件
    out.open(filepath + ".huffman", ios::out);
    out << "HuffmanCode:" << endl;
    int types = 0;
    for (int i = 0; i < 256; i++) {
      if (byte_value_frequency[i] > 0) {
        types++;
      }
    }
    out << "TypesOfCharacter: " << types << endl;

    out.setf(ios::left);
    for (int i = 0; i < 256; i++) {
      if (byte_value_frequency[i] > 0) {
        out << setw(5) << i << huffman_code[i] << endl;
      }
    }

    out << "WriteBitSize: " << bit_size << endl;
  }
}

void MyHuffman::get_huffman_code() {
  ifstream in_huffman(filepath + ".huffman", ios::in);  // 以文本方式打开
  // 读取哈夫曼编码信息
  string temp_str, code_str;           // 临时字符串 哈夫曼编码
  int types_character, value;          // 字节种类 字节值
  in_huffman >> temp_str >> temp_str;  // 跳过不必要的信息
  in_huffman >> types_character;       // 读入字节种数
  for (int i = 0; i < types_character; i++) {
    in_huffman >> value >> code_str;
    huffman_code[value] = code_str;
  }
  in_huffman >> temp_str >> bit_size;
  in_huffman.close();
}

void MyHuffman::huffman_code_to_huffman_tree() {
  root = new HuffmanNode;
  HuffmanNode *temp_parent, *temp_child;  // temp_child用于开辟空间
  for (int i = 0; i < 256; i++) {
    // 没有该字节的哈夫曼编码就跳过
    if (huffman_code[i].size() == 0) continue;

    temp_parent = root;
    for (int j = 0; j < huffman_code[i].size(); j++) {
      if (huffman_code[i][j] == '0') {
        if (temp_parent->left == NULL) {
          temp_child = new HuffmanNode;  // 该节点不存在就开辟空间
          temp_parent->left = temp_child;
        }
        temp_parent = temp_parent->left;
      } else if (huffman_code[i][j] == '1') {
        if (temp_parent->right == NULL) {
          temp_child = new HuffmanNode;  // 该节点不存在就开辟空间
          temp_parent->right = temp_child;
        }
        temp_parent = temp_parent->right;
      }
    }
    temp_parent->value = i;
  }
}

void MyHuffman::decompress() {
  string exten;  // 后缀名
  for (int i = filepath.size() - 1; i >= 0; i--) {
    exten.push_back(filepath[i]);
    if (filepath[i] == '.') break;
  }
  reverse(exten.begin(), exten.end());

  // 以二进制的方式打开文件
  ifstream in_compress(filepath + ".compress", ios::in | ios::binary);
  ofstream out_decompress(filepath + ".decompress" + exten,
                          ios::out | ios::binary);

  if (!(in_compress && out_decompress)) {
    cout << "File is not exist or can't create!!!" << endl;
  } else {
    /* // 哈夫曼树不存在 读取哈夫曼编码
    if (root == NULL) {
      get_huffman_code();              // 读取哈夫曼编码
      huffman_code_to_huffman_tree();  // 构建哈夫曼树
    } */

    unsigned char temp_read = 0;   // 临时变量 读入每个字节
    unsigned char temp_write = 0;  // 临时变量 写入每个字节
    long long now_bit_size = 0;    // 现在读入的bit总数
    HuffmanNode *temp = root;
    // show_huffman_code();
    while (in_compress.peek() != EOF) {
      in_compress.read((char *)&temp_read, sizeof(unsigned char));
      for (int i = 7; i >= 0; i--) {
        if ((temp_read >> i & 1) == 0) {  // 该bit为0
          temp = temp->left;
        } else if ((temp_read >> i & 1) == 1) {  // 该bit为1
          temp = temp->right;
        }
        // 找到了对应的字节
        if (temp->left == NULL && temp->right == NULL) {
          temp_write = temp->value;
          out_decompress.write((char *)&temp_write, sizeof(unsigned char));
          temp = root;
        }
        now_bit_size++;
        if (now_bit_size == bit_size) break;
      }
    }
    in_compress.close();
    out_decompress.close();
  }
}

void MyHuffman::show_huffman_code() {
  for (int i = 0; i < 256; i++) {
    if (huffman_code[i].size() > 0) {
      cout << i << " " << huffman_code[i] << endl;
    }
  }
}