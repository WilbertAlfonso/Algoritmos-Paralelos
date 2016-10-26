#include <stdio.h>
#include <device_launch_parameters.h>
#include <cuda_runtime.h>
#include <cuda.h>
#include <Windows.h>
#include <iostream>
using namespace std;
#define BLOCK 10
__global__
void matrizXescalarK(float* A, float B, float* C, int f,int c,int e){
	int fila = blockIdx.y*blockDim.y + threadIdx.y;
	int columna = blockIdx.x*blockDim.x + threadIdx.x;
	int espacio = blockIdx.z*blockDim.z + threadIdx.z;
	if ((fila < f) && (columna < c) && (espacio < e)) {
		C[fila*c + columna + espacio*c*f] = B * A[fila*c + columna+espacio*c*f];
	}
}
void matrizXescalar(float* A, float B, float* C, int f,int c,int e) {
	int size = f * c * e *  sizeof(float);
	float *d_A, *d_C;
	cudaMalloc((void **)&d_A, size);
	cudaMemcpy(d_A, A, size, cudaMemcpyHostToDevice);
	cudaMalloc((void **)&d_C, size);
	int NumBlocks1 = c / BLOCK;
	int NumBlocks2 = f / BLOCK;
	int NumBlocks3 = e / BLOCK;
	if (c % BLOCK) NumBlocks1++;
	if (f % BLOCK) NumBlocks2++;
	if (e % BLOCK) NumBlocks3++;
	dim3 dime1(NumBlocks1, NumBlocks2, NumBlocks3);
	dim3 dime2(BLOCK, BLOCK, BLOCK);
	matrizXescalarK << < dime1, dime2 >> > (d_A, B, d_C, f, c,e);
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
	int fila = 10;
	int columna = 10;
	int espacio3 = 10;
	float* A = (float*)malloc(fila*columna*espacio3*sizeof(float));
	float B = 2;
	float* C = (float*)malloc(fila*columna*espacio3*sizeof(float));
	for (int i = 0; i < fila*columna*espacio3; i++)
	{
		A[i] = i;
	}
	mostrarM(A, fila, columna*espacio3);
	matrizXescalar(A, B, C, fila,columna,espacio3);
	mostrarM(C, fila, columna*espacio3);
	system("PAUSE");
	exit(0);
}
