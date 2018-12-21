#include<fstream>
#include<iostream>
using namespace std;

const char FILE_NAME_1[]="Version/VersionNumbers.txt";
const char FILE_NAME_2[]="Source/Version/VersionNumber.h";

int main(){
    fstream fs(FILE_NAME_1,ios::in);
    if(!fs.is_open()){return 1;}
    int major,minor,patch,build;
    fs>>major>>minor>>patch>>build;
    fs.close();

    build++;

    fs.open(FILE_NAME_1,ios::out);
    if(!fs.is_open()){return 1;}
    fs<<major<<endl<<minor<<endl<<patch<<endl<<build<<endl;
    fs.close();

    fs.open(FILE_NAME_2,ios::out);
    if(!fs.is_open()){return 1;}
    fs<<"#ifndef VERSIONNUMBER_H_"<<endl
        <<"#define VERSIONNUMBER_H_"<<endl
        <<endl
        <<"#define VERSION_STRING \""<<major<<"."<<minor<<"."<<patch<<"."<<build<<"\""<<endl
        <<endl
        <<"enum VERSION_NUMBER{"<<endl
        <<"    VERSION_MAJOR="<<major<<","<<endl
        <<"    VERSION_MINOR="<<minor<<","<<endl
        <<"    VERSION_PATCH="<<patch<<","<<endl
        <<"    VERSION_BUILD="<<build<<endl
        <<"};"<<endl
        <<endl
        <<"#endif"<<endl
        <<endl;
    fs.close();

    cout<<"Version "<<major<<"."<<minor<<"."<<patch<<"."<<build<<endl;
}
