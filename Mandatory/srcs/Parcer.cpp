#include "Parser.hpp"
#include <iostream>

Parser::Parser(const std::string &input) : _input(input) {}

std::string Parser::_removeSpaces(const std::string &s)
{
    std::string result;
    for (char c : s)
        if (c != ' ')
            result += c;
    return result;
}

std::vector<std::string> Parser::_splitTerms(const std::string &s)
{
    std::vector<std::string> tokens;
    std::string              current;

    for (size_t i = 0; i < s.size(); i++)
    {
        char c = s[i];

        if ((c == '+' || c == '-') && i != 0)
        {
            if (!current.empty())
                tokens.push_back(current);
            current = c;
        }
        else
            current += c;
    }
    // Push the last term
    if (!current.empty())
        tokens.push_back(current);

    return tokens;
}

bool Parser::_parseTerm(const std::string &s, Term &t)
{
    // check if it's a plain number (no '*' and no 'X')
    size_t starPos = s.find('*');
    if (starPos == std::string::npos)
    {
        if (s.find('X') != std::string::npos || s.find('x') != std::string::npos)
        {
            std::cerr << "Error: invalid term format: [" << s << "]" << std::endl;
            return false;
        }
        try
        {
            t.coefficient = std::stod(s);
            t.exponent    = 0;
        }
        catch (...)
        {
            std::cerr << "Error: invalid number in term: [" << s << "]" << std::endl;
            return false;
        }
        return true;
    }

    std::string coeffStr = s.substr(0, starPos);       // "4.5"
    std::string xPart    = s.substr(starPos + 1);      // "X^2"

    size_t caretPos = xPart.find('^');
    if (caretPos == std::string::npos)
    {
        std::cerr << "Error: missing '^' in term: [" << s << "]" << std::endl;
        return false;
    }

    std::string varName = xPart.substr(0, caretPos);   // "X"
    std::string expStr  = xPart.substr(caretPos + 1);  // "2"

    if (varName != "X" && varName != "x")
    {
        std::cerr << "Error: variable must be 'X', got: [" << varName << "]" << std::endl;
        return false;
    }

    try
    {
        t.coefficient = std::stod(coeffStr);
        t.exponent    = std::stoi(expStr);
    }
    catch (...)
    {
        std::cerr << "Error: invalid number in term: [" << s << "]" << std::endl;
        return false;
    }

    return true;
}

bool Parser::parse()
{
    std::string clean = _removeSpaces(_input);

    size_t eqPos = clean.find('=');
    if (eqPos == std::string::npos)
    {
        std::cerr << "Error: no '=' found in equation." << std::endl;
        return false;
    }

    std::string leftSide  = clean.substr(0, eqPos);       // before '='
    std::string rightSide = clean.substr(eqPos + 1);      // after '='

    if (leftSide.empty() || rightSide.empty())
    {
        std::cerr << "Error: equation has empty side." << std::endl;
        return false;
    }

    std::vector<std::string> leftTokens  = _splitTerms(leftSide);
    std::vector<std::string> rightTokens = _splitTerms(rightSide);

    for (const std::string &s : leftTokens)
    {
        Term t;
        t.side = 1;
        if (!_parseTerm(s, t))
            return false;
        _terms.push_back(t);
    }
    for (const std::string &s : rightTokens)
    {
        Term t;
        t.side = -1;
        if (!_parseTerm(s, t))
            return false;
        _terms.push_back(t);
    }

    return true;
}

const std::vector<Term> &Parser::getTerms() const
{
    return _terms;
}