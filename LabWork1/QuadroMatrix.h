#pragma once
#include "stdafx.h"
#include <iostream>
using namespace std;

class QuadroMatrix {
public:
	int bigMatrixSize;
	int smallMatrixSize;
	int ****qMatrix;

	QuadroMatrix(int bigMatrixSize=1, int smallMatrixSize=1) {
		this->bigMatrixSize = bigMatrixSize;
		this->smallMatrixSize = smallMatrixSize;
		allocate();
		fill();		
	}

	int**** getPointer() {
		return qMatrix;
	}

	void show() {
		for (int i = 0; i < bigMatrixSize; i++) {
			for (int j = 0; j < bigMatrixSize; j++) {
				cout << "Small matrix " << i << " " << j << endl;
				for (int k = 0; k < smallMatrixSize; k++) {
					for (int l = 0; l < smallMatrixSize; l++) {
						cout << *(*(*(*(qMatrix + i) + j) + k) + l) << "  ";
					}
					cout << endl;
				}
			}
		}
	}

	~QuadroMatrix() {
		for (int i = 0; i < bigMatrixSize; i++) {
			for (int j = 0; j < bigMatrixSize; j++) {
				for (int k = 0; k < smallMatrixSize; k++) {
					free(*(*(*(qMatrix + i) + j) + k));
				}
				free(*(*(qMatrix + i) + j));
			}
			free(*(qMatrix + i));
		}
		free(qMatrix);
	}

private:
	void allocate() {
		qMatrix = (int****)malloc(bigMatrixSize * sizeof(int));
		for (int i = 0; i < bigMatrixSize; i++) {
			*(qMatrix + i) = (int***)malloc(bigMatrixSize * sizeof(int));
			for (int j = 0; j < bigMatrixSize; j++) {
				*(*(qMatrix + i) + j) = (int**)malloc(smallMatrixSize * sizeof(int));
				for (int k = 0; k < smallMatrixSize; k++) {
					*(*(*(qMatrix + i) + j) + k) = (int*)malloc(smallMatrixSize * sizeof(int));
				}
			}
		}
	}

	void fill() {
		for (int i = 0; i < bigMatrixSize; i++) {
			for (int j = 0; j < bigMatrixSize; j++) {
				for (int k = 0; k < smallMatrixSize; k++) {
					for (int l = 0; l < smallMatrixSize; l++) {
						*(*(*(*(qMatrix + i) + j) + k) + l) = l + k * smallMatrixSize + j * bigMatrixSize*smallMatrixSize + i * bigMatrixSize*smallMatrixSize*smallMatrixSize;
					}
				}
			}
		}
	}
};