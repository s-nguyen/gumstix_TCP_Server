#include "leptonSPI.h"
#include "TCP_Server.h"
#include <iostream>

using namespace std;

int main() {

    leptonSPI c;
    TCP_Server datapipe;
    datapipe.tcp_init();

    while(1) {
        c.getFrame();

        datapipe.tcp_write(c.frame);
    }


    datapipe.~TCP_Server();
    return 0;
}
// system("modprobe g_serial");
