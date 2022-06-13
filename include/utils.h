#ifndef __UTILS_H__
#define __UTILS_H__
#include <iostream>

#define LOGF std::cerr << "(" << __FUNCTION__ << ":" << __LINE__ << ")" << std::endl
#define PR(x) std::cerr << (#x) << " = " << (x) << std::endl

#endif