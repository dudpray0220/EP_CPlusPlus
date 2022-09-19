#include <libvirt/libvirt.h>

#include <cstring>
#include <iostream>
using namespace std;

int main() {
    // declare variables
    virConnectPtr conn;
    virDomainPtr *domains, dom;
    //	char *xmlconfig = NULL;
    char uuid[VIR_UUID_STRING_BUFLEN];  // 배열에 대한 포인터 이므로
    const char *domName;
    int domNum, domUUID;
    unsigned int i, domID;
    unsigned int flags = VIR_CONNECT_LIST_DOMAINS_ACTIVE | VIR_CONNECT_LIST_DOMAINS_INACTIVE | VIR_CONNECT_LIST_DOMAINS_PERSISTENT;

    conn = virConnectOpen("qemu:///system");
    domNum = virConnectListAllDomains(conn, &domains, flags);  // 켜진(활성) 도메인 수

    if (conn == NULL) {
        cout << "Connect HyperVisor Failed..." << endl;
        return 1;
    }
    cout << "Connect HyperVisor Success!" << endl;

    if (domNum < 0) {  // 활성 도메인이 없으면 (=다 꺼진 상태면)
        cout << "No Active Domain in KVM!" << endl;
    } else {
        cout << "Domain Numbers: " << domNum << endl;
        // 활성 도메인이 있으면 for문으로 정보 가져옴.
        for (i = 0; i < domNum; i++) {
            domID = virDomainGetID(domains[i]);
            domUUID = virDomainGetUUIDString(domains[i], uuid);
            domName = virDomainGetName(domains[i]);

            cout << "\nDomain's Name: " << domName << endl;
            cout << "Domain's ID: " << domID << endl;
            cout << "Domain's UUID: " << uuid << endl;

            // if (domName == "DLS_2") {
            //     virDomainShutdown(domains[i]);
            //     cout << domName << " VM ShutDown." << endl;
            // }

            if (!strcmp(domName, "DLS_2")) {  // str1, str2가 같으면 0. 즉 False
                                              //                virDomainShutdown(domains[i]);
                virDomainCreate(domains[i]);
                cout << domName << " VM Start." << endl;
            };

            if (!strcmp(domName, "StarVm1")) {  // str1, str2가 같으면 0. 즉 False
                                                //                virDomainSuspend(domains[i]);
                virDomainCreate(domains[i]);
                cout << domName << " VM Start." << endl;
            };
        }
    }

    cout
        << "------------------------------------------------------------------------"
        << endl;
    //	dom = virDomainDefineXML(conn, xmlconfig);
    //	fprintf(stderr, "Guest %s is defined\n", virDomainGetName(dom));

    system("virsh list --all");
    virConnectClose(conn);
    return 0;
}
