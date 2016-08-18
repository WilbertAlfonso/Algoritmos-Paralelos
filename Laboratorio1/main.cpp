#include <iostream>
#include   <cstdlib>
#include   <ctime>
#include   <sys/timeb.h>

using namespace std;

int main()
{

    double **A= new double*[10000];
    for(int i=0;i<10000;i++)
        A[i]=new double[10000];
    double *x=new double[10000];
    double *y=new double[10000];
    for(int p=0;p<10000;p++)
    {
        x[p]=20;
        for(int l=0;l<10000;l++)
            A[p][l]=20;
    }
    clock_t startC, finishC;
    struct timeb startT, finishT;
    unsigned int seconds, milliseconds;
    startC = clock();
    ftime(&startT);

    for(int i=0; i<10000;i++)
        for(int j=0; j<10000;j++)
            y[i]+=A[i][j]*x[j];

    finishC = clock();
    ftime(&finishT);
    seconds = finishT.time - startT.time - 1;
    milliseconds = (1000 - startT.millitm) + finishT.millitm;

    cout << "Time 1 (clock): "    << (finishC - startC)/CLOCKS_PER_SEC << endl;
    cout << "Time 1 (difftime): " << difftime(finishC, startC) << endl;
    cout << "Time 1 (ftime): " << (milliseconds + seconds * 1000) << endl;

    startC = clock();
    ftime(&startT);
    for(int j=0; j<10000;j++)
        for(int i=0; i<10000;i++)
            y[i]+=A[i][j]*x[j];

    finishC = clock();
    ftime(&finishT);
    seconds = finishT.time - startT.time - 1;
    milliseconds = (1000 - startT.millitm) + finishT.millitm;

    cout << "Time 2 (clock): "    << (finishC - startC)/CLOCKS_PER_SEC << endl;
    cout << "Time 2 (difftime): " << difftime(finishC, startC) << endl;
    cout << "Time 2 (ftime): " << (milliseconds + seconds * 1000) << endl;
    return 0;
}
