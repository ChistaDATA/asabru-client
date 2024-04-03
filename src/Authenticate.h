////
//// Created by JOSEPH ABRAHAM on 07/04/24.
////
//
#ifndef CHISTA_ASABRU_AUTHENTICATE_H
#define CHISTA_ASABRU_AUTHENTICATE_H

#include <nlohmann/json.hpp>

#include "CommandDispatcher.h"
#include "http_message.h"

using namespace std;

 bool AuthenticateUser(const std::string username, const std::string password,
                             const std::string host, const int port);
 bool RetrieveStringFromContext(ComputationContext &context,
                                      const std::string &key, std::string &value);

#endif// CHISTA_ASABRU_AUTHENTICATE_H