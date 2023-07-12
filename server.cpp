/*
	Live Server on port 8888
*/
#include<io.h>
#include<stdio.h>
#include<winsock2.h>
#include <iostream>
#include <sstream>
#pragma comment(lib,"ws2_32.lib") //Winsock Library
using namespace std;

int main(int argc , char *argv[])
{
	WSADATA wsa;
	SOCKET s , new_socket;
	struct sockaddr_in server , client;
	int c;
	char *message;

	printf("\nInitialising Winsock...");
	if (WSAStartup(MAKEWORD(2,2),&wsa) != 0)
	{
		printf("Failed. Error Code : %d",WSAGetLastError());
		return 1;
	}
	
	printf("Initialised.\n");
	
	//Create a socket
	if((s = socket(AF_INET , SOCK_STREAM , 0 )) == INVALID_SOCKET)
	{
		printf("Could not create socket : %d" , WSAGetLastError());
	}

	printf("Socket created.\n");
	
	//Prepare the sockaddr_in structure
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons( 8888 );
	
	//Bind
	if( bind(s ,(struct sockaddr *)&server , sizeof(server)) == SOCKET_ERROR)
	{
		printf("Bind failed with error code : %d" , WSAGetLastError());
		exit(EXIT_FAILURE);
	}
	
	puts("Bind done");

	//Listen to incoming connections
	listen(s , 3);
	
	//Accept and incoming connection
	puts("Waiting for incoming connections...");
	
	c = sizeof(struct sockaddr_in);
	int no;
	char ans;
	while( (new_socket = accept(s , (struct sockaddr *)&client, &c)) != INVALID_SOCKET )
	{   
	    if (new_socket == INVALID_SOCKET){
		    printf("accept failed with error code : %d" , WSAGetLastError());
		    return 1;
	    }
		puts("Connection accepted");
		cout<<"Any Obstruction : ";
		//cin>>ans;
		//if(ans=='y'){
			//cout<<"No of Nodes Blocked = ";
			int arr;
			cin>>arr;
			stringstream ss2;
			//int arr[no];
			//for(int i=0; i<no; i++){
		        ss2 << arr;
		        message = (char*)ss2.str().c_str();
		        if(send(new_socket , message , strlen(message) , 0)<0){
			    cout<<"Error"<<endl;
			    return 1;
			    }
		    //}
			cout<<"Obstructions Updated\n";  

		/*
		char *message_aaya = (char *)malloc(sizeof(char)*25);
	    if(recv(new_socket, message_aaya, 25,0) == SOCKET_ERROR){
	        puts("Error occured");	
		}
		puts(message_aaya);
	    */
	}

	closesocket(s);
	WSACleanup();
	
	return 0;
}
