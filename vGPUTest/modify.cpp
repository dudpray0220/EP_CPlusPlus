#include <array>
#include <cstdio>
#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>

#include "pugixml.hpp"
using namespace pugi;

int main() {
    xml_document doc;
    xml_parse_result result = doc.load_file("vGPUTest/modified_example.xml");

    if (!result) {
        std::cout << "Error" << std::endl;
        return -1;
    } else {
        std::cout << "Success!" << std::endl;
    }

    std::cout << "Load Status: " << result.description() << std::endl;
    std::cout << "------------------------------------------------------------" << std::endl;

    // Load Data
    // xml_node _devices = doc.child("domain").child("devices");

    // popen (terminal result)
    // FILE *pipe;
    // char buffer[128];

    // if ((pipe = popen("ls -al", "r")) == NULL) {
    //     // error processing and exit
    //     perror("popen() Fail!");
    //     return -1;
    // }
    // while (!feof(pipe)) {
    //     if (fgets(buffer, 128, pipe) != NULL) {
    //         std::cout << buffer;
    //     }
    // }
    // pclose(pipe);

    std::cout << "\n------------------------------------------------------------" << std::endl;

    // popen 2
    std::array<char, 128> buffer2;
    std::string result2;
    std::unique_ptr<FILE, decltype(&pclose)> pipe2(popen("ls -al", "r"), pclose);
    if (!pipe2) {
        throw std::runtime_error("popen2() failed!");
    }
    while (fgets(buffer2.data(), buffer2.size(), pipe2.get()) != nullptr) {
        result2 = buffer2.data();
        if (result2.length() < 20) {
            std::cout << "첫 줄 " << result2;
            continue;
        } else {
            result2 = result2.substr(40);
            std::cout << result2;
        }
    }
}
