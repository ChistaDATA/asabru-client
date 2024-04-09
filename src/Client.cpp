//
// Created by JOSEPH ABRAHAM on 16/03/24.
//

#include <cstdio> // contains declarations and functions to help with input output operations
#include <cstring> // contains helper functions for string operations
#include <fstream> // predefines a set or operations for handling files related to input and output.
#include <iostream> // contains declarations and functions to help with input output operations

#include "Authenticate.h"
#include "Client.h"

int main(int argc, char **argv) {
    if (argc != 5) {
        printf("Usage: %s <user-name> <password> <host> <port> \n\n", argv[0]);
        exit(0) ;
    }

  const std::string username = argv[1];
  const std::string password = argv[2];
  const std::string host = argv[3];
  const int port = std::atoi(argv[4]);

  if (AuthenticateUser(username, password, host, port)) {
    printf("Welcome! You can exit by pressing Ctrl+C at any time...\n");
    // By default, readline does filename completion. We disable this
    // by asking readline to just insert the TAB character itself.
    rl_bind_key('\t', rl_insert);
    char *buf;
    while ((buf = readline(">> ")) != nullptr) {
      if (strlen(buf) > 0) {
        add_history(buf);
      }
      command_dispatch(buf);
      // readline malloc's a new buffer every time.
      free(buf);
    }
  } else {
    std::cout << "User is not Authenticated. Please check credentials... "
              << std::endl;
    return -1;
  }
  return 0;
}