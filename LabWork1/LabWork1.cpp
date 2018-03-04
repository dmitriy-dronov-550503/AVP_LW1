// LabWork1.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "QuadroMatrix.h"
#include "Matrix.h"
#include "MatrixMultiply.h"
#include <iomanip>

#define ENABLE_COMMON_MULTIPLY
#define ENABLE_NOT_VECTORIZED_MULTIPLY
#define ENABLE_VECTORIZED_MULTIPLY
#define ENABLE_SSE_MULTIPLY
#define ENABLE_AVX_MULTIPLY

void show(int** matrix, int matrixSize) {
	for (int i = 0; i < matrixSize; i++) {
		for (int j = 0; j < matrixSize; j++) {
			cout << matrix[i][j] << "  ";
		}
		cout << endl;
	}
}

void compare(int**** qm1, int**** qm2, int bigMatrixSize, int smallMatrixSize) {
	for (int i = 0; i < bigMatrixSize; i++) {
		for (int j = 0; j < bigMatrixSize; j++) {
			MatrixMultiply::compare(qm1[i][j], qm2[i][j], smallMatrixSize);
		}
	}
}

int main() {
	int bigMatrixSize = 4; //16
	int smallMatrixSize = 512; //256
	QuadroMatrix qmx1(bigMatrixSize, smallMatrixSize);
	QuadroMatrix qmx2(bigMatrixSize, smallMatrixSize);
	QuadroMatrix qmx3(bigMatrixSize, smallMatrixSize, true);
	QuadroMatrix qmx4(bigMatrixSize, smallMatrixSize, true);

	int ****qm1, ****qm2, ****qm3, ****qm4;
	qm1 = qmx1.getPointer();
	qm2 = qmx2.getPointer();
	qm3 = qmx3.getPointer();
	qm4 = qmx4.getPointer();

	ULONGLONG startTime, endTime;
	ULONGLONG simpleMultiplyTimeTest, vectorizedMultiplyTimeTest;

#ifdef ENABLE_COMMON_MULTIPLY
	/************************************
	*
	*	TEST: Common multiply
	*
	*************************************/
	cout << "Common multiply: " << endl;
	startTime = GetTickCount64();

	for (int i = 0; i < bigMatrixSize; i++) {
		for (int j = 0; j < bigMatrixSize; j++) {
			MatrixMultiply::multiply(qm1[i][j], qm2[i][j], qm3[i][j], smallMatrixSize);
		}
	}
	endTime = GetTickCount64();
	simpleMultiplyTimeTest = endTime - startTime;
	cout << "Tick count: " << simpleMultiplyTimeTest << endl << endl;

	//qmx3.show();
#endif

#ifdef ENABLE_NOT_VECTORIZED_MULTIPLY
	/************************************
	*
	*	TEST: Not vectorized multiply
	*
	*************************************/
	qmx4.clear();
	cout << "Not vectorized multiply: " << endl;
	startTime = GetTickCount64();

	for (int i = 0; i < bigMatrixSize; i++) {
		for (int j = 0; j < bigMatrixSize; j++) {
			MatrixMultiply::multiplyNotVectorized(qm1[i][j], qm2[i][j], qm4[i][j], smallMatrixSize);
		}
	}

	endTime = GetTickCount64();
	simpleMultiplyTimeTest = endTime - startTime;
	cout << "Tick count: " << simpleMultiplyTimeTest << endl << endl;

#endif

#ifdef ENABLE_VECTORIZED_MULTIPLY
	/************************************
	*
	*	TEST: Vectorized multiply
	*
	*************************************/
	qmx4.clear();
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

	compare(qm3, qm4, bigMatrixSize, smallMatrixSize);
#endif

#ifdef ENABLE_SSE_MULTIPLY
	/************************************
	*
	*	TEST: SSE multiply
	*
	*************************************/
	qmx4.clear();
	cout << "Manual SSE vectorized multiply: " << endl;
	startTime = GetTickCount64();

	for (int i = 0; i < bigMatrixSize; i++) {
		for (int j = 0; j < bigMatrixSize; j++) {
			MatrixMultiply::multiplySse(qm1[i][j], qm2[i][j], qm4[i][j], smallMatrixSize);
		}
	}

	endTime = GetTickCount64();
	vectorizedMultiplyTimeTest = endTime - startTime;
	cout << "Tick count: " << vectorizedMultiplyTimeTest << endl << endl;

	//qmx4.show();

	compare(qm3, qm4, bigMatrixSize, smallMatrixSize);
#endif

#ifdef ENABLE_AVX_MULTIPLY
	/************************************
	*
	*	TEST: AVX multiply
	*
	*************************************/
	qmx4.clear();
	cout << "Manual AVX vectorized multiply: " << endl;
	startTime = GetTickCount64();

	for (int i = 0; i < bigMatrixSize; i++) {
		for (int j = 0; j < bigMatrixSize; j++) {
			MatrixMultiply::multiplyAvx(qm1[i][j], qm2[i][j], qm4[i][j], smallMatrixSize);
		}
	}

	endTime = GetTickCount64();
	vectorizedMultiplyTimeTest = endTime - startTime;
	cout << "Tick count: " << vectorizedMultiplyTimeTest << endl << endl;

	//qmx4.show();

	compare(qm3, qm4, bigMatrixSize, smallMatrixSize);
#endif

	system("pause");
    return 0;
}
