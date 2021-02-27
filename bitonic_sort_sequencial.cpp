/* C++ Program for Bitonic Sort. Note that this program 
   works only when size of input is a power of 2. */
#include<bits/stdc++.h> 
#include "chrono.cpp"
#include "utils.cpp"

using namespace std; 
  
/*The parameter dir indicates the sorting direction, ASCENDING 
   or DESCENDING; if (a[i] > a[j]) agrees with the direction, 
   then a[i] and a[j] are interchanged.*/
void compAndSwap(float *a, int i, int j, int dir) 
{ 
    if (dir==(a[i]>a[j])) 
        swap(a[i],a[j]); 
} 
  
/*It recursively sorts a bitonic sequence in ascending order, 
  if dir = 1, and in descending order otherwise (means dir=0). 
  The sequence to be sorted starts at index position low, 
  the parameter cnt is the number of elements to be sorted.*/
void bitonicMerge(float *a, int low, int cnt, int dir) 
{ 
    if (cnt>1) 
    { 
        int k = cnt/2; 
        for (int i=low; i<low+k; i++) 
            compAndSwap(a, i, i+k, dir); 
        bitonicMerge(a, low, k, dir); 
        bitonicMerge(a, low+k, k, dir); 
    } 
} 
  
/* This function first produces a bitonic sequence by recursively 
    sorting its two halves in opposite sorting orders, and then 
    calls bitonicMerge to make them in the same order */
void bitonicSort(float *a,int low, int cnt, int dir) 
{ 
    if (cnt>1) 
    { 
        int k = cnt/2; 
  
        // sort in ascending order since dir here is 1 
        bitonicSort(a, low, k, 1); 
  
        // sort in descending order since dir here is 0 
        bitonicSort(a, low+k, k, 0); 
  
        // Will merge wole sequence in ascending order 
        // since dir=1. 
        bitonicMerge(a,low, cnt, dir); 
    } 
} 
  
/* Caller of bitonicSort for sorting the entire array of 
   length N in ASCENDING order */
void sort(float *a, int N, int up) 
{ 
    bitonicSort(a,0, N, up); 
} 
  
// Driver code 
int main(int argc, char **argv) 
{
    float *a; 
    int n_elements = 0;
    bool isPrintable = false;

    extractArgs(argc, argv, &n_elements, &isPrintable);
    if (n_elements == 0) {
        printf("Insert the size of arr after '-n' flag\n");
        return 1;
    }

    a = (float*) malloc(n_elements * sizeof(float));
    fillArr(a, n_elements);
    if (isPrintable) {
        printArr(a, n_elements);
    }
    chronometer_t *chrono = (chronometer_t *) malloc(sizeof(chronometer_t));

    chrono_start(chrono);   
    sort(a, n_elements, 1); 
    chrono_stop(chrono);

    chrono_deltaT(chrono);
    
    if (isPrintable) {
        printArr(a, n_elements);
    }
    
    free(chrono);
    return 0; 
} 