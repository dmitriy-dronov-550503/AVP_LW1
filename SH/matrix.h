#pragma once
#include <stdlib.h>
#include <time.h>
#include <immintrin.h>
#include <stdbool.h>
#include <inttypes.h>

float**** matrixAlloc(int, int);
void matrixGenerate(float ****, int, int);
bool equals(float ****, float ****, int, int);
void matrixMultiply(float ****, float ****, float ****, int, int);
void matrixMultiplySSE(float ****, float ****, float ****, int, int);
