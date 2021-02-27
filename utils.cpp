#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

float generateRandomFloat(){
    return (float)rand()/(float)RAND_MAX;
}

void fillArr(float *arr, int length){
    srand(time(NULL));
    for (int i = 0; i < length; ++i) {
        arr[i] = generateRandomFloat();
    }
}

void printArr(float *arr, int length){
    for (int i = 0; i < length; ++i) {
        printf("%1.3f ", arr[i]);
    }
    printf("\n");
}

void extractArgs(int argc, char **argv, int *n_el, bool *isPrint) {
    for (int i = 1; i <argc; i++) {
        if (strcmp(argv[i], "-n") == 0) {
            *n_el = atoi(argv[i+1]);
        }

        if (strcmp(argv[i], "-p") == 0) {
            *isPrint = true;
        }
    }
}

bool isCorrectSolution(float *expectedArr, float *givenArr, int n_elements) {
    for (int i = 0; i < n_elements; i++) {
        if (expectedArr[i] != givenArr[i]) {
            return false;
        }
    }
    return true;
}