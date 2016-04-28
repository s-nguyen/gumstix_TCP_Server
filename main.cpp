#include "leptonSPI.h"
#include "TCP_Server.h"
#include <iostream>

using namespace std;

int main() {

    leptonSPI *c = new leptonSPI();
    TCP_Server *datapipe = new TCP_Server;

    uint8_t ack[1];
    datapipe->tcp_init();
    cout << "connected" << endl;

    while(1) {
        c->getFrame();
        cout << "Recieved Frame" << endl;
        datapipe->tcp_write(c->frame, 9840);
        datapipe->tcp_read(ack, 1);
    }


    datapipe->~TCP_Server();
    return 0;
}
// system("modprobe g_serial");
