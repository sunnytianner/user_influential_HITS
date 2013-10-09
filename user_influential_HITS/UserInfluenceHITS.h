//
//  UserInfluenceHITS.h
//  user_influential_HITS
//
//  Created by Eagle on 13-10-8.
//  Copyright (c) 2013年 Eagle. All rights reserved.
//

#ifndef user_influential_HITS_UserInfluenceHITS_h
#define user_influential_HITS_UserInfluenceHITS_h

#include <string>
#include <map>

using namespace std;

struct TwitterHITS{
    double auth;
    double hub;
};

class UserInfluenceHITS{
private:
    map<string,TwitterHITS>* hits;   //the data structure for all HITS values of twitters
    map<string,map<string,double> >* authGraph;  //source twitter:<reference twitters:value>
    map<string,map<string,double> >* hubGraph;  //reference twitter:<source twitters:value>
    string graphInputFileName;  //the source file name of the graph
    string authOutputFileName;  //the file name put the auth values:userName\tauthValue
    string hubOutputFileName;   //the file name put the auth values:userName\thubValue
public:
    UserInfluenceHITS(string file);
    UserInfluenceHITS();
    ~UserInfluenceHITS();
    
    //void initHITS();
    void initGraphBoolean(string file);     //the side value is 0 or 1
    void initGraphCount(string file);       //the side value is the count value
    void initGraphLog(string file);         //the side value is the log(count)
    void HITS(int step);                            //the HITS algorithm
    void resultOutPut();                    //putput the HITS result
};

#endif
