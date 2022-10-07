#include <array>
#include <cstring>
#include <iostream>
#include <list>
#include <memory>
#include <regex>
#include <string>

int main() {
    // pci address example 0000\:03\:00.0 --- domain\:bus\:slot.function
    // 일단 GPU 1개로 가정
    // simple pci 구하기
    std::string simple_pci, detailed_pci, pci_function, vgpu_namePath, vgpu_available, vgpu_available_command, uuid, vgpuCreate_command, vgpuDefine_command;
    int index;
    std::string pci_command = "virsh nodedev-list --cap pci| grep ";
    std::string mdev_path = "cd /sys/bus/pci/devices/";
    std::string vGPU_name = "T4-1Q";
    std::string name_search_command = "grep -l \"" + vGPU_name + "\" nvidia-*/name";
    std::array<char, 128> buffer;
    std::list<std::string> vgpu_uuidList;

    // simple pci 구하기
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen("lspci | grep NVIDIA", "r"), pclose);

    if (!pipe) {
        throw std::runtime_error("pipe popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        simple_pci = buffer.data();
        simple_pci = simple_pci.substr(0, 7);  // example 00:02.0
        break;
    }

    // detailed pci 구하기
    pci_command += simple_pci;
    char *detail_pci_command = const_cast<char *>(pci_command.c_str());  // 형 변환
    std::cout << pci_command << std::endl;
    std::cout << detail_pci_command << std::endl;

    std::unique_ptr<FILE, decltype(&pclose)> pipe2(popen(detail_pci_command, "r"), pclose);
    if (!pipe2) {
        throw std::runtime_error("pipe2 popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe2.get()) != nullptr) {
        detailed_pci = buffer.data();  // example pci_0000_02_00_0 (domain_bus_slot_function)
        detailed_pci = detailed_pci.substr(4);
        pci_function = "." + detailed_pci.substr(11);
        detailed_pci = std::regex_replace(detailed_pci, std::regex("_"), "\\:");
        detailed_pci = detailed_pci.substr(0, 12) + pci_function;  // example 0000\:02\:00.0
        break;
    }

    // mdev_supported_types 경로로 이동
    mdev_path += detailed_pci += "/mdev_supported_types/";
    char *mdev_path_char = const_cast<char *>(mdev_path.c_str());  // 형 변환
    system(mdev_path_char);

    // 이름으로 디렉토리 출력
    char *name_search_command_char = const_cast<char *>(name_search_command.c_str());  // 형 변환
    std::unique_ptr<FILE, decltype(&pclose)> pipe3(popen(name_search_command_char, "r"), pclose);
    if (!pipe3) {
        throw std::runtime_error("pipe3 popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe3.get()) != nullptr) {
        vgpu_namePath = buffer.data();  // example nvidia-41/name
        index = vgpu_namePath.find("/");
        vgpu_namePath = vgpu_namePath.substr(0, index);  // example nvidia-41
        break;
    }

    // 생성 가능 수 표시
    vgpu_available_command = "cat " + vgpu_namePath + "/available_instances";
    char *vgpu_available_command_char = const_cast<char *>(vgpu_available_command.c_str());  // 형 변환

    std::unique_ptr<FILE, decltype(&pclose)> pipe4(popen(vgpu_available_command_char, "r"), pclose);
    if (!pipe4) {
        throw std::runtime_error("pipe4 popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe4.get()) != nullptr) {
        vgpu_available = buffer.data();  // 3 숫자로 형변환?
        break;
    }

    // UUID generate
    std::unique_ptr<FILE, decltype(&pclose)> uuidGenPipe(popen("uuidgen", "r"), pclose);
    if (!uuidGenPipe) {
        throw std::runtime_error("uuidGenPipe popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), uuidGenPipe.get()) != nullptr) {
        uuid = buffer.data();
        break;
    }

    // vGPU Create
    vgpuCreate_command = "echo \"" + uuid + "\" > " + vgpu_namePath + "/create";
    char *vgpuCreate_command_char = const_cast<char *>(vgpuCreate_command.c_str());  // 형 변환
    system(vgpuCreate_command_char);

    // vGPU Define
    vgpuDefine_command = "mdevctl define --auto --uuid " + uuid;
    char *vgpuDefine_command_char = const_cast<char *>(vgpuDefine_command.c_str());  // 형 변환
    system(vgpuDefine_command_char);

    // 정의된 vGPU UUID List에 넣기
    vgpu_uuidList.push_back(uuid);

    // 출력
    std::cout << "Simple pci 주소: " << simple_pci << std::endl;
    std::cout << "파싱한 pci 주소: " << detailed_pci << std::endl;
    std::cout << "vGPU 유형 이름: " << vgpu_namePath << std::endl;
    std::cout << "vGPU 유형 생성 가능 수: " << vgpu_available << std::endl;
    std::cout << "vGPU Create Command: " << vgpuCreate_command_char << std::endl;
    std::cout << "vGPU Define Command: " << vgpuDefine_command_char << std::endl;
    std::cout << "List 갯수: " << vgpu_uuidList.size() << std::endl;
    for (auto item : vgpu_uuidList) {
        std::cout << "할당된 vGPU UUID: " << item << std::endl;
    }
}