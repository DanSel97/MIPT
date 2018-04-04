#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <locale.h>

double f(double x)
{
    return -log(sin(x));
}

double freg(double x)
{
    return -log(sin(x)) + log(x);
}

double integratestep(double x1, double x2)
{
    return (x2 - x1)/2*(f((x1 + x2)/2 - (x2 - x1)/2/sqrt(3)) + f((x1 + x2)/2 + (x2 - x1)/2/sqrt(3)));
}

double integratestepreg(double x1, double x2)
{
    return (x2 - x1)/2*(freg((x1 + x2)/2 - (x2 - x1)/2/sqrt(3)) + freg((x1 + x2)/2 + (x2 - x1)/2/sqrt(3)));
}

double integrateall(double x1, double x2, double tau)
{
    int N = (int)((x2 - x1)/tau);
    int i;
    double integral = 0;
    double a = x1;
    double b = x1 + tau;
    for (i = 0; i < N; i++)
    {
        integral += integratestep(a, b);
        a += tau;
        b += tau;
    }
    return integral;
}

double integrateallreg(double x1, double x2, double tau)
{
    int N = (int)((x2 - x1)/tau);
    int i;
    double integral = 0;
    double a = x1;
    double b = x1 + tau;
    for (i = 0; i < N; i++)
    {
        integral += integratestepreg(a, b);
        a += tau;
        b += tau;
    }
    return integral;
}

int main()
{
    setlocale(LC_ALL, "rus");
    double x1 = 0;
    double x2 = M_PI/2;
    double tau0 = 0.1;
    int iter = 0;
    double a = integrateall(x1, x2, tau0);
    double b = integrateall(x1, x2, tau0/2);
    double integral = 16/15*b - 1/15*a;
    printf("Интегрируем нерегуляризованную функцию -log(sin(x)):\n\n");
    printf("Стадия интегрирования h = %d, текущее значение tau0 = %0.11f, значения интеграла с шагом интегрирования tau0 - %0.11f, с шагом tau0/2 - %0.11f\n", iter, tau0, tau0/2, a, b);
    while(fabs(a - b) >= 1e-10 && iter < 20)
    {
        iter++;
        tau0 = tau0/2;
        a = integrateall(x1, x2, tau0);
        b = integrateall(x1, x2, tau0/2);
        integral = 16/15*b - 1/15*a;
        printf("Стадия интегрирования h = %d, текущее значение tau0 = %0.11f, значения интеграла с шагом интегрирования tau0 - %0.11f, с шагом tau0/2 - %0.11f\n", iter, tau0, a, b);
    }
    printf("Значение интеграла экстраполяцией Ричардсона: %0.11f\n", integral);
    printf("\nИнтегрируем регуляризованную фукнцию -log(sin(x)/x):\n");
    x1 = 0;
    x2 = M_PI/2;
    tau0 = 0.1;
    iter = 0;
    a = integrateallreg(x1, x2, tau0);
    b = integrateallreg(x1, x2, tau0/2);
    integral = 16/15*b - 1/15*a;
    printf("%f, %f\n", a, b);
    printf("Стадия интегрирования h = %d, текущее значение tau0 = %0.11f, значения интеграла с шагом интегрирования tau0 - %0.11f, с шагом tau0/2 - %0.11f\n", iter, tau0, a, b);
    while(fabs(a - b) >= 1e-10 || tau0 > 5*1e-7)
    {
        iter++;
        tau0 = tau0/2;
        a = integrateallreg(x1, x2, tau0);
        b = integrateallreg(x1, x2, tau0/2);
        integral = 16/15*b - 1/15*a;
        printf("Стадия интегрирования h = %d, текущее значение tau0 = %0.11f, значения интеграла с шагом интегрирования tau0 - %0.11f, с шагом tau0/2 - %0.11f\n", iter, tau0, a, b);
    }
    printf("Искомое значение интеграла = %0.11f + pi/2(1 + ln(2) - ln(Pi)) = %0.10f",integral, integral + M_PI/2*(1 + log(2) - log(M_PI)));
    return 0;
}
