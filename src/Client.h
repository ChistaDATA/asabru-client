//
// Created by JOSEPH ABRAHAM on 16/03/24.
//

#ifndef CHISTA_ASABRU_CLIENT_H
#define CHISTA_ASABRU_CLIENT_H

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

#include <readline/readline.h>
#include <readline/history.h>

#include "CommandDispatcher.h"

typedef struct
{
    int type; // variant
    union
    {
        char buffer[255];
        int  value;
        float fvalue;
        double dvalue;
        void *ptr;
        char *ptr_str;
    }DATA;
}T_PARAM;

typedef struct
{
    int p_count; // parameter count
    T_PARAM param_one[16]; //---
}T_PARAM_LIST;

typedef   T_PARAM_LIST*  (*HANDLE_COMMAND)(T_PARAM_LIST * );

typedef struct
{
    char command_name[100];
    HANDLE_COMMAND handler;
}T_ACTION;

//////////////////////////////////////////
// A Subroutine to split a string to a list of string
//
vector<string> split (string s, string delimiter);

T_PARAM_LIST  SetupParams(vector<string>& strvector );

void dump_params(T_PARAM_LIST *lst );

//
// T_ACTION *handle_command_xxx(T_PARAM_LIST * param_list );
//
T_PARAM_LIST * handle_command_show( T_PARAM_LIST *param_list )
{
    fprintf(stdout,"%s\n","SHOW .....");
    dump_params(param_list);
    return 0;
}

T_PARAM_LIST *handle_command_list( T_PARAM_LIST *param_list )
{
    fprintf(stdout,"%s\n","LIST ...");
    dump_params(param_list);
    return 0;
}

T_PARAM_LIST *handle_command_pwd( T_PARAM_LIST *param_list )
{
    fprintf(stdout,"%s\n","PWD ...");
    dump_params(param_list);
    return 0;
}

T_PARAM_LIST *handle_command_ls( T_PARAM_LIST *param_list )
{
    fprintf(stdout,"%s\n","LS ..");
    dump_params(param_list);
    return 0;
}

T_PARAM_LIST *handle_command_upload( T_PARAM_LIST *param_list )
{
    fprintf(stdout,"%s\n","Upload file ..");
    dump_params(param_list);
    return 0;
    //return UploadFile(param_list[0],param_list[1],param_list[2]);
}

T_ACTION actions[] =
{
    {"show",    handle_command_show   },
    {"list",    handle_command_list   },
    {"pwd",     handle_command_pwd    },
    {"ls",      handle_command_ls     },
    {"upload",  handle_command_upload }
};

bool command_dispatch(char *buff )
{
    string ns(buff);
    vector<string> vs = split(ns,string(" "));
    T_PARAM_LIST res = SetupParams(vs);
    if ( strcmp(vs[0].c_str(),"show") == 0 )
    {
        actions[0].handler(&res);
        return true;
    }
    else if ( strcmp(vs[0].c_str(),"list") == 0 )
    {
        actions[1].handler(&res);
        return true;
    }
    else if ( strcmp(vs[0].c_str(),"pwd") == 0 )
    {
        actions[2].handler(&res);
        return true;
    }
    else if ( strcmp(vs[0].c_str(),"ls") == 0 )
    {
        actions[3].handler(&res);
        return true;
    }
    else if ( strcmp(vs[0].c_str(),"upload") == 0 )
    {
        actions[4].handler(&res);
        return true;
    }
    else if ( strcmp(vs[0].c_str(),"exit") == 0 )
    {
        exit(0);
        return false;
    }
    printf("[%s]\n", buff);
    return true;
}

vector<string> split (string s, string delimiter)
{
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    string token;
    vector<string> res;
    //--- Leveraging std::string::find, we split the string
    while ((pos_end = s.find (delimiter, pos_start)) != string::npos)
    {
        token = s.substr (pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back (token);
    }
    //---- Retrieve the remaining string and push to return vector
    res.push_back (s.substr (pos_start));
    return res;
}

////////////////////////////////////////
//
//
T_PARAM_LIST  SetupParams(vector<string>& strvector )
{
    int ns = strvector.size();
    T_PARAM_LIST ls;
    ls.p_count = ns;
    int i=0;
    for( string str : strvector )
    {
        ls.param_one[i].type = 1; //string
        strcpy(ls.param_one[i].DATA.buffer,str.c_str());
        i++;
    }
    return ls;
}

void dump_params(T_PARAM_LIST *lst )
{
    int ns = lst->p_count;
    for(int i=1; i<ns; ++i )
    {
        cout << ".." << lst->param_one[i].DATA.buffer << "..";
    }
    cout << endl;
    return;
}

bool AuthenticateUser(std::string username,std::string password,std::string host ,int port );

#endif // CHISTA_ASABRU_CLIENT_H
