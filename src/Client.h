//
// Created by JOSEPH ABRAHAM on 16/03/24.
//

#ifndef CHISTA_ASABRU_CLIENT_H
#define CHISTA_ASABRU_CLIENT_H

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

#include <readline/history.h>
#include <readline/readline.h>

#include "FileUpload.h"
#include "UploadZipFile.h"

enum T_PARAM_TYPE {
  ILLEGAL_PARAM = -1,
  T_CHAR,
  T_INT,
  T_FLOAT,
  T_DOUBLE,
  T_BUFFER,
  T_VOID_PTR,
  T_CHAR_PTR
};

typedef struct {
  int type; // variant
  union {
    char buffer[255];
    int value;
    float fvalue;
    double dvalue;
    void *ptr;
    char *ptr_str;
  } DATA;
} T_PARAM;

typedef struct {
  int p_count;        // parameter count
  T_PARAM params[16]; //---
} T_PARAM_LIST;

typedef T_PARAM_LIST *(*HANDLE_COMMAND)(T_PARAM_LIST *);

typedef struct {
  char command_name[100];
  HANDLE_COMMAND handler;
} T_ACTION;

T_PARAM_LIST SetupParams(vector<string> &strvector);

void dump_params(T_PARAM_LIST *lst);

//
// T_ACTION *handle_command_xxx(T_PARAM_LIST * param_list );
//
T_PARAM_LIST *handle_command_show(T_PARAM_LIST *param_list) {
  fprintf(stdout, "%s\n", "SHOW .....");
  dump_params(param_list);
  return 0;
}

T_PARAM_LIST *handle_command_list(T_PARAM_LIST *param_list) {
  fprintf(stdout, "%s\n", "LIST ...");
  dump_params(param_list);
  return 0;
}

T_PARAM_LIST *handle_command_pwd(T_PARAM_LIST *param_list) {
  fprintf(stdout, "%s\n", "PWD ...");
  dump_params(param_list);
  return 0;
}

T_PARAM_LIST *handle_command_ls(T_PARAM_LIST *param_list) {
  fprintf(stdout, "%s\n", "LS ..");
  dump_params(param_list);
  return 0;
}

T_PARAM_LIST *handle_command_upload(T_PARAM_LIST *param_list) {
  fprintf(stdout, "%s\n", "Upload file ..");
  dump_params(param_list);
  const char *file_name = param_list->params[1].DATA.buffer;
  const char *server_name = param_list->params[2].DATA.buffer;
  int port = atoi(param_list->params[3].DATA.buffer);
  return reinterpret_cast<T_PARAM_LIST *>(
      UploadFile(file_name, server_name, port));
}

T_PARAM_LIST *handle_command_upload_metrics_plugin(T_PARAM_LIST *param_list) {
	fprintf(stdout, "%s\n", "Upload metrics plugin file ..");
	dump_params(param_list);
	//const char *file_name = param_list->params[1].DATA.buffer;
	//const char *server_name = param_list->params[2].DATA.buffer;
	//int port = atoi(param_list->params[3].DATA.buffer);
	return reinterpret_cast<T_PARAM_LIST *>(
		UploadZipFile());
}

T_PARAM_LIST *handle_command_exit(T_PARAM_LIST *param_list) {
  fprintf(stdout, "%s\n", "EXIT ..");
  dump_params(param_list);
  exit(0);
  return 0;
}

// Map of commands.New commands can be added at the end
std::map<const std::string, HANDLE_COMMAND> actions = {
    {"show", handle_command_show},     {"list", handle_command_list},
    {"pwd", handle_command_pwd},       {"ls", handle_command_ls},
    {"upload", handle_command_upload}, {"upload-metrics-plugin", handle_command_upload_metrics_plugin},
	{"exit", handle_command_exit}};

bool command_dispatch(char *buff) {
  string ns(buff);
  vector<string> commands = Utils::split(ns, " ");

  // Check if the command is empty
  if (commands.empty()) {
    // Handle error or return false
    return false;
  }

  // Check if the command exists in the map
  auto it = actions.find(commands[0]);

  if (it == actions.end()) {
    // Handle unknown command or return false
    return false;
  }

  // Set up parameters
  T_PARAM_LIST res = SetupParams(commands);

  // Invoke the function if the command exists
  it->second(&res);

  printf("[%s]\n", buff);
  // Optionally, return true to indicate success
  return true;
}

////////////////////////////////////////
//
//
T_PARAM_LIST SetupParams(vector<string> &strvector) {
  int ns = strvector.size();
  T_PARAM_LIST ls;
  ls.p_count = ns;
  int i = 0;
  for (string str : strvector) {
    ls.params[i].type = 1; // string
    strcpy(ls.params[i].DATA.buffer, str.c_str());
    i++;
  }
  return ls;
}

void dump_params(T_PARAM_LIST *lst) {
  int ns = lst->p_count;
  for (int i = 1; i < ns; ++i) {
    cout << ".." << lst->params[i].DATA.buffer << "..";
  }
  cout << endl;
  return;
}

#endif // CHISTA_ASABRU_CLIENT_H
