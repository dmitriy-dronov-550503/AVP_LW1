#pragma once
#include "stdafx.h"
#include <iostream>


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
		for (uint32_t i = 0; i < size; i++) {
			result = mx3[i];
			for (uint32_t j = 0; j < size; j++) {
				broadcast = mx1[i][j];
				mx2shared = mx2[j];
				for (uint32_t k = 0; k < size; k++) {
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
		for (uint32_t i = 0; i < size; i++) {
			result = mx3[i];
			for (uint32_t j = 0; j < size; j++) {
				broadcast = mx1[i][j];
				mx2shared = mx2[j];
				#pragma loop(no_vector)
				for (uint32_t k = 0; k < size; k++) {
					result[k] += broadcast * mx2shared[k];
				}
			}
		}
	}

	static void multiplySse(int** mx1, int** mx2, int** mx3, int matrixSize) {
		__m128i result;
		__m128i br0, br1, br2, br3;
		__m128i mx20, mx21, mx22, mx23;
		const int mxSize = matrixSize;
		const int mxQuatroSize = mxSize / 4;
		for (int i = 0; i < mxSize; i++) {
			__m128i* sourcePtr = (__m128i*)mx3[i];
			for (int j = 0; j < mxSize; j+=4) {
				__m128i* mxp20 = (__m128i*)mx2[j + 0];
				__m128i* mxp21 = (__m128i*)mx2[j + 1];
				__m128i* mxp22 = (__m128i*)mx2[j + 2];
				__m128i* mxp23 = (__m128i*)mx2[j + 3];
				br0 = _mm_set1_epi32(mx1[i][j + 0]);
				br1 = _mm_set1_epi32(mx1[i][j + 1]);
				br2 = _mm_set1_epi32(mx1[i][j + 2]);
				br3 = _mm_set1_epi32(mx1[i][j + 3]);
				for (int k = 0; k < mxQuatroSize; k++) {
					result = _mm_lddqu_si128(sourcePtr + k);

					mx20 = _mm_lddqu_si128(mxp20 + k);
					mx20 = _mm_mullo_epi32(br0, mx20);
					mx21 = _mm_lddqu_si128(mxp21 + k);
					mx21 = _mm_mullo_epi32(br1, mx21);
					mx22 = _mm_lddqu_si128(mxp22 + k);
					mx22 = _mm_mullo_epi32(br2, mx22);
					mx23 = _mm_lddqu_si128(mxp23 + k);
					mx23 = _mm_mullo_epi32(br3, mx23);

					mx20 = _mm_add_epi32(mx20, mx21);
					mx22 = _mm_add_epi32(mx22, mx23);
					mx20 = _mm_add_epi32(mx20, mx22);
					result = _mm_add_epi32(result, mx20);

					_mm_storeu_si128(sourcePtr + k, result);
				}
			}
		}
	}

	static void multiplyAvx(int** mx1, int** mx2, int** mx3, int matrixSize) {
		__m256i result;
		__m256i br0, br1, br2, br3;
		__m256i mx20, mx21, mx22, mx23;
		const int mxSize = matrixSize;
		const int mxOctoSize = mxSize / 8;
		for (int i = 0; i < mxSize; i++) {
			__m256i* sourcePtr = (__m256i*)mx3[i];
			for (int j = 0; j < mxSize; j += 4) {
				__m256i* mxp20 = (__m256i*)mx2[j + 0];
				__m256i* mxp21 = (__m256i*)mx2[j + 1];
				__m256i* mxp22 = (__m256i*)mx2[j + 2];
				__m256i* mxp23 = (__m256i*)mx2[j + 3];
				br0 = _mm256_set1_epi32(mx1[i][j + 0]);
				br1 = _mm256_set1_epi32(mx1[i][j + 1]);
				br2 = _mm256_set1_epi32(mx1[i][j + 2]);
				br3 = _mm256_set1_epi32(mx1[i][j + 3]);
				for (int k = 0; k < mxOctoSize; k++) {
					result = _mm256_lddqu_si256(sourcePtr + k);

					mx20 = _mm256_lddqu_si256(mxp20 + k);
					mx20 = _mm256_mullo_epi32(br0, mx20);
					mx21 = _mm256_lddqu_si256(mxp21 + k);
					mx21 = _mm256_mullo_epi32(br1, mx21);
					mx22 = _mm256_lddqu_si256(mxp22 + k);
					mx22 = _mm256_mullo_epi32(br2, mx22);
					mx23 = _mm256_lddqu_si256(mxp23 + k);
					mx23 = _mm256_mullo_epi32(br3, mx23);

					mx20 = _mm256_add_epi32(mx20, mx21);
					mx22 = _mm256_add_epi32(mx22, mx23);
					mx20 = _mm256_add_epi32(mx20, mx22);
					result = _mm256_add_epi32(result, mx20);

					_mm256_storeu_si256(sourcePtr + k, result);
				}
			}
		}
	}

	static void compare(int** mx1, int** mx2, int matrixSize) {
		int mistakes = 0;
		const int max = 1;
		for (int i = 0; i < matrixSize && (mistakes <= max); i++) {
			for (int j = 0; j < matrixSize && (mistakes <= max); j++) {
				if (mx1[i][j] != mx2[i][j]) {
					mistakes++;
					cout << "Mistake founed in: [" << i << ";" << j << "]" << endl;
				}
			}
		}
		if (mistakes >= max) {
			cout << "..." << endl << endl;
		}
	}

};