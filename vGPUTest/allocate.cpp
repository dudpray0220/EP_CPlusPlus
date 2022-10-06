#include <iostream>
#include <string>

#include "pugixml.hpp"
using namespace pugi;

int main() {
    xml_document doc;
    xml_parse_result result = doc.load_file("vGPUTest/example.xml");
    char vgpu_uuid[] = "85006552-1b4b-45ef-ad62-de05be9171df";
    std::string vgpu_uuid2 = "83c32df7-d52e-4ec1-9668-1f3c7e4df107";
    char *c = const_cast<char *>(vgpu_uuid2.c_str());

    if (!result) {
        std::cout << "Error" << std::endl;
        return -1;
    } else {
        std::cout << "Success!" << std::endl;
    }

    std::cout << "Load Status: " << result.description() << std::endl;
    std::cout << "------------------------------------------------------------" << std::endl;

    // Load Data
    xml_node _domain = doc.child("domain");
    xml_node _name = doc.child("domain").child("name");
    xml_node _uuid = doc.child("domain").child("uuid");
    xml_node _devices = doc.child("domain").child("devices");
    xml_node _osType = doc.child("domain").child("os").child("type");
    xml_node _disk = doc.child("domain").child("devices").child("disk");

    std::cout << "domain type: " << _domain.attribute("type").value() << std::endl;
    std::cout << "VM Name: " << _name.child_value() << std::endl;
    std::cout << "VM UUID: " << _uuid.child_value() << std::endl;
    std::cout << "OS Machine : " << _osType.attribute("machine").value() << std::endl;
    std::cout << "Disk Type : " << _disk.attribute("type").value() << std::endl;

    // Write Data (vGPU)
    xml_node _hostdev = _devices.append_child("hostdev");
    _hostdev.append_attribute("mode") = "subsystem";
    _hostdev.append_attribute("type") = "mdev";
    _hostdev.append_attribute("model") = "vfio-pci";

    xml_node _source = _hostdev.append_child("source");
    xml_node _address = _source.append_child("address");
    _address.append_attribute("uuid") = c;

    // Saving File
    std::cout << "Saving result: " << doc.save_file("vGPUTest/modified_example.xml") << std::endl;
}