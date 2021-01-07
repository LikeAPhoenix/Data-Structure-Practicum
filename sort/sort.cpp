#include <windows.h>

#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
using namespace std;

void bubble_sort(int *arr, int left, int right);  // 冒泡排序
void insert_sort(int *arr, int left, int right);  // 插入排序
void select_sort(int *arr, int left, int right);  // 选择排序
void shell_sort(int *arr, int left, int right);  // 基于sedgewick增量的希尔排序
void quick_sort(int *arr, int left, int right);  // 快速排序
void heap_sort(int *arr, int left, int right);   // 堆排序 递归实现
void merge_sort_recursion(int *arr, int left, int right);  // 归并排序 递归实现
void merge_sort_circulation(int *arr, int left,
                            int right);  // 归并排序 循环实现
void radix_sort_LSD(int *arr, int left, int right);  // 基数排序 低位优先
void radix_sort_MSD(int *arr, int left, int right);  // 基数排序 高位优先

void swap(int &a, int &b) {
  int temp = a;
  a = b;
  b = temp;
}

// 冒泡排序
void bubble_sort(int *arr, int left, int right) {
  bool sorted;
  for (int i = left; i < right; i++) {  // 第i+1趟
    sorted = true;
    for (int j = left; j < right - i;
         j++) {  // 从left到right - i 最大的数放在right-i位置上
      if (arr[j] > arr[j + 1]) {
        swap(arr[j], arr[j + 1]);
        sorted = false;
      }
    }
    if (sorted == true) {
      return;
    }
  }
}

// 插入排序
void insert_sort(int *arr, int left, int right) {
  for (int i = left + 1; i <= right; i++) {
    int temp = arr[i];  // 当前位置的值给temp
    int j = i;
    for (; j > 0 && arr[j - 1] > temp; j--) {
      // 假如arr[j-1]>temp 后移
      arr[j] = arr[j - 1];
    }
    arr[j] = temp;
  }
}

// 选择排序
void select_sort(int *arr, int left, int right) {
  for (int i = left; i < right; i++) {
    int index = i;
    for (int j = i + 1; j <= right; j++) {
      if (arr[index] > arr[j]) {
        index = j;
      }
    }
    if (index != i) {
      swap(arr[index], arr[i]);
    }
  }
}

// 基于sedgewick增量的希尔排序
void shell_sort(int *arr, int left, int right) {
  // 希尔增量序列
  int sedgewick[15] = {146305, 64769, 36289, 16001, 8929, 3905, 929, 2161,
                       505,    209,   109,   41,    19,   5,    1};
  int h = 0, len = right - left + 1, temp;
  for (h = 0; sedgewick[h] >= len; h++)
    ;

  for (int i = sedgewick[h]; i > 0;
       i = sedgewick[++h]) {         // 不同的增量，增量递减
    for (int j = i; j < len; j++) {  // 间隔为i的插入排序
      temp = arr[j];
      int k = j;
      for (; k >= i && arr[k - i] > temp; k -= i) {
        // 假如arr[k-i]>temp 后移
        arr[k] = arr[k - i];
      }
      arr[k] = temp;
    }
  }
}

// 快速排序
void quick_sort(int *arr, int left, int right) {
  if (left >= right) return;
  int i = left, j = right;  // i、j为两个“指针”
  int base = arr[left];     // 基准
  while (i < j) {
    while (i < j && arr[j] >= base) j--;  // 循环结束时 a[j] < base或i == j;
    while (i < j && arr[i] <= base) i++;  // 循环结束时 a[i] > base或i == j
    if (i < j) swap(arr[i], arr[j]);      // 交换
  }
  swap(arr[left], arr[i]);  // 交换
  // 递归对左右区间进行排序
  quick_sort(arr, left, i - 1);   // 还没有到右边界
  quick_sort(arr, i + 1, right);  // 还没有到左边界
}

// 最大堆的建立
void perc_down(int *arr, int p, int len) {
  int parent, child, value = arr[p];  // 父节点 子节点 根节点存放的值
  for (parent = p; parent * 2 + 1 < len; parent = child) {
    child = parent * 2 + 1;
    if (child != len - 1 && arr[child] < arr[child + 1]) {
      child++;  // child指向左右子节点的较大者
    }
    if (value >= arr[child]) {
      break;
    } else {
      arr[parent] = arr[child];  // 下滤
    }
  }
  arr[parent] = value;
}

// 堆排序
void heap_sort(int *arr, int left, int right) {
  int len = right - left + 1;
  int i;
  for (i = len / 2 + 1; i >= 0; i--) {  // 建立最大堆
    perc_down(arr, i, len);
  }
  for (i = len - 1; i >= 0; i--) {
    // 删除最大堆顶
    swap(arr[0], arr[i]);
    perc_down(arr, 0, i);
  }
}

