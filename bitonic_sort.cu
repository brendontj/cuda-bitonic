#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "chrono.cpp"
#include "utils.cpp"
#include <math.h>
#include <bits/stdc++.h>
 
//Every thread gets exactly one value in the unsorted array

// N_Elements Exp^2 Threads Blocks
// 1024       2^9        4     256
// 8192       2^13       8     1024
// 131072     2^17      32     4096      
// 1048576    2^21     128     16384
// 16777216   2^24     512     32768

#define THREADS 512 
#define BLOCKS 32768 
#define NUM_VALS THREADS*BLOCKS
 
struct Arranger {
    int threads, blocks, values; 
};

struct Arranger testedExecutions[5] = {
    {4, 256, 1024},
    {8, 1024, 8192},
    {32, 4096, 131072},
    {128, 16384, 1048576},
    {512, 32768, 16777216}
};

//CUDA kernel
__global__ 
void bitonic_sort_kernel(float *deviceArr, int j, int k){
    unsigned int i, ixj;
    i = threadIdx.x + blockDim.x * blockIdx.x;
    ixj = i ^ j;
 
    if ((ixj)>i) {
        if ((i & k)==0) {
            if (deviceArr[i]>deviceArr[ixj]) {
                float tmp = deviceArr[i];
                deviceArr[i] = deviceArr[ixj];
                deviceArr[ixj] = tmp;
            }
        }
        if ((i & k)!=0) {
            if (deviceArr[i] < deviceArr[ixj]) {
                float tmp = deviceArr[i];
                deviceArr[i] = deviceArr[ixj];
                deviceArr[ixj] = tmp;
            }
        }
    }
}
 
int main(int argc, char **argv){
    float *hostArrToBeSorted;
    int threadsToUse, blocksToUse = 0;
    int n_elements = 0;
    bool isPrintable = false;

    extractArgs(argc, argv, &n_elements, &isPrintable);

    if (n_elements == 0) {
        printf("Insert the size of arr after '-n' flag\n");
        return 1;
    }

    for (int i=0; i < 5; i++) {
        if (n_elements == testedExecutions[i].values) {
            threadsToUse = testedExecutions[i].threads;
            blocksToUse = testedExecutions[i].blocks;
        }
    }

    if (threadsToUse == 0 && blocksToUse == 0) {
        threadsToUse = THREADS;
        blocksToUse = BLOCKS;
    }

    chronometer_t *chrono = (chronometer_t *) malloc(sizeof(chronometer_t));

    hostArrToBeSorted = (float*) malloc(n_elements * sizeof(float));
    fillArr(hostArrToBeSorted, n_elements);

    if (isPrintable) {
        printArr(hostArrToBeSorted, n_elements);
    }
    
    float *deviceArrToBeSorted;
    cudaMalloc((void**) &deviceArrToBeSorted, n_elements * sizeof(float));
    cudaMemcpy(deviceArrToBeSorted, hostArrToBeSorted, n_elements * sizeof(float), cudaMemcpyHostToDevice);
    
    
    dim3 blocks(blocksToUse,1); 
    dim3 threads(threadsToUse,1); 

    chrono_start(chrono);
   
    for (int k = 2; k <= blocksToUse*threadsToUse; k <<= 1) {
        for (int j= k>>1; j>0; j=j>>1) {
            bitonic_sort_kernel<<<blocks, threads>>>(deviceArrToBeSorted, j, k);
        }
    }
    
    chrono_stop(chrono);

    float *hostSortedArr = (float*) malloc(n_elements * sizeof(float));
    cudaMemcpy(hostSortedArr, deviceArrToBeSorted, n_elements * sizeof(float), cudaMemcpyDeviceToHost);
   
    chrono_deltaT(chrono);

    chrono_reset(chrono);

    chrono_start(chrono);
    std::sort(hostArrToBeSorted, hostArrToBeSorted+n_elements);
    chrono_stop(chrono);

    chrono_deltaT(chrono);

    if (isCorrectSolution(hostArrToBeSorted, hostSortedArr, n_elements)) {
        printf("The solution is correct!\n");
    } else {
        printf("The solution is not correct!\n");
    }

    if (isPrintable) {
        printArr(hostSortedArr, n_elements);
    }
    
    cudaFree(hostArrToBeSorted);
    cudaFree(deviceArrToBeSorted);
    cudaFree(hostSortedArr);
    free(chrono);

    return 0;
}