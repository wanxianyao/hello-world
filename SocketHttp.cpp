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
*   ����������HttpGet����
*   ����˵����
*               strUrl��     Http����URL
*               strData��    POST�����͵�����
*               strResponse��Http������Ӧ
*   �� �� ֵ��
*               1��ʾ�ɹ�
*               0��ʾʧ��
**/
int HttpRequest::HttpGet(const char* strUrl, char* strResponse)
{
    return HttpRequestExec("GET", strUrl, NULL, strResponse);
}


/**
*   ����������HttpPost����
*   ����˵����
*               strUrl��     Http����URL
*               strData��    POST�����͵�����
*               strResponse��Http������Ӧ
*   �� �� ֵ��
*               1��ʾ�ɹ�
*               0��ʾʧ��
**/
int HttpRequest::HttpPost(const char* strUrl, const char* strData, char* strResponse)
{
    return HttpRequestExec("POST", strUrl, strData, strResponse);
}


//ִ��HTTP����GET��POST
int HttpRequest::HttpRequestExec(const char* strMethod, const char* strUrl, const char* strData, char* strResponse)
{
    //�ж�URL�Ƿ���Ч
    if((strUrl == NULL) || (0 == strcmp(strUrl, ""))) 
	{
        return 0;
    }

    //����URL����
    if(URLSIZE < strlen(strUrl)) 
	{
        return 0;
    }

    //����HTTPЭ��ͷ
    char* strHttpHead = HttpHeadCreate(strMethod, strUrl, strData);

    //�ж��׽���m_iSocketFd�Ƿ���Ч����Ч��ֱ�ӷ�������
    if(m_iSocketFd != INVALID_SOCKET) 
	{
        //���SocketFd�Ƿ�Ϊ��д���ɶ�״̬
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

    //��������ʽ����
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


//����HTTP��Ϣͷ
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


//����HTTP���󲢽�����Ӧ
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
    if (ret == 0) //���ӹر�
    {
        closesocket(iSockFd);
        return NULL;
    }
	else if(ret < 0) //����
	{
		closesocket(iSockFd);
		return NULL;

	} 
    else if(ret > 0) {
        return buf;
    }

	return NULL;
}


//��HTTP����URL�л�ȡ������ַ����ַ���ߵ��ʮ����IP��ַ
char* HttpRequest::GetHostAddrFromUrl(const char* strUrl)
{
    char url[URLSIZE] = {0};
    strcpy(url, strUrl);

    char* strAddr = strstr(url, "http://");//�ж���û��http://
    if(strAddr == NULL) {
        strAddr = strstr(url, "https://");//�ж���û��https://
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


//��HTTP����URL�л�ȡHTTP�����
char* HttpRequest::GetParamFromUrl(const char* strUrl)
{
    char url[URLSIZE] = {0};
    strcpy(url, strUrl);

    char* strAddr = strstr(url, "http://");//�ж���û��http://
    if(strAddr == NULL) {
        strAddr = strstr(url, "https://");//�ж���û��https://
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


//��HTTP����URL�л�ȡ�˿ں�
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


//��HTTP����URL�л�ȡIP��ַ
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

    //�ж��Ƿ�Ϊ���ʮ����IP��ַ������ͨ��������ַ��ȡIP��ַ
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