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
 
    int     _getDegree();
    bool    _isAllZero();
    void    _displayReducedForm();
    void    _solveZero();
    void    _solveFirst();
    void    _solveSecond();
};