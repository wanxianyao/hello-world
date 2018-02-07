#pragma once
#include <stdio.h>
#include <WinSock2.h>
#pragma comment (lib, "ws2_32")

#pragma warning(disable:4996)

#define BUFSIZE 41000
#define URLSIZE 1024

class HttpRequest
{
public:
    HttpRequest();
    ~HttpRequest();

    int HttpGet(const char* strUrl, char* strResponse);
    int HttpPost(const char* strUrl, const char* strData, char* strResponse);

private:
    int   HttpRequestExec(const char* strMethod, const char* strUrl, const char* strData, char* strResponse);
    char* HttpHeadCreate(const char* strMethod, const char* strUrl, const char* strData);
    char* HttpDataTransmit(char *strHttpHead, const int iSockFd);

    int   GetPortFromUrl(const char* strUrl);
    char* GetIPFromUrl(const char* strUrl);
    char* GetParamFromUrl(const char* strUrl);
    char* GetHostAddrFromUrl(const char* strUrl);

    int   SocketFdCheck(const int iSockFd);

    SOCKET m_iSocketFd;
};
