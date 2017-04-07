//
//  tag.hpp
//  tag_for_ar
//
//  Created by sofiawu on 2017/3/31.
//  Copyright © 2017年 sofiawu. All rights reserved.
//

#ifndef tag_hpp
#define tag_hpp

#include <stdio.h>
#include <string>
#include <vector>
#include <opencv2/opencv.hpp>

#include "file_manager.hpp"

namespace tagging {
    
    class Tag {
    public:
        Tag(const std::string& video_file,
            const std::string& dir_path,
            const std::string& ref_file);
        
        bool AutoTag( const std::string& frame_file,
                     const std::string& roi_file);
        
        /* load and save the roi file*/
        bool LoadROIFile();
        bool SaveROIFile();
        
        void Process();
        void ShowAndDrawPoints();
        void DrawPoints();
        
        inline ImageFileManager* GetFileManager() { return &file_manager_; }
        inline std::vector<cv::Point2f> GetROIConers() { return roi_corners_; }
        inline void AddCorner(const cv::Point2f& pt) { roi_corners_.push_back(pt); }
        
        
        static void OnMouse(int event, int x, int y, int flags, void* param);
        
    private:
        void detect(const std::string& file,
                    cv::Mat& desc,
                    std::vector<cv::KeyPoint>& kpts);
    private:
        ImageFileManager file_manager_;
        
        cv::Ptr<cv::Feature2D> detector_;
        
        //target image
        cv::Mat traget_desc_;
        std::vector<cv::KeyPoint> target_kpts_;
        std::vector<cv::Point2f> target_corners_;
        std::string target_file_;
        
        // load or calc result for current frame
        std::vector<cv::Point2f> roi_corners_;
        std::string roi_file_;
        std::string frame_file_;
        cv::Mat curr_frame_;
    };
    
}

#endif /* tag_hpp */
