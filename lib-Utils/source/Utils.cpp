//#############################################################################
//  File:      Utils.cpp
//  Author:    Marcus Hudritsch
//  Date:      May 2019
//  Codestyle: https://github.com/cpvrlab/SLProject/wiki/Coding-Style-Guidelines
//  Copyright: Marcus Hudritsch
//             This software is provide under the GNU General Public License
//             Please visit: http://opensource.org/licenses/GPL-3.0
//#############################################################################

#include "Utils.h"

#include <sstream>
#include <cstddef>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <cstdarg>
#include <cstring>
#include <vector>
#include <algorithm>
#include <thread>
#include <asio.hpp>
#include <asio/ip/tcp.hpp>

#if defined(_WIN32)
#    if _MSC_VER >= 1912
#        include <filesystem>
#        define USE_STD_FILESYSTEM
namespace fs = std::experimental::filesystem;
#    else
#        include <direct.h> //_getcwd
#    endif
#elif defined(__APPLE__)
#    include <dirent.h>
#    include <unistd.h>   //getcwd
#    include <sys/stat.h> //dirent
#elif defined(ANDROID) || defined(ANDROID_NDK)
#    include <android/log.h>
#    include <dirent.h>
#    include <unistd.h> //getcwd
#    include <sys/stat.h>
#elif defined(linux) || defined(__linux) || defined(__linux__)
#    include <dirent.h>
#    include <unistd.h> //getcwd
#    include <sys/types.h>
#    include <sys/stat.h>
#endif

using namespace std;
using asio::ip::tcp;

///////////////////////////////
// String Handling Functions //
///////////////////////////////

//-----------------------------------------------------------------------------
//! Returns a string from a float with max. one trailing zero
string Utils::toString(float f, int roundedDecimals)
{
    stringstream ss;
    ss << fixed << setprecision(roundedDecimals) << f;
    string num = ss.str();
    if (num == "-0.0") num = "0.0";
    return num;
}
//-----------------------------------------------------------------------------
//! Returns a string from a double with max. one trailing zero
string Utils::toString(double d, int roundedDecimals)
{
    stringstream ss;
    ss << fixed << setprecision(roundedDecimals) << d;
    string num = ss.str();
    if (num == "-0.0") num = "0.0";
    return num;
}
//-----------------------------------------------------------------------------
//! Returns a string in lower case
string Utils::toLowerString(string s)
{
    string cpy(s);
    transform(cpy.begin(), cpy.end(), cpy.begin(), ::tolower);
    return cpy;
}
//-----------------------------------------------------------------------------
//! Returns a string in upper case
string Utils::toUpperString(string s)
{
    string cpy(s);
    transform(cpy.begin(), cpy.end(), cpy.begin(), ::toupper);
    return cpy;
}

