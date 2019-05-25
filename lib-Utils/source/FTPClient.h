//#############################################################################
//  File:      FTPClient.h
//  Purpose:   Client for the FTP protocol using asio
//  Author:    Original from https://github.com/DanB91
//  Date:      April 2019
//  Copyright: Marcus Hudritsch, Switzerland
//             THIS SOFTWARE IS PROVIDED FOR EDUCATIONAL PURPOSE ONLY AND
//             WITHOUT ANY WARRANTIES WHETHER EXPRESSED OR IMPLIED.
//#############################################################################

#pragma once

#include <string>

#include <FTPConnection.h>

using namespace std;

//-----------------------------------------------------------------------------
class FTPClient
{

    public:
    const string& getResponse() const { return this->lastResponseFromConnection; }
    void          connect(const string& hostname, const string& port);
    void          login(const string& username = "anonymous", const string& password = "");
    void          printWorkingDir();
    void          changeDir(const string& path = "");
    string        listDir(const string& path = "");

    private:
    string        lastResponseFromConnection;
    FTPConnection connection;
    FTPConnection dataConnection;

    void sendAndReceiveCmd(string commandAndArgument);
};
//-----------------------------------------------------------------------------
