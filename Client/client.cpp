#include "utils.h"

int main() {
    programInit();
    SOCKET connSocket = createSocket();

    bool stayOnline = true;

    while (stayOnline) {
        stayOnline = executeProgram(connSocket);
    }

    // Closing connections and Winsock.
    cout << "Time Client: Closing Connection.\n";
    closesocket(connSocket);

    system("pause");
    return 0;
}

void programInit(){
    // Initialize Winsock (Windows Sockets).

    WSAData wsaData;
    if (NO_ERROR != WSAStartup(MAKEWORD(2, 2), &wsaData)){
        cout << "Time Client: Error at WSAStartup()\n";
        return;
    }
}

bool executeProgram(SOCKET& m_Socket) {
    bool stayOnline = true;
    sockaddr_in server = createSockaddr();
    // Send and receive data.

    char sendBuff[255];
    char recvBuff[255];

    int reqNum = generateRequest(sendBuff);

    if (EXIT == reqNum) {
        stayOnline = false;
    }
    else if (4 == reqNum) {
        getClientToServerDelayEstimation(sendBuff, recvBuff, m_Socket, server);
    }
    else if (5 == reqNum) {
        measureRTT(sendBuff, recvBuff, m_Socket, server);
    }
    else {
        int bytesSent = sendDataTo(m_Socket, sendBuff, server);
        cout << "Time Client: Sent: " << bytesSent << "/" << strlen(sendBuff) << " bytes of \"" << sendBuff << "\" message.\n";

        int bytesRecv = receiveData(m_Socket, recvBuff);
        cout << "Time Client: Recieved: " << bytesRecv << " bytes of \"" << recvBuff << "\" message.\n";
    }

    return stayOnline;
}
