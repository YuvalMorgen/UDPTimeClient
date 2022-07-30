#pragma once
#define _CRT_SECURE_NO_WARNINGS
#pragma comment(lib, "Ws2_32.lib")
#include <iostream>
#include <WinSock2.h>
#include <Windows.h>
#include <string.h>
#include <time.h>

using namespace std;

#define TIME_PORT 27015

#define EXIT 0
#define GET_TIME "GetTime"
#define GET_TIME_WITHOUT_DATE "GetTimeWithoutDate"
#define GET_TIME_SINCE_EPOCH "GetTimeSinceEpoch"
#define GET_CLIENT_TO_SERVER_DELAY_EST "GetClientToServerDelayEstimation"
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

#define GMT_TOKYO 9
#define GMT_MELBOURNE 11
#define GMT_SAN_FRANCISCO -8
#define GMT_PORTO 0
#define TIME_UTC 0

SOCKET createSocket();
int receiveData(SOCKET& m_socket, char* recvBuff, sockaddr& client_addr, int& client_addr_len);
int sendDataTo(SOCKET& m_socket, char* sendBuff, sockaddr& client_addr, int& client_addr_len);

void getOutputByAction(char* sendBuff, char* request);
void getTime(char* sendBuff);
void getTimeWithoutDate(char* sendBuff);
void getTimeSinceEpoch(char* sendBuff);
void getClientToServerDelayEstimation(char* sendBuff);
void getTimeWithoutDateOrSeconds(char* sendBuff);
void getYear(char* sendBuff);
void getMonthAndDay(char* sendBuff);
void getSecondsSinceBeginingOfMonth(char* sendBuff);
void getWeekOfYear(char* sendBuff);
void getDaylightSavings(char* sendBuff);
void GetTimeWithoutDateInCity(char* sendBuff, char* args = nullptr);
void measureTimeLap(char* sendBuff);
void readFormatRequest(char* sendBuff, char* request, char* cityName);
void getOutputByAction(char* sendBuff, char* request, char* cityName = nullptr);
void programInit();
void executeProgram(SOCKET& m_socket);
void endProgram(SOCKET& m_socket);
