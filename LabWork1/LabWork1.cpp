// LabWork1.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "QuadroMatrix.h"
#include "Matrix.h"
#include "MatrixMultiply.h"
#include <iostream>
#include <iomanip>
using namespace std;
#define ENABLE_COMMON_MULTIPLY
//#define ENABLE_VECTORIZED_MULTIPLY
//#define ENABLE_NOT_VECTORIZED_MULTIPLY
#define ENABLE_SSE_MULTIPLY


void show(int** matrix, int matrixSize) {
	for (int i = 0; i < matrixSize; i++) {
		for (int j = 0; j < matrixSize; j++) {
			cout << matrix[i][j] << "  ";
		}
		cout << endl;
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
	MatrixMultiply::multiply(mx1, mx2, mx3, matrixSize);
	show(mx3, matrixSize);
}

int main() {
	cout << "Sizeof int: " << sizeof(int) << endl;
	int bigMatrixSize = 1;
	int smallMatrixSize = 4;
	QuadroMatrix qmx1(bigMatrixSize, smallMatrixSize);
	QuadroMatrix qmx2(bigMatrixSize, smallMatrixSize);
	QuadroMatrix qmx3(bigMatrixSize, smallMatrixSize, true);
	QuadroMatrix qmx4(bigMatrixSize, smallMatrixSize, true);
	QuadroMatrix qmx5(bigMatrixSize, smallMatrixSize, true);
	QuadroMatrix qmx6(bigMatrixSize, smallMatrixSize, true);

	cout << "QMatrix1: " << endl;
	//qmx1.show();
	cout << endl;

	cout << "QMatrix2: " << endl;
	//qmx2.show();
	cout << endl;

	int ****qm1, ****qm2, ****qm3, ****qm4, ****qm5, ****qm6;
	qm1 = qmx1.getPointer();
	qm2 = qmx2.getPointer();
	qm3 = qmx3.getPointer();
	qm4 = qmx4.getPointer();
	qm5 = qmx5.getPointer();
	qm6 = qmx6.getPointer();
	
	ULONGLONG startTime, endTime;
	ULONGLONG simpleMultiplyTimeTest, vectorizedMultiplyTimeTest;

#ifdef ENABLE_COMMON_MULTIPLY
	/************************************
	*
	*	TEST: Common multiply
	*
	*************************************/
	cout << "Simple multiply: " << endl;
	startTime = GetTickCount64();

	for (int i = 0; i < bigMatrixSize; i++) {
		for (int j = 0; j < bigMatrixSize; j++) {
			MatrixMultiply::multiply(qm1[i][j], qm2[i][j], qm3[i][j], smallMatrixSize);
		}
	}
	endTime = GetTickCount64();
	simpleMultiplyTimeTest = endTime - startTime;
	cout << "Tick count: " << simpleMultiplyTimeTest << endl << endl;
	//show(qm3[0][0], smallMatrixSize);
#endif

#ifdef ENABLE_VECTORIZED_MULTIPLY
	/************************************
	*
	*	TEST: Vectorized multiply
	*
	*************************************/
	cout << "Vectorized multiply: " << endl;
	startTime = GetTickCount64();

	for (int i = 0; i < bigMatrixSize; i++) {
		for (int j = 0; j < bigMatrixSize; j++) {
			MatrixMultiply::multiplyVectorized(qm1[i][j], qm2[i][j], qm4[i][j], smallMatrixSize);
		}
	}

	endTime = GetTickCount64();
	vectorizedMultiplyTimeTest = endTime - startTime;
	cout << "Tick count: " << vectorizedMultiplyTimeTest << endl;
	cout << "Total speed up: x" << setprecision(3) << (double)simpleMultiplyTimeTest / (double)vectorizedMultiplyTimeTest << endl << endl;
#endif

#ifdef ENABLE_NOT_VECTORIZED_MULTIPLY
	/************************************
	*
	*	TEST: Not vectorized multiply
	*
	*************************************/
	cout << "Not vectorized multiply: " << endl;
	startTime = GetTickCount64();

	for (int i = 0; i < bigMatrixSize; i++) {
		for (int j = 0; j < bigMatrixSize; j++) {
			MatrixMultiply::multiplyNotVectorized(qm1[i][j], qm2[i][j], qm5[i][j], smallMatrixSize);
		}
	}

	endTime = GetTickCount64();
	vectorizedMultiplyTimeTest = endTime - startTime;
	cout << "Tick count: " << vectorizedMultiplyTimeTest << endl << endl;
#endif

#ifdef ENABLE_SSE_MULTIPLY
	/************************************
	*
	*	TEST: SSE multiply
	*
	*************************************/
	cout << "Not vectorized multiply: " << endl;
	startTime = GetTickCount64();

	for (int i = 0; i < bigMatrixSize; i++) {
		for (int j = 0; j < bigMatrixSize; j++) {
			MatrixMultiply::multiplySSE(qm1[i][j], qm2[i][j], qm6[i][j], smallMatrixSize);
		}
	}

	endTime = GetTickCount64();
	vectorizedMultiplyTimeTest = endTime - startTime;
	cout << "Tick count: " << vectorizedMultiplyTimeTest << endl << endl;
	//show(qm6[0][0], smallMatrixSize);
#endif

	// Right caclculaions control
#ifdef ENABLE_VECTORIZED_MULTIPLY
	for (int i = 0; i < bigMatrixSize; i++) {
		for (int j = 0; j < bigMatrixSize; j++) {
			MatrixMultiply::compare(qm3[i][j], qm4[i][j], smallMatrixSize);
		}
	}
#endif
#ifdef ENABLE_NOT_VECTORIZED_MULTIPLY
	for (int i = 0; i < bigMatrixSize; i++) {
		for (int j = 0; j < bigMatrixSize; j++) {
			MatrixMultiply::compare(qm3[i][j], qm5[i][j], smallMatrixSize);
		}
	}
#endif
#ifdef ENABLE_SSE_MULTIPLY
	for (int i = 0; i < bigMatrixSize; i++) {
		for (int j = 0; j < bigMatrixSize; j++) {
			MatrixMultiply::compare(qm3[i][j], qm6[i][j], smallMatrixSize);
		}
	}
#endif

	system("pause");
    return 0;
}
