#include<stdio.h>
#include<winsock2.h>
#include<windows.h>
#pragma comment(lib,"ws2_32.lib")

int main()
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		printf("WSAStartup failed\n");
		return -1;
	}
	SOCKET sServer = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (sServer == SOCKET_ERROR)
	{
		printf("socket failed\n");
		WSACleanup();
		return -1;
	}
	SOCKADDR_IN addrServer;
	addrServer.sin_family = AF_INET;
	addrServer.sin_port = htons(6000);
	addrServer.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	bind(sServer, (sockaddr*)&addrServer, sizeof(sockaddr));
	char buf[1024];
	while (true)
	{
		memset(buf, 0, 1024);
		SOCKADDR_IN addrClient;
		memset(&addrClient, 0, sizeof(sockaddr_in));
		int clientLen = sizeof(sockaddr);
		int ret = recvfrom(sServer, buf, sizeof(buf), 0, (sockaddr*)&addrClient, &clientLen);
		printf("接收消息[%s] from IP [%s] Port[%d]\n", buf, inet_ntoa(addrClient.sin_addr), htons(addrClient.sin_port));
		sendto(sServer, "good job", sizeof("good job"), 0, (sockaddr*)&addrClient, clientLen);
		printf("send msg back to IP[%s] Port[%d]\n", inet_ntoa(addrClient.sin_addr), ntohs(addrClient.sin_port));
	}
	closesocket(sServer);
	WSACleanup();
	return 0;
}