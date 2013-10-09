//
//  main.cpp
//  user_influential_HITS
//
//  Created by Eagle on 13-10-8.
//  Copyright (c) 2013年 Eagle. All rights reserved.
//

#include <iostream>
#include "UserInfluenceHITS.h"

int main(int argc, const char * argv[])
{
    UserInfluenceHITS *hits = new UserInfluenceHITS();
    hits->initGraphLog("/Users/tian19880220/workspace/user_influential_HITS/data/data_format.txt");
    hits->HITS(50);
    hits->resultOutPut();
}

