#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <locale.h>

void P(double* P, double P1, int N, double h)
{
    P[1] = P1;
    int i;
    for (i = 1; i <= N - 1; i++)
    {
        P[i + 1] =  1/(2 + h*h - P[i]);
    }
}

void Q(double* Q, double Q1, double* P, int N, double h)
{
    Q[1] = Q1;
    int i;
    for (i = 1; i <= N - 1; i++)
    {
        Q[i + 1] =  (Q[i] + 2*h*h*sin(i*h))/(2 + h*h - P[i]);
    }
}

void u(double* u, double* Q, double* P, int N, double uN)
{
    u[N] = uN;
    int i;
    for (i = N; i >= 1; i--)
    {
        u[i - 1] = P[i]*u[i] + Q[i];
    }
}

void initA(double** A, int N, double h)
{
    int i,j;
    for (i = 0; i <= N; i++)
    {
        for (j = 0; j <= N; j++)
        {
            if ((i >= 1 && i <= N - 1 && j == i - 1) || (i >= 1 && i <= N - 1 && j == i + 1))
            {
                A[i][j] = -1;
            }
            else if ((i == 0 && j == 0) || (i == N && j == N))
            {
                A[i][j] = 1;
            }
            else if (i >= 1 && i <= N - 1 && j == i)
            {
                A[i][j] = 2 + h*h;
            }
            else
            {
                A[i][j] = 0;
            }
        }
    }
    A[1][0] = 0;
    A[N - 1][N] = 0;
}

void initU(double** U, double** A, int N)
{
    int i,j;
    for (i = 0; i <= N; i++)
    {
        for (j = 0; j <= N; j++)
        {
            if (j > i)
            {
                U[i][j] = A[i][j];
            }
            else
            {
                U[i][j] = 0;
            }
        }
    }
}

void initLD(double** LD, double** A, int N)
{
    int i,j;
    for (i = 0; i <= N; i++)
    {
        for (j = 0; j <= N; j++)
        {
            if (j <= i)
            {
                LD[i][j] = A[i][j];
            }
            else
            {
                LD[i][j] = 0;
            }
        }
    }
}

void initb(double* b, int N, double h)
{
    int i;
    b[0] = 0;
    b[N] = 0;
    for (i = 1; i < N; i++)
    {
        b[i] = 2*h*h*sin(i*h);
    }
}

void iterZeidel(double** LD, double** U, double* uact, double* b, int N)
{
    int i, k;
    double* uiter = (double*)calloc(N + 1, sizeof(double));
    for (i = 0; i <= N; i++)
    {
        uiter[i] = 0;
        for (k = 0; k <= N; k++)
        {
            uiter[i] = uiter[i] + U[i][k]*uact[k];
        }
        uiter[i] = -1*uiter[i] + b[i];
    }
    uact[0] = uiter[0]/LD[0][0];
    double start = 0;
    for (i = 1; i <= N; i++)
    {
        start = 0;
        for (k = 0; k < i; k++)
        {
            start = start + LD[i][k]*uact[k];
        }
        uact[i] = (uiter[i] - start)/LD[i][i];
    }
}

double norma1(double* a, double* b, int N)
{
    double ans = fabs(a[0] - b[0]);
    int i;
    for (i = 0; i <= N; i++)
    {
        if (fabs(a[i] - b[i]) >= ans)
        {
            ans = fabs(a[i] - b[i]);
        }
    }
    return ans;
}

double norma2(double* a, double* b, int N)
{
    double ans = 0;
    int i;
    for (i = 0; i <= N; i++)
    {
        ans = ans + fabs(a[i] - b[i]);
    }
    return ans;
}

double norma3(double* a, double* b, int N)
{
    double ans = 0;
    int i;
    for (i = 0; i <= N; i++)
    {
        ans = ans + fabs(a[i] - b[i])*fabs(a[i] - b[i]);
    }
    return sqrt(ans);
}

