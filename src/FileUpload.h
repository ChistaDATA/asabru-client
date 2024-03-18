/**
 * FileUpload.h : Defines the class for socket connection and communication used in the client side
 */
#ifndef _FILE_UPLOAD_DOT_H
#define _FILE_UPLOAD_DOT_H
#ifdef WINDOWS_OS
#include <windows.h>
#else
#include <unistd.h>       // defines miscellaneous symbolic constants and types, and declares miscellaneous functions.
#include <sys/socket.h>   // contains sockets definitions
#include <netinet/in.h>   // contains definitions for the internet protocol family.
#include <arpa/inet.h>    // contains definitions for internet operations
#include <netdb.h>        // contains definitions for network database operations.
#include <pthread.h>      // contains function declarations and mappings for threading interfaces and defines a number of constants used by those functions.
#endif
#include <string.h>         // contains helper functions for string operations
#include <stdio.h>          // contains declarations and functions to help with input output operations
#include <iostream>         // contains declarations and functions to help with input output operations
#include <fstream>          // predefines a set or operations for handling files related to input and output.

#include "file_transfer/Payload.h"

using namespace std;

// Helper Function To Start Socket - Initialize Socket Engine ..No code inside the Posix
bool StartSocket();

#ifdef WINDOWS_OS
void Cleanup() {WSACleanup();}

bool StartSocket()
{
  WORD Ver;
  WSADATA wsd;
  Ver = MAKEWORD(2, 2);
  if (WSAStartup(Ver, &wsd) == SOCKET_ERROR)
  {
    WSACleanup();
    return false;
  }

  return true;
}

int SocketGetLastError()
{
  return  WSAGetLastError();
}

int CloseSocket( SOCKET s ) {
  closesocket(s);
  return 0;
}
#else
#define SOCKET_ERROR (-1)
#define SOCKET int
void Cleanup() {}
bool StartSocket(){ return true; }
int SocketGetLastError(){ return  0xFFFF; }
int CloseSocket( int s ) { shutdown (s, 2); return 0; }
#endif

class CClientSocket
{
    char m_ServerName[255];
    int m_PortNumber;
    struct sockaddr_in m_Server;
    struct hostent *m_HostPointer;
    unsigned int m_addr;
    SOCKET m_ConnectSock;

public:
    /**
     * Constructor
     * 
     * @param ServerName The address of the remote server
     * @param PortNumber The port to which the socket should bind
     */
    CClientSocket(char *ServerName, int PortNumber)
    {
        strcpy(m_ServerName, ServerName);
        m_PortNumber = PortNumber;
    }

    SOCKET GetSocket() { return m_ConnectSock; }

    /**
     * Resolves the sockaddr_in object
     */
    bool Resolve()
    {
        if (isalpha(m_ServerName[0])) {
            m_HostPointer = gethostbyname(m_ServerName);
        } else {
            /* Convert nnn.nnn address to a usable one */
            m_addr = inet_addr(m_ServerName);
            m_HostPointer = gethostbyaddr((char *)&m_addr, 4, AF_INET);
        }
        
        if (m_HostPointer == NULL)
        {
            return false;
        }

        memset(&m_Server, 0, sizeof(m_Server));

        memcpy(&(m_Server.sin_addr), m_HostPointer->h_addr, m_HostPointer->h_length);
        m_Server.sin_family = m_HostPointer->h_addrtype;
        m_Server.sin_port = htons(m_PortNumber);
        return true;
    }

    /**
     * Creates a socket and connects to the remote server
     */
    bool Connect()
    {
        m_ConnectSock = socket(AF_INET, SOCK_STREAM, 0);
        if (m_ConnectSock < 0)
        {
            return false;
        }

        if (connect(m_ConnectSock,
                    (struct sockaddr *)&m_Server,
                    sizeof(m_Server)) == SOCKET_ERROR)
        {
            return false;
        }
        return true;
    }

    /**
     * Sends the data from the buffer to the socket
     */
    bool Send(void *buffer, int len)
    {
        int RetVal = send(m_ConnectSock, (const char *)buffer, len, 0);
        if (RetVal == SOCKET_ERROR)
        {
            return false;
        }

        return true;
    }

    /**
    * Receives the data from the socket and moves it to the buffer input
    */
    bool Receive(void *buffer, int *len)
    {
        int RetVal = recv(m_ConnectSock, (char *)buffer, *len, 0);

        if (RetVal == 0 || RetVal == -1)
        {
            printf("Error at socket(): %d\n", SocketGetLastError());
            return false;
        }

        *len = RetVal;
        return true;
    }

    /**
     * Closes the socket connection
     */
    int Close()
    {
        CloseSocket(m_ConnectSock);
        return 0;
    }
};

#endif
