#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#pragma comment(lib, "Ws2_32.lib")
#include <iostream>
#include <WinSock2.h>
#include <string.h>


using namespace std;

#define TIME_PORT 27015
#define TIME_IP "127.0.0.1"

#define EXIT 0
#define CITY_OPTION 12
#define GET_TIME "GetTime"
#define GET_TIME_WITHOUT_DATE "GetTimeWithoutDate"
#define GET_TIME_SINCE_EPOCH "GetTimeSinceEpoch"
#define GET_CLIENT_TO_SERVER_DELAY_EST "GetClientToServerDelayEstimation"
#define MEASURE_RTT "MeasureRTT"
#define GET_TIME_WITHOUT_DATE_OR_SECONDS "GetTimeWithoutDateOrSeconds"
#define GET_YEAR "GetYear"
#define GET_MONTH_AND_DAY "GetMonthAndDay"
#define GET_SECONDS_SINCE_BEGINING_OF_MONTH "getSecondsSinceBeginingOfMonth"
#define GET_WEEK_OF_YEAR "GetWeekOfYear"
#define GET_DAY_LIGHT_SAVINGS "GetDaylightSavings"
#define GET_TIME_WITHOUT_DATE_IN_CITY "GetTimeWithoutDateInCity"
#define MEASURE_TIME_LAP "MeasureTimeLap"

#define ARG_TOKYO "Tokyo"
#define ARG_MELBOURNE "Melbourne"
#define ARG_SAN_FRANCISCO "SanFransisco"
#define ARG_PORTO "Porto"
#define ARG_UTC "Utc"



SOCKET createSocket();
sockaddr_in createSockaddr();
int sendDataTo(SOCKET& m_Socket, char* sendBuff, sockaddr_in& m_Server);
int receiveData(SOCKET& m_Socket, char* recvBuff);

void printMenu();
int getChoice();
void printCityMenu();
int generateRequest(char* sendBuff);
void handleRequest(char* request, int reqNum);
void handleParams(char* cityName);
void getClientToServerDelayEstimation(char* sendBuff, char* recvBuff, SOCKET& m_Socket, sockaddr_in& m_Server);
void measureRTT(char* sendBuff, char* recvBuff, SOCKET& m_Socket, sockaddr_in& m_Server);
void programInit();
bool executeProgram(SOCKET& m_Socket);
