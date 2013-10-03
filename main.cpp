#include <stdio.h>
#include <winsock2.h> 
#include <windows.h>

#define MY_PORT    8082
#define HELLO "Hello!\n"

DWORD WINAPI EchoConversation(LPVOID client_socket);

int main()
{
	char buff[1024];  



	if (WSAStartup(0x0202,(WSADATA*) &buff[0])) 
	{
		return -1;
	}


	SOCKET mysocket;

	if ((mysocket = socket(AF_INET,SOCK_STREAM,0)) < 0)
	{
		return -1;
	}

	sockaddr_in local_addr;
	local_addr.sin_family = AF_INET;
	local_addr.sin_port=htons(MY_PORT);
	local_addr.sin_addr.s_addr = 0;


	if (bind(mysocket,(sockaddr *) &local_addr,
		sizeof(local_addr)))
	{
		WSACleanup();
		return -1;
	}

	if (listen(mysocket, 0x100))
	{
		closesocket(mysocket);
		WSACleanup();
		return -1;
	}

	SOCKET client_socket;  
	sockaddr_in client_addr;   



	while((client_socket = accept(mysocket, (sockaddr *)
		&client_addr, NULL)))
	{
		DWORD thID;
		CreateThread(NULL,NULL,EchoConversation,
		&client_socket,NULL,&thID);
	}


	
	closesocket(mysocket);
	WSACleanup();

	return 0;
}


DWORD WINAPI EchoConversation(LPVOID client_socket)
{
	SOCKET my_sock;
	
	my_sock = ((SOCKET *) client_socket)[0];
	
	char buff[1024];
 
	send(my_sock,HELLO,sizeof(HELLO),0);

	int bytes_recv;

	while((bytes_recv = recv(my_sock,&buff[0],sizeof(buff),0)) && bytes_recv != SOCKET_ERROR)
	{
		send(my_sock,&buff[0],bytes_recv,0);
	}

	closesocket(my_sock);

	return 0;
}