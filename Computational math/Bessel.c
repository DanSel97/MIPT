#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <locale.h>
double Bessel(int iter, double x)
{
    double bessel = 1;
    double x_act = 1;
    int i;
    for (i = 1; i <= iter; i++)
    {
        if (i % 2 == 0)
        {
            x_act = x_act * pow(x, 2)/(4 * pow(i, 2));
            bessel = bessel + x_act;
        }
        if (i % 2 == 1)
        {
            x_act = x_act * pow(x, 2)/(4 * pow(i, 2));
            bessel = bessel - x_act;
        }
    }
    return(bessel);
}

int fact(long int k)
{
    if (k == 0)
    {
        return 1;
    }
    else
    {
        int i;
        long int fact = 1;
        for (i = 1; i <= k; i++)
        {
            fact = fact*i;
        }
        return fact;
    }
}

int k_min(double eps)
{
    int k = 0;
    while ((double)(1/(pow(2, k)*fact(k + 1)) + 1/(fact(k)*fact(k + 1)*pow(2, 2*k + 1))) >= eps)
    {
        k++;
    }
    return k;
}

double Bessel_derivative(int iter, double x, double h_opt)
{
    return ((Bessel(iter, x + h_opt) - Bessel(iter, x))/h_opt);
}

int k_min_accuracy(double eps)
{
    int k = 0;
    while ((double)(1/(pow(2, 2*k + 3)*fact(k + 2)*fact(k + 1))) >= eps)
    {
        k++;
    }
    return k;
}

double Bessel_derivative_accuracy(int iter, double x)
{
    int i = 0;
    double bessel = -(double)(x/2);
    double x_act = (double)(x/2);
    for (i = 1; i <= iter; i++)
    {
        if (i % 2 == 0)
        {
            x_act = x_act * pow(x, 2)/(4 * i * (i + 1));
            printf("bessel_dirivative_act = %.7f, x_act = %.7f при k = %d\n", bessel, -x_act, i);
            bessel = bessel - x_act;
        }
        if (i % 2 == 1)
        {
            x_act = x_act * pow(x, 2)/(4 * i * (i + 1));
            printf("bessel_dirivative_act = %.7f, x_act = %.7f при k = %d\n", bessel, x_act, i);
            bessel = bessel + x_act;
        }
    }
    printf("bessel_dirivative = %.7f при k = %d\n", bessel, iter);
    return(bessel);
}

int k_min_2(double eps)
{
    int k = 0;
    while ((double)(9/(64 * pow(4*eps, 3)* pow(2, 4*k) * pow(fact(k + 1), 4))) >= 1)
    {
        k++;
    }
    return k;
}

double Bessel_derivative_2(int iter, double x, double h_opt)
{
    return ((Bessel(iter, x + h_opt) - Bessel(iter, x - h_opt))/(2*h_opt));
}

int main()
{
    setlocale(LC_ALL, "rus");
    int iter = 5;
    double x = 1;
    double eps = 1e-6;
    printf("Минимальное число членов Тейлора для формулы первого порядка - %d\n", k_min(eps));
    int kmin = k_min(eps);
    double h_opt = (double)(1/pow(2, kmin)/fact(kmin + 1));
    printf("Значение производной функции Бесселя с точностью %.6f в точке %.6f при шаге h_opt = %.8f по формуле первого порядка - %.6f\n", eps, x, h_opt, Bessel_derivative(kmin, x, h_opt));
    int kmin_accuracy = k_min_accuracy(eps);
    printf("Минимальное число членов Тейлора для производной - %d\n", k_min_accuracy(eps));
    printf("Значение производной функции Бесселя с точностью %.6f в точке %.6f при использовании её ряда Тейлора - %.6f\n", eps, x, Bessel_derivative_accuracy(kmin_accuracy , x));
    double eps_new = 1e-7;
    kmin = k_min(eps_new);
    kmin_accuracy = k_min_accuracy(eps_new);
    h_opt = (double)(1/pow(2, kmin)/fact(kmin + 1));
    printf("Уточнение: минимальное число членов Тейлора для формулы первого порядка с точностью %.7f - %d\n", eps_new, kmin);
    printf("Уточнение: минимальное число членов Тейлора для производной с точностью %.7f - %d\n", eps_new, kmin_accuracy);
    printf("Уточнение: значение производной функции Бесселя с точностью %.7f в точке %.6f при шаге h_opt = %.8f по формуле первого порядка  - %.7f\n", eps_new, x, h_opt, Bessel_derivative(kmin, x, h_opt));
    printf("Уточнение: значение производной функции Бесселя с точностью %.7f в точке %.6f при использовании её ряда Тейлора - %.7f\n", eps_new, x, Bessel_derivative_accuracy(kmin_accuracy , x));
    eps = 1e-6;
    kmin = k_min_2(eps);
    printf("Минимальное число членов Тейлора для формулы второго порядка - %d\n", kmin);
    h_opt = exp(log((double)(3/(pow(2, 13) * pow(fact(6), 2))))/3);
    printf("Значение производной функции Бесселя с точностью %.6f в точке %.6f при шаге h_opt = %.8f по формуле второго порядка - %.6f\n", eps, x, h_opt, Bessel_derivative_2(kmin, x, h_opt));
    return 0;
}
