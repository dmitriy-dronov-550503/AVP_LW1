// LabWork1.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "QuadroMatrix.h"
#include "Matrix.h"
#include "emmintrin.h"
#include "smmintrin.h"
#include <iostream>
using namespace std;

void show(int** matrix, int matrixSize) {
	for (int i = 0; i < matrixSize; i++) {
		for (int j = 0; j < matrixSize; j++) {
			cout << matrix[i][j] << "  ";
		}
		cout << endl;
	}
}

void multiply(int** mx1, int** mx2, int** mx3, int matrixSize) {
	for (int i = 0; i < matrixSize; i++) {
		for (int j = 0; j < matrixSize; j++) {
			mx3[i][j] = 0;
			for (int k = 0; k < matrixSize; k++) {
				mx3[i][j] += mx1[i][k] * mx2[k][j];
			}
		}
	}
}

void multiplyVectorized(int** mx1, int** mx2, int** mx3, int matrixSize) {
	int *temp = nullptr;
	int temp1 = 0;
	int *temp2 = nullptr;
	uint32_t size = matrixSize;
	for (uint32_t i = 0; i < size; ++i) {
		temp = mx3[i];
		for (uint32_t j = 0; j < size; ++j) {
			temp1 = mx1[i][j];
			temp2 = mx2[j];
			for (uint32_t k = 0; k < size; ++k)
				temp[k] += temp1 * temp2[k];
		}
	}
}

void multiplyNotVectorized(int** mx1, int** mx2, int** mx3, int matrixSize) {
	int *temp = nullptr;
	int temp1 = 0;
	int *temp2 = nullptr;
	uint32_t size = matrixSize;
	for (uint32_t i = 0; i < size; ++i) {
		temp = mx3[i];
		for (uint32_t j = 0; j < size; ++j) {
			temp1 = mx1[i][j];
			temp2 = mx2[j];
			#pragma loop(no_vector)  
			for (uint32_t k = 0; k < size; ++k)
				temp[k] += temp1 * temp2[k];
		}
	}
}

void multiplySSE(int** mx1, int** mx2, int** mx3, int matrixSize) {
	for (int j = 0; j < matrixSize; j++) {
		for (int i = 0; i < matrixSize; i++) {
			__m128i result;
			__m128i *rmx = (__m128i*)&mx3[i][0];
			result = _mm_set1_epi32(mx1[i][j]);	// Broadcast number from A matrix to low addresses.
			result = _mm_mullo_epi32(result, *(__m128i*)&mx2[j][0]);	// Multiply
			*rmx = _mm_add_epi32(result, *rmx);
		}
	}
}

void matrixTest() {
	const int matrixSize = 3;
	Matrix matrix1(0);
	Matrix matrix2(matrix1.getLastGeneratedNumber());
	Matrix matrix3(0);
	int **mx1, **mx2, **mx3;
	mx1 = matrix1.getPointer();
	mx2 = matrix2.getPointer();
	mx3 = matrix3.getPointer();

	cout << "Matrix 1" << endl;
	show(mx1, matrixSize);

	cout << "Matrix 2" << endl;
	show(mx2, matrixSize);

	cout << "Result" << endl;
	multiply(mx1, mx2, mx3, matrixSize);
	show(mx3, matrixSize);
}

int main() {
	cout << "Sizeof int: " << sizeof(int) << endl;
	int bigMatrixSize = 1;
	int smallMatrixSize = 1024;
	QuadroMatrix qmx1(bigMatrixSize, smallMatrixSize);
	QuadroMatrix qmx2(bigMatrixSize, smallMatrixSize);
	QuadroMatrix qmx3(bigMatrixSize, smallMatrixSize, true);
	QuadroMatrix qmx4(bigMatrixSize, smallMatrixSize, true);
	QuadroMatrix qmx5(bigMatrixSize, smallMatrixSize, true);

	cout << "QMatrix1: " << endl;
	//qmx1.show();
	cout << endl;

	cout << "QMatrix2: " << endl;
	//qmx2.show();
	cout << endl;

	int ****qm1, ****qm2, ****qm3, ****qm4, ****qm5;
	qm1 = qmx1.getPointer();
	qm2 = qmx2.getPointer();
	qm3 = qmx3.getPointer();
	qm4 = qmx4.getPointer();
	qm5 = qmx5.getPointer();
	
	DWORD startTime, endTime;
	DWORD simpleMultiplyTimeTest, vectorizedMultiplyTimeTest;

	/************************************
	*
	*	TEST: Common multiply
	*
	*************************************/
	cout << "Simple multiply: " << endl;
	startTime = GetTickCount();

	for (int i = 0; i < bigMatrixSize; i++) {
		for (int j = 0; j < bigMatrixSize; j++) {
			multiply(qm1[i][j], qm2[i][j], qm3[i][j], smallMatrixSize);
		}
	}

	cout << "Result: " << endl;
	endTime = GetTickCount();
	simpleMultiplyTimeTest = endTime - startTime;
	cout << "Tick count: " << simpleMultiplyTimeTest << endl;

	/************************************
	*
	*	TEST: Vectorized multiply
	*
	*************************************/
	cout << "Vectorized multiply: " << endl;
	startTime = GetTickCount();

	for (int i = 0; i < bigMatrixSize; i++) {
		for (int j = 0; j < bigMatrixSize; j++) {
			multiplyVectorized(qm1[i][j], qm2[i][j], qm4[i][j], smallMatrixSize);
		}
	}

	cout << "Result: " << endl;
	endTime = GetTickCount();
	vectorizedMultiplyTimeTest = endTime - startTime;
	cout << "Tick count: " << vectorizedMultiplyTimeTest << endl;
	cout << "Total speed up: x" << (double)(simpleMultiplyTimeTest / vectorizedMultiplyTimeTest) << endl;

	/************************************
	*
	*	TEST: Not vectorized multiply
	*
	*************************************/
	cout << "Vectorized multiply: " << endl;
	startTime = GetTickCount();

	for (int i = 0; i < bigMatrixSize; i++) {
		for (int j = 0; j < bigMatrixSize; j++) {
			multiplyNotVectorized(qm1[i][j], qm2[i][j], qm4[i][j], smallMatrixSize);
		}
	}

	cout << "Result: " << endl;
	endTime = GetTickCount();
	vectorizedMultiplyTimeTest = endTime - startTime;
	cout << "Tick count: " << vectorizedMultiplyTimeTest << endl;

	system("pause");
    return 0;
}
