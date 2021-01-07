#include <windows.h>

#include <iostream>

#include "MyHuffman.h"
using namespace std;

void show();                           //开始界面
void option_1();                       // 压缩
void option_2();                       // 解压
void count_time(void (*fun_point)());  // 计时函数

int main() {
  while (1) {
    show();
    int option;
    cin >> option;
    if (option == 1) {
      count_time(option_1);
    } else if (option == 2) {
      count_time(option_2);
    } else {
      break;
    }
  }
  return 0;
}

void show() {
  cout << "------------哈夫曼编码------------" << endl;
  cout << "1 压缩文件" << endl;
  cout << "2 解压文件" << endl;
  cout << "0 退出" << endl;
  cout << "------------哈夫曼编码------------" << endl;
  cout << "请输入选项：";
}

void option_1() {
  string filepath;
  cout << "请输入要压缩的文件路径：";
  cin >> filepath;
  MyHuffman huffman(filepath);
  huffman.count_byte_frequency();
  huffman.create_huffman_tree();
  huffman.huffman_tree_to_huffman_code();
  huffman.compress();
  cout << "压缩成功！！！" << endl;
  cout << "是否解压该压缩文件" << endl;
  cout << "1 是" << endl;
  cout << "2 否" << endl;
  int is_decompress;
  cin >> is_decompress;
  if (is_decompress == 1) {
    huffman.decompress();
    cout << "解压成功！！！" << endl;
  }
}

void option_2() {
  string filepath;
  cout << "请输入被压缩原文件的路径：";
  cin >> filepath;
  MyHuffman Huffman(filepath);
  Huffman.get_huffman_code();
  Huffman.huffman_code_to_huffman_tree();
  Huffman.decompress();
  cout << "解压成功！！！" << endl;
}

void count_time(void (*fun_point)()) {
  LARGE_INTEGER start_time, end_time, time_clock;  // time_clock时钟频率
  QueryPerformanceCounter(&start_time);
  QueryPerformanceFrequency(&time_clock);
  fun_point();
  QueryPerformanceCounter(&end_time);
  double time = (double)((end_time.QuadPart - start_time.QuadPart) * 1000) /
                (double)time_clock.QuadPart;
  printf("用时：%.2lfms\n", time);
}