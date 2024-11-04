#include <stdio.h>
#include <stdlib.h>
#include <time.h>
void rands(int* data, int sum);
void sw(int* a, int* b);
int partition(int* a, int sta, int end);
void quickSort(int* a, int sta, int end);
void rands(int* data, int sum) {        //生成随机数 从 0 到 9999999
    for (int i = 0; i < sum; i++) {
        data[i] = rand() % 100000000;
    }
}

void sw(int* a, int* b) {
    int t = *a;
    *a = *b;
    *b = t;
}

int partition(int* a, int sta, int end) {
    int i = sta, j = end + 1;
    int x = a[sta];
    while (1) {
        while (a[++i] < x && i < end);
        while (a[--j] > x);
        if (i >= j)
            break;
        sw(&a[i], &a[j]);
    }
    a[sta] = a[j];          //a[sta]最终位置是a[j];
    a[j] = x;              //a[j]最终位置的值为x;
    return j;
}

void quickSort(int* a, int sta, int end) {
    if (sta < end) {
        int mid = partition(a, sta, end);
        quickSort(a, sta, mid - 1);         //左递归
        quickSort(a, mid + 1, end);         //右递归
    }
}

int main() {
    const int n = 8000000;  // 数据量设为 8000000
    int* data = (int*)malloc(sizeof(int) * n);

    rands(data, n);

    clock_t start = clock();
    quickSort(data, 0, n - 1);
    clock_t end = clock();

    printf("串行线程快速排序处理时间 : %lf s\n", (double)(end - start) / CLOCKS_PER_SEC);

    free(data);
    return 0;
}
