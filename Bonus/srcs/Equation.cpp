#include "Equation.hpp"
#include "Utils.hpp"
#include <iostream>
#include <sstream>

Equation::Equation(const std::vector<Term> &terms)
{
    for (const Term &t : terms)
        _coeffs[t.exponent] += t.coefficient * t.side;
}

std::string Equation::_doubleToStr(double n)
{
    std::ostringstream oss;
    oss << n;
    return oss.str();
}

static bool isWhole(double x)
{
    return x == (int)x;
}

std::string Equation::_toFraction(double num, double den)
{
    if (num == 0.0)
        return "0";

    if (!isWhole(num) || !isWhole(den))
        return _doubleToStr(num / den);

    int numerator   = (int)num;
    int denominator = (int)den;
    int sign        = ((numerator < 0) != (denominator < 0)) ? -1 : 1;
    int absNum      = ft_abs_int(numerator);
    int absDen      = ft_abs_int(denominator);
    int divisor     = ft_gcd(absNum, absDen);

    absNum /= divisor;
    absDen /= divisor;

    std::string result = (sign < 0) ? "-" : "";
    result += std::to_string(absNum);
    if (absDen != 1)
        result += "/" + std::to_string(absDen);
    return result;
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

std::string Equation::_termToStr(double coeff, int exp)
{
    std::string s;

    if (exp == 0)
        s = _doubleToStr(coeff);
    else if (exp == 1)
    {
        if (coeff == 1.0)
            s = "X";
        else if (coeff == -1.0)
            s = "-X";
        else
            s = _doubleToStr(coeff) + " * X";
    }
    else
    {
        if (coeff == 1.0)
            s = "X^" + std::to_string(exp);
        else if (coeff == -1.0)
            s = "-X^" + std::to_string(exp);
        else
            s = _doubleToStr(coeff) + " * X^" + std::to_string(exp);
    }
    return s;
}

void Equation::_displayReducedForm()
{
    std::cout << "Reduced form: ";

    if (_isAllZero())
    {
        std::cout << "0 = 0" << std::endl;
        return;
    }

    int  degree = _getDegree();
    bool first  = true;

    for (int i = 0; i <= degree; i++)
    {
        double coeff = _coeffs.count(i) ? _coeffs[i] : 0.0;

        if (coeff == 0.0 && i != 0)
            continue;

        if (first)
        {
            std::cout << _termToStr(coeff, i);
            first = false;
        }
        else
        {
            if (coeff < 0)
                std::cout << " - " << _termToStr(-coeff, i);
            else
                std::cout << " + " << _termToStr(coeff, i);
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

    std::cout << std::endl;
    std::cout << "Intermediate steps:" << std::endl;
    std::cout << "  a = " << b << ",  b = " << c << std::endl;
    std::cout << "  X = -b / a" << std::endl;
    std::cout << "  X = -(" << c << ") / (" << b << ")" << std::endl;

    double solution = -c / b;
    if (solution == 0.0)
        solution = 0.0;
    std::string fracStr = _toFraction(-c, b);

    std::cout << std::endl;
    std::cout << "The solution is:" << std::endl;

    if (fracStr != _doubleToStr(solution))
        std::cout << fracStr << " = " << solution << std::endl;
    else
        std::cout << solution << std::endl;
}

std::string Equation::_buildImagStr(double discriminant, double denom)
{
    double negDisc = -discriminant;
    double imagNum = ft_sqrt(negDisc);

    // not an integer, can't simplify the root
    if (!isWhole(negDisc))
        return _doubleToStr(imagNum / denom) + "i";

    // factor out perfect squares from under the root
    int sqrtCoeff, sqrtRemainder;
    ft_simplify_sqrt((int)negDisc, sqrtCoeff, sqrtRemainder);

    // reduce the fraction
    int intDenom = (int)denom;
    int divisor = ft_gcd(ft_abs_int(sqrtCoeff), ft_abs_int(intDenom));
    sqrtCoeff /= divisor;
    intDenom /= divisor;

    // drop the "1" prefix
    std::string outerCoeff = (sqrtCoeff == 1) ? "" : std::to_string(sqrtCoeff);

    if (sqrtRemainder == 1)
    {
        if(intDenom == 1)
            return outerCoeff + "i";
        return outerCoeff + "i/" + std::to_string(intDenom);
    }

    // still a root left inside
    std::string sqrtPart = outerCoeff + "√" + std::to_string(sqrtRemainder);
    if(intDenom == 1)
        return sqrtPart + "i";
    return sqrtPart + "i/" + std::to_string(intDenom);
}

// Δ > 0
void Equation::_solvePositiveDisc(double a, double b, double discriminant)
{
    double sqrtD = ft_sqrt(discriminant);
    double denom = 2.0 * a;

    std::cout << "  √Δ = " << sqrtD << std::endl;
    std::cout << "  x1 = (-b + √Δ) / (2a) = (" << -b << " + " << sqrtD << ") / " << denom << std::endl;
    std::cout << "  x2 = (-b - √Δ) / (2a) = (" << -b << " - " << sqrtD << ") / " << denom << std::endl;
    std::cout << std::endl;

    double x1 = (-b + sqrtD) / denom;
    double x2 = (-b - sqrtD) / denom;

    std::string s1 = _toFraction(-b + sqrtD, denom);
    std::string s2 = _toFraction(-b - sqrtD, denom);

    if (x1 < x2)
    {
        std::swap(x1, x2);
        std::swap(s1, s2);
    }

    std::cout << "Discriminant is strictly positive, the two solutions are:" << std::endl;
    if (s1 != _doubleToStr(x1))
        std::cout << s1 << " = " << x1 << std::endl;
    else
        std::cout << x1 << std::endl;
    if (s2 != _doubleToStr(x2))
        std::cout << s2 << " = " << x2 << std::endl;
    else
        std::cout << x2 << std::endl;
}

// Δ = 0
void Equation::_solveZeroDisc(double a, double b)
{
    double denom = 2.0 * a;
    double x     = -b / denom;
    if (x == 0.0)
        x = 0.0;

    std::cout << "  x = -b / (2a) = " << -b << " / " << denom << std::endl;
    std::cout << std::endl;

    std::string frac = _toFraction(-b, denom);

    std::cout << "Discriminant is zero, the solution is:" << std::endl;
    if (frac != _doubleToStr(x))
        std::cout << frac << " = " << x << std::endl;
    else
        std::cout << x << std::endl;
}

// Δ < 0 
void Equation::_solveNegativeDisc(double a, double b, double discriminant)
{
    double sqrtD   = ft_sqrt(-discriminant);
    double denom   = 2.0 * a;
    double realNum = -b;

    std::cout << "  √(-Δ) = √(" << -discriminant << ") = " << sqrtD << std::endl;
    std::cout << "  real part = -b / (2a)    = " << (realNum == 0.0 ? 0.0 : realNum) << " / " << denom << std::endl;
    std::cout << "  imag part = √(-Δ) / (2a) = " << sqrtD << " / " << denom << std::endl;
    std::cout << std::endl;

    std::cout << "Discriminant is strictly negative, the two complex solutions are:" << std::endl;

    std::string realStr;
    if (realNum == 0.0)
        realStr = "0";
    else
        realStr = _toFraction(realNum, denom);

    std::string imagStr = _buildImagStr(discriminant, denom);

    if (realStr == "0")
        std::cout << "±" << imagStr << std::endl;
    else
    {
        std::cout << realStr << " + " << imagStr << std::endl;
        std::cout << realStr << " - " << imagStr << std::endl;
    }
}

void Equation::_solveSecond()
{
    double a = _coeffs.count(2) ? _coeffs[2] : 0.0;
    double b = _coeffs.count(1) ? _coeffs[1] : 0.0;
    double c = _coeffs.count(0) ? _coeffs[0] : 0.0;

    double discriminant = b * b - 4.0 * a * c;

    std::cout << std::endl;
    std::cout << "Intermediate steps:" << std::endl;
    std::cout << "  a = " << a << ",  b = " << b << ",  c = " << c << std::endl;
    std::cout << "  Discriminant (Δ) = b² - 4ac" << std::endl;
    std::cout << "  Δ = (" << b << ")² - 4 * (" << a << ") * (" << c << ")" << std::endl;
    std::cout << "  Δ = " << (b * b) << " - (" << (4.0 * a * c) << ")" << std::endl;
    std::cout << "  Δ = " << discriminant << std::endl;
    std::cout << std::endl;

    if (discriminant > 0.0)
        _solvePositiveDisc(a, b, discriminant);
    else if (discriminant == 0.0)
        _solveZeroDisc(a, b);
    else
        _solveNegativeDisc(a, b, discriminant);
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

    if (degree >= 1)
        std::cout << "Polynomial degree: " << degree << std::endl;

    if      (degree == 0)
        _solveZero();
    else if (degree == 1)
        _solveFirst();
    else if (degree == 2)
        _solveSecond();
    else
        std::cout << "The polynomial degree is strictly greater than 2, I can't solve." << std::endl;
}
