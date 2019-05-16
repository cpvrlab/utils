//#############################################################################
//  File:      Utils.h
//  Author:    Marcus Hudritsch
//  Date:      May 2019
//  Codestyle: https://github.com/cpvrlab/SLProject/wiki/Coding-Style-Guidelines
//  Copyright: Marcus Hudritsch
//             This software is provide under the GNU General Public License
//             Please visit: http://opensource.org/licenses/GPL-3.0
//#############################################################################

#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>

using namespace std;

//-----------------------------------------------------------------------------
//! Utils provides static utility functions for string and file handling
class Utils
{
    public:
    //! Returns a string from a float with max. one trailing zero
    static string toString(float f, int roundedDecimals = 1);

    //! Returns a string from a double with max. one trailing zero
    static string toString(double d, int roundedDecimals = 1);

    //! Returns a string in lower case
    static string toLower(string s);

    //! Returns a string in upper case
    static string toUpper(string s);

    //! Returns the path w. '\\' of path-filename string
    static string getPath(const string& pathFilename);

    //! Returns the filename of path-filename string
    static string getFileName(const string& pathFilename);

    //! Returns the filename without extension
    static string getFileNameWOExt(const string& pathFilename);

    //! Returns the file extension without dot in lower case
    static string getFileExt(string filename);

    //! Returns a vector of storted filesnames in dirName
    static vector<string> getFileNamesInDir(const string dirName);

    //! Trims a string at the end
    static string trim(string& s, const string& drop = " ");

    //! Splits an input string at a delimeter character into a string vector
    static void split(const string& s, char delimiter, vector<string>& splits);

    //! Replaces in the source string the from string by the to string
    static void replaceString(string& source, const string& from, const string& to);

    //! Returns local time as string
    static string getLocalTimeString();

    //! Returns a formatted string as sprintf
    static string formatString(const string fmt_str, ...);

    //! Returns true if container contains the search string
    static bool contains(const string container, const string search);

    //! Returns the inputDir string with unified forward slashes, e.g.: "dirA/dirB/"
    static string unifySlashes(const string& inputDir);

    //! Returns true if a directory exists.
    static bool dirExists(string& path);

    //! Creates a directory with given path
    static void makeDir(const string& path);

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

    //! logs a formatted string platform independently
    static void log(const char* appString,
                    const char* format,
                    ...);

    //! Terminates the application with a message. No leak cheching.
    static void exitMsg(const char* msg,
                        const char* appString,
                        const int   line,
                        const char* file);

    static void warnMsg(const char* msg,
                        const char* appString,
                        const int   line,
                        const char* file);

    //! Returns in release config the max. NO. of threads otherwise 1
    static unsigned int maxThreads();
};
//-----------------------------------------------------------------------------
#endif