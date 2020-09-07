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

typedef Machine<InStr, char, std::string> Parser;
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

void testPass(const std::string& s)
{
    InStr in(s);
    std::string out;
    parser.process(in, out);
    std::cout << "in:" << s << std::endl;
    std::cout << "out:" << out << std::endl;
}

int main(int argc, const char * argv[]) {
    testPass("32");
    testPass("101.57");
    return 0;
}