//-----------------------------------------------------------------------------
//! Utils::trims a string at the end
string Utils::trimString(const string& s, const string& drop)
{
    string r = s;
    r        = r.erase(r.find_last_not_of(drop) + 1);
    return r.erase(0, r.find_first_not_of(drop));
}
//-----------------------------------------------------------------------------
//! Splits an input string at a delimeter character into a string vector
void Utils::splitString(const string&   s,
                        char            delimiter,
                        vector<string>& splits)
{
    string::size_type i = 0;
    string::size_type j = s.find(delimiter);

    while (j != string::npos)
    {
        splits.push_back(s.substr(i, j - i));
        i = ++j;
        j = s.find(delimiter, j);
        if (j == string::npos)
            splits.push_back(s.substr(i, s.length()));
    }
}
//-----------------------------------------------------------------------------
//! Replaces in the source string the from string by the to string
void Utils::replaceString(string&       source,
                          const string& from,
                          const string& to)
{
    // Code from: http://stackoverflow.com/questions/2896600/
    // how-to-replace-all-occurrences-of-a-character-in-string
    string newString;
    newString.reserve(source.length()); // avoids a few memory allocations

    string::size_type lastPos = 0;
    string::size_type findPos;

    while (string::npos != (findPos = source.find(from, lastPos)))
    {
        newString.append(source, lastPos, findPos - lastPos);
        newString += to;
        lastPos = findPos + from.length();
    }

    // Care for the rest after last occurrence
    newString += source.substr(lastPos);
    source.swap(newString);
}
//-----------------------------------------------------------------------------
//! Returns local time as string
string Utils::getLocalTimeString()
{
    time_t tm;
    time(&tm);
    struct tm* t2 = localtime(&tm);
    char       buf[1024];
    strftime(buf, sizeof(buf), "%c", t2);
    return string(buf);
}
//-----------------------------------------------------------------------------
//! Returns a formatted string as sprintf
string Utils::formatString(const string fmt_str, ...)
{
    // Reserve two times as much as the length of the fmt_str
    int                final_n, n = ((int)fmt_str.size()) * 2;
    string             str;
    unique_ptr<char[]> formatted;
    va_list            ap;
    while (1)
    {
        formatted.reset(new char[n]); /* Wrap the plain char array into the unique_ptr */
        strcpy(&formatted[0], fmt_str.c_str());
        va_start(ap, fmt_str);
        final_n = vsnprintf(&formatted[0], (unsigned long)n, fmt_str.c_str(), ap);
        va_end(ap);
        if (final_n < 0 || final_n >= n)
            n += abs(final_n - n + 1);
        else
            break;
    }
    return string(formatted.get());
}
//-----------------------------------------------------------------------------
//! Returns true if container contains the search string
bool Utils::containsString(const string container, const string search)
{
    return (container.find(search) != string::npos);
}
//-----------------------------------------------------------------------------
//! Returns inputDir with unified forward slashes
string Utils::unifySlashes(const string& inputDir)
{
    string copy = inputDir;
    string curr;
    string delimiter = "\\";
    size_t pos       = 0;
    string token;
    while ((pos = copy.find(delimiter)) != string::npos)
    {
        token = copy.substr(0, pos);
        copy.erase(0, pos + delimiter.length());
        curr.append(token);
        curr.append("/");
    }

    curr.append(copy);
    if (curr.size() && curr.back() != '/')
        curr.append("/");

    return curr;
}
//-----------------------------------------------------------------------------
//! Returns the path w. '\\' of path-filename string
string Utils::getPath(const string& pathFilename)
{
    size_t i1, i2;
    i1 = pathFilename.rfind('\\', pathFilename.length());
    i2 = pathFilename.rfind('/', pathFilename.length());
    if ((i1 != string::npos && i2 == string::npos) ||
        (i1 != string::npos && i1 > i2))
    {
        return (pathFilename.substr(0, i1 + 1));
    }

    if ((i2 != string::npos && i1 == string::npos) ||
        (i2 != string::npos && i2 > i1))
    {
        return (pathFilename.substr(0, i2 + 1));
    }
    return pathFilename;
}

/////////////////////////////
// File Handling Functions //
/////////////////////////////

