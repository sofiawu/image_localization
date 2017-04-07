//
//  tag.cpp
//  tag_for_ar
//
//  Created by sofiawu on 2017/3/31.
//  Copyright © 2017年 sofiawu. All rights reserved.
//

#include "tag.hpp"

#include <fstream>
#include <iostream>

#include <opencv2/xfeatures2d.hpp>

#include "utils.hpp"

namespace tagging {
    
    ///////////////
    
    Tag::Tag(const std::string& video_file,
             const std::string& dir_path,
             const std::string& target_file) : target_file_(target_file) {
        //ConvertVideoIntoImages(video_file, dir_path);
        file_manager_ = ImageFileManager(dir_path);
        detector_ = cv::xfeatures2d::SIFT::create(1000);
        detect(target_file, traget_desc_, target_kpts_);
        cv::namedWindow("AR Tagging");
        
        cv::setMouseCallback("AR Tagging", OnMouse, this);
    }
    
    void Tag::OnMouse(int event, int x, int y, int flags, void *param) {
        Tag* temp = reinterpret_cast<Tag*>(param);
        if(temp == NULL) return;
        
        if(event == CV_EVENT_LBUTTONDOWN) {
            if(temp->GetROIConers().size() < 4) {
                temp->AddCorner(cv::Point2f(x, y));
                temp->SaveROIFile();
                temp->DrawPoints();
            }
        }
    }
    
    void Tag::Process(){
        while(!file_manager_.eof()) {
            ShowAndDrawPoints();
            
            int key = cv::waitKey(0);
            if(key == 3) file_manager_.NextIndex();
            else if(key == 2) file_manager_.PrevIndex();
            else if(key == 27) {
                roi_corners_.clear();
                SaveROIFile();
            }
        }
    }
    
    void Tag::ShowAndDrawPoints() {
        frame_file_ = file_manager_.CurrentImage();
        roi_file_ = ReplaceSuffix(frame_file_, ".roi");
        
        curr_frame_ = cv::imread(frame_file_);
        if(curr_frame_.empty()) return;
        
        if(!LoadROIFile()) {
            AutoTag(frame_file_, roi_file_);
            LoadROIFile();
        }
        
        DrawPoints();
    }
    
    void Tag::DrawPoints() {
        int num = 1;
        for(auto pt : roi_corners_) {
            cv::circle(curr_frame_, pt, 5, cv::Scalar(0, 0, 255), 2);
            cv::putText(curr_frame_, std::to_string(num), cv::Point2f(pt.x + 8, pt.y + 8), cv::FONT_HERSHEY_PLAIN, 1.0, cv::Scalar(0, 255, 0));
            num++;
        }
        cv::putText(curr_frame_, "Image No." + std::to_string(file_manager_.CurrentIndex()), cv::Point2f(20, 20), cv::FONT_HERSHEY_DUPLEX, 1.0, cv::Scalar(255, 0, 0));
        
        cv::imshow("AR Tagging", curr_frame_);
    }
    
    bool Tag::AutoTag( const std::string& frame_file, const std::string &roi_file) {
        cv::Mat img = cv::imread(frame_file, 0);
        std::vector<cv::KeyPoint> kpts;
        cv::Mat desc;
        
        detector_->detectAndCompute(img, cv::noArray(), kpts, desc);

        cv::Ptr<cv::DescriptorMatcher> matcher = cv::DescriptorMatcher::create("BruteForce-SL2");
        std::vector<std::vector<cv::DMatch> > dmatches;
        
        matcher->knnMatch(desc, traget_desc_, dmatches, 2);
        
        std::vector< cv::DMatch > good_matches;
        float dist1 = 0.0, dist2 = 0.0;
        for(size_t i = 0; i < dmatches.size(); i++) {
            cv::DMatch dmatch = dmatches[i][0];
            dist1 = dmatches[i][0].distance;
            dist2 = dmatches[i][1].distance;
            
            if(dist1 < 0.7 * dist2) {
                good_matches.push_back(cv::DMatch(dmatch.queryIdx, dmatch.trainIdx, dmatch.distance));
            }
        }
        
        //-- Localize the object
        std::vector<cv::Point2f> obj;
        std::vector<cv::Point2f> scene;
        
        for( int i = 0; i < good_matches.size(); i++ ) {
            //-- Get the keypoints from the good matches
            scene.push_back( kpts[ good_matches[i].queryIdx ].pt );
            obj.push_back( target_kpts_[ good_matches[i].trainIdx ].pt );
        }
        
        cv::Mat H = cv::findHomography( obj, scene, CV_RANSAC );
        cv::perspectiveTransform( target_corners_, roi_corners_, H);
        
        /*cv::line(img, scene_corners[0], scene_corners[1], cv::Scalar(0, 255, 0), 4);
        cv::line(img, scene_corners[1], scene_corners[2], cv::Scalar(0, 255, 0), 4);
        cv::line(img, scene_corners[2], scene_corners[3], cv::Scalar(0, 255, 0), 4);
        cv::line(img, scene_corners[3], scene_corners[0], cv::Scalar(0, 255, 0), 4);
        
        //-- Show detected matches
        imshow( "Good Matches & Object detection", img );
        
        cv::waitKey(0);*/
        SaveROIFile();
        
        return true;
    }
    
    bool Tag::LoadROIFile() {
        std::fstream is(roi_file_, std::ios::in);
        if(!is.is_open()) {
            std::cerr << roi_file_ << "is not exist." << std::endl;
            return false;
        }
        
        roi_corners_.clear();
        cv::Point2f pt;
        while(is >> pt.x >> pt.y) {
            roi_corners_.push_back(pt);
        }
        is.close();
        
        //if(roi_corners_.size() != 4) return false;
        
        return true;
    }
    
    bool Tag::SaveROIFile() {
        std::fstream os(roi_file_, std::ios::out);
        for(auto pt : roi_corners_) {
            os << pt.x << " " << pt.y << " ";
        }
        os.close();
        
        return true;
    }
    
    
    ////
    void Tag::detect(const std::string &file, cv::Mat &desc, std::vector<cv::KeyPoint> &kpts) {
        cv::Mat img = cv::imread(file, 0);
        detector_->detectAndCompute(img, cv::noArray(), kpts, desc);
        target_corners_.resize(4);
        target_corners_[0] = cv::Point2f(0, 0);
        target_corners_[1] = cv::Point2f( img.cols, 0 );
        target_corners_[2] = cv::Point2f( img.cols, img.rows );
        target_corners_[3] = cv::Point2f( 0, img.rows );
    }
}
