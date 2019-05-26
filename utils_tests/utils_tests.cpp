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
#include <ftplib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

int ftpCallbackXfer(off64_t xfered, void* arg)
{
    cout << xfered << endl;
}

int main(int argc, char* argv[])
{
    string cwd = Utils::getCurrentWorkingDir();
    string awd = Utils::getAppsWritableDir();
    cout << "Local time          : " << Utils::getLocalTimeString() << endl;
    cout << "Current working dir : " << cwd << endl;
    cout << "Apps writable dir   : " << awd << endl;

    //    cout << "All files in cwd    : " << endl;
    //    for (auto& file : Utils::getFileNamesInDir(cwd))
    //        cout << "                      " << file << endl;
    //    vector<string> pathSplits;
    //    Utils::splitString(cwd, '/', pathSplits);
    //    cout << "String splitting    : ";
    //    for (auto& split : pathSplits)
    //        cout << split << "-";
    //    cout << endl;

    //Utils::httpGet("http://cpvrlab.github.io/SLProject_doc/images/app-Demo-Skybox.png", awd);
    //Utils::httpGet("http://pallas.bfh.ch/test/Christoffel.zip", awd);

    ftplib ftp;
    ftp.Connect("pallas.bfh.ch:21");
    ftp.Login("upload", "FaAdbD3F2a");
    ftp.Chdir("test");
    ftp.Dir(nullptr, "");
    ftp.Get("Christoffel.zip", "Christoffel.zip", ftplib::transfermode::ascii);

    ftp.SetCallbackXferFunction(ftpCallbackXfer);

    string fileToUpload = "../data/calibrations/lbfmodel.yaml";
    uint   sizeToUpload = Utils::getFileSize(fileToUpload);
    cout << "Filesize to upload: " << sizeToUpload << endl;

    //    if (Utils::fileExists(fileToUpload))
    //    {
    //        ftp.Put(fileToUpload.c_str(),
    //                Utils::getFileName(fileToUpload).c_str(),
    //                ftplib::transfermode::ascii);
    //    }

    ftp.Quit();

    //lbfmodel.yaml

    return 0;
}
//-----------------------------------------------------------------------------
