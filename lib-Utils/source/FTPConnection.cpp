
#include <Utils.h>
#include <FTPConnection.h>

//-----------------------------------------------------------------------------
void FTPConnection::connect(const string& hostname, const string& port)
{
    try
    {
        //resolves host name
        tcp::resolver        resolver(this->ios);
        tcp::resolver::query query(hostname, port);

        //connects to host
        asio::connect(this->socket, resolver.resolve(query));
    }
    catch (exception& e)
    {
        Utils::log("Exception in FTPConnection::connect: %s\n", e.what());
        exit(1);
    }
}
//-----------------------------------------------------------------------------
//reads '\r\n' terminated line in
string FTPConnection::readLine()
{

    string          retStr;
    asio::streambuf responseBuffer;

    try
    {
        asio::read_until(this->socket, responseBuffer, "\r\n");
        istream responseStream(&responseBuffer);
        getline(responseStream, retStr);
    }
    catch (exception& e)
    {
        Utils::log("Exception in FTPConnection::readLine: %s\n", e.what());
        exit(1);
    }

    return retStr;
}
//-----------------------------------------------------------------------------
void FTPConnection::writeLine(const string& buffer)
{
    asio::streambuf b;
    ostream         os(&b);
    os << buffer;
    try
    {
        asio::write(this->socket, b);
    }
    catch (exception& e)
    {
        Utils::log("Exception in FTPConnection::writeLine: %s\n", e.what());
        exit(1);
    }
}
//-----------------------------------------------------------------------------
