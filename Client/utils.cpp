#define _CRT_SECURE_NO_WARNINGS
#include "utils.h"

SOCKET createSocket() {
    // Client side:
    // Create a socket and connect to an internet address.

    SOCKET connSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (INVALID_SOCKET == connSocket)  {
        string msg("Time Client: Error at socket(): ");

        msg += WSAGetLastError();

        cout << msg;
        WSACleanup();

    }

    return connSocket;
}

sockaddr_in createSockaddr() {
    // For a client to communicate on a network, it must connect to a server.

    // Need to assemble the required data for connection in sockaddr structure.

    // Create a sockaddr_in object called server.
    sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(TIME_IP);
    server.sin_port = htons(TIME_PORT);

    return server;
}

int sendDataTo(SOCKET& m_Socket, char* sendBuff, sockaddr_in& m_Server) {
    // The send function sends data on a connected socket.
    // The buffer to be sent and its size are needed.
    // The fourth argument is an idicator specifying the way in which the call is made (0 for default).
    // The two last arguments hold the details of the server to communicate with.
    // NOTE: the last argument should always be the actual size of the client's data-structure (i.e. sizeof(sockaddr)).
    int bytesSent = sendto(m_Socket, sendBuff, (int)strlen(sendBuff), 0, (const sockaddr*)&m_Server, sizeof(m_Server));

    if (SOCKET_ERROR == bytesSent) {
        string msg("Time Client: Error at sendto(): ");

        msg += WSAGetLastError();

        cout << msg;
        WSACleanup();
    }

    return bytesSent;
}

int receiveData(SOCKET& m_Socket, char* recvBuff) {
    // Gets the server's answer using simple recieve (no need to hold the server's address).
    int bytesRecv = recv(m_Socket, recvBuff, 255, 0);

    if (SOCKET_ERROR == bytesRecv) {
        string msg("Time Client: Error at recv(): ");

        msg += WSAGetLastError();

        cout << msg;
        WSACleanup();
    }

    recvBuff[bytesRecv] = '\0'; // add the null-terminating to make it a string

    return bytesRecv;
}


void printMenu() {
    cout << "Menu" << endl << endl;
    cout << "0.  Exit" << endl;
    cout << "1.  " << GET_TIME << endl;
    cout << "2.  " << GET_TIME_WITHOUT_DATE << endl;
    cout << "3.  " << GET_TIME_SINCE_EPOCH << endl;
    cout << "4.  " << GET_CLIENT_TO_SERVER_DELAY_EST << endl;
    cout << "5.  " << MEASURE_RTT << endl;
    cout << "6.  " << GET_TIME_WITHOUT_DATE_OR_SECONDS << endl;
    cout << "7.  " << GET_YEAR << endl;
    cout << "8.  " << GET_MONTH_AND_DAY << endl;
    cout << "9.  " << GET_SECONDS_SINCE_BEGINING_OF_MONTH << endl;
    cout << "10. " << GET_WEEK_OF_YEAR << endl;
    cout << "11. " << GET_DAY_LIGHT_SAVINGS << endl;
    cout << "12. " << GET_TIME_WITHOUT_DATE_IN_CITY << endl;
    cout << "13. " << MEASURE_TIME_LAP << endl;
}

void printCityMenu() {
    cout << "City menu:" << endl;
    cout << "1. Tokyo" << endl;
    cout << "2. Melbourne" << endl;
    cout << "3. San Fransisco" << endl;
    cout << "4. Porto" << endl;
    cout << "5. UTC" << endl;
}

int generateRequest(char* sendBuff) {
    int choice = 0;
    char cityName[255] = "", request[255] = "";

    printMenu();
    choice = getChoice();
    system("cls");
 
    if (EXIT != choice) {
        if (CITY_OPTION == choice){
            handleParams(cityName);
        }

        handleRequest(request, choice);

        sprintf(sendBuff, "%s? %s ", request, cityName);
    }

    return choice;
}

int getChoice() {
    int choice = -1;
    bool validChoice = false;

    while (!validChoice) {
        cin >> choice;
        if (choice > -1 && choice < 14) {
            validChoice = true;
        }
        else {
            system("cls");
            cout << "please Chooce an option from the menu. " << endl;
            printMenu();
        }
    }
    return choice;
}

void handleRequest(char* request, int choice) {
    switch (choice){
    case 1:
        strcpy(request, GET_TIME);
        break;
    case 2:
        strcpy(request, GET_TIME_WITHOUT_DATE);
        break;
    case 3:
        strcpy(request, GET_TIME_SINCE_EPOCH);
        break;
    case 4:
        strcpy(request, GET_CLIENT_TO_SERVER_DELAY_EST);
        break;
    case 5:
        strcpy(request, GET_TIME);
        break;
    case 6:
        strcpy(request, GET_TIME_WITHOUT_DATE_OR_SECONDS);
        break;
    case 7:
        strcpy(request, GET_YEAR);
        break;
    case 8:
        strcpy(request, GET_MONTH_AND_DAY);
        break;
    case 9:
        strcpy(request, GET_SECONDS_SINCE_BEGINING_OF_MONTH);
        break;
    case 10:
        strcpy(request, GET_WEEK_OF_YEAR);
        break;
    case 11:
        strcpy(request, GET_DAY_LIGHT_SAVINGS);
        break;
    case 12:
        strcpy(request, GET_TIME_WITHOUT_DATE_IN_CITY);
        break;
    case 13:
        strcpy(request, MEASURE_TIME_LAP);
        break;
    }
}

void handleParams(char* cityName) {
    int cityNum;
    printCityMenu();
    
    cin >> cityNum;

    switch (cityNum) {
    case 1:
        strcpy(cityName, ARG_TOKYO);
        break;
    case 2:
        strcpy(cityName, ARG_MELBOURNE);
        break;
    case 3:
        strcpy(cityName, ARG_SAN_FRANCISCO);
        break;
    case 4:
        strcpy(cityName, ARG_PORTO);
        break;
    case 5:
    default:
        strcpy(cityName, ARG_UTC);
        break;
    }
    system("cls");
}

void getClientToServerDelayEstimation(char* sendBuff, char* recvBuff, SOCKET& m_Socket, sockaddr_in& m_Server) {
    const int numOfRequests = 100;
    double tickAtSend[numOfRequests];
    double delayEst, totalDelay = 0;
    int i = 0;

    // send requests
    while (i < numOfRequests) {
        sendDataTo(m_Socket, sendBuff, m_Server);
        tickAtSend[i] = GetTickCount();
        i++;
    }

    // recv ticks
    i = 0;
    while (i < numOfRequests) {
        receiveData(m_Socket, recvBuff);
        totalDelay += atof(recvBuff) - tickAtSend[i];
        i++;
    }

    delayEst = (totalDelay / numOfRequests) / CLOCKS_PER_SEC;

    cout << "Time Client: delay est is: " << delayEst << endl;
}

void measureRTT(char* sendBuff, char* recvBuff, SOCKET& m_Socket, sockaddr_in& m_Server) {
    const int numOfRequests = 100;
    double delayEst, totalDelay = 0;
    int i;
    unsigned long startTick, endTick;

    // send requests
    for (i = 0; i < numOfRequests; ++i) {
        sendDataTo(m_Socket, sendBuff, m_Server);
        startTick = GetTickCount();

        receiveData(m_Socket, recvBuff);
        endTick = GetTickCount();

        totalDelay += endTick - startTick;
    }
    delayEst = totalDelay / numOfRequests;

    cout << "Time Client: rtt est is: " << delayEst << endl;
}
