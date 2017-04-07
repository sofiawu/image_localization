//
//  file_manager.cpp
//  tag_for_ar
//
//  Created by sofiawu on 2017/3/31.
//  Copyright © 2017年 sofiawu. All rights reserved.
//

#include "file_manager.hpp"

#include "utils.hpp"

namespace tagging {
    
    ImageFileManager::ImageFileManager(const std::string& dir_path) : dir_path_(dir_path) {
        GetFilesInDir(dir_path, files_, "jpg");
        curr_index_ = 0;
    }
    
    void ImageFileManager::ResetDirPath(const std::string& dir_path) {
        dir_path_ = dir_path;
        GetFilesInDir(dir_path, files_, "jpg");
        curr_index_ = 0;
    }
    
    
}
