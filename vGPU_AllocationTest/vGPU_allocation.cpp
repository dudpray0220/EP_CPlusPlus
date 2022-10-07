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
    xml_parse_result result = doc.load_file("/etc/libvirt/qemu/StarVm3.xml");
    char *vgpu_uuid;

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

    // Get UUID
    std::array<char, 128> buffer;
    std::string uuid;
    std::unique_ptr<FILE, decltype(&pclose)> pipe2(popen("mdevctl list", "r"), pclose);

    if (!pipe2) {
        throw std::runtime_error("popen2() failed!");
    }

    while (fgets(buffer.data(), buffer.size(), pipe2.get()) != nullptr) {
        uuid = buffer.data();
        uuid = uuid.substr(0, 36);                     // 문자열 자르기
        vgpu_uuid = const_cast<char *>(uuid.c_str());  // 형 변환
        break;
    }
    std::cout << "vGPU UUID: " << uuid << std::endl;

    // Write Data (vGPU)
    xml_node _hostdev = _devices.append_child("hostdev");
    _hostdev.append_attribute("mode") = "subsystem";
    _hostdev.append_attribute("type") = "mdev";
    _hostdev.append_attribute("model") = "vfio-pci";

    xml_node _source = _hostdev.append_child("source");
    xml_node _address = _source.append_child("address");
    _address.append_attribute("uuid") = vgpu_uuid;

    // Saving File
    std::cout << "Saving result: " << doc.save_file("/etc/libvirt/qemu/StarVm3.xml") << std::endl;

    // Define XML Config
    system("virsh define /etc/libvirt/qemu/StarVm3.xml");
}