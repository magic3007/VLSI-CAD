#pragma once
#include "sop.h"
#include <iostream>

class SOPFactory{
public:
    static SOP parseFromFile(std::istream &is);
};
