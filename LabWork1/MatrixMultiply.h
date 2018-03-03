#pragma once
#include "stdafx.h"
#include <iostream>
using namespace std;

class MatrixMultiply {
public:

	static void multiply(int** mx1, int** mx2, int** mx3, int matrixSize) {
		for (int i = 0; i < matrixSize; i++) {
			for (int j = 0; j < matrixSize; j++) {
				mx3[i][j] = 0;
				for (int k = 0; k < matrixSize; k++) {
					mx3[i][j] += mx1[i][k] * mx2[k][j];
				}
			}
		}
	}

	static void multiplyVectorized(int** mx1, int** mx2, int** mx3, int matrixSize) {
		int* result = nullptr;
		int broadcast = 0;
		int* mx2shared = nullptr;
		uint32_t size = matrixSize;
		for (uint32_t i = 0; i < size; ++i) {
			result = mx3[i];
			for (uint32_t j = 0; j < size; ++j) {
				broadcast = mx1[i][j];
				mx2shared = mx2[j];
				for (uint32_t k = 0; k < size; ++k) {
					result[k] += broadcast * mx2shared[k];
				}
			}
		}
	}

	static void multiplyNotVectorized(int** mx1, int** mx2, int** mx3, int matrixSize) {
		int* result = nullptr;
		int broadcast = 0;
		int* mx2shared = nullptr;
		uint32_t size = matrixSize;
		for (uint32_t i = 0; i < size; ++i) {
			result = mx3[i];
			for (uint32_t j = 0; j < size; ++j) {
				broadcast = mx1[i][j];
				mx2shared = mx2[j];
				#pragma loop(no_vector)
				for (uint32_t k = 0; k < size; ++k) {
					result[k] += broadcast * mx2shared[k];
				}
			}
		}
	}

	static void multiplySSE(int** mx1, int** mx2, int** mx3, int matrixSize) {
		/*for (int j = 0; j < matrixSize; j++) {
			for (int i = 0; i < matrixSize; i++) {
				__m128i result;
				__m128i *rmx = (__m128i*)&mx3[i][0];
				result = _mm_set1_epi32(mx1[i][j]);	// Broadcast number from A matrix to low addresses.
				result = _mm_mullo_epi32(result, *(__m128i*)&mx2[j][0]);	// Multiply
				*rmx = _mm_add_epi32(result, *rmx);
			}
		}*/

		__m128i result;
		__m128i broadcast;
		__m128i* rmx;
		int* mx2shared = nullptr;
		uint32_t size = matrixSize;
		for (uint32_t i = 0; i < size; ++i) {
			for (uint32_t j = 0; j < size; ++j) {
				broadcast = _mm_set1_epi32(mx1[i][j]);	// Broadcast number from A matrix to low addresses.
				for (uint32_t k = 0; k < size; k+=4) {
					rmx = (__m128i*)&mx3[i][k];
					result = _mm_mullo_epi32(broadcast, *(__m128i*)&mx2[j][k]);	// Multiply
					*rmx = _mm_add_epi32(result, *rmx);
				}
			}
		}
	}


	static void compare(int** mx1, int** mx2, int matrixSize) {
		bool isMistakeFounded = false;
		for (int i = 0; i < matrixSize; i++) {
			for (int j = 0; j < matrixSize; j++) {
				if (mx1[i][j] != mx2[i][j]) {
					cout << "Mistake founed in: [" << i << ";" << j << "]" << endl;
					isMistakeFounded = true;
				}
			}
		}
		if (isMistakeFounded) {
			cout << "Mistake was founded." << endl;
		}
	}

};