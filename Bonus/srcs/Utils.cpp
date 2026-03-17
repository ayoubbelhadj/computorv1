#include "Utils.hpp"

double ft_sqrt(double n)
{
    if (n < 0)
        return -1;
    if (n == 0)
        return 0;
 
    double x = n;
    for (int i = 0; i < 1000; i++)
        x = (x + n / x) / 2.0;
    return x;
}

double ft_abs(double n)
{
    return (n < 0) ? -n : n;
}

int ft_abs_int(int n)
{
    return (n < 0) ? -n : n;
}

int ft_gcd(int a, int b)
{
    if (a == 0) return b;
    if (b == 0) return a;

    while (a != b)
    {
        if (a > b)
            a = a - b;
        else
            b = b - a;
    }
    return a;
}

void ft_simplify_sqrt(int n, int &coeff, int &remainder)
{
    coeff = 1;
    remainder = n;

    for (int i = 2; i * i <= remainder; i++)
    {
        while (remainder % (i * i) == 0)
        {
            coeff *= i;
            remainder /= (i * i);
        }
    }
}