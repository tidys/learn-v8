#ifndef _CPP_CLASS_
#define _CPP_CLASS_
#include "stdio.h"
#include <iostream>
#include <fstream>
#include <string>
class CPP
{
public:
    CPP(int v)
    {
        this->number = v;
    }
    int number = 0;

    void numberAdd()
    {
        this->number++;
    }
    void say(std::string &str)
    {
        printf("[%d] %s\n", this->number, str.c_str());
    }
    void brother(CPP *cpp)
    {
        cpp->number = 66;
        std::string ok("66");
        cpp->say(ok);
    }
};

#endif // !_CPP_CLASS_