// 将有序的arr[l]~arr[l-1]和arr[r]~arr[right_end]归并成一个有序序列
void merge(int *arr, int *temp_arr, int l, int r, int right_end) {
  int left_end = r - 1;  // 左边终点位置
  int temp = l, i = l;  // temp i有序序列的起始位置 i用于后期的复制

  while (l <= left_end && r <= right_end) {
    if (arr[l] <= arr[r]) {
      temp_arr[temp++] = arr[l++];  // 将左边元素复制到temp_arr
    } else {
      temp_arr[temp++] = arr[r++];  // 将右边元素复制到temp_arr
    }
  }

  while (l <= left_end) {
    temp_arr[temp++] = arr[l++];  // 直接复制左边剩下的
  }
  while (r <= right_end) {
    temp_arr[temp++] = arr[r++];  // 直接复制右边剩下的
  }

  for (; i <= right_end; i++) {
    arr[i] = temp_arr[i];
  }
}

void m_sort(int *arr, int *temp_arr, int l, int right_end) {
  // 核心递归排序程序
  int mid;
  if (l < right_end) {
    mid = (l + right_end) / 2;
    m_sort(arr, temp_arr, l, mid);
    m_sort(arr, temp_arr, mid + 1, right_end);
    merge(arr, temp_arr, l, mid + 1, right_end);
  }
}

// 归并排序 递归实现
void merge_sort_recursion(int *arr, int left, int right) {
  int len = right - left + 1;
  int *temp_arr = new int[len];
  if (temp_arr != NULL) {
    m_sort(arr, temp_arr, left, right);
    delete temp_arr;
  } else {
    cout << "Space is not enough!!!" << endl;
  }
}

// 两两归并相邻有序子列
void merge_pass(int *arr, int *temp_arr, int left, int right, int length) {
  int len = right - left + 1;  // 数组长度  length 合并的子序列的长度
  int i, j;
  for (i = left; i <= len - 2 * length; i += 2 * length) {
    merge(arr, temp_arr, i, i + length, i + 2 * length - 1);
  }
  if (i + length <= right) {
    merge(arr, temp_arr, i, i + length, right);
  } else {
    for (j = i; j <= right; j++) {
      temp_arr[j] = arr[j];
    }
  }
}

// 归并排序 循环实现
void merge_sort_circulation(int *arr, int left, int right) {
  int len = right - left + 1;  // 归并的数组长度
  int length = 1;              // 最开始的子序列长度
  int *temp_arr = new int[len];
  if (temp_arr != NULL) {
    while (length < len) {
      merge_pass(arr, temp_arr, left, right, length);
      length *= 2;
      merge_pass(temp_arr, arr, left, right, length);
      length *= 2;
    }
    delete temp_arr;
  } else {
    cout << "Space is not enough!!!" << endl;
  }
}

// 基数排序
// 桶元素节点
const int MaxDigit = 5;  // 关键字
const int Radix = 10;    // 基数
typedef struct Node *PtrToNode;
struct Node {
  int key;
  PtrToNode next;
};
// 桶头节点
struct HeadNode {
  PtrToNode head, tail;
};
typedef struct HeadNode Bucket[Radix];
// 默认次位D=1, 主位D<=MaxDigit
int get_digit(int X, int D) {
  int d, i;

  for (i = 1; i <= D; i++) {
    d = X % Radix;
    X /= Radix;
  }
  return d;
}

// 基数排序 低位优先
void radix_sort_LSD(int *arr, int left, int right) {
  int d, di, i;
  Bucket B;
  PtrToNode temp, p, list = NULL;
  for (i = 0; i < Radix; i++) {  // 初始化每个桶为空链表
    B[i].head = B[i].tail = NULL;
  }
  for (i = left; i <= right; i++) {  // 将原始序列逆序存入初始链表list
    temp = new Node;
    temp->key = arr[i];
    temp->next = list;
    list = temp;
  }

  // 下面开始排序
  for (d = 1; d <= MaxDigit; d++) {  // 对数据得每一位循环处理
    // 下面时分配的过程
    p = list;
    while (p) {
      di = get_digit(p->key, d);
      temp = p;
      p = p->next;
      temp->next = NULL;
      if (B[di].head == NULL) {
        B[di].head = B[di].tail = temp;
      } else {
        B[di].tail->next = temp;
        B[di].tail = temp;
      }
    }
    // 下面是收集的过程
    list = NULL;
    for (di = Radix - 1; di >= 0; di--) {
      if (B[di].head) {
        B[di].tail->next = list;
        list = B[di].head;
        B[di].head = B[di].tail = NULL;
      }
    }
  }
  for (i = left; i <= right; i++) {
    temp = list;
    list = list->next;
    arr[i] = temp->key;
    delete temp;
  }
}

