#include <array>
#include <cstdio>
#include <iostream>
#include <list>
#include <memory>
#include <regex>
#include <sstream>
#include <stdexcept>
#include <string>

int main() {
    std::string uuid = "85006552-1b4b-45ef-ad62-de05be9171df 0000:00:02.0 i915-GVTg_V4_4";
    std::string str2 = "drwxr-xr-x  3 byh byh 4096 Sep 20 16:00 ";
    std::string simple_pci = "00:02.0";
    std::string detailed_pci = "pci_0000_02_00_0";  // to 0000\:02\:00.0
    std::string mdev_path = "cd /sys/bus/pci/devices/";
    std::string vGPU_name = "T4-1Q";
    std::string name_search = "grep -l \"" + vGPU_name + "\" nvidia-*/name";
    std::string vgpu_path = "nvidia-41/name";
    std::string vgpuCreate_command, vgpuDefine_command;
    std::list<std::string> vgpu_uuidList;

    // uuid parsing
    uuid = uuid.substr(0, 36);

    // pci parsing
    // detailed_pci = std::regex_replace(detailed_pci.substr(4), std::regex("_"), "\\:");
    detailed_pci = detailed_pci.substr(4);
    std::string pci_function = "." + detailed_pci.substr(11);
    detailed_pci = std::regex_replace(detailed_pci, std::regex("_"), "\\:");
    detailed_pci = detailed_pci.substr(0, 12) + pci_function;

    // mdev path create
    mdev_path += detailed_pci += "/mdev_supported_types/";
    char *mdev_path_char = const_cast<char *>(mdev_path.c_str());  // 형 변환

    // vgpu path parsing
    int index;
    index = vgpu_path.find("/");
    vgpu_path = vgpu_path.substr(0, index);

    // vgpu create command parsing
    vgpuCreate_command = "echo \"" + uuid + "\" > " + vgpu_path + "/create";

    // vgpu define command parsing
    vgpuDefine_command = "mdevctl define --auto --uuid " + uuid;

    // List에 값 넣기
    for (int i = 1; i <= 10; i++) {
        vgpu_uuidList.push_back("Hi " + std::to_string(i));
    }

    // print
    std::cout << uuid << std::endl;
    std::cout << uuid.length() << std::endl;
    std::cout << str2.length() << std::endl;
    std::cout << simple_pci.length() << std::endl;
    std::cout << detailed_pci << std::endl;
    std::cout << mdev_path << std::endl;
    std::cout << name_search << std::endl;
    std::cout << index << std::endl;
    std::cout << vgpu_path << std::endl;
    std::cout << vgpuCreate_command << std::endl;
    std::cout << vgpuDefine_command << std::endl;

    // List 출력
    std::cout << vgpu_uuidList.size() << std::endl;
    for (auto item : vgpu_uuidList) {
        std::cout << item << std::endl;
    }
}
