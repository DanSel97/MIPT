#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <locale.h>

double f(double t, double u)
{
    return(-2*t*u + 2*t);
}

void Adams(int N, double tau, double* u_n, double y0, double* t_n)
{
    u_n[0] = y0;
    u_n[1] = y0 + tau*f(0, y0);
    int i;
    for (i = 1; i <= N - 1; i++)
    {
        u_n[i + 1] = u_n[i] + tau/2*(3*f(t_n[i], u_n[i]) - f(t_n[i - 1], u_n[i - 1]));
    }
}

void Adamsspeed(int N, double* u_n, double y0, double* t_n, double* points, double* max)
{
    double tau0 = (double)1/(double)1000;
    int i, l;
    for (i = 0; i < 10; i++)
    {
        points[i] = (i + 1)*tau0;
        for (l = 0; l <= N; l++)
        {
            t_n[l] = l*points[i];
            u_n[l] = 0;
        }
        Adams(N, points[i], u_n, y0, t_n);
        double x = 0;
        int j;
        for (j = 0; j <= N; j++)
        {
            if (x < fabs(u_n[j] - (1 + exp(-t_n[j]*t_n[j]))))
            {
                x = fabs(u_n[j] - (1 + exp(-t_n[j]*t_n[j])));
            }
            u_n[j] = 0;
        }
        max[i] = x;
    }

}

int main()
{
    int N = 1000;
    double tau = (double)1/(double)N;
    double* u_n = (double*)calloc(N + 1, sizeof(double));
    double* t_n = (double*)calloc(N + 1, sizeof(double));
    double* max = (double*)calloc(10, sizeof(double));
    double* points = (double*)calloc(10, sizeof(double));
    int i;
    for (i = 0; i <= N; i++)
    {
        t_n[i] = i*tau;
        u_n[i] = 0;
    }
    double y0 = 2;
    Adams(N, tau, u_n, y0, t_n);
    for (i = 0; i <= N; i++)
    {
        printf("u[%f] = %f, solve[%f] = %f\n", i*tau, u_n[i], i*tau, 1 + exp(-i*i*tau*tau));

    }
    Adamsspeed(N, u_n, y0, t_n, points, max);
    for (i = 0; i < 10; i++)
    {
        printf("tau = %f, max = %f, max/tau^2 = %f\n", points[i], max[i], max[i]/points[i]/points[i]);
    }
    return 0;
}
