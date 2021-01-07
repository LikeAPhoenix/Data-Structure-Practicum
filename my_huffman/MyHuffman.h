#ifndef MYHUFFMAN_H
#define MYHUFFMAN_H

#include <iostream>
#include <string>
using namespace std;

struct HuffmanNode {
  unsigned char value = 0;
  int frequency = 0;
  struct HuffmanNode *left = NULL;
  struct HuffmanNode *right = NULL;
};

class MyHuffman {
 private:
  string filepath;                           // 要压缩的文件路径
  long long byte_value_frequency[256] = {};  // 值为0-255的字节出现的次数
  HuffmanNode *root = NULL;                  // 哈夫曼树根节点
  string huffman_code[256] = {};  // 值为0-255字节对应的哈夫曼编码
  long long bit_size = 0;         // 压缩文件bit总数
 public:
  MyHuffman(string filepath);
  ~MyHuffman();

  // 压缩所用函数
  void count_byte_frequency();  // 统计不同值得字节出现次数
  void create_huffman_tree();   // 创建哈夫曼树
  // 成员函数的默认参数只能在类内声明
  // 根据哈夫曼树得到哈夫曼编码
  void huffman_tree_to_huffman_code(HuffmanNode *root = NULL, string code = "");
  void compress();  // 压缩

  // 解压所用函数
  void get_huffman_code();  // 读取huffman文件得到哈夫曼编码
  void huffman_code_to_huffman_tree();  // 根据哈夫曼编码构建哈夫曼树
  void decompress();                    // 解压

  void show_huffman_tree();  // 展示哈夫曼树
  void show_huffman_code();  // 展示哈夫曼编码
};

#endif  // MYHUFFMAN_H