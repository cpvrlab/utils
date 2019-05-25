//#############################################################################
//  File:      FTPCodes.h
//  Purpose:   Client for the FTP protocol using asio
//  Author:    Original from https://github.com/DanB91
//  Date:      April 2019
//  Copyright: Marcus Hudritsch, Switzerland
//             THIS SOFTWARE IS PROVIDED FOR EDUCATIONAL PURPOSE ONLY AND
//             WITHOUT ANY WARRANTIES WHETHER EXPRESSED OR IMPLIED.
//#############################################################################

#pragma once
#include <string>

namespace FTPCodes
{
const std::string RDY_FOR_LOGIN = "220"; //ready for login
const std::string USR_GOOD      = "331"; //user is good, input password
const std::string LOGGEDIN      = "230"; //user logged in
const std::string CLOSING_CMD   = "421"; //fatal error where the command socket must be closed
const std::string PWD           = "257"; //print working directory successful
const std::string CWD           = "250"; //change directory successful
}
