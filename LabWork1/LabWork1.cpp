// LabWork1.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "QuadroMatrix.h"
#include "Matrix.h"
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

int main()
{
	int bigMatrixSize = 3;
	int smallMatrixSize = 3;
	QuadroMatrix qmx1(bigMatrixSize, smallMatrixSize);
	QuadroMatrix qmx2(bigMatrixSize, smallMatrixSize);
	QuadroMatrix qmx3(bigMatrixSize, smallMatrixSize);

	cout << "QMatrix1: " << endl;
	qmx1.show();
	cout << endl;

	cout << "QMatrix2: " << endl;
	qmx2.show();
	cout << endl;

	int ****qm1, ****qm2, ****qm3;
	qm1 = qmx1.getPointer();
	qm2 = qmx2.getPointer();
	qm3 = qmx3.getPointer();

	for (int i = 0; i < bigMatrixSize; i++) {
		for (int j = 0; j < bigMatrixSize; j++) {
			multiply(qm1[i][j], qm2[i][j], qm3[i][j], smallMatrixSize);
		}
	}
	cout << "Result: " << endl;
	qmx3.show();

	system("pause");
    return 0;
}