//-----------------------------------------------------------------------------
//! Returns the filename of path-filename string
string Utils::getFileName(const string& pathFilename)
{
    size_t i = 0, i1, i2;
    i1       = pathFilename.rfind('\\', pathFilename.length());
    i2       = pathFilename.rfind('/', pathFilename.length());

    if (i1 != string::npos && i2 != string::npos)
        i = max(i1, i2);
    else if (i1 != string::npos)
        i = i1;
    else if (i2 != string::npos)
        i = i2;

    return pathFilename.substr(i + 1, pathFilename.length() - i);
}
//-----------------------------------------------------------------------------
//! Returns the filename without extension
string Utils::getFileNameWOExt(const string& pathFilename)
{
    string filename = getFileName(pathFilename);
    size_t i;
    i = filename.rfind('.', filename.length());
    if (i != string::npos)
    {
        return (filename.substr(0, i));
    }

    return (filename);
}
//-----------------------------------------------------------------------------
//! Returns the file extension without dot in lower case
string Utils::getFileExt(const string filename)
{
    size_t i;
    i = filename.rfind('.', filename.length());
    if (i != string::npos)
        return toLowerString(filename.substr(i + 1, filename.length() - i));
    return ("");
}
//-----------------------------------------------------------------------------
//! Returns a vector of storted filesnames with path in dir
vector<string> Utils::getFileNamesInDir(const string dirName)
{
    vector<string> filePathNames;

#if defined(USE_STD_FILESYSTEM)
    if (fs::exists(dirName) && fs::is_directory(dirName))
    {
        for (const auto& entry : fs::directory_iterator(dirName))
        {
            auto filename = entry.path().filename();
            if (fs::is_regular_file(entry.status()))
                filePathNames.push_back(dirName + "/" + filename.u8string());
        }
    }
#else
    DIR* dir;
    dir = opendir(dirName.c_str());

    if (dir)
    {
        struct dirent* dirContent;
        int            i = 0;

        while ((dirContent = readdir(dir)) != nullptr)
        {
            i++;
            string name(dirContent->d_name);
            if (name != "." && name != "..")
                filePathNames.push_back(dirName + name);
        }
        closedir(dir);
    }
#endif

    return filePathNames;
}
//-----------------------------------------------------------------------------
//! Returns true if a directory exists.
bool Utils::dirExists(const string& path)
{
#if defined(USE_STD_FILESYSTEM)
    return fs::exists(path) && fs::is_directory(path);
#else
    struct stat info;
    if (stat(path.c_str(), &info) != 0)
        return false;
    else if (info.st_mode & S_IFDIR)
        return true;
    else
        return false;
#endif
}
//-----------------------------------------------------------------------------
//! Creates a directory with given path
void Utils::makeDir(const string& path)
{
#if defined(USE_STD_FILESYSTEM)
    fs::create_directories(path);
#else
#    if defined(_WIN32)
    _mkdir(path.c_str());
#    else
    mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
#    endif
#endif
}
//-----------------------------------------------------------------------------
//! Removes a directory with given path
void Utils::removeDir(const string& path)
{

#if defined(USE_STD_FILESYSTEM)
    fs::remove_all(path);
#else
#    if defined(_WIN32)
    int ret = _rmdir(path.c_str());
    if (ret != 0)
    {
        errno_t err;
        _get_errno(&err);
        log("Could not remove directory: %s\nErrno: %s\n", path.c_str(), strerror(errno));
    }
#    else
    rmdir(path.c_str());
#    endif
#endif
}
//-----------------------------------------------------------------------------
//! Returns true if a file exists.
bool Utils::fileExists(const string& pathfilename)
{
#if defined(USE_STD_FILESYSTEM)
    return fs::exists(pathfilename);
#else
    struct stat info;
    return (stat(pathfilename.c_str(), &info) == 0);
#endif
}
//-----------------------------------------------------------------------------
//! Returns the file size in bytes
unsigned int Utils::getFileSize(const string& filename)
{
#if defined(USE_STD_FILESYSTEM)
    if (fs::exists(pathfilename))
        return fs::file_size(pathfilename);
    else
        return 0;
#else
    struct stat st;
    if (stat(filename.c_str(), &st) != 0)
        return 0;
    return (unsigned int)st.st_size;
#endif
}
//-----------------------------------------------------------------------------
//! Returns the writable configuration directory with trailing forward slash
string Utils::getAppsWritableDir()
{
#if defined(_WIN32)
    string appData   = getenv("APPDATA");
    string configDir = appData + "/SLProject";
    Utils::replaceString(configDir, "\\", "/");
    if (!dirExists(configDir))
        makeDir(configDir.c_str());
    return configDir + "/";
#elif defined(__APPLE__)
    string home      = getenv("HOME");
    string appData   = home + "/Library/Application Support";
    string configDir = appData + "/SLProject";
    if (!dirExists(configDir))
        mkdir(configDir.c_str(), S_IRWXU);
    return configDir + "/";
#elif defined(ANDROID) || defined(ANDROID_NDK)
    // @todo Where is the app data path on Andoroid?
#elif defined(linux) || defined(__linux) || defined(__linux__)
    // @todo Where is the app data path on Linux?
    string home      = getenv("HOME");
    string configDir = home + "/.SLProject";
    if (!dirExists(configDir))
        mkdir(configDir.c_str(), S_IRWXU | S_IRWXG | S_IRWXO);
    return configDir + "/";
#else
#    error "No port to this OS"
#endif
}
//-----------------------------------------------------------------------------
//! Returns the working directory with forward slashes inbetween and at the end
string Utils::getCurrentWorkingDir()
{
#if defined(_WIN32)
#    if defined(USE_STD_FILESYSTEM)
    return fs::current_path().u8string();
#    else
    int   size   = 256;
    char* buffer = (char*)malloc(size);
    if (_getcwd(buffer, size) == buffer)
    {
        string dir = buffer;
        replaceString(dir, "\\", "/");
        return dir + "/";
    }

    free(buffer);
    return "";
#    endif
#else
    size_t size   = 256;
    char*  buffer = (char*)malloc(size);
    if (getcwd(buffer, size) == buffer)
        return string(buffer) + "/";

    free(buffer);
    return "";
#endif
}
//-----------------------------------------------------------------------------
//! Deletes a file on the filesystem
bool Utils::deleteFile(string& pathfilename)
{
    if (Utils::fileExists(pathfilename))
        return remove(pathfilename.c_str()) != 0;
    return false;
}

