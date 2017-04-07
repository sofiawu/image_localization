//
//  file_manager.hpp
//  tag_for_ar
//
//  Created by sofiawu on 2017/3/31.
//  Copyright © 2017年 sofiawu. All rights reserved.
//

#ifndef file_manager_hpp
#define file_manager_hpp

#include <stdio.h>
#include <string>
#include <vector>

namespace tagging {
    /*
     Manager files in the provided dir
     */
    class ImageFileManager {
    public:
        ImageFileManager(){}
        ImageFileManager(const std::string& dir_path);
        
        inline std::string NextImage() {
            if(eof()) return "";
            return files_[curr_index_++];
        }
        inline void NextIndex() {
            curr_index_ ++;
            if(curr_index_ >= files_.size()) curr_index_ = (int)(files_.size() - 1);
        }
        inline void PrevIndex() {
            curr_index_ --;
            if(curr_index_ < 0) curr_index_ = 0;
        }
        inline std::string CurrentImage() const { return files_[curr_index_]; }
        inline int CurrentIndex() const { return curr_index_; }
        inline void SetIndex(int index) {
            if(index < 0) curr_index_ = 0;
            if(index >= files_.size()) curr_index_ = (int)(files_.size() - 1);
            curr_index_ = index;
        }
        inline void Reset() {
            curr_index_ = 0;
        }
        inline bool eof() const {
            return curr_index_ == (int)files_.size();
        }
        inline int TotalNum() const { return (int)files_.size();}
        inline std::vector<std::string> GetAllFiles() const {
            return files_;
        }
        
        std::string operator[](const int index) { return files_[index];}
        
        void ResetDirPath(const std::string& dir_path);
        
    private:
        std::string dir_path_;
        
        std::vector<std::string> files_;
        
        int curr_index_;
    };
    

}

#endif /* file_manager_hpp */
