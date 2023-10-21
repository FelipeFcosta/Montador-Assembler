#ifndef INCLUDES_HPP
#define INCLUDES_HPP

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <iomanip>

using namespace std;

#define DEBUG 0
#if DEBUG
#define LOG_D(x) cout << x << endl
#else
#define LOG_D(x)
#endif

#endif