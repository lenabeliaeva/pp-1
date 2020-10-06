//
// Created by lenab on 02.10.2020.
//

#include <omp.h>

#ifndef PP_P_H
#define PP_P_H

using namespace std;

class Base {
public:
    virtual double calc(int N, int **a, int **b) = 0;

    virtual void getName() = 0;

protected:
    static int **initResultMatrix(int N) {
        int **result = new int *[N];
        for (int i = 0; i < N; ++i) {
            result[i] = new int[N];
        }
        return result;
    }
};

class Serial : public Base {
    int **result;
    double sum = 0.0;

    double calc(int N, int **a, int **b) override {
        result = initResultMatrix(N);
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                result[i][j] = 0;
                for (int k = 0; k < N; ++k) {
                    result[i][j] += a[i][k] * b[k][j];
                }
                sum += result[i][j];
            }
        }
        return sum;
    }

public:
    void displayResults(int N, int **a, int **b) {
        calc(N, a, b);
        cout << "Произведение матриц" << endl;
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                cout << result[i][j] << " ";
            }
            cout << endl;
        }
        cout << "Сумма элементов результирующего массива " << sum << endl;
    }

    void getName() override {
        cout << "Последовательно   ";
    }
};

class Row : public Base {

    double calc(int N, int **a, int **b) override {
        double sum = 0;
        int **result = initResultMatrix(N);
#pragma omp parallel for reduction(+: sum)
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                result[i][j] = 0;
                for (int k = 0; k < N; ++k) {
                    result[i][j] += a[i][k] * b[k][j];
                }
                sum += result[i][j];
            }
        }
        return sum;
    }

    void getName() override {
        cout << "По строкам    ";
    }
};

class Column : public Base {
    double calc(int N, int **a, int **b) override {
        int j;
        double sum = 0.0;
        int **result = initResultMatrix(N);
        for (int i = 0; i < N; ++i) {
#pragma omp parallel for private(j) reduction(+: sum)
            for (j = 0; j < N; ++j) {
                result[i][j] = 0;
                for (int k = 0; k < N; ++k) {
                    result[i][j] += a[i][k] * b[k][j];
                }
                sum += result[i][j];
            }
        }
        return sum;
    }

    void getName() override {
        cout << "По столбцам   ";
    }
};

class Blocks : public Base {
    double calc(int N, int **a, int **b) override {
        int i, j;
        double sum = 0.0;
        int **result = initResultMatrix(N);
        omp_set_nested(true);
#pragma omp parallel for reduction(+: sum)
        for (i = 0; i < N; ++i) {
#pragma omp parallel for private(j) reduction(+: sum)
            for (j = 0; j < N; ++j) {
                result[i][j] = 0;
                for (int k = 0; k < N; ++k) {
                    result[i][j] += a[i][k] * b[k][j];
                }
                sum += result[i][j];
            }
        }
        return sum;
    }

    void getName() override {
        cout << "По блокам   ";
    }
};

class RowScheduleStatic : public Base {
    double calc(int N, int **a, int **b) override {
        double sum = 0;
        int **result = initResultMatrix(N);
#pragma omp parallel for schedule(static) reduction(+: sum)
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                result[i][j] = 0;
                for (int k = 0; k < N; ++k) {
                    result[i][j] += a[i][k] * b[k][j];
                }
                sum += result[i][j];
            }
        }
        return sum;
    }

    void getName() override {
        cout << "По строкам с static расписанием   ";
    }
};

class RowScheduleDynamic : public Base {
    double calc(int N, int **a, int **b) override {
        double sum = 0;
        int **result = initResultMatrix(N);
#pragma omp parallel for schedule(dynamic) reduction(+: sum)
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                result[i][j] = 0;
                for (int k = 0; k < N; ++k) {
                    result[i][j] += a[i][k] * b[k][j];
                }
                sum += result[i][j];
            }
        }
        return sum;
    }

    void getName() override {
        cout << "По строкам с dynamic расписанием ";
    }
};

class RowScheduleGuided : public Base {
    double calc(int N, int **a, int **b) override {
        double sum = 0;
        int **result = initResultMatrix(N);
#pragma omp parallel for schedule(guided) reduction(+: sum)
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                result[i][j] = 0;
                for (int k = 0; k < N; ++k) {
                    result[i][j] += a[i][k] * b[k][j];
                }
                sum += result[i][j];
            }
        }
        return sum;
    }

    void getName() override {
        cout << "По строкам с guided расписанием   ";
    }
};

class RowScheduleStaticChunk : public Base {
    int chunk;

    double calc(int N, int **a, int **b) override {
        double sum = 0;
        int **result = initResultMatrix(N);
#pragma omp parallel for schedule(static, chunk) reduction(+: sum)
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                result[i][j] = 0;
                for (int k = 0; k < N; ++k) {
                    result[i][j] += a[i][k] * b[k][j];
                }
                sum += result[i][j];
            }
        }
        return sum;
    }

    void getName() override {
        getChunk();
        cout <<"По строкам static chunk = "<<chunk<<" ";
    }

    void getChunk(){
        cout<<"Введите размер chunk:";
        cin>>chunk;
    }
};

#endif //PP_P_H
