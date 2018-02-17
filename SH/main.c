//Улучшить обработку памяти (структоризовать память)

#include <stdio.h>
#include <stdlib.h>
#include "matrix.h"


#define K 12
#define M 300

int main()
{
	uint64_t clkStart, clkEnd, r1, r2;
	float ****m1, ****m2, ****m3, ****m4;
	m1 = matrixAlloc(K, M);
	m2 = matrixAlloc(K, M);
	m3 = matrixAlloc(K, M);
	m4 = matrixAlloc(K, M);
	matrixGenerate(m1, K, M);
	matrixGenerate(m2, K, M);
	
	//vector
	clkStart = __rdtsc(); //получение времени начала в тактах
	matrixMultiply(m1, m2, m3, K, M);
	clkEnd = __rdtsc(); //получение времени окончания перемножения
	r1 = clkStart - clkEnd;
	printf("Usual multiplication tact count: %" PRIu64 "\n", r1);
	
	//SSE
	clkStart = __rdtsc();
	matrixMultiplySSE(m1, m2, m4, K, M);
	clkEnd = __rdtsc();
	r2 = clkEnd - clkStart;
	printf("SIMD multiplication tact count: %" PRIu64 "\n", r2);
	
	printf("r1/r2 = %f\n", (double)r1/r2);
	equals(m3, m4, K, M) ? printf("Result matrixes are equals\n") 
		: printf("Result matrixes are not equals\n");

	system("PAUSE");
	return 0;
}
