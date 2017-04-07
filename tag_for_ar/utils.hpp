//
//  utils.hpp
//  tag_for_ar
//
//  Created by sofiawu on 2017/3/31.
//  Copyright © 2017年 sofiawu. All rights reserved.
//

#ifndef utils_hpp
#define utils_hpp

#include <stdio.h>
#include <string>
#include <vector>

int ConvertVideoIntoImages(const std::string& video_name,
                            const std::string& image_path);

int ConvertImagesIntoVideo(const std::string& image_path,
                           const std::string& video_name);

void GetFilesInDir(const std::string& dir_path,
                   std::vector<std::string>& files,
                   const std::string& suffix);

std::string ReplaceSuffix(const std::string& file_name, const std::string& suffix);

#endif /* utils_hpp */
