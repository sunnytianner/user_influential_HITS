//
//  UserInfluenceHITSImprove.h
//  user_influential_HITS
//
//  Created by Eagle on 13-10-14.
//  Copyright (c) 2013年 Eagle. All rights reserved.
//

#ifndef __user_influential_HITS__UserInfluenceHITSImprove__
#define __user_influential_HITS__UserInfluenceHITSImprove__

#include <string>
#include <map>

using namespace std;

struct TwitterHITS{
    string name;
    double auth;
    double hub;
};

class UserInfluenceHITSImprove{
private:
    map<string,TwitterHITS>* hits;   //the data structure for all HITS values of twitters
    map<string,map<string,double> >* authGraph;  //source twitter:<reference twitters:value>
    map<string,map<string,double> >* hubGraph;  //reference twitter:<source twitters:value>
    string graphInputFileName;  //the source file name of the graph
    string authOutputFileName;  //the file name put the auth values:userName\tauthValue
    string hubOutputFileName;   //the file name put the auth values:userName\thubValue
    string authSortedOutputFileName;  //the sorted file name put the auth values:userName\tauthValue
    string hubSortedOutputFileName;   //the sorted file name put the auth values:userName\thubValue
    double endPointCount;       //记录虚拟终止节点的个数
public:
    UserInfluenceHITSImprove(string file);
    UserInfluenceHITSImprove();
    ~UserInfluenceHITSImprove();
    
    //void initHITS();
    void initGraphBoolean(string file);     //the side value is 0 or 1
    void initGraphCount(string file);       //the side value is the count value
    void initGraphLog(string file);         //the side value is the log(count)
    void HITS(int step);                            //the HITS algorithm
    void resultOutPut();                    //putput the HITS result
    static bool biggerAuth(const TwitterHITS & t1, const TwitterHITS & t2){return t1.auth > t2.auth;}
    static bool biggerHub(const TwitterHITS & t1, const TwitterHITS & t2){return t1.hub > t2.hub;}
    void resultSortedOutput();
};

#endif 
