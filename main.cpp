#include <iostream>
#include <omp.h>
#include <windows.h>
#include "parallizeoptions.h"

using namespace std;

void display(int N, int** a, int** b){
    cout<<"Первая матрица"<<endl;
    for (int i = 0; i < N; ++i){
        for (int j = 0; j < N; ++j){
            cout<<a[i][j]<<" ";
        }
        cout<<endl;
    }
    cout<<"Вторая матрица"<<endl;
    for (int i = 0; i < N; ++i){
        for (int j = 0; j < N; ++j){
            cout<<b[i][j]<<" ";
        }
        cout<<endl;
    }
}

int** generateMatrix(int N){
    int** m = new int*[N];
    for (int i = 0; i < N; ++i){
        m[i] = new int[N];
    }
    for (int i = 0; i < N; ++i){
        for (int j = 0; j < N; ++j)
            m[i][j] = rand() % 10 + 1;
    }
    return m;
}

void perform(int N, int **a, int **b, Base* o){
    double startTime, endTime, maxTime, minTime, oneTime, avgTime = 0.0, sum;
    o -> getName();
    startTime = omp_get_wtime();
    sum = o->calc(N, a, b);
    endTime = omp_get_wtime();
    maxTime = minTime = endTime - startTime;
    for (int i = 0; i < 9; ++i){
        startTime = omp_get_wtime();
        sum = o->calc(N, a, b);
        endTime = omp_get_wtime();
        oneTime = endTime - startTime;
        avgTime += oneTime;
        if (oneTime > maxTime)
            maxTime = oneTime;
        if (oneTime < minTime)
            minTime = oneTime;
    }
    cout << "Сумма " << sum <<"  Среднее время: " << avgTime / 10 << "  MAX: "<<maxTime<<"  MIN: "<<minTime <<endl;
}

int main() {

    SetConsoleOutputCP(CP_UTF8);
    int N;
    cout << "Введите N (размер матрицы)" <<endl;
    cin>>N;
    int** first = generateMatrix(N);
    int** second = generateMatrix(N);
//    display(N, first, second);
//    Serial serial;
//    serial.displayResults(N, first, second);

//    perform(N, first, second, new Serial());
//    perform(N, first, second, new Row());
//    perform(N, first, second, new Column());
//    perform(N, first, second, new Blocks());
//    perform(N, first, second, new RowScheduleStatic());
//    perform(N, first, second, new RowScheduleDynamic());
//    perform(N, first, second, new RowScheduleGuided());
    perform(N, first, second, new RowScheduleDynamicChunk());

    delete [] first;
    delete [] second;

    return 0;
}
