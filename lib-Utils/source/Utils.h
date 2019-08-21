//#############################################################################
//  File:      Utils.h
//  Author:    Marcus Hudritsch
//  Date:      May 2019
//  Codestyle: https://github.com/cpvrlab/SLProject/wiki/Coding-Style-Guidelines
//  Copyright: Marcus Hudritsch
//             This software is provide under the GNU General Public License
//             Please visit: http://opensource.org/licenses/GPL-3.0
//#############################################################################

#ifndef CPLVRLAB_UTILS_H
#define CPLVRLAB_UTILS_H

#include <string>
#include <vector>

using namespace std;

//-----------------------------------------------------------------------------
//! Utils provides static utility functions for string and file handling
class Utils
{
    public:
    ///////////////////////////////
    // String Handling Functions //
    ///////////////////////////////

    //! Returns a string from a float with max. one trailing zero
    static string toString(float f, int roundedDecimals = 1);

    //! Returns a string from a double with max. one trailing zero
    static string toString(double d, int roundedDecimals = 1);

    //! Returns a string in lower case
    static string toLowerString(string s);

    //! Returns a string in upper case
    static string toUpperString(string s);

    //! Trims a string at the end
    static string trimString(const string& s, const string& drop = " ");

    //! Splits an input string at a delimeter character into a string vector
    static void splitString(const string& s, char delimiter, vector<string>& splits);

    //! Replaces in the source string the from string by the to string
    static void replaceString(string& source, const string& from, const string& to);

    //! replaces non-filename characters: /\|?%*:"<>'
    static string replaceNonFilenameChars(string source, char replaceChar = '-');

    //! Returns local time as string like "Wed Feb 13 15:46:11 2019"
    static string getLocalTimeString();

    //! Returns local time as string like "13.02.19-15:46"
    static string getDateTime1String();

    //! Returns local time as string like "20190213-154611"
    static string getDateTime2String();

    //! Returns the computer name
    static string getHostName();

    //! Returns a formatted string as sprintf
    static string formatString(const string& fmt_str, ...);

    //! Returns true if container contains the search string
    static bool containsString(const string& container, const string& search);

    //! Returns the inputDir string with unified forward slashes, e.g.: "dirA/dirB/"
    static string unifySlashes(const string& inputDir);

    //! Returns true if content of file could be put in a vector of strings
    static bool getFileContent(const string&   fileName,
                               vector<string>& vecOfStrings);

    //! Naturally compares two strings (used for filename sorting)
    static bool compareNatural(const string& a, const string& b);

    /////////////////////////////
    // File Handling Functions //
    /////////////////////////////

    //! Returns the path w. '\\' of path-filename string
    static string getPath(const string& pathFilename);

    //! Returns the filename of path-filename string
    static string getFileName(const string& pathFilename);

    //! Returns the filename without extension
    static string getFileNameWOExt(const string& pathFilename);

    //! Returns the file extension without dot in lower case
    static string getFileExt(const string& filename);

    //! Returns a vector of storted filesnames in dirName
    static vector<string> getFileNamesInDir(const string& dirName);

    //! Returns true if a directory exists.
    static bool dirExists(const string& path);

    //! Returns the file size in bytes
    static unsigned int getFileSize(const string& filename);

    //! Creates a directory with given path
    static bool makeDir(const string& path);

    //! RemoveDir deletes a directory with given path
    static void removeDir(const string& path);

    //! Returns true if a file exists.
    static bool fileExists(const string& pathfilename);

    //! Returns the writable configuration directory
    static string getAppsWritableDir();

    //! Returns the working directory
    static string getCurrentWorkingDir();

    //! Deletes a file on the filesystem
    static bool deleteFile(string& pathfilename);

    ///////////////////////
    // Logging Functions //
    ///////////////////////

    static string logAppName; //!< Name of logging application for android

    //! logs a formatted string platform independently
    static void log(const char* format,
                    ...);

    //! Terminates the application with a message. No leak cheching.
    [[noreturn]] static void exitMsg(const char* appString,
                                     int         line,
                                     const char* file);

    static void warnMsg(const char* appString,
                        int         line,
                        const char* file);

    //! Returns in release config the max. NO. of threads otherwise 1
    static unsigned int maxThreads();

    ////////////////////////////////
    // Network Handling Functions //
    ////////////////////////////////

    //! Download a file from an http url into the outFile
    static uint64_t httpGet(const string& httpURL, const string& outFolder = "");
};
//-----------------------------------------------------------------------------
#endif
