//
// Created by JOSEPH ABRAHAM on 16/03/24.
//

#include <cstring>         // contains helper functions for string operations
#include <cstdio>          // contains declarations and functions to help with input output operations
#include <iostream>         // contains declarations and functions to help with input output operations
#include <fstream>          // predefines a set or operations for handling files related to input and output.

#include "Client.h"

//int UploadFile(char * file_name, char * server_name, int port );

int main(int argc, char **argv ) {

    const std::string username = argv[1];
    const std::string password = argv[2];
    const std::string  host = argv[3];
    int port = std::atoi(argv[4]);

    if (argc != 5) {
        printf("Usage: %s <file-name> <server-name> <port> \n\n", argv[0]);
        return -1;
    }

    if(AuthenticateUser(username,password,host,port))
    {
        printf("Welcome! You can exit by pressing Ctrl+C at any time...\n");
        if (argc > 1 && std::string(argv[1]) == "-d") {
          // By default, readline does filename completion. With -d, we disable this
          // by asking readline to just insert the TAB character itself.
          rl_bind_key('\t', rl_insert);
        }
        char* buf;
        while ((buf = readline(">> ")) != nullptr) {
          if (strlen(buf) > 0) {
            add_history(buf);
          }
          command_dispatch(buf);
          // readline malloc's a new buffer every time.
          free(buf);
        }
   }
   else
   {
       std::cout<<"User is not Authenticated. Please check credentials... " << std::endl;
       return -1;
   }
    return 0;

// call fileupload command when required
  //return UploadFile(file_name,server_name,port);
}

bool AuthenticateUser(std::string username,std::string password,std::string host ,int port )
{
    // Resolve the Handler class
    CommandDispatcher::RegisterCommand<BaseComputationCommand>("HTTPBasicLoginRequestCommand");
    ComputationContext context ;
    std::string request = "TestRequest";
    context.Put("username",username);
    context.Put("password",password);
    context.Put("host",host);
    context.Put("port",port);
    context.Put("request", request);

    CommandDispatcher::Dispatch("HTTPBasicLoginRequestCommand", &context);
    std::string response = std::any_cast<std::string>(context.Get("response"));
    std::string token =std::any_cast<std::string>(context.Get("token"));
    // std::cout << response << std::endl;
    //std::cout << "token"<< token << std::endl;

    return true;
}
