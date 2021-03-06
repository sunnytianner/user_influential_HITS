//
//  UserInfluenceHITS.cpp
//  user_influential_HITS
//
//  Created by Eagle on 13-10-8.
//  Copyright (c) 2013年 Eagle. All rights reserved.
//

#include "UserInfluenceHITS.h"
#include "constants.h"
#include "util.h"
#include <string>
#include <map>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <algorithm>
#include <functional>

using namespace std;

UserInfluenceHITS::UserInfluenceHITS(){
    authOutputFileName = "/Users/tian19880220/workspace/user_influential_HITS/result/auth_result";
    hubOutputFileName = "/Users/tian19880220/workspace/user_influential_HITS/result/hub_result";
    authSortedOutputFileName = "/Users/tian19880220/workspace/user_influential_HITS/result/auth_result_sorted";
    hubSortedOutputFileName = "/Users/tian19880220/workspace/user_influential_HITS/result/hub_result_sorted";
    hits = new map<string,TwitterHITS>;
    authGraph = new map<string,map<string,double> >;
    hubGraph = new map<string,map<string,double> >;
}

UserInfluenceHITS::UserInfluenceHITS(string file){
    graphInputFileName = file;
    UserInfluenceHITS();
}

UserInfluenceHITS::~UserInfluenceHITS(){
    if (hits) {
        delete hits;
    }
    if (authGraph) {
        delete authGraph;
    }
    if (hubGraph) {
        delete hubGraph;
    }
}

void UserInfluenceHITS::initGraphBoolean(string file){
    cout << "init the boolean graph......" << endl;
    graphInputFileName = file;
    ifstream inputFile;
    inputFile.open(graphInputFileName.c_str());
    
    while (!inputFile.eof()) {
        string line;
        getline(inputFile,line);
        if (line.size() == 0) {
            continue;
        }
        
//        vector<string> parts = Util::split(line, TAB);
        vector<string> parts = Util::split(line, SPACE);
        if (parts.size() < 2) {
            cout<< "the parts is less then 2!"<< endl;
            continue;
        }
        vector<string> items = Util::split(parts[1], COMMA);
        
        for (int i = 0; i < items.size(); i++) {
            vector<string> points = Util::split(items[i], ";");
            if (points.size() != 2) {
                cout << "the points is less then 2!" << endl;
                continue;
            }
            string source = points[1];  //the person who is the source of the information
            string target = points[0];  //the person who refer to the source for the information
            
            //init hits with auth=1.0 hubs=1.0
            if (hits->find(source) == hits->end()) {
                TwitterHITS object = {source,1.0,1.0};
                hits->insert(make_pair(source, object));
            }
            if (hits->find(target) == hits->end()) {
                TwitterHITS object = {target,1.0,1.0};
                hits->insert(make_pair(target, object));
            }
            
            //init authGraph
            if (authGraph->find(source) == authGraph->end()) {
                map<string,double>* object = new map<string,double>;
                authGraph->insert(make_pair(source, *object));
            }
            
            if ((*authGraph)[source].find(target) == (*authGraph)[source].end()) {
                (*authGraph)[source].insert(make_pair(target, 1.0));
            }
            else{
                //here do nothing
            }
            
            //init hubGraph
            if (hubGraph->find(target) == hubGraph->end()) {
                map<string,double>* object = new map<string,double>;
                hubGraph->insert(make_pair(target, *object));
            }
            
            if ((*hubGraph)[target].find(source) == (*hubGraph)[target].end()) {
                (*hubGraph)[target].insert(make_pair(source, 1.0));
            }
            else{
                //here do nothing
            }
        }
    }
}

void UserInfluenceHITS::initGraphCount(string file){
    cout << "init the count graph......" << endl;
    graphInputFileName = file;
    ifstream inputFile;
    inputFile.open(graphInputFileName.c_str());
    
    while (!inputFile.eof()) {
        string line;
        getline(inputFile,line);
        if (line.size() == 0) {
            continue;
        }
        
        //        vector<string> parts = Util::split(line, TAB);
        vector<string> parts = Util::split(line, SPACE);
        if (parts.size() < 2) {
            cout<< "the parts is less then 2!"<< endl;
            continue;
        }
        vector<string> items = Util::split(parts[1], COMMA);
        
        for (int i = 0; i < items.size(); i++) {
            vector<string> points = Util::split(items[i], ";");
            if (points.size() != 2) {
                cout << "the points is less then 2!" << endl;
                continue;
            }
            string source = points[1];  //the person who is the source of the information
            string target = points[0];  //the person who refer to the source for the information
            
            //init hits with auth=1.0 hubs=1.0
            if (hits->find(source) == hits->end()) {
                TwitterHITS object = {source,1.0,1.0};
                hits->insert(make_pair(source, object));
            }
            if (hits->find(target) == hits->end()) {
                TwitterHITS object = {target,1.0,1.0};
                hits->insert(make_pair(target, object));
            }
            
            //init authGraph
            if (authGraph->find(source) == authGraph->end()) {
                map<string,double>* object = new map<string,double>;
                authGraph->insert(make_pair(source, *object));
            }
            
            if ((*authGraph)[source].find(target) == (*authGraph)[source].end()) {
                (*authGraph)[source].insert(make_pair(target, 1.0));
            }
            else{
                (*authGraph)[source][target] += 1;
            }
            
            //init hubGraph
            if (hubGraph->find(target) == hubGraph->end()) {
                map<string,double>* object = new map<string,double>;
                hubGraph->insert(make_pair(target, *object));
            }
            
            if ((*hubGraph)[target].find(source) == (*hubGraph)[target].end()) {
                (*hubGraph)[target].insert(make_pair(source, 1.0));
            }
            else{
                (*hubGraph)[target][source] += 1;
            }
        }
    }
}

