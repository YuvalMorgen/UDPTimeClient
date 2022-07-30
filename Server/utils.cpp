#define _CRT_SECURE_NO_WARNINGS
#include "utils.h"

SOCKET createSocket()
{
    // Server side:
    // Create and bind a socket to an internet address.
    // After initialization, a SOCKET object is ready to be instantiated.

    // Create a SOCKET object called m_socket.
    // For this application:	use the Internet address family (AF_INET),
    //							datagram sockets (SOCK_DGRAM),
    //							and the UDP/IP protocol (IPPROTO_UDP).
    SOCKET m_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    // Check for errors to ensure that the socket is a valid socket.
    // Error detection is a key part of successful networking code.
    // If the socket call fails, it returns INVALID_SOCKET.
    // The "if" statement in the previous code is used to catch any errors that
    // may have occurred while creating the socket. WSAGetLastError returns an
    // error number associated with the last error that occurred.
    if (INVALID_SOCKET == m_socket) {
        string msg("Time Server: Error at socket(): ");

        msg += WSAGetLastError();

        cout << msg;
        WSACleanup();
    }

    // For a server to communicate on a network, it must first bind the socket to
    // a network address.

    // Need to assemble the required data for connection in sockaddr structure.

    // Create a sockaddr_in object called serverService.
    sockaddr_in serverService;
    // Address family (must be AF_INET - Internet address family).
    serverService.sin_family = AF_INET;
    // IP address. The sin_addr is a union (s_addr is a unsigdned long (4 bytes) data type).
    // INADDR_ANY means to listen on all interfaces.
    // inet_addr (Internet address) is used to convert a string (char *) into unsigned int.
    // inet_ntoa (Internet address) is the reverse function (converts unsigned int to char *)
    // The IP address 127.0.0.1 is the host itself, it's actually a loop-back.
    serverService.sin_addr.s_addr = INADDR_ANY; //inet_addr("127.0.0.1");
    // IP Port. The htons (host to network - short) function converts an
    // unsigned short from host to TCP/IP network byte order (which is big-endian).
    serverService.sin_port = htons(TIME_PORT);

    // Bind the socket for client's requests.

    // The bind function establishes a connection to a specified socket.
    // The function uses the socket handler, the sockaddr structure (which
    // defines properties of the desired connection) and the length of the
    // sockaddr structure (in bytes).
    if (SOCKET_ERROR == bind(m_socket, (SOCKADDR*)&serverService, sizeof(serverService))) {
        string msg("Time Server: Error at bind(): ");

        msg += WSAGetLastError();

        cout << msg;
        closesocket(m_socket);
        WSACleanup();
    }
    return m_socket;
}

int receiveData(SOCKET& m_socket, char* recvBuff, sockaddr& client_addr, int& client_addr_len) {
    // Get client's requests and answer them.
    // The recvfrom function receives a datagram and stores the source address.
    // The buffer for data to be received and its available size are
    // returned by recvfrom. The fourth argument is an idicator
    // specifying the way in which the call is made (0 for default).
    // The two last arguments are optional and will hold the details of the client for further communication.
    // NOTE: the last argument should always be the actual size of the client's data-structure (i.e. sizeof(sockaddr)).
    int bytesRecv = recvfrom(m_socket, recvBuff, 255, 0, &client_addr, &client_addr_len);

    if (SOCKET_ERROR == bytesRecv) {
        string msg("Time Server: Error at recvfrom(): ");

        msg += WSAGetLastError();

        cout << msg;
        closesocket(m_socket);
        WSACleanup();
    }

    recvBuff[bytesRecv] = '\0'; //add the null-terminating to make it a string
    return bytesRecv;
}

int sendDataTo(SOCKET& m_socket, char* sendBuff, sockaddr& client_addr, int& client_addr_len) {
    // Sends the answer to the client, using the client address gathered
    // by recvfrom.
    int bytesSent = sendto(m_socket, sendBuff, (int)strlen(sendBuff), 0, (const sockaddr*)&client_addr, client_addr_len);

    if (SOCKET_ERROR == bytesSent) {
        string msg("Time Server: Error at sendto(): ");

        msg += WSAGetLastError();

        cout << msg;
        closesocket(m_socket);
        WSACleanup();

        throw exception(msg.c_str());
    }

    return bytesSent;
}


void getOutputByAction(char* sendBuff, char* request, char* cityName) {
    if (0 == strcmp(request, GET_TIME)) {
        getTime(sendBuff);
    }
    else if (0 == strcmp(request, GET_TIME_WITHOUT_DATE)) {
        getTimeWithoutDate(sendBuff);
    }
    else if (0 == strcmp(request, GET_TIME_SINCE_EPOCH)) {
        getTimeSinceEpoch(sendBuff);
    }
    else if (0 == strcmp(request, GET_CLIENT_TO_SERVER_DELAY_EST)) {
        getClientToServerDelayEstimation(sendBuff);
    }
    else if (0 == strcmp(request, GET_TIME_WITHOUT_DATE_OR_SECONDS)) {
        getTimeWithoutDateOrSeconds(sendBuff);
    }
    else if (0 == strcmp(request, GET_YEAR)) {
        getYear(sendBuff);
    }
    else if (0 == strcmp(request, GET_MONTH_AND_DAY)) {
        getMonthAndDay(sendBuff);
    }
    else if (0 == strcmp(request, GET_SECONDS_SINCE_BEGINING_OF_MONTH)) {
        getSecondsSinceBeginingOfMonth(sendBuff);
    }
    else if (0 == strcmp(request, GET_WEEK_OF_YEAR)) {
        getWeekOfYear(sendBuff);
    }
    else if (0 == strcmp(request, GET_TIME_WITHOUT_DATE_IN_CITY)) {
        GetTimeWithoutDateInCity(sendBuff, cityName);
    }
    else if (0 == strcmp(request, MEASURE_TIME_LAP)) {
        measureTimeLap(sendBuff);
    }
    else if (0 == strcmp(request, GET_DAY_LIGHT_SAVINGS)) {
        getDaylightSavings(sendBuff);
    }
}

