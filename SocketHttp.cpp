//#include "stdafx.h"
#include "SocketHttp.h"


HttpRequest::HttpRequest()
{
	WSADATA data;
	WSAStartup(0x0202, &data);
	m_iSocketFd = INVALID_SOCKET;
}

HttpRequest::~HttpRequest()
{
	if ( INVALID_SOCKET != m_iSocketFd )
		closesocket(m_iSocketFd);
	WSACleanup();
}

/**
*   功能描述：HttpGet请求
*   参数说明：
*               strUrl：     Http请求URL
*               strData：    POST请求发送的数据
*               strResponse：Http请求响应
*   返 回 值：
*               1表示成功
*               0表示失败
**/
int HttpRequest::HttpGet(const char* strUrl, char* strResponse)
{
    return HttpRequestExec("GET", strUrl, NULL, strResponse);
}


/**
*   功能描述：HttpPost请求
*   参数说明：
*               strUrl：     Http请求URL
*               strData：    POST请求发送的数据
*               strResponse：Http请求响应
*   返 回 值：
*               1表示成功
*               0表示失败
**/
int HttpRequest::HttpPost(const char* strUrl, const char* strData, char* strResponse)
{
    return HttpRequestExec("POST", strUrl, strData, strResponse);
}


//执行HTTP请求，GET或POST
int HttpRequest::HttpRequestExec(const char* strMethod, const char* strUrl, const char* strData, char* strResponse)
{
    //判断URL是否有效
    if((strUrl == NULL) || (0 == strcmp(strUrl, ""))) 
	{
        return 0;
    }

    //限制URL长度
    if(URLSIZE < strlen(strUrl)) 
	{
        return 0;
    }

    //创建HTTP协议头
    char* strHttpHead = HttpHeadCreate(strMethod, strUrl, strData);

    //判断套接字m_iSocketFd是否有效，有效就直接发送数据
    if(m_iSocketFd != INVALID_SOCKET) 
	{
        //检查SocketFd是否为可写不可读状态
        //if(SocketFdCheck(m_iSocketFd) > 0) 
		{
            char* strResult = HttpDataTransmit(strHttpHead, m_iSocketFd);
            if(NULL != strResult) 
			{
                strcpy(strResponse, strResult);
                return 1;
            }
        }
    }

    //Create socket
    m_iSocketFd = INVALID_SOCKET;
    m_iSocketFd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (m_iSocketFd == INVALID_SOCKET ) 
	{
        return 0;
    }

    //Bind address and port
    int iPort = GetPortFromUrl(strUrl);
    if(iPort < 0) 
	{
        return 0;
    }
    char* strIP = GetIPFromUrl(strUrl);
    if(strIP == NULL) 
	{
        return 0;
    }

	sockaddr_in		servaddr;
	servaddr.sin_family=AF_INET;
	servaddr.sin_port=htons(iPort);
	servaddr.sin_addr.S_un.S_addr=inet_addr(strIP);

    //非阻塞方式连接
    int iRet = connect(m_iSocketFd, (struct sockaddr *)&servaddr, sizeof(servaddr));
    if(iRet == 0) 
	{
        char* strResult = HttpDataTransmit(strHttpHead, m_iSocketFd);
        if(NULL != strResult) {
            strcpy(strResponse, strResult);
            free(strResult);
            return 1;
        } else {
            closesocket(m_iSocketFd);
            m_iSocketFd = INVALID_SOCKET;
            free(strResult);
            return 0;
        }
    }
    else if(iRet < 0) {

    }

    return 1;
}


//构建HTTP消息头
char* HttpRequest::HttpHeadCreate(const char* strMethod, const char* strUrl, const char* strData)
{
    char* strHost = GetHostAddrFromUrl(strUrl);
    char* strParam = GetParamFromUrl(strUrl);

    char* strHttpHead = (char*)malloc(BUFSIZE);
    memset(strHttpHead, 0, BUFSIZE);

    strcat(strHttpHead, strMethod);
    strcat(strHttpHead, " /");
    strcat(strHttpHead, strParam);
    free(strParam);
    strcat(strHttpHead, " HTTP/1.1\r\n");
    strcat(strHttpHead, "Accept: */*\r\n");
    strcat(strHttpHead, "Accept-Language: cn\r\n");
    strcat(strHttpHead, "User-Agent: Mozilla/4.0\r\n");
    strcat(strHttpHead, "Host: ");
    strcat(strHttpHead, strHost);
    strcat(strHttpHead, "\r\n");
    strcat(strHttpHead, "Cache-Control: no-cache\r\n");
    strcat(strHttpHead, "Connection: Keep-Alive\r\n");
    if(0 == strcmp(strMethod, "POST"))
    {
        char len[8] = {0};
        unsigned uLen = strlen(strData);
        sprintf(len, "%d", uLen);

        strcat(strHttpHead, "Content-Type: application/x-www-form-urlencoded\r\n");
        strcat(strHttpHead, "Content-Length: ");
        strcat(strHttpHead, len);
        strcat(strHttpHead, "\r\n\r\n");
        strcat(strHttpHead, strData);
    }
    strcat(strHttpHead, "\r\n\r\n");

    free(strHost);

    return strHttpHead;
}