int main()
{
    setlocale(LC_ALL, "rus");
    int N = 20;
    double h = (double)(M_PI/N);
    int i;
    double** A;
    A = (double**)calloc(N + 1, sizeof(double*));
    for (i = 0; i <= N; i++)
    {
        A[i] = (double*)calloc(N + 1, sizeof(double));
    }
    initA(A, N, h);
    printf("Матрица системы A:\n");
    int j;
    for (i = 0; i <= N; i++)
    {
        for (j = 0; j <= N; j++)
        {
            printf("%.1f ", A[i][j]);
        }
        printf("\n");
    }
    double* b = (double*)calloc(N + 1, sizeof(double));
    initb(b, N, h);
    printf("Вектор правой части b:\n");
    for (i = 0; i <= N; i++)
    {
        printf("b[%d] = %f\n", i, b[i]);
    }
    double* uact = (double*)calloc(N + 1, sizeof(double));
    double **U;
    U = (double**)calloc(N + 1, sizeof(double*));
    for (i = 0; i <= N; i++)
    {
        U[i] = (double*)calloc(N + 1, sizeof(double));
    }
    initU(U, A, N);
    double **LD;
    LD = (double**)calloc(N + 1, sizeof(double*));
    for (i = 0; i <= N; i++)
    {
        LD[i] = (double*)calloc(N + 1, sizeof(double));
    }
    initLD(LD, A, N);
    for (i = 0; i <= N; i++)
    {
        for (j = 0; j <= N; j++)
        {
            printf("%.1f ", U[i][j]);
        }
        printf("\n");
    }
    printf("\n");
    for (i = 0; i <= N; i++)
    {
        for (j = 0; j <= N; j++)
        {
            printf("%.1f ", LD[i][j]);
        }
        printf("\n");
    }
    double* unext = (double*)calloc(N + 1, sizeof(double));
    for (i = 0; i <= N; i++)
    {
        unext[i] = 0;
        uact[i] = 0;
    }
    iterZeidel(LD, U, unext, b, N);
    printf("Запускаем итерации метода Зейделя, используя остановку по норме-1:\n");
    int iter = 1;
    double eps = 1e-6;
    while (norma1(unext, uact, N) >= eps)
    {
        iter++;
        for (i = 0; i <= N; i++)
        {
            uact[i] = unext[i];
        }
        iterZeidel(LD, U, unext, b, N);
    }
    printf("По норме-1 достаточно сделать %d итераций метода Зейделя. Полученный результат сравним с предыдущим:\n", iter);

    double P1 = 0, Q1 = 0;
    double* P_n = (double*)calloc(N + 1, sizeof(double));
    double* Q_n = (double*)calloc(N + 1, sizeof(double));
    P(P_n, P1, N, h);
    Q(Q_n, Q1, P_n, N, h);
    double uN = 0;
    double* u_n = (double*)calloc(N + 1, sizeof(double));
    u(u_n, Q_n, P_n, N, uN);

    printf("Метод Зейделя: Метод прогонки:\n");
    for (i = 0; i <= N; i++)
    {
        printf("u[%d] = %f, u[%d] = %f\n", i, unext[i], i, u_n[i]);
    }
    double error = 0;
    for (i = 0; i <= N; i++)
    {
        if (fabs(unext[i] - u_n[i]) >= error)
        {
            error = fabs(unext[i] - u_n[i]);
        }
    }
    printf("Вычислим максимальную величину ошибки между результатами: error = %f\n", error);
    printf("Запускаем итерации метода Зейделя, используя остановку по норме-2:\n");
    iter = 1;
    eps = 1e-6;
    for (i = 0; i <= N; i++)
    {
        unext[i] = 0;
        uact[i] = 0;
    }
    iterZeidel(LD, U, unext, b, N);
    while (norma2(unext, uact, N) >= eps)
    {
        iter++;
        for (i = 0; i <= N; i++)
        {
            uact[i] = unext[i];
        }
        iterZeidel(LD, U, unext, b, N);
    }
    printf("По норме-2 достаточно сделать %d итераций метода Зейделя. Полученный результат сравним с предыдущим:\n", iter);
    printf("Метод Зейделя: Метод прогонки:\n");
    for (i = 0; i <= N; i++)
    {
        printf("u[%d] = %f, u[%d] = %f\n", i, unext[i], i, u_n[i]);
    }
    error = 0;
    for (i = 0; i <= N; i++)
    {
        if (fabs(unext[i] - u_n[i]) >= error)
        {
            error = fabs(unext[i] - u_n[i]);
        }
    }
    printf("Вычислим максимальную величину ошибки между результатами: error = %f\n", error);
     iter = 1;
    eps = 1e-6;
    for (i = 0; i <= N; i++)
    {
        unext[i] = 0;
        uact[i] = 0;
    }
    iterZeidel(LD, U, unext, b, N);
    printf("Запускаем итерации метода Зейделя, используя остановку по норме-2:\n");
    while (norma3(unext, uact, N) >= eps)
    {
        iter++;
        for (i = 0; i <= N; i++)
        {
            uact[i] = unext[i];
        }
        iterZeidel(LD, U, unext, b, N);
    }
    printf("По норме-3 достаточно сделать %d итераций метода Зейделя. Полученный результат сравним с предыдущим:\n", iter);
    printf("Метод Зейделя: Метод прогонки:\n");
    for (i = 0; i <= N; i++)
    {
        printf("u[%d] = %f, u[%d] = %f\n", i, unext[i], i, u_n[i]);
    }
    error = 0;
    for (i = 0; i <= N; i++)
    {
        if (fabs(unext[i] - u_n[i]) >= error)
        {
            error = fabs(unext[i] - u_n[i]);
        }
    }
    printf("Вычислим максимальную величину ошибки между результатами: error = %f\n", error);
    return 0;
}
