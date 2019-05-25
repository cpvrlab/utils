//#############################################################################
//  File:      FTPConnection.h
//  Purpose:   Connection for the FTP protocol using asio
//  Author:    Original from https://github.com/DanB91
//  Date:      April 2019
//  Copyright: Marcus Hudritsch, Switzerland
//             THIS SOFTWARE IS PROVIDED FOR EDUCATIONAL PURPOSE ONLY AND
//             WITHOUT ANY WARRANTIES WHETHER EXPRESSED OR IMPLIED.
//#############################################################################

#pragma once
#pragma clang diagnostic ignored "-Wparentheses"

#include <vector>
#include <string>
#include <asio.hpp>
#include <array>

using namespace std;
using asio::ip::tcp;

class FTPConnection
{

    public:
    void   connect(const string& hostname, const std::string& port);
    string readLine(); //reads a /r/n terminated line from the socket
    void   writeLine(const string& buffer);
    void   close() { this->socket.close(); }
    FTPConnection() : socket(ios) {}
    ~FTPConnection() { this->close(); }

    private:
    asio::io_service ios;
    tcp::socket      socket;
};
