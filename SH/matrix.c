#include "matrix.h"
#include <stdio.h>

float**** matrixAlloc(int lsize, int bsize)
{
	float ****matrix = (float****)malloc(bsize * sizeof(float***));
	for (int i = 0; i < bsize; i++) 
	{
		matrix[i] = (float***)malloc(bsize * sizeof(float**));
		for (int j = 0; j < bsize; j++) 
		{
			matrix[i][j] = (float**)malloc(lsize * sizeof(float*));
			for (int k = 0; k < lsize; k++) 
			{
				matrix[i][j][k] = (float*)malloc(lsize * sizeof(float));
				for (int l = 0; l < lsize; l++)
 					matrix[i][j][k][l] = .0;
			}
		}
	}
	return matrix;
}

void matrixGenerate(float ****m, int lsize, int bsize)
{
	srand((unsigned)time(NULL));
	for (int i = 0; i < bsize; i++) {
		for (int j = 0; j < bsize; j++) {
			for (int k = 0; k < lsize; k++) {
				for (int l = 0; l < lsize; l++) {
					m[i][j][k][l] = (float)((rand()%100)/10);
				}
			}
		}
	}
}

bool equals(float ****m1, float ****m2, int lsize, int bsize)
{
	for (int i = 0; i < bsize; i++) {
		for (int j = 0; j < bsize; j++) {
			for (int k = 0; k < lsize; k++) {
				for (int l = 0; l < lsize; l++)
                {
                    //printf("%f     %f",m1[i][j][k][l], m2[i][j][k][l] );
					if (m1[i][j][k][l] != m2[i][j][k][l])
						return false;
				}
			}
		}
	}
	return true;
}

void matrixMultiply(float ****m1, float ****m2, float ****result, int lsize, int bsize)
{
	for (int i = 0; i < bsize; i++)
		for (int j = 0; j < bsize; j++)
			for (int k = 0; k < lsize; k++)
				for (int l = 0; l < lsize; l++)
					for (int m = 0; m < lsize; m++)
						result[i][j][k][l] += m1[i][j][k][m] * m2[i][j][m][l];
}


void matrixMultiplySSE(float ****m1, float ****m2, float ****result, int lsize, int bsize)
{
    for (int m = 0; m < bsize; m++) {
        for (int n = 0; n < bsize; n++) {
            for (int i = 0; i < lsize; i++)
            {
                float  *temp = result[m][n][i];
                for (int j = 0; j < lsize; j++)
                {
                    float temp1 = m1[m][n][i][j];
                    float *temp2 = m2[m][n][j];
                    __m128 t1 = _mm_set1_ps(temp1);
                    
                    for (int k = 0; k < lsize; k += 4)
                    {
                        __m128 t0 = _mm_load_ps(temp + k);
                        __m128 t2 = _mm_load_ps(temp2 + k);
                        t0 = _mm_add_ps(t0, _mm_mul_ps(t1,t2));
                        _mm_store_ps(temp + k, t0);
                    }
                }
            }
        }
    }
}