///////////////////////
// Logging Functions //
///////////////////////

//-----------------------------------------------------------------------------
string Utils::logAppName = "";
//-----------------------------------------------------------------------------
//! logs a formatted string platform independently
void Utils::log(const char* format,
                ...)
{
    char    log[4096];
    va_list argptr;
    va_start(argptr, format);
    vsprintf(log, format, argptr);
    va_end(argptr);

#if defined(SL_OS_ANDROID)
    __android_log_print(ANDROID_LOG_INFO, logAppName.c_str(), log);
#else
    cout << log << std::flush;
#endif
}
//-----------------------------------------------------------------------------
//! Terminates the application with a message. No leak checking.
void Utils::exitMsg(const char* msg,
                    const int   line,
                    const char* file)
{
#if defined(ANDROID) || defined(ANDROID_NDK)
    __android_log_print(ANDROID_LOG_INFO,
                        logAppName.c_str(),
                        "Exit %s at line %d in %s\n",
                        msg,
                        line,
                        file);
#else
    log("%s: Exit %s at line %d in %s\n",
        logAppName.c_str(),
        msg,
        line,
        file);
#endif

    exit(-1);
}
//-----------------------------------------------------------------------------
//! Warn message output
void Utils::warnMsg(const char* msg,
                    const int   line,
                    const char* file)
{
#if defined(ANDROID) || defined(ANDROID_NDK)
    __android_log_print(ANDROID_LOG_INFO,
                        logAppName.c_str(),
                        "Warning: %s at line %d in %s\n",
                        msg,
                        line,
                        file);
#else
    log("%s: Warning %s at line %d in %s\n",
        logAppName.c_str(),
        msg,
        line,
        file);
#endif
}
//-----------------------------------------------------------------------------
//! Returns in release config the max. NO. of threads otherwise 1
unsigned int Utils::maxThreads()
{
#ifdef _DEBUG
    return 1;
#else
    return max(thread::hardware_concurrency(), 1U);
#endif
}