//发送HTTP请求并接受响应
char* HttpRequest::HttpDataTransmit(char *strHttpHead, const int iSockFd)
{
    char* buf = (char*)malloc(BUFSIZE);
    memset(buf, 0, BUFSIZE);
    int ret = send(iSockFd,(const char *)strHttpHead,strlen(strHttpHead),0);
    free(strHttpHead);
    if (ret < 0) 
	{
        closesocket(iSockFd);
        return NULL;
    }

    ret = recv(iSockFd, (char *)buf, BUFSIZE,0);
    if (ret == 0) //连接关闭
    {
        closesocket(iSockFd);
        return NULL;
    }
	else if(ret < 0) //出错
	{
		closesocket(iSockFd);
		return NULL;

	} 
    else if(ret > 0) {
        return buf;
    }

	return NULL;
}


//从HTTP请求URL中获取主机地址，网址或者点分十进制IP地址
char* HttpRequest::GetHostAddrFromUrl(const char* strUrl)
{
    char url[URLSIZE] = {0};
    strcpy(url, strUrl);

    char* strAddr = strstr(url, "http://");//判断有没有http://
    if(strAddr == NULL) {
        strAddr = strstr(url, "https://");//判断有没有https://
        if(strAddr != NULL) {
            strAddr += 8;
        }
    } else {
        strAddr += 7;
    }

    if(strAddr == NULL) {
        strAddr = url;
    }
    int iLen = strlen(strAddr);
    char* strHostAddr = (char*)malloc(iLen+1);
    memset(strHostAddr, 0, iLen+1);
    for(int i=0; i<iLen+1; i++) {
        if(strAddr[i] == '/') {
            break;
        } else {
            strHostAddr[i] = strAddr[i];
        }
    }

    return strHostAddr;
}


//从HTTP请求URL中获取HTTP请参数
char* HttpRequest::GetParamFromUrl(const char* strUrl)
{
    char url[URLSIZE] = {0};
    strcpy(url, strUrl);

    char* strAddr = strstr(url, "http://");//判断有没有http://
    if(strAddr == NULL) {
        strAddr = strstr(url, "https://");//判断有没有https://
        if(strAddr != NULL) {
            strAddr += 8;
        }
    } else {
        strAddr += 7;
    }

    if(strAddr == NULL) {
        strAddr = url;
    }
    int iLen = strlen(strAddr);
    char* strParam = (char*)malloc(iLen+1);
    memset(strParam, 0, iLen+1);
    int iPos = -1;
    for(int i=0; i<iLen+1; i++) {
        if(strAddr[i] == '/') {
            iPos = i;
            break;
        }
    }
    if(iPos == -1) {
        strcpy(strParam, "");;
    } else {
        strcpy(strParam, strAddr+iPos+1);
    }
    return strParam;
}


//从HTTP请求URL中获取端口号
int HttpRequest::GetPortFromUrl(const char* strUrl)
{
    int iPort = -1;
    char* strHostAddr = GetHostAddrFromUrl(strUrl);
    if(strHostAddr == NULL) {
        return -1;
    }

    char strAddr[URLSIZE] = {0};
    strcpy(strAddr, strHostAddr);
    free(strHostAddr);

    char* strPort = strchr(strAddr, ':');
    if(strPort == NULL) {
        iPort = 80;
    } else {
        iPort = atoi(++strPort);
    }
    return iPort;
}


//从HTTP请求URL中获取IP地址
char* HttpRequest::GetIPFromUrl(const char* strUrl)
{
    char* strHostAddr = GetHostAddrFromUrl(strUrl);
    int iLen = strlen(strHostAddr);
    char* strAddr = (char*)malloc(iLen+1);
    memset(strAddr, 0, iLen+1);
    int iCount = 0;
    int iFlag = 0;
    for(int i=0; i<iLen+1; i++) {
        if(strHostAddr[i] == ':') {
            break;
        }

        strAddr[i] = strHostAddr[i];
        if(strHostAddr[i] == '.') {
            iCount++;
            continue;
        }
        if(iFlag == 1) {
            continue;
        }

        if((strHostAddr[i] >= '0') || (strHostAddr[i] <= '9')) {
            iFlag = 0;
        } else {
            iFlag = 1;
        }
    }
    free(strHostAddr);

    if(strlen(strAddr) <= 1) {
        return NULL;
    }

    //判断是否为点分十进制IP地址，否则通过域名地址获取IP地址
    if((iCount == 3) && (iFlag == 0)) {
        return strAddr;
    } else {
        struct hostent *he = gethostbyname(strAddr);
        free(strAddr);
        if (he == NULL) {
            return NULL;
        } else {
            struct in_addr** addr_list = (struct in_addr **)he->h_addr_list;
            for(int i = 0; addr_list[i] != NULL; i++) {
                return inet_ntoa(*addr_list[i]);
            }
            return NULL;
        }
    }
}