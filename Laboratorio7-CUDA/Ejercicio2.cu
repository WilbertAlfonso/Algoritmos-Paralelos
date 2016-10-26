#include <stdio.h>
#include <device_launch_parameters.h>
#include <cuda_runtime.h>
#include <cuda.h>
#include <Windows.h>
#include <iostream>

using namespace std;
__global__
void vecMultKernel(float* A, float* B, float* C, int n){
	int i = threadIdx.x + blockDim.x * blockIdx.x;
	if (i<n){
		C[i] = 0;
		for (int j = 0; j<n; j++)
			C[i] += A[j*n + i] * B[j];
	}
}
void matrizXescalar(float* A, float* B, float* C, int n) {
	int size = n * n * sizeof(float);
	int sizevect = n * sizeof(float);
	float *d_A, *d_B, *d_C;
	cudaMalloc((void **)&d_A, size);
	cudaMemcpy(d_A, A, size, cudaMemcpyHostToDevice);
	cudaMalloc((void **)&d_B, sizevect);
	cudaMemcpy(d_B, B, sizevect, cudaMemcpyHostToDevice);
	cudaMalloc((void **)&d_C, sizevect);
	vecMultKernel <<< ceil((n*n) / 256.0), 256 >>> (d_A, d_B, d_C, n);
	cudaMemcpy(C, d_C, sizevect, cudaMemcpyDeviceToHost);
	cudaFree(d_A); cudaFree(d_B); cudaFree(d_C);
}
void mostrarM(float* matriz, float fila, float columna)
{
	for (int x = 0; x < fila; x++)
	{
		for (int y = 0; y < columna; y++)
		{
			int puesto = x*columna + y;
			printf("%3.0f ", matriz[puesto]);
		}
		printf("\n");
	}
}
int main() {
	int fila = 3;
	int columna = 3;
	float* A = (float*)malloc(fila*columna*sizeof(float));
	float* B = (float*)malloc(fila*sizeof(float));
	float* C = (float*)malloc(fila*sizeof(float));
	for (int i = 0; i < fila*columna; i++)
	{
		A[i] = i;
	}
	for (int i = 0; i < fila; i++)
	{
		B[i] = i;
	}
	mostrarM(A, fila, columna);
	cout << endl;
	mostrarM(B, 1, columna);
	matrizXescalar(A, B, C, fila);
	mostrarM(C, 1, columna);
	system("PAUSE");
	exit(0);
}
