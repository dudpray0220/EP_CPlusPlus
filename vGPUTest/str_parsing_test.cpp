#include <array>
#include <cstdio>
#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>

int main() {
    std::string str = "85006552-1b4b-45ef-ad62-de05be9171df 0000:00:02.0 i915-GVTg_V4_4";
    std::string str2 = "drwxr-xr-x  3 byh byh 4096 Sep 20 16:00 ";
    str = str.substr(0, 36);
    std::cout << str << std::endl;
    std::cout << str.length() << std::endl;
    std::cout << str2.length() << std::endl;
}