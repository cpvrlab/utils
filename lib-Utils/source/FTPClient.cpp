
#include <Utils.h>
#include <FTPClient.h>
#include <FTPCodes.h>

//-----------------------------------------------------------------------------
void sendCmdToConnection(FTPConnection& connection, string cmdAndArgs)
{
    if (cmdAndArgs.find("\r\n") == string::npos)
        cmdAndArgs += "\r\n";

    connection.writeLine(cmdAndArgs);
}
//-----------------------------------------------------------------------------
string receiveFromConnection(FTPConnection& controlConnection)
{
    string serverResponse = controlConnection.readLine();

    //special case if server needs to disconnect
    if (serverResponse.find(FTPCodes::CLOSING_CMD) == 0)
    {
        Utils::log("FTP client got disconnected");
    }
    return serverResponse;
}
//-----------------------------------------------------------------------------
/*!
Sends an FTP command to the control connection and receives a response from
the server. If that response doesn't reflect the expectedFTPCode, it throws
an NonFatalUnexpectedFTPCodeException. If the control socket needs to
disconnect, it will throw an FTPConnection Saves the connection response to
the lastResponse
*/
void FTPClient::sendAndReceiveCmd(string cmdAndArgs)
{
    sendCmdToConnection(connection, cmdAndArgs);
    lastResponseFromConnection = receiveFromConnection(connection);
}
//-----------------------------------------------------------------------------
//connects the control connection
void FTPClient::connect(const string& hostname, const string& port)
{
    connection.connect(hostname, port);
    lastResponseFromConnection = receiveFromConnection(connection);
}
//-----------------------------------------------------------------------------
void FTPClient::login(const string& username, const string& password)
{
    sendAndReceiveCmd("USER " + username);
    sendAndReceiveCmd("PASS " + password);
}
//-----------------------------------------------------------------------------
void FTPClient::printWorkingDir()
{
    sendAndReceiveCmd("PWD");
}
//-----------------------------------------------------------------------------
void FTPClient::changeDir(const string& path)
{
    string command = "CWD";

    //if there is a supplied argument, put it in
    if (path.size() > 0)
        command += " " + path;

    sendAndReceiveCmd(command);
}
//-----------------------------------------------------------------------------
string FTPClient::listDir(const string& path)
{
    return "";
}
//-----------------------------------------------------------------------------
