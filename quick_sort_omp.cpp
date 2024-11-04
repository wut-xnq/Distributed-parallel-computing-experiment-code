#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
void rands(int* data, int sum);
void sw(int* a, int* b);
int partition(int* a, int sta, int end);
void quickSort_parallel(int* a, int lenArray, int numThreads);
void quickSort_parallel_internal(int* a, int left, int right, int cutoff);

void rands(int* data, int sum) {
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
    a[sta] = a[j];
    a[j] = x;
    return j;
}

// 并行快速排序
void quickSort_parallel(int* a, int lenArray, int numThreads) {
    int cutoff = 1000;
#pragma omp parallel num_threads(numThreads)       
    {
#pragma omp single      
        {
            quickSort_parallel_internal(a, 0, lenArray - 1, cutoff);
        }
    }
}
// 并行快速排序的内部实现
void quickSort_parallel_internal(int* a, int left, int right, int cutoff) {
    int i = left, j = right;
    int tmp;
    int pivot = a[(left + right) / 2];

    while (i <= j) {
        while (a[i] < pivot)
            i++;
        while (a[j] > pivot)
            j--;
        if (i <= j) {
            tmp = a[i];
            a[i] = a[j];
            a[j] = tmp;
            i++;
            j--;
        }
    }

    if ((right - left) < cutoff) {
        if (left < j) {
            quickSort_parallel_internal(a, left, j, cutoff);
        }
        if (i < right) {
            quickSort_parallel_internal(a, i, right, cutoff);
        }
    }
    else {
#pragma omp task        
        {
            quickSort_parallel_internal(a, left, j, cutoff);
        }
#pragma omp task
        {
            quickSort_parallel_internal(a, i, right, cutoff);
        }
    }
}

int main() {
    const int n = 8000000;  // 数据量设为 8000000
    int* data = (int*)malloc(sizeof(int) * n);
    int numThreads = 64; // 设定使用的线程数量

    rands(data, n);

    double omp_time_start = omp_get_wtime();
    quickSort_parallel(data, n, numThreads);
    double omp_time_end = omp_get_wtime();

    printf("并行线程快速排序处理时间 : %lf s\n", omp_time_end - omp_time_start);

    free(data);
    return 0;
}
