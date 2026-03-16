#pragma once
#include <string>
#include <vector>


struct Term
{
    double  coefficient;
    int     exponent;
    int     side;        // +1 = left of '=',  -1 = right of '='
};


class Parser
{
public:
    Parser(const std::string &input);
 
    bool                        parse();
    const std::vector<Term>     &getTerms() const;
 
private:
    std::string         _input;
    std::vector<Term>   _terms;
 
    std::string                 _removeSpaces(const std::string &s);
    std::vector<std::string>    _splitTerms(const std::string &s);
    bool                        _parseTerm(const std::string &s, Term &t);
    bool                        _isValidNumber(const std::string &s);
    bool                        _isValidExponent(const std::string &s); 
};