void MSD(int *arr, int l, int r, int d) {
  int di, i, j;
  Bucket B;
  PtrToNode temp, list = NULL;
  if (d == 0) {
    return;
  }
  for (i = 0; i < Radix; i++) {  // 初始化每个桶为空链表
    B[i].head = B[i].tail = NULL;
  }
  for (i = l; i <= r; i++) {  // 将原始序列逆序存入初始链表list
    temp = new Node;
    temp->key = arr[i];
    temp->next = list;
    list = temp;
  }

  // 下面是分配的过程
  PtrToNode p = list;
  while (p) {
    di = get_digit(p->key, d);

    temp = p;
    p = p->next;

    if (B[di].head == NULL) {
      B[di].tail = temp;
    }
    temp->next = B[di].head;
    B[di].head = temp;
  }

  // 下面是收集的过程
  i = j = l;
  for (di = 0; di < Radix; di++) {
    if (B[di].head) {
      p = B[di].head;
      while (p) {
        temp = p;
        p = p->next;
        arr[j++] = temp->key;
        delete temp;
      }
      MSD(arr, i, j - 1, d - 1);
      i = j;
    }
  }
}

// 基数排序 高位优先
void radix_sort_MSD(int *arr, int left, int right) {
  MSD(arr, left, right, MaxDigit);
}

struct fun {
  void (*fun_point)(int *, int, int);
  string fun_name;
} fun_arr[10] = {{bubble_sort, "bubble_sort"},
                 {insert_sort, "insert_sort"},
                 {select_sort, "select_sort"},
                 {shell_sort, "shell_sort"},
                 {quick_sort, "quick_sort"},
                 {heap_sort, "heap_sort"},
                 {merge_sort_recursion, "merge_sort_recursion"},
                 {merge_sort_circulation, "merge_sort_circulation"},
                 {radix_sort_LSD, "radix_sort_LSD"},
                 {radix_sort_MSD, "radix_sort_MSD"}};

// 运行指定排序函数并将排好序的序列写入txt文件中
void run(fun function, double &time, int arr[], int temp_arr[], int left,
         int right) {
  int len = right - left + 1;
  for (int i = 0; i < len; i++) {
    temp_arr[i] = arr[i];
  }

  // 低精度计时 精确到1ms
  /*
  clock_t start_time, end_time;
  start_time = clock();
  function.fun_point(temp_arr, left, right);  // 执行相应的排序函数

  end_time = clock();

  time = end_time - start_time;
  cout << function.fun_name << ":";
  cout << time << "ms" << endl;
  */

  LARGE_INTEGER start_time, end_time, time_clock;  // time_clock时钟频率
  QueryPerformanceFrequency(&time_clock);
  QueryPerformanceCounter(&start_time);

  function.fun_point(temp_arr, left, right);  // 执行相应的排序函数

  QueryPerformanceCounter(&end_time);
  time = (double)((end_time.QuadPart - start_time.QuadPart) * 1000) /
         (double)time_clock.QuadPart;  // ms毫秒
  cout << function.fun_name << ":";
  cout << fixed << setprecision(2) << time << "ms" << endl;

  fstream out(function.fun_name + ".txt", ios::out);
  for (int i = left; i <= right; i++) {
    out.setf(ios::left);
    out << setw(6) << temp_arr[i] << " ";
  }
  out.close();
}

int main() {
  srand(time(NULL));

  int len = 100000;
  int arr[len];       // 随机数数组
  int temp_arr[len];  // 临时数组

  int cnt = 1, fun_size = 10;  // cnt:每个排序执行次数 fun_size:函数个数
  double time[fun_size][cnt];  // 存储临时时间数组

  // 每个排序的同一个随机数数组调用cnt次
  for (int k = 0; k < cnt; k++) {
    // cout << k + 1 << endl;
    for (int i = 0; i < len; i++) {
      arr[i] = rand();
    }
    for (int i = 0; i < fun_size; i++) {
      run(fun_arr[i], time[i][k], arr, temp_arr, 0, len - 1);
    }
  }

  // 写入时间
  fstream out("time.txt", ios::out);
  for (int i = 0; i < fun_size; i++) {
    out << fun_arr[i].fun_name << ":";
    for (int j = 0; j < cnt; j++) {
      out << fixed << setprecision(2) << time[i][j] << " ";
    }
    out << endl;
  }
  return 0;
}