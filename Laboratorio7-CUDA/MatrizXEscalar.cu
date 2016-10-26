#include <stdio.h>
#include <device_launch_parameters.h>
#include <cuda_runtime.h>
#include <cuda.h>
#include <Windows.h>
#include <iostream>
using namespace std;
#define BLOCK 16
__global__
void matrizXescalarK(float* A, float B, float* C, int f,int c){
	int fila = blockIdx.y*blockDim.y + threadIdx.y;
	int columna = blockIdx.x*blockDim.x + threadIdx.x;
	if ((fila < f) && (columna < c)) {
		C[fila*c + columna] = B * A[fila*c + columna];
	}
}
void matrizXescalar(float* A, float B, float* C, int f,int c) {
	int size = f * c * sizeof(float);
	float *d_A, *d_C;
	cudaMalloc((void **)&d_A, size);
	cudaMemcpy(d_A, A, size, cudaMemcpyHostToDevice);
	cudaMalloc((void **)&d_C, size);
	int NumBlocks1 = c / BLOCK;
	int NumBlocks2 = f / BLOCK;
	if (c % BLOCK) NumBlocks1++;
	if (f%BLOCK) NumBlocks2++;
	dim3 dime1(NumBlocks1, NumBlocks2);
	dim3 dime2(BLOCK, BLOCK);
	matrizXescalarK << < dime1, dime2 >> > (d_A, B, d_C, f, c);
	cudaMemcpy(C, d_C, size, cudaMemcpyDeviceToHost);
	cudaFree(d_A); cudaFree(d_C);
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
	int fila = 17;
	int columna = 17;
	int espacio3 = 7;
	float* A = (float*)malloc(fila*columna*sizeof(float));
	float B = 2;
	float* C = (float*)malloc(fila*columna*sizeof(float));
	for (int i = 0; i < fila*columna; i++)
	{
		A[i] = i;
	}
	mostrarM(A, fila, columna);
	matrizXescalar(A, B, C, fila,columna);
	mostrarM(C, fila, columna);
	system("PAUSE");
	exit(0);
}