////////////////////////////////
// Network Handling Functions //
////////////////////////////////
//-----------------------------------------------------------------------------
/*! Downloads the file at httpURL with the same name in the outFolder. If the
outFolder is empty it is stored in the current working directory.
*/
void Utils::httpGet(const string& httpURL, const string& outFolder)
{
    try
    {
        // Remove "http://"
        string url = httpURL;
        Utils::replaceString(url, "http://", "");

        // Get server name and get command
        string serverName  = url.substr(0, url.find('/'));
        string getCommand  = url.substr(url.find('/'), url.length());
        string outFilename = url.substr(url.find_last_of('/') + 1, url.length());

        asio::io_service io_service;

        // Get a list of endpoints corresponding to the server name.
        tcp::resolver           resolver(io_service);
        tcp::resolver::query    query(serverName, "http");
        tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
        tcp::resolver::iterator end;

        // Try each endpoint until we successfully establish a connection.
        tcp::socket      socket(io_service);
        asio::error_code error = asio::error::host_not_found;
        while (error && endpoint_iterator != end)
        {
            socket.close();
            socket.connect(*endpoint_iterator++, error);
        }

        asio::streambuf request;
        ostream         request_stream(&request);

        request_stream << "GET " << getCommand << " HTTP/1.0\r\n";
        request_stream << "Host: " << serverName << "\r\n";
        request_stream << "Accept: */*\r\n";
        request_stream << "Connection: close\r\n\r\n";

        // Send the request.
        asio::write(socket, request);

        // Read the response status line.
        asio::streambuf response;
        asio::read_until(socket, response, "\r\n");

        // Check that response is OK.
        istream response_stream(&response);
        string  httpVersion;
        response_stream >> httpVersion;
        unsigned int statusCode;
        response_stream >> statusCode;
        string statusMsg;
        getline(response_stream, statusMsg);
        statusMsg = trimString(statusMsg);
        replaceString(statusMsg, "\r", "");
        replaceString(statusMsg, "\n", "");

        // Check HTTP response status (400 means bad request)
        if (statusCode != 200)
        {
            log("httpGet: HTTP Response returned status code: %d (%s)\n",
                statusCode,
                statusMsg.c_str());
            return;
        }

        //cout << endl << "Http-Status: " << statusCode << endl;

        // Read the response headers, which are terminated by a blank line.
        asio::read_until(socket, response, "\r\n\r\n");

        // Process the response headers.
        string headerLine;
        while (std::getline(response_stream, headerLine) && headerLine != "\r")
        {
            size_t splitPos = headerLine.find_first_of(':');
            string info     = headerLine.substr(0, splitPos);
            string value    = headerLine.substr(splitPos + 2);
            //cout << info << ":" << value << endl;
        }

        // Build full outFolderFilename
        string outFolderFilename;
        string cwd = getCurrentWorkingDir();
        if (outFolder.empty())
        {
            outFolderFilename = cwd + outFilename;
        }
        else
        {
            if (dirExists(outFolder))
                outFolderFilename = outFolder + outFilename;
            else
            {
                string msg = "Outfolder not found: " + outFolder;
                exitMsg(msg.c_str(), __LINE__, __FILE__);
            }
        }

        ofstream outFile(outFolderFilename, ofstream::out | ofstream::binary);

        if (outFile.is_open())
        {
            // Some statistics
            size_t bytesRead  = 0;
            size_t totalBytes = 0;
            size_t numChunks  = 0;

            // Write whatever content we already have to output.
            if (response.size() > 0)
            {
                numChunks++;
                totalBytes = response.size();
                outFile << &response;
            }

            // Read until EOF, writing data to output as we go.
            do
            {
                bytesRead = asio::read(socket,
                                       response,
                                       asio::transfer_at_least(1),
                                       error);
                if (bytesRead)
                {
                    numChunks++;
                    totalBytes += bytesRead;
                    outFile << &response;
                }
            } while (bytesRead);

            cout << "TotalBytes read: " << totalBytes << " in " << numChunks << " chunks." << endl;
            outFile.close();
        }
        else
        {
            log("File cannot be opened for writing in Utils::httpGet: %s\n",
                outFolderFilename.c_str());
            exit(1);
        }
    }
    catch (exception& e)
    {
        log("Exception in Utils::httpGet: %s\n", e.what());
        exit(1);
    }
}
//-----------------------------------------------------------------------------