void UserInfluenceHITS::initGraphLog(string file){
    initGraphCount(file);
    
    cout << "init the log graph......" << endl;
    //log the authGraph
    map<string,map<string,double> >::iterator sourceIter;
    for (sourceIter = authGraph->begin(); sourceIter != authGraph->end(); sourceIter++) {
        map<string,double> targets = sourceIter->second;
        map<string,double>::iterator targetIter;
        for (targetIter = targets.begin(); targetIter != targets.end(); targetIter++) {
//            targetIter->second = log((targetIter->second)+1);
            (*authGraph)[sourceIter->first][targetIter->first] = log(targetIter->second + 1);
        }
    }
    
    //log the hubGraph
    map<string,map<string,double> >::iterator targetIter;
    for (targetIter = hubGraph->begin(); targetIter != hubGraph->end(); targetIter++) {
        map<string,double> sources = targetIter->second;
        map<string,double>::iterator sourceIter;
        for (sourceIter = sources.begin(); sourceIter != sources.end(); sourceIter++) {
//            sourceIter->second = log((sourceIter->second)+1);
            (*hubGraph)[targetIter->first][sourceIter->first] = log(sourceIter->second + 1);
        }
    }
}

void UserInfluenceHITS::HITS(int step){
    cout << "Start HITS algorithm..." << endl;
    double norm;
    for (int i = 0; i<step; i++) {
        cout << "Step " << i+1 << "..." << endl;
        norm = 0;
        map<string,TwitterHITS>::iterator iter;
        
        //auth
        for (iter = hits->begin(); iter != hits->end(); iter++) {
            iter->second.auth=0;
            map<string,double> target = (*authGraph)[iter->first];
            for (map<string,double>::iterator it = target.begin(); it != target.end(); it++) {
                iter->second.auth += ((*hits)[it->first].hub) * (it->second);
            }
            norm += (iter->second.auth) * (iter->second.auth);
        }
        norm = sqrt(norm);
        for (iter = hits->begin(); iter != hits->end(); iter++) {
            iter->second.auth = iter->second.auth / norm;
        }
        
        //hub
        for (iter = hits->begin(); iter != hits->end(); iter++) {
            iter->second.hub=0;
            map<string,double> source = (*hubGraph)[iter->first];
            for (map<string,double>::iterator it = source.begin(); it != source.end(); it++) {
                iter->second.hub += ((*hits)[it->first].auth) * (it->second);
            }
            norm += (iter->second.hub) * (iter->second.hub);
        }
        norm = sqrt(norm);
        for (iter = hits->begin(); iter != hits->end(); iter++) {
            iter->second.hub = iter->second.hub / norm;
        }
    }
}

void UserInfluenceHITS::resultOutPut(){
    cout << "start writing the result files..." << endl;
    ofstream authOutputFile,hubOutputFile;
    authOutputFile.open(authOutputFileName.c_str());
    hubOutputFile.open(hubOutputFileName.c_str());
    
    map<string,TwitterHITS>::iterator iter;
    for (iter = hits->begin(); iter!=hits->end(); iter++) {
        authOutputFile << iter->first << "\t" << iter->second.auth << endl;
        hubOutputFile << iter->first << "\t" << iter->second.hub << endl;
    }
    authOutputFile.close();
    hubOutputFile.close();
    
}

void UserInfluenceHITS::resultSortedOutput(){
    cout << "start writing the sorted result files..." << endl;
    ofstream authSortedOutputFile,hubSortedOutputFile;
    authSortedOutputFile.open(authSortedOutputFileName.c_str());
    hubSortedOutputFile.open(hubSortedOutputFileName.c_str());
    
    vector<TwitterHITS> hitsVect;
    map<string,TwitterHITS>::iterator iter;
    for (iter = hits->begin(); iter!=hits->end(); iter++) {
        hitsVect.push_back(iter->second);
    }
    
    sort(hitsVect.begin(),hitsVect.end(),biggerAuth);
    for (int i=0; i<hitsVect.size(); i++) {
        authSortedOutputFile << hitsVect[i].name << "\t" << hitsVect[i].auth << endl;
    }
    authSortedOutputFile.close();
    
    sort(hitsVect.begin(),hitsVect.end(),biggerHub);
    for (int i=0; i<hitsVect.size(); i++) {
        hubSortedOutputFile << hitsVect[i].name << "\t" << hitsVect[i].hub << endl;
    }
    hubSortedOutputFile.close();
}