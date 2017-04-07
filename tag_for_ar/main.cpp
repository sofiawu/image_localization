//
//  main.cpp
//  tag_for_ar
//
//  Created by sofiawu on 2017/3/31.
//  Copyright © 2017年 sofiawu. All rights reserved.
//

#include <iostream>
#include "tag.hpp"

#include "utils.hpp"

using namespace tagging;

int main(int argc, const char * argv[]) {
    Tag tag("/Users/sofiawu/Work/Projects/temp/0361.mov", "/Users/sofiawu/Work/Projects/temp/images", "/Users/sofiawu/Work/Projects/temp/train.jpg");
    tag.Process();
    
    //ConvertImagesIntoVideo("/Users/sofiawu/Work/Projects/temp/images", "/Users/sofiawu/Work/Projects/temp/0361.mov");
    return 0;
}
