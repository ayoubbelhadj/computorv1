#pragma once
#include <vector>
#include <map>
#include "Parser.hpp"

class Equation
{
public:
    Equation(const std::vector<Term> &terms);
    void    solve();
 
private:
    std::map<int, double>   _coeffs;
 
    int         _getDegree();
    bool        _isAllZero();
    std::string _doubleToStr(double n);
    std::string _toFraction(double num, double den);
    std::string _termToStr(double coeff, int exp);
    std::string _buildImagStr(double discriminant, double denom);
 
    void    _displayReducedForm();

    void    _solveZero();
    void    _solveFirst();
    void    _solveSecond();
    
    void    _solvePositiveDisc(double a, double b, double discriminant);
    void    _solveZeroDisc(double a, double b);
    void    _solveNegativeDisc(double a, double b, double discriminant);
};