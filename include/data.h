#pragma once
#include <iostream>
#include <string>
#include <map>

using namespace std;
namespace data {
  map<string, string> colors = {
    { "black", "\033[30m" },
    { "red", "\033[31m" },
    { "green", "\033[32m" },
    { "yellow", "\033[33m" },
    { "blue", "\033[34m" },
    { "cyan", "\033[36m" }
  };
}