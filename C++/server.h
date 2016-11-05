#include <tchar.h>
#include<conio.h>
#include<io.h>
//#include<iostream>
//#include<stdio.h>
#include<stdio.h>
#include<winsock2.h>
#include<windows.h>
#include<WinBase.h>
#pragma comment(lib,"ws2_32.lib") //Winsock Library

#include <sstream>
#include <string>
class Server{	

public:
	double x,y,z,a,b,c,startflag;				//####xyz,abc: position coordinats   //Calib Flag: if at calibrated position, calib flag=1;
	double x_k,y_k,z_k,a_k,b_k,c_k,calibflag;
	unsigned long long int IPOC;
	Server()
	{                                     //Initializing Parameters
		x=0.0;
		y=0.0;
		z=0.0;
		a=0.0;
		b=0.0;
		c=0.0;
		calibflag=-9.0;									
		startflag=-9.0;
		x_k=0.0;
		y_k=0.0;
		z_k=0.0;
		a_k=0.0;
		b_k=0.0;
		c_k=0.0;
		IPOC = 0;
		
	}

	//// send immediately incoming IPO counter to have a timestamp
 //   std::string mirrorIPOC(std::string receive, std::string send)
 //   {
 //           //// separate IPO counter as string

 //           //int startdummy = receive.IndexOf("<IPOC>") + 6;
	//		int startdummy = receive.find("<IPOC>") + 6;
	//		
 //           //int stopdummy = receive.IndexOf("</IPOC>", startdummy);
	//		int stopdummy = receive.find("</IPOC>", startdummy);
 //           //string Ipocount = receive.Substring(startdummy, stopdummy - startdummy);
	//		std::string Ipocount = receive.substr(startdummy, stopdummy - startdummy);
 //           //// find the insert position      
 //           //startdummy = send.IndexOf("<IPOC>") + 6;
 //           //stopdummy = send.IndexOf("</IPOC>");

	//		// find the insert position      
 //           startdummy = send.find("<IPOC>") + 6;
 //           stopdummy = send.find("</IPOC>");

 //           //// remove the old value an insert the actual value
 //           send = send.erase(startdummy, stopdummy - startdummy);
 //           send = send.insert(startdummy, Ipocount);			

 //           //// send back the string
 //           return send;
 //   }


