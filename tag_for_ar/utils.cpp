//
//  utils.cpp
//  tag_for_ar
//
//  Created by sofiawu on 2017/3/31.
//  Copyright © 2017年 sofiawu. All rights reserved.
//

#include "utils.hpp"
#include <opencv2/opencv.hpp>
#include <sstream>

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

#include "file_manager.hpp"

std::string GetImageFileName(const std::string& dir_path, int count) {
    std::ostringstream ss;
    ss << count;
    std::string res = "";
    for(int i = 0; i < 5 - ss.str().length(); ++i) res.append("0");
    res += ss.str();
    res += ".jpg";
    return res;
}

void Rotate90(const cv::Mat& src, cv::Mat &dst) {
    cv::transpose(src, dst);
    cv::flip(dst, dst, 1);
}

int ConvertVideoIntoImages(const std::string& video_name,
                            const std::string& image_path) {
    cv::VideoCapture cap(video_name);
    if(!cap.isOpened()) {
        std::cerr << "failed to open the video " << video_name << std::endl;
        exit(-1);
    }
    
    cv::Mat img_ori, img;
    int count = 0;
    while(true) {
        cap >> img_ori;
        if(img_ori.empty()) {
            std::cout << "Finished !" << std::endl;
            break;
        }
        
        if(img.cols < img.rows) Rotate90(img_ori, img);
        else img = img_ori;
        
        std::string save_file_name = GetImageFileName(image_path, count);
        cv::imwrite(save_file_name, img);
        std::cout << "save to " << save_file_name << std::endl;
        
        count ++;
    }
    
    return 0;
}

int ConvertImagesIntoVideo(const std::string& image_path,
                           const std::string& video_name) {
    tagging::ImageFileManager manager(image_path);
    
    
    int frame_rate = 25;
    cv::Mat img = cv::imread(manager.CurrentImage());
    cv::Size frame_size (img.cols, img.rows);
    
    cv::VideoWriter writer(video_name, CV_FOURCC('M','J','P','G'), frame_rate, frame_size, true);
    
    while(!manager.eof()) {
        cv::Mat img = cv::imread(manager.CurrentImage());
        manager.NextIndex();
        
        writer << img;
        cv::waitKey(frame_rate);
    }
    
    return 0;
}

void GetFilesInDir(const std::string& dir_path, std::vector<std::string>& files, const std::string& suffix) {
    files.clear();
    
    //open
    DIR* dir = opendir(dir_path.c_str());
    struct dirent *drnt = NULL;
    if(dir) {
        //read
        while((drnt = readdir(dir))) {
            std::string file_name = std::string(drnt->d_name);
            
            //match the suffix
            if (file_name.find(suffix) != std::string::npos) {
                std::string temp = dir_path + "/" + drnt->d_name;
                files.push_back(temp);
            }
        }
        closedir(dir);
    }
    else{
        printf("Can not open directory '%s'\n", dir_path.c_str());
    }
}

std::string ReplaceSuffix(const std::string& file_name, const std::string& suffix) {
    size_t pos = file_name.find_last_of(".");
    if(pos == std::string::npos) return file_name;
    
    return file_name.substr(0, pos) + suffix;
}




