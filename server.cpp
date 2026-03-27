// Jomikael Ruiz
// June 3rd, 2024
// server.cpp
// Purpose: developing a server application to be used in tandem with
// client.cpp. Receives an arithmetic operation from client and sends back
// the result of the operation

// include statements
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <cctype>
#include <sstream>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/types.h>

using namespace std;

int main(int argc, char *argv[]) {
  // variables
  struct sockaddr_in socketOne, newSocket;
  int server, childSocket;
  int portNum;
  int sendResult, listenResult, receiveResult;

  // Get the port number from the command-line arguments
  if (argc == 2) {
    portNum = atoi(argv[1]);
  } else {
    cout << "usage: server-tcp <portnum>" << endl;
    exit(-1);
  }

  // Populate the data structure
  // ip address and port number
  socketOne.sin_family = AF_INET;
  socketOne.sin_addr.s_addr = INADDR_ANY;
  socketOne.sin_port = htons(portNum);

  // creates socket and binds to a port
  server = socket(PF_INET, SOCK_STREAM, 0);
  bind(server, (struct sockaddr *) &socketOne, sizeof(socketOne));

  cout << "Server started" << endl;
  cout << "Listening on port " << portNum << endl;
  cout << "looking for clients" << endl;

  // listens for clients to create a queue of clients
  listenResult = listen(server, 5);
  if(listenResult == -1) {
      cout << "listen failed" << endl;
      close(server);
      exit(-1);
  }

   // makes sure server is always running
   while(1) { 
      // creates a socket for the client under childSocket
      socklen_t addressLength = sizeof(newSocket);
      childSocket = accept(server, (struct sockaddr*) &newSocket, &addressLength);
      
      // while loop for while the client is still connected
      while(childSocket != -1) {

         // buffer for messages
         const int BUFFER_SIZE = 1024;
         char buffer[BUFFER_SIZE] = {0}; // Create a buffer of size 1024 Bytes and default it to 0.

         // receives message from the client with error handling
         receiveResult = recv(childSocket, buffer, BUFFER_SIZE, 0);
         if(receiveResult <= 0) {
            close(childSocket);
            break;
         }

         // prints the data received from the buffer
         cout << "Socket " << childSocket << " sent message: " << buffer << endl;

         // handles the calculations from the message
         string convertedBuffer(buffer);
         string clientResponse = "balls";
         const char *message_arg = clientResponse.c_str();

         // returns the calculated response back to client
         cout << "sending reply: " << clientResponse << endl;
         sendResult = send(childSocket, message_arg, clientResponse.length(), 0);
         if(sendResult == -1) {
            cout << "send failed" << endl;
            break;
         }
      }
      close(childSocket);
   }
   cout << "exiting program" << endl;
   close(server);

   return 0;
}

