#pragma once
#include "stdafx.h"
#include <iostream>
using namespace std;

class Matrix {

public:
	int fillPosition;
	const int matrixSize = 3;
	int** dMatrix;

	Matrix(int startGenerateNumber) {
		fillPosition = startGenerateNumber;
		dMatrix = (int**)malloc(matrixSize * sizeof(int));
		if (dMatrix == nullptr) throw "Memory allocation failed.";
		for (int i = 0; i < matrixSize; i++) {
			*(dMatrix + i) = (int*)malloc(matrixSize * sizeof(int));
		}
		fill();
	}

	int** getPointer() {
		return dMatrix;
	}

	int getLastGeneratedNumber() {
		return fillPosition;
	}

	void fill() {
		for (int i = 0; i < matrixSize; i++) {
			for (int j = 0; j < matrixSize; j++) {
				dMatrix[i][j] = fillPosition++;
			}
		}
	}

	void fillRandom() {
		for (int i = 0; i < matrixSize; i++) {
			for (int j = 0; j < matrixSize; j++) {
				dMatrix[i][j] = rand() % 100;
			}
		}
	}

	~Matrix() {
		for (int i = 0; i < matrixSize; i++) {
			free(*(dMatrix + i));
		}
		free(dMatrix);
	}
};