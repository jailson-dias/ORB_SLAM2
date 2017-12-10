/**
* This file is part of ORB-SLAM2.
*
* Copyright (C) 2014-2016 Raúl Mur-Artal <raulmur at unizar dot es> (University of Zaragoza)
* For more information see <https://github.com/raulmur/ORB_SLAM2>
*
* ORB-SLAM2 is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* ORB-SLAM2 is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with ORB-SLAM2. If not, see <http://www.gnu.org/licenses/>.
*/

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include "FrameDrawer.h"
#include "Tracking.h"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include<mutex>

#include <cmath>

#include <thread>

namespace ORB_SLAM2
{
void FrameDrawer::setText() 
{
    cout << "text an" << endl;
    while(true) {
        int acc;
        cin >> acc;
        if (acc == 1) {
            unique_lock<mutex> lock(mText);
            speak = true;
            cout << "text in" << endl;
        } else if(acc== 2) {
            break;
        }
    }
}

FrameDrawer::FrameDrawer(Map* pMap):mpMap(pMap)
{
    mState=Tracking::SYSTEM_NOT_READY;
    mIm = cv::Mat(480,640,CV_8UC3, cv::Scalar(0,0,0));
    
    std::thread (&FrameDrawer::setText, this).detach(); 

}

cv::Mat FrameDrawer::DrawFrame()
{
    cv::Mat im;
    vector<cv::KeyPoint> vIniKeys; // Initialization: KeyPoints in reference frame
    vector<int> vMatches; // Initialization: correspondeces with reference keypoints
    vector<cv::KeyPoint> vCurrentKeys; // KeyPoints in current frame
    vector<bool> vbVO, vbMap; // Tracked MapPoints in current frame
    int state; // Tracking state


    // cout << "Image: " << name << ", ";
    // cout << position.at<int>(0) << " ";
    // cout << position.at<int>(1) << " ";
    // cout << position.at<int>(2) << " ";
    // cout << position.at<int>(3) << endl << endl;

    //Copy variables within scoped mutex
    {
        unique_lock<mutex> lock(mMutex);
        state=mState;
        if(mState==Tracking::SYSTEM_NOT_READY)
            mState=Tracking::NO_IMAGES_YET;

        mIm.copyTo(im);

        if(mState==Tracking::NOT_INITIALIZED)
        {
            vCurrentKeys = mvCurrentKeys;
            vIniKeys = mvIniKeys;
            vMatches = mvIniMatches;
        }
        else if(mState==Tracking::OK)
        {
            vCurrentKeys = mvCurrentKeys;
            vbVO = mvbVO;
            vbMap = mvbMap;
        }
        else if(mState==Tracking::LOST)
        {
            vCurrentKeys = mvCurrentKeys;
        }
    } // destroy scoped mutex -> release mutex

    if(im.channels()<3) //this should be always true
        cvtColor(im,im,CV_GRAY2BGR);

    //Draw
    if(state==Tracking::NOT_INITIALIZED) //INITIALIZING
    {
        for(unsigned int i=0; i<vMatches.size(); i++)
        {
            if(vMatches[i]>=0)
            {
                cv::line(im,vIniKeys[i].pt,vCurrentKeys[vMatches[i]].pt,
                        cv::Scalar(0,255,0));
            }
        }        
    }
    else if(state==Tracking::OK) //TRACKING
    {
        mnTracked=0;
        mnTrackedVO=0;
        const float r = 5;
        const int n = vCurrentKeys.size();
        // int np = (mpMap->GetAllMapPoints()).size();
        // cout << "n: " << n << " - np: " << np << endl; 
        for(int i=0;i<n;i++)
        {
            if(vbVO[i] || vbMap[i])
            {
                cv::Point2f pt1,pt2, pt3, pt4;
                pt1.x=vCurrentKeys[i].pt.x-r;
                pt1.y=vCurrentKeys[i].pt.y-r;
                pt2.x=vCurrentKeys[i].pt.x+r;
                pt2.y=vCurrentKeys[i].pt.y+r;

                pt3.x=300;
                pt3.y=300;
                pt4.x=310;
                pt4.y=310;
                // cout << "(" << pt1.x << ", " << pt1.y << ")  p1" << endl;
                // cout << "(" << pt2.x << ", " << pt2.y << ")  p2" << endl;

                // This is a match to a MapPoint in the map
                if(vbMap[i])
                {
                    cv::rectangle(im,pt1,pt2,cv::Scalar(0,255,255));
                    // cv::rectangle(im,pt3,pt4,cv::Scalar(0,0,255));
                    cv::circle(im,vCurrentKeys[i].pt,2,cv::Scalar(0,255,0),-1);
                    mnTracked++;
                }
                else // This is match to a "visual odometry" MapPoint created in the last frame
                {
                    cv::rectangle(im,pt1,pt2,cv::Scalar(255,0,0));
                    cv::circle(im,vCurrentKeys[i].pt,2,cv::Scalar(255,0,0),-1);
                    mnTrackedVO++;
                }
            }
        }
    }

    for (int i = 0; i < position.size(); i++) {
        cv::Point2f rec1, rec2, ptname;
        rec1.x = position[i].at<int>(1);
        rec1.y = position[i].at<int>(0);
        rec2.x = position[i].at<int>(2);
        rec2.y = position[i].at<int>(3);


        ptname.x = position[i].at<int>(1) + 30;
        ptname.y = position[i].at<int>(0) + 50;

        cv::rectangle(im,rec1,rec2,cv::Scalar(255,0,0), 5);
        cv::putText(im,name[i], ptname, cv::FONT_HERSHEY_COMPLEX_SMALL, 2, cv::Scalar(0,0,255),1,CV_AA);
    }

    objectPoints.clear();

    cv::Mat imWithInfo;
    DrawTextInfo(im,state, imWithInfo);

    return imWithInfo;
}


void FrameDrawer::DrawTextInfo(cv::Mat &im, int nState, cv::Mat &imText)
{
    stringstream s;
    if(nState==Tracking::NO_IMAGES_YET)
        s << " WAITING FOR IMAGES";
    else if(nState==Tracking::NOT_INITIALIZED)
        s << " TRYING TO INITIALIZE ";
    else if(nState==Tracking::OK)
    {
        if(!mbOnlyTracking)
            s << "SLAM MODE |  ";
        else
            s << "LOCALIZATION | ";
        int nKFs = mpMap->KeyFramesInMap();
        int nMPs = mpMap->MapPointsInMap();
        s << "KFs: " << nKFs << ", MPs: " << nMPs << ", Matches: " << mnTracked;
        if(mnTrackedVO>0)
            s << ", + VO matches: " << mnTrackedVO;
    }
    else if(nState==Tracking::LOST)
    {
        s << " TRACK LOST. TRYING TO RELOCALIZE ";
    }
    else if(nState==Tracking::SYSTEM_NOT_READY)
    {
        s << " LOADING ORB VOCABULARY. PLEASE WAIT...";
    }

    int baseline=0;
    cv::Size textSize = cv::getTextSize(s.str(),cv::FONT_HERSHEY_PLAIN,1,1,&baseline);

    imText = cv::Mat(im.rows+textSize.height+10,im.cols,im.type());
    im.copyTo(imText.rowRange(0,im.rows).colRange(0,im.cols));
    imText.rowRange(im.rows,imText.rows) = cv::Mat::zeros(textSize.height+10,im.cols,im.type());
    cv::putText(imText,s.str(),cv::Point(5,imText.rows-5),cv::FONT_HERSHEY_PLAIN,1,cv::Scalar(255,255,255),1,8);

}

void FrameDrawer::Update(Tracking *pTracker)
{
    unique_lock<mutex> lock(mMutex);
    pTracker->mImGray.copyTo(mIm);
    mvCurrentKeys=pTracker->mCurrentFrame.mvKeys;
    N = mvCurrentKeys.size();
    mvbVO = vector<bool>(N,false);
    mvbMap = vector<bool>(N,false);
    mbOnlyTracking = pTracker->mbOnlyTracking;

    name = pTracker->name;
    position = pTracker->position;


    if(pTracker->mLastProcessedState==Tracking::NOT_INITIALIZED)
    {
        mvIniKeys=pTracker->mInitialFrame.mvKeys;
        mvIniMatches=pTracker->mvIniMatches;
    }
    else if(pTracker->mLastProcessedState==Tracking::OK)
    {
        vector<cv::Mat> centro(position.size());
        vector<int> firstPoint(position.size(),0);
        for(int i=0;i<N;i++)
        {
            MapPoint* pMP = pTracker->mCurrentFrame.mvpMapPoints[i];
            if(pMP)
            {
                if (pMP->mbTrackInView) {
                    for (int j = 0;j< position.size();j++) {
                        cv::Point2f rec1, rec2;
                        rec1.x = position[j].at<int>(1);
                        rec1.y = position[j].at<int>(0);
                        rec2.x = position[j].at<int>(2);
                        rec2.y = position[j].at<int>(3);
                        if (pMP->mTrackProjX >= rec1.x && pMP->mTrackProjX <= rec2.x && pMP->mTrackProjY >= rec1.y && pMP->mTrackProjY <= rec2.y) {
                            cv::Point2f pt1,pt2;
                            int r = 3;
                            pt1.x=pMP->mTrackProjX-r;
                            pt1.y=pMP->mTrackProjY-r;
                            pt2.x=pMP->mTrackProjX+r;
                            pt2.y=pMP->mTrackProjY+r;
                            objectPoints.push_back(make_pair(pt1, pt2));
                            cv::Mat point3D = pMP->GetNormal();

                            if (firstPoint[j] == 0) {
                                centro[j] = point3D;
                                firstPoint[j] = 1;
                            } else {
                                centro[j].at<float>(0) = (centro[j].at<float>(0) + point3D.at<float>(0)) / 2;
                                centro[j].at<float>(1) = (centro[j].at<float>(1) + point3D.at<float>(1)) / 2;
                                centro[j].at<float>(2) = (centro[j].at<float>(2) + point3D.at<float>(2)) / 2;
                            }
                            
                        }
                    }
                }
                if(!pTracker->mCurrentFrame.mvbOutlier[i])
                {
                    if(pMP->Observations()>0)
                        mvbMap[i]=true;
                    else
                        mvbVO[i]=true;
                }
            }
        }
        if (position.size()) {
            cv::Mat cameraPose = pTracker->mCurrentFrame.GetCameraCenter();
            vector<pair<float, string> > distanceObjects;
            for (int j = 0;j<position.size();j++) {
                if (firstPoint[j] == 1) {
                    distanceObjects.push_back(make_pair(sqrt(pow(centro[j].at<float>(0) - cameraPose.at<float>(0),2) + pow(centro[j].at<float>(1) - cameraPose.at<float>(1),2) + pow(centro[j].at<float>(2) - cameraPose.at<float>(2),2)),name[j]));
                    // cout << centro[j] << endl;
                }
            }
            sort(distanceObjects.begin(),distanceObjects.end());
            // for (int j = 0;j<distanceObjects.size();j++) {
            //     cout << distanceObjects[j].second << ": " << distanceObjects[j].first << endl;
            // }
            // cout << "distance an" << endl;
            if (distanceObjects.size() > 0) {
                unique_lock<mutex> lock(mText);
                if (speak) {
                    speak = false;

                    char a[1000] = "";

                    sprintf(a, "curl -X POST -u 05913210-7e6f-45dd-90a9-a0617ff7f8d7:4zpq6ngoVwwW \
                        --header 'Content-Type: application/json' \
                        --header 'Accept: audio/wav' \
                        --data '{\"text\":\"the %s is %.2f meters ahead\"}' \
                        --output rv2.wav \
                        'https://stream.watsonplatform.net/text-to-speech/api/v1/synthesize?voice=en-US_AllisonVoice'", (char*)distanceObjects[0].second.c_str(), distanceObjects[0].first);
                    system(a);
                    cout << a << endl;
                    cout << distanceObjects[0].second << ": " << distanceObjects[0].first << endl;
                }
            }
            // cout << "distance dp" << endl;
            // cout << endl << endl << endl;
            usleep(1000);
        }
    }
    mState=static_cast<int>(pTracker->mLastProcessedState);
}

} //namespace ORB_SLAM
