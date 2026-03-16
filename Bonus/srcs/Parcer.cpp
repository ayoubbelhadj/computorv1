#include "Parser.hpp"
#include <iostream>

Parser::Parser(const std::string &input) : _input(input) {}

std::string Parser::_removeSpaces(const std::string &s)
{
    std::string result;
    for (char c : s)
        if (c != ' ' && c != '\t')
            result += c;
    return result;
}

bool Parser::_isValidNumber(const std::string &s)
{
    if (s.empty())
        return false;

    size_t i = 0;

    if (s[i] == '+' || s[i] == '-')
        i++;

    // must have at least one digit
    if (i >= s.size())
        return false;

    bool hasDot = false;
    for (; i < s.size(); i++)
    {
        if (s[i] == '.')
        {
            if (hasDot) return false;
            hasDot = true;
        }
        else if (s[i] < '0' || s[i] > '9')
            return false;
    }
    return true;
}

bool Parser::_isValidExponent(const std::string &s)
{
    if (s.empty())
        return false;

    size_t i = 0;

    if (s[i] == '+' || s[i] == '-')
        i++;

    // must have at least one digit
    if (i >= s.size())
        return false;

    for (; i < s.size(); i++)
    {
        if (s[i] < '0' || s[i] > '9')
        {
            std::cerr << "Error: exponent must be a whole number, got: ["
                      << s << "]" << std::endl;
            return false;
        }
    }
    return true;
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
    // find position of X (or x)
    size_t xPos = s.find('X');
    if (xPos == std::string::npos)
        xPos = s.find('x');
 
    if (xPos == std::string::npos)
    {
        if (!_isValidNumber(s))
        {
            std::cerr << "Error: invalid term: [" << s << "]" << std::endl;
            return false;
        }
        t.coefficient = std::stod(s);
        t.exponent    = 0;
        return true;
    }

    size_t starPos  = s.find('*');
    size_t caretPos = s.find('^');
 
    // "4.5*X^2"
    if (starPos != std::string::npos && caretPos != std::string::npos)
    {
        std::string coeffStr = s.substr(0, starPos);    // "4.5"
        std::string expStr   = s.substr(caretPos + 1);  // "2"
 
        if (!_isValidNumber(coeffStr))
        {
            std::cerr << "Error: invalid coefficient in: [" << s << "]" << std::endl;
            return false;
        }
        if (!_isValidExponent(expStr))
        {
            std::cerr << "Error: invalid exponent in: [" << s << "]" << std::endl;
            return false;
        }
        t.coefficient = std::stod(coeffStr);
        t.exponent    = std::stoi(expStr);
        return true;
    }

    // "4*X"
    if (starPos != std::string::npos && caretPos == std::string::npos)
    {
        std::string coeffStr = s.substr(0, starPos); // "4"
 
        if (!_isValidNumber(coeffStr))
        {
            std::cerr << "Error: invalid coefficient in: [" << s << "]" << std::endl;
            return false;
        }
        t.coefficient = std::stod(coeffStr);
        t.exponent    = 1;
        return true;
    }
 
    // "X^2" - "5X^2" - "-X^2"
    if (starPos == std::string::npos && caretPos != std::string::npos)
    {
        std::string beforeX  = s.substr(0, xPos);   // "" or "5" or "-"
        std::string expStr   = s.substr(caretPos + 1); // "2"

        std::string betweenXandCaret = s.substr(xPos + 1, caretPos - xPos - 1);
        if (!betweenXandCaret.empty())
        {
            std::cerr << "Error: invalid term: [" << s << "]" << std::endl;
            return false;
        }
 

        if (beforeX.empty() || beforeX == "+")
            t.coefficient = 1.0;
        else if (beforeX == "-")
            t.coefficient = -1.0;
        else
        {
            if (!_isValidNumber(beforeX))
            {
                std::cerr << "Error: invalid coefficient in: [" << s << "]" << std::endl;
                return false;
            }
            t.coefficient = std::stod(beforeX);
        }
 
        if (!_isValidExponent(expStr))
        {
            std::cerr << "Error: invalid exponent in: [" << s << "]" << std::endl;
            return false;
        }
        t.exponent = std::stoi(expStr);
        return true;
    }

    // "X", "-X", "+X", "3X"
    std::string beforeX = s.substr(0, xPos);   // "" or "-" or "3"

    // check after X "3Xabc"
    std::string afterX = s.substr(xPos + 1);
    if (!afterX.empty())
    {
        std::cerr << "Error: invalid term: [" << s << "]" << std::endl;
        return false;
    }

    if (beforeX.empty() || beforeX == "+")
        t.coefficient = 1.0;
    else if (beforeX == "-")
        t.coefficient = -1.0;
    else
    {
        if (!_isValidNumber(beforeX))
        {
            std::cerr << "Error: invalid coefficient in: [" << s << "]" << std::endl;
            return false;
        }
        t.coefficient = std::stod(beforeX);
    }
    t.exponent = 1;
    return true;
}

bool Parser::parse()
{
    std::string clean = _removeSpaces(_input);

    if (clean.empty())
    {
        std::cerr << "Error: empty input." << std::endl;
        return false;
    }

    size_t eqPos = clean.find('=');
    if (eqPos == std::string::npos)
    {
        std::cerr << "Error: no '=' found in equation." << std::endl;
        return false;
    }

    if (clean.find('=', eqPos + 1) != std::string::npos)
    {
        std::cerr << "Error: multiple '=' found." << std::endl;
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