void getTime(char* sendBuff) {
    // Get the current time.
    time_t timer;

    time(&timer);
    // Parse the current time to printable string.
    strcpy(sendBuff, ctime(&timer));
    sendBuff[strlen(sendBuff) - 1] = '\0'; // to remove the new-line from the created string
}

void getTimeWithoutDate(char* sendBuff) {
    time_t timer;

    time(&timer);
    struct tm* times = localtime(&timer);

    sprintf(sendBuff, "%d:%d:%d", times->tm_hour, times->tm_min, times->tm_sec);
}

void getTimeSinceEpoch(char* sendBuff) {
    time_t timer;

    time(&timer);
    sprintf(sendBuff, "%d", timer);
}

void getClientToServerDelayEstimation(char* sendBuff) {
    sprintf(sendBuff, "%lu", GetTickCount());
}

void getTimeWithoutDateOrSeconds(char* sendBuff) {
    time_t timer;

    time(&timer);
    struct tm* times = localtime(&timer);

    sprintf(sendBuff, "%d:%d", times->tm_hour, times->tm_min);
}

void getYear(char* sendBuff) {
    time_t timer;

    time(&timer);
    struct tm* times = localtime(&timer);

    sprintf(sendBuff, "%d", times->tm_year + 1900);
}

void getMonthAndDay(char* sendBuff)
{
    time_t timer;

    time(&timer);
    struct tm* times = localtime(&timer);

    sprintf(sendBuff, "%d/%d", times->tm_mday, times->tm_mon + 1);
}

void getSecondsSinceBeginingOfMonth(char* sendBuff) {
    time_t timer;

    time(&timer);
    struct tm* times = localtime(&timer);
    int secondFromDay = times->tm_mday * 24 * 60 * 60;
    int secondFromHour = times->tm_hour * 60 * 60;
    int secondFromMin = times->tm_min * 60;
    int seconds = times->tm_sec;
    int res = secondFromDay + secondFromHour + secondFromMin + seconds;

    sprintf(sendBuff, "%d", res);
}

void getWeekOfYear(char* sendBuff) {
    time_t timer;

    time(&timer);
    struct tm* times = localtime(&timer);

    sprintf(sendBuff, "%d", times->tm_yday / 7);
}

void getDaylightSavings(char* sendBuff) {
    time_t timer;

    time(&timer);
    struct tm* times = localtime(&timer);

    sprintf(sendBuff, "%d", times->tm_isdst);
}

void GetTimeWithoutDateInCity(char* sendBuff, char* cityName) {
    int offset = 0;
    time_t timer;

    time(&timer);
    struct tm* times = gmtime(&timer);

    if (nullptr == cityName || 0 == strcmp(cityName, ARG_UTC)) {
        offset = TIME_UTC;
    }
    else if (0 == strcmp(cityName, ARG_TOKYO)) {
        offset = GMT_TOKYO;
    }
    else if (0 == strcmp(cityName, ARG_MELBOURNE)) {
        offset = GMT_MELBOURNE;
    }
    else if (0 == strcmp(cityName, ARG_SAN_FRANCISCO)) {
        offset = GMT_SAN_FRANCISCO;
    }
    else if (0 == strcmp(cityName, ARG_PORTO)) {
        offset = GMT_PORTO;
    }

    sprintf(sendBuff, "%d:%d:%d", (times->tm_hour + offset) % 24, times->tm_min, times->tm_sec);
}

void measureTimeLap(char* sendBuff) {
    static double startTime = 0;
    static bool isFirst = true;
    double currTime, timePass;

    if (isFirst) {
        startTime = GetTickCount();
        isFirst = false;
        
        strcpy(sendBuff, "measureTimeLap started.");
    }
    else {
        currTime = GetTickCount();
        timePass = currTime - startTime;

        if (timePass / CLOCKS_PER_SEC > 180) {
            strcpy(sendBuff, "measureTimeLap stopped after 3min.");
        }
        else {
            sprintf(sendBuff, "%s%f", "measure time lap is: ", timePass / CLOCKS_PER_SEC);
        }

        // reset for next time
        startTime = 0;
        isFirst = true;
    }
}

void readFormatRequest(char* sendBuff, char* request, char* cityName) {
    char* token = strtok(sendBuff, "?");

    strcpy(request, token);
    token = strtok(NULL, "?");
    if (token != NULL) {
        strcpy(cityName, token);
    }
}
