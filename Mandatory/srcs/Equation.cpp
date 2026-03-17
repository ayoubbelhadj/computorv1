#include "Equation.hpp"
#include "Utils.hpp"
#include <iostream>
 
Equation::Equation(const std::vector<Term> &terms)
{
    for (const Term &t : terms)
        _coeffs[t.exponent] += t.coefficient * t.side;
}

int Equation::_getDegree()
{
    int degree = 0;
    for (auto &p : _coeffs)
        if (p.second != 0.0)
            degree = p.first;
    return degree;
}

bool Equation::_isAllZero()
{
    for (auto &p : _coeffs)
        if (p.second != 0.0)
            return false;
    return true;
}

void Equation::_displayReducedForm()
{
    std::cout << "Reduced form: ";

    if (_isAllZero())
    {
        std::cout << "0 * X^0 = 0" << std::endl;
        return;
    }

    int  degree = _getDegree();
    bool first  = true;

    for (int i = 0; i <= degree; i++)
    {
        double coeff = _coeffs.count(i) ? _coeffs[i] : 0.0;
 
        if (first)
        {
            std::cout << coeff << " * X^" << i;
            first = false;
        }
        else
        {
            if (coeff < 0)
                std::cout << " - " << -coeff << " * X^" << i;
            else
                std::cout << " + " << coeff  << " * X^" << i;
        }
    }
    std::cout << " = 0" << std::endl;
}

void Equation::_solveZero()
{
    double c = _coeffs.count(0) ? _coeffs[0] : 0.0;

    if (c == 0.0)
        std::cout << "Any real number is a solution." << std::endl;
    else
        std::cout << "No solution." << std::endl;
}

void Equation::_solveFirst()
{
    double c = _coeffs.count(0) ? _coeffs[0] : 0.0;
    double b = _coeffs.count(1) ? _coeffs[1] : 0.0;
 
    double solution = -c / b;
 
    std::cout << "The solution is:" << std::endl;
    std::cout << solution << std::endl;
}


void Equation::_solveSecond()
{
    double a = _coeffs.count(2) ? _coeffs[2] : 0.0;
    double b = _coeffs.count(1) ? _coeffs[1] : 0.0;
    double c = _coeffs.count(0) ? _coeffs[0] : 0.0;

    double discriminant = b * b - 4.0 * a * c;

    if (discriminant > 0.0)
    {
        double sqrtD = ft_sqrt(discriminant);
        double x1    = (-b + sqrtD) / (2.0 * a);
        double x2    = (-b - sqrtD) / (2.0 * a);

        if (x1 < x2)
        {
            double tmp = x1;
            x1 = x2;
            x2 = tmp;
        }

        std::cout << "Discriminant is strictly positive, the two solutions are:" << std::endl;
        std::cout << x1 << std::endl;
        std::cout << x2 << std::endl;
    }
    else if (discriminant == 0.0)
    {
        double x = -b / (2.0 * a);
        if (x == 0.0)
            x = 0.0;
        std::cout << "Discriminant is zero, the solution is:" << std::endl;
        std::cout << x << std::endl;
    }
    else
    {
        // Complex solutions: (-b ± i√(-Δ)) / 2a
        double sqrtD  = ft_sqrt(-discriminant);
        double denom  = 2.0 * a;
        double realNum = -b;
        double imagNum = sqrtD;

        std::cout << "Discriminant is strictly negative, the two complex solutions are:" << std::endl;

        bool realWhole  = (realNum == (int)realNum);
        bool imagWhole  = (imagNum == (int)imagNum);
        bool denomWhole = (denom   == (int)denom);

        if (realWhole && imagWhole && denomWhole)
        {
            int rn = (int)realNum;
            int in = (int)imagNum;
            int dn = (int)denom;
 
            int gr = ft_gcd(ft_abs_int(rn), ft_abs_int(dn));
            int gi = ft_gcd(ft_abs_int(in), ft_abs_int(dn));
 
            int rNum = rn / gr;
            int rDen = dn / gr;

            int iNum = in / gi;
            int iDen = dn / gi;

            // build real string
            std::string realStr;
            if (rNum == 0)
                realStr = "0";
            else if (rDen == 1)
                realStr = std::to_string(rNum);
            else
                realStr = std::to_string(rNum) + "/" + std::to_string(rDen);

            // build imag string
            std::string imagStr;
            if (iDen == 1)
                imagStr = std::to_string(iNum) + "i";
            else
                imagStr = std::to_string(iNum) + "i/" + std::to_string(iDen);

            std::cout << realStr << " + " << imagStr << std::endl;
            std::cout << realStr << " - " << imagStr << std::endl;
        }
        else
        {
            double realPart = realNum / denom;
            double imagPart = imagNum / denom;

            if (realPart == 0.0) realPart = 0.0;

            std::cout << realPart << " + " << imagPart << "i" << std::endl;
            std::cout << realPart << " - " << imagPart << "i" << std::endl;
        }
    }
}

void Equation::solve()
{
    _displayReducedForm();
 
    if (_isAllZero())
    {
        _solveZero();
        return;
    }
 
    int degree = _getDegree();
 
    // Only print degree for degree >= 1
    if (degree >= 1)
        std::cout << "Polynomial degree: " << degree << std::endl;
 
    if      (degree == 0) _solveZero();
    else if (degree == 1) _solveFirst();
    else if (degree == 2) _solveSecond();
    else
        std::cout << "The polynomial degree is strictly greater than 2, I can't solve." << std::endl;
}
 