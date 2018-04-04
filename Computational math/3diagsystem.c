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

void proverka(double** A, double *b, double* u_n, int N)
{
    int i, k;
    for (i = 0; i <= N; i++)
    {
        for (k = 0; k <= N; k++)
        {
            b[i] = b[i] - A[i][k]*u_n[k];
        }
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

int main()
{
    setlocale(LC_ALL, "rus");
    int N = 40;
    double h = (double)(M_PI/N);
    double P1 = 0, Q1 = 0;
    double* P_n = (double*)calloc(N + 1, sizeof(double));
    double* Q_n = (double*)calloc(N + 1, sizeof(double));
    int i;
    P(P_n, P1, N, h);
    Q(Q_n, Q1, P_n, N, h);
    printf("Íàõîäèì ïðîãîíî÷íûå êîýôôèöèåíòû P_n è Q_n:\n");
    for (i = 1; i <= N; i++)
    {
        printf("P[%d] = %f, Q[%d] = %f\n", i, P_n[i], i, Q_n[i]);
    }
    double uN = 0;
    double* u_n = (double*)calloc(N + 1, sizeof(double));
    u(u_n, Q_n, P_n, N, uN);
    printf("Ïî íàéäåííûì ïðîãîíî÷íûì êîýôôèöèåíòàì P_n è Q_n âîññòàíàâëèâàåì ðåøåíèå ñèñòåìû:\n");
    for (i = 0; i <= N; i++)
    {
        printf("u[%d] = %f\n", i, u_n[i], i);
    }
    double** A;
    A = (double**)calloc(N + 1, sizeof(double*));
    for (i = 0; i <= N; i++)
    {
        A[i] = (double*)calloc(N + 1, sizeof(double));
    }
    initA(A, N, h);
    printf("Ìàòðèöà ñèñòåìû A:\n");
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
    printf("Âåêòîð ïðàâîé ÷àñòè b:\n");
    for (i = 0; i <= N; i++)
    {
        printf("b[%d] = %f\n", i, b[i]);
    }
    printf("Ïðîâåðèì êîððåêòíîñòü ïîëó÷åííîãî ðåçóëüòàòà, âû÷èñëèâ âåêòîð b - Au, ãäå u - âåêòîð-ðåçóëüòàò, ïîëó÷åííûé âûøå:\n b - Au = \n");
    proverka(A, b, u_n, N);
    for (i = 0; i <= N; i++)
    {
        printf("%.10e\n", b[i]);
    }
    printf("Âèäèì, ÷òî âåêòîð çàíóëèëñÿ ñ òî÷íîñòüþ äî 10 çíàêîâ ïîñëå çàïÿòîé, òàê ÷òî ìîæíî ñ÷èòàòü íàéäåííûé âåêòîð êîððåêòíûì.\nÒåïåðü ñðàâíèì âåëè÷èíû u_n è sin(n*h), âû÷èñëèâ âåëè÷èíó eps_max = |u_n - sin(n*h)|:\n");
    double eps_max = 0;
    for (i = 0; i <= N; i++)
    {
        if (fabs(u_n[i] - sin(i*h)) >= eps_max)
        {
            eps_max =  fabs(u_n[i] - sin(i*h));
        }
    }
    for (i = 0; i <= N; i++)
    {
         printf("u[%d] - sin(%d*%f) = %f\n",i, i, h, u_n[i] - sin(i*h));
    }
    printf("eps_max = %f\n", eps_max);
    return 0;
}
