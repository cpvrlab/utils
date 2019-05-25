//#############################################################################
//  File:      tests.cpp
//  Purpose:   Test app for the Utils library
//  Author:    Marcus Hudritsch
//  Date:      April 2016 (FS16)
//  Copyright: Marcus Hudritsch, Switzerland
//             THIS SOFTWARE IS PROVIDED FOR EDUCATIONAL PURPOSE ONLY AND
//             WITHOUT ANY WARRANTIES WHETHER EXPRESSED OR IMPLIED.
//#############################################################################

#include <Utils.h>
#include <FTPClient.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

int main(int argc, char* argv[])
{
    string cwd = Utils::getCurrentWorkingDir();
    string awd = Utils::getAppsWritableDir();
    cout << "Local time          : " << Utils::getLocalTimeString() << endl;
    cout << "Current working dir : " << cwd << endl;
    cout << "Apps writable dir   : " << awd << endl;
    cout << "All files in cwd    : " << endl;
    for (auto& file : Utils::getFileNamesInDir(cwd))
        cout << "                      " << file << endl;
    vector<string> pathSplits;
    Utils::splitString(cwd, '/', pathSplits);
    cout << "String splitting    : ";
    for (auto& split : pathSplits)
        cout << split << "-";
    cout << endl;

    Utils::httpGet("http://cpvrlab.github.io/SLProject_doc/images/app-Demo-Skybox.png", awd);
    Utils::httpGet("http://pallas.bfh.ch/test/Christoffel.zip", awd);
    Utils::httpPost(awd + "app-Demo-Skybox.png",
                    "http://pallas.bfh.ch/libs/app-Demo-Skybox.png");
    //upload:FaAdbD3F2a@pallas.bfh.ch

    FTPClient client;
    client.connect("pallas.bfh.ch", "21");
    client.login("upload", "FaAdbD3F2a");
    return 0;
}
//-----------------------------------------------------------------------------