	void Connect(bool *complete)
	{
//void main(){
		WSADATA wsa;
		SOCKET s,new_socket ;
		struct sockaddr_in server,client;
		int c_sock;
		char *send_message = "",*recv_message="", in_buffer[1024];
 
		printf("\nInitialising Winsock...");
		if (WSAStartup(MAKEWORD(2,2),&wsa) != 0)
		{
			printf("Failed. Error Code : %d",WSAGetLastError());
			//return 1;
		}
		else
		printf("Initialised.\n");
     
		//Create a socket
		if((s = socket(AF_INET , SOCK_STREAM , 0 )) == INVALID_SOCKET)
		{
			printf("Could not create socket : %d" , WSAGetLastError());
		}
		else
		printf("Socket created.\n");
     
		//Prepare the sockaddr_in structure
		server.sin_family = AF_INET;
		server.sin_addr.s_addr = inet_addr("192.168.1.80");
		server.sin_port = htons( 6008 );
     
		//Bind
		if( bind(s ,(struct sockaddr *)&server , sizeof(server)) == SOCKET_ERROR)
		{
			printf("Bind failed with error code : %d" , WSAGetLastError());
			exit(EXIT_FAILURE);
		}
		else
		puts("Bind done");
 
		//Listen to incoming connections
		listen(s , 3);
     
		//Accept and incoming connection
		puts("Waiting for incoming connections...");
     
		c_sock = sizeof(struct sockaddr_in);

		new_socket = accept(s , (struct sockaddr *)&client, &c_sock);
		while (new_socket  == INVALID_SOCKET)
		{
			puts("connect error");
			new_socket = accept(s , (struct sockaddr *)&client, &c_sock);
			//return 1;
		}
     
		
			puts("Connected");
			while(new_socket != INVALID_SOCKET && !(*complete))
			{
				  

				//Receive a reply from the server
			std::string strSend = "",strReceive = "";
			//*send_message = "",*recv_message="";
			if(recv(new_socket, in_buffer, 1024,0) == SOCKET_ERROR)
				{
					puts("recv failed");
					break;
				}
     
				//puts("Reply received\n");
			
			std::string inBuf(in_buffer);
			std::size_t foundEnd = inBuf.find("</Rob>");
			//cout<<foundEnd;
			while(foundEnd == std::string::npos)
			{
				strReceive += in_buffer;
				recv(s, in_buffer, 1024,0);
				inBuf = std::string(in_buffer);
				foundEnd = inBuf.find("</Rob>");
				//printf(" Message received = %s \n ",in_buffer);
			}
			strReceive += in_buffer;
			//int found2 = strReceive.find("</IPOC");
			
			//printf(" Message received = %s \n ",in_buffer);
			strReceive = strReceive.substr (0,foundEnd+6);
			//cout<<"\n"<<found2;
			recv_message = const_cast<char*>(strReceive.c_str());
			//printf(" Message received = %s \n ",recv_message);
			sscanf(recv_message,("<Rob Type=\"KUKA\">\n<RIst X=\"%lf\" Y=\"%lf\" Z=\"%lf\" A=\"%lf\" B=\"%lf\" C=\"%lf\"/>\n<calibflag>%lf</calibflag>\n<IPOC>%llu</IPOC>\n</Rob>"),&x_k,&y_k,&z_k,&a_k,&b_k,&c_k,&calibflag,&IPOC);
			//sscanf("<Rob Type=\"KUKA\">\n<RIst X=\"12.0\" Y=\"12\" Z=\"12\" A=\"12\" B=\"12\" C=\"12\"/>\n<calibflag>12</calibflag>\n<IPOC>12</IPOC>\n</Rob>",("<Rob Type=\"KUKA\"><RIst X=\"%lf\" Y=\"%lf\" Z=\"%lf\" A=\"%lf\" B=\"%lf\" C=\"%lf\"/><calibflag>%lf</calibflag><IPOC>%llu</IPOC></Rob>"),&c_x,&y_k,&z_k,&a_k,&b_k,&c_k,&calibflag,&IPOC);
			//Send some data
			//Reply to the client
			//send_message = "Hello Client , I have received your connection. But I have to go now, bye\n";
			std::stringstream ss;
			//c = c_k;
			ss<<"<Cam X=\""<<x<<"\" Y=\""<<y<<"\" Z=\""<<z<<"\" A=\""<<a<<"\" B=\""<<b<<"\" C=\""<<c<<"\"/>\n<startflag>"<<startflag<<"</startflag>"<<"\n<IPOC>"<< IPOC<<"</IPOC>\n</Sen>";
			strSend = ss.str();
			//sprintf(strSend, "<Cam X=\"%d\" Y=\"%d\" Z=\"%d\" A=\"%d\" B=\"%d\" C=\"%d\"/>\n<startflag>%d</startflag>",x,y,z,a,b,c,startflag);
			strSend = "<Sen Type=\"Camera\">\n<EStr>Vision PC Connected</EStr>\n" + strSend ;
            //strSend = mirrorIPOC(strReceive, strSend);
            //std::cout<<"Send msg:"<<strSend;
			send_message = const_cast<char*>(strSend.c_str());
			if( send(new_socket , send_message , strlen(send_message) , 0) < 0)
				{
					puts("Send failed");
					break;
					//return 1;
				}
			//puts("Data Send\n");
			//getchar();
			//vallue for reference
			//x_kG = x_k;
			//calibflagG = calibflag;
			}	
     

		puts("socket closed\n");
		 
		//if (s == INVALID_SOCKET)
		//{
		//	printf("accept failed with error code : %d" , WSAGetLastError());
		//	//return 1;
		//}
		fflush(stdin);
		//getchar();
		closesocket(s);
		WSACleanup();
		
    
		//return 0;
	}
	};