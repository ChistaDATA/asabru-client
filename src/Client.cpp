//
// Created by JOSEPH ABRAHAM on 16/03/24.
//

#include <string.h>         // contains helper functions for string operations
#include <stdio.h>          // contains declarations and functions to help with input output operations
#include <iostream>         // contains declarations and functions to help with input output operations
#include <fstream>          // predefines a set or operations for handling files related to input and output.

#include "Client.h"

//int UploadFile(char * file_name, char * server_name, int port );

int main(int argc, char **argv ) {
//  if (argc != 4) {
//    printf("Usage: %s <file-name> <server-name> <port> \n\n", argv[0]);
//    return -1;
//  }

  // Get params from arguments
  char *file_name = argv[1];
  char *server_name = argv[2];
  int port = atoi(argv[3]);


    printf("Welcome! You can exit by pressing Ctrl+C at any time...\n");
    if (argc > 1 && std::string(argv[1]) == "-d") {
      // By default readline does filename completion. With -d, we disable this
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
    return 0;





// call fileupload command when required
  //return UploadFile(file_name,server_name,port);
}
