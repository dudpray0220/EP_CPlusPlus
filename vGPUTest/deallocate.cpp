#include <iostream>

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
    xml_node _devices = doc.child("domain").child("devices");

    // Delete Data (vGPU)
    if (_devices.remove_child("hostdev")) {
        std::cout << "Delete Sucess" << std::endl;
        // Saving File
        std::cout << "Saving result: " << doc.save_file("vGPUTest/modified_example.xml") << std::endl;
        return 0;
    } else {
        std::cout << "Delete Fail!" << std::endl;
        return 1;
    }
}