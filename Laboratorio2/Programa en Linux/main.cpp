#include <iostream>
#include <stdio.h>
#include <time.h> //--------->
#include <sys/time.h> //----->Si desea medir el tiempo en linux.
//#include <windows.h> //Comentar en caso desee medir tiempo en linux.
#define MAX 100
using namespace std;
//-------------Tiempo en Linux----------------------
double timeval_diff(struct timeval *a, struct timeval *b)
{
 return
   (double)(a->tv_sec + (double)a->tv_usec/1000000) -
   (double)(b->tv_sec + (double)b->tv_usec/1000000);
}
//-------------------Tiempo en Windows-----------------
/*double performancecounter_diff(LARGE_INTEGER *a, LARGE_INTEGER *b)
{
  LARGE_INTEGER freq;
  QueryPerformanceFrequency(&freq);
  return (double)(a->QuadPart - b->QuadPart) / (double)freq.QuadPart;
}*/

void Mul(double **X,double **Y,double **Res)
{
    for(int i=0;i<MAX;i++)
    {
        for(int j=0;j<MAX;j++)
        {
            for(int k=0;k<MAX;k++)
                Res[i][j]+=X[i][k]*Y[k][j];
        }
    }

}
void MulBloq(double **X,double **Y,double **Res)
{
    int tb=MAX/10;
    for(int i=0;i<MAX;i+=tb)
    {
        for(int j=0;j<MAX;j+=tb)
        {
            for(int k=0;k<MAX;k+=tb)
                {
                    for(int i2=i;i2<i+tb;i2++)
                    {
                        for(int j2=j;j2<j+tb;j2++)
                        {
                            for(int k2=k;k2<k+tb;k2++)
                                Res[i2][j2]+=X[i2][k2]*Y[k2][j2];
                        }
                    }
                }
        }
    }

}
int main()
{
    double **A=new double*[MAX];
    for(int i=0;i<MAX;i++)
        A[i]=new double[MAX];
    double **B=new double*[MAX];
    for(int i=0;i<MAX;i++)
        B[i]=new double[MAX];
    double **Res=new double*[MAX];
    for(int i=0;i<MAX;i++)
        Res[i]=new double[MAX];
    for(int i=0;i<MAX;i++)
    {
        for(int j=0;j<MAX;j++)
        {
            A[i][j]=i*j;
            B[i][j]=i*j;
            Res[i][j]=0;
        }
    }
    //Tiempo en Linux
    struct timeval t_ini, t_fin;
    //LARGE_INTEGER t_ini, t_fin;
    double secs;
    gettimeofday(&t_ini, NULL);
    //QueryPerformanceCounter(&t_ini);
    //Mul(A,B,Res);
    MulBloq(A,B,Res);
    //QueryPerformanceCounter(&t_fin);
    gettimeofday(&t_fin, NULL);
    secs = timeval_diff(&t_fin, &t_ini);
    //secs = performancecounter_diff(&t_fin, &t_ini);
    printf("%.16g milliseconds\n", secs * 1000.0);
    delete[] A;
    delete[] B;
    delete[] Res;
    return 0;
}
