//
//  main.cpp
//  Machine
//
//  Created by Daniel Pav on 4/27/20.
//  Copyright © 2020 Daniel Pav. All rights reserved.
//

#include "machine.hpp"
#include <iostream>

class InStr
{
public:
    InStr(const std::string& s) : m_s(s), m_pos(0) { }
    ~InStr() {}
    
public:
    char read()
    {
        char c = (char) 0;
        if (m_pos < m_s.length())
        {
            c = m_s[m_pos];
            ++m_pos;
        }
        return c;
    }
    
private:
    std::string m_s;
    size_t m_pos;
};

void LoadChar(InStr& in, char& c)
{
    c = in.read();
}

void SaveChar(std::string& out, const char& c)
{
    if (c != (char) 0)
        out += c;
}

bool IsDigit(const char& c)
{
    return (c >= '0' && c <= '9');
}

bool IsDecimal(const char& c)
{
    return (c == '.');
}

bool IsDone(const char& c)
{
    return (c == (char) 0);
}

typedef machine::Machine<InStr, char, std::string> Parser;
typedef Parser::State State;
typedef Parser::Link Link;

static State sStart;
static State sDigit1(SaveChar);
static State sDecimal;
static State sDigit2(SaveChar);
static State sEnd(SaveChar);
static Link links[] = {
    {sStart, sDigit1, IsDigit},
    {sDigit1, sDigit1, IsDigit},
    {sDigit1, sDecimal, IsDecimal, SaveChar},
    {sDigit1, sEnd, IsDone},
    {sDecimal, sDigit2, IsDigit},
    {sDecimal, sEnd, IsDone},
    {sDigit2, sDigit2, IsDigit},
    {sDigit2, sEnd, IsDone}
};

static Parser parser(sStart, sEnd, LoadChar);

void testPass1(const std::string& s)
{
    std::cout << "in:" << s << std::endl;
    InStr in(s);
    std::string out;
    if (!parser.process(in, out))
    {
        std::cout << "ERROR" << std::endl;
    }
    std::cout << "out:" << out << std::endl;
}

void testPass2(const std::string& s)
{
    std::cout << "in:" << s << std::endl;
    InStr in(s);
    std::string out;
    Parser::Iterator it(parser, in, out);
    size_t numSteps = 0;
    while (!it.atEnd())
    {
        std::cout << "step=" << numSteps << " out=" << out << std::endl;
        ++numSteps;
        if (!parser.processStep(it))
        {
            std::cout << "ERROR" << std::endl;
            break;
        }
    }
    std::cout << "steps=" << numSteps << std::endl;
    std::cout << "out:" << out << std::endl;
}

int main(int argc, const char * argv[]) {
    testPass1("32");
    testPass1("101.57");
    testPass1("32,000");
    testPass2("32");
    testPass2("101.57");
    testPass2("32,000");
    return 0;
}
