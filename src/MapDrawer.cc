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
#include <vector>
#include <set>

#include "MapDrawer.h"
#include "MapPoint.h"
#include "KeyFrame.h"
#include <pangolin/pangolin.h>
#include <mutex>


namespace ORB_SLAM2
{


MapDrawer::MapDrawer(Map* pMap, const string &strSettingPath):mpMap(pMap)
{
    cv::FileStorage fSettings(strSettingPath, cv::FileStorage::READ);

    mKeyFrameSize = fSettings["Viewer.KeyFrameSize"];
    mKeyFrameLineWidth = fSettings["Viewer.KeyFrameLineWidth"];
    mGraphLineWidth = fSettings["Viewer.GraphLineWidth"];
    mPointSize = fSettings["Viewer.PointSize"];
    mCameraSize = fSettings["Viewer.CameraSize"];
    mCameraLineWidth = fSettings["Viewer.CameraLineWidth"];

}

void line(set<MapPoint*> points) {
    // int first = 0;
    vector<cv::Mat> p;
    // cout << "aqui" << endl;
    for(set<MapPoint*>::iterator sit=points.begin(), send=points.end(); sit!=send; sit++)
    {
        if((*sit)->isBad())
            continue;
        p.push_back((*sit)->GetWorldPos());
        // glVertex3f(pos.at<float>(0),pos.at<float>(1),pos.at<float>(2));

    }
    cout << "meio" << endl;
    vector<cv::Mat> res;
    // res.push_back(a);
    // cout << "copy 2" << endl;
    res.insert(res.end(), p.begin(), p.end());
    // cout << "copy" << endl;
    while (!res.empty()) {
        cv::Mat a = res[0];
        // cout << "Erase w" << endl;
        res.erase(res.begin());
        // cout << "Erase" << endl;
        vector<cv::Mat> aux;
        for (int i = res.size() - 1; i >= 0;i--) {
            // cout << "for" << endl;
            if (abs(res[i].at<float>(0) - a.at<float>(0)) < 0.05) {
                // res.push_back()
                aux.push_back(res[i]);
                res.erase(res.begin() + i);
            }
            // cout << "while" << endl;
        }
        glBegin(GL_LINE_STRIP);
        glColor3f(1.0,0.0,0.0);
        if (aux.size() > 5) {
            for (int i = 0;i<aux.size();i++) {
                glVertex3f(aux[i].at<float>(0),aux[i].at<float>(1),aux[i].at<float>(2));
            }
        }
        glEnd();
    }
   /* res.insert(res.end(), p.begin(), p.end());
    // cout << "copy" << endl;
    while (!res.empty()) {
        cv::Mat a = res[0];
        // cout << "Erase w" << endl;
        res.erase(res.begin());
        // cout << "Erase" << endl;
        vector<cv::Mat> aux;
        for (int i = res.size() - 1; i >= 0;i--) {
            // cout << "for" << endl;
            if (abs(res[i].at<float>(1) - a.at<float>(1)) < 0.05) {
                // res.push_back()
                aux.push_back(res[i]);
                res.erase(res.begin() + i);
            }
            // cout << "while" << endl;
        }
        glBegin(GL_LINE_STRIP);
        glColor3f(0.0,1.0,0.0);
        if (aux.size() > 5) {
            for (int i = 0;i<aux.size();i++) {
                glVertex3f(aux[i].at<float>(0),aux[i].at<float>(1),aux[i].at<float>(2));
            }
        }
        glEnd();
    }
    res.insert(res.end(), p.begin(), p.end());
    // cout << "copy" << endl;
    while (!res.empty()) {
        cv::Mat a = res[0];
        // cout << "Erase w" << endl;
        res.erase(res.begin());
        // cout << "Erase" << endl;
        vector<cv::Mat> aux;
        for (int i = res.size() - 1; i >= 0;i--) {
            // cout << "for" << endl;
            if (abs(res[i].at<float>(2) - a.at<float>(2)) < 0.05) {
                // res.push_back()
                aux.push_back(res[i]);
                res.erase(res.begin() + i);
            }
            // cout << "while" << endl;
        }
        glBegin(GL_LINE_STRIP);
        glColor3f(0.0,0.0,1.0);
        if (aux.size() > 5) {
            for (int i = 0;i<aux.size();i++) {
                glVertex3f(aux[i].at<float>(0),aux[i].at<float>(1),aux[i].at<float>(2));
            }
        }
        glEnd();
    }*/
    // cout << "depois" << endl;
}

float distance(cv::Mat v1, cv::Mat v2) {
    float divi = v1.at<float>(0) * v2.at<float>(0) + 
                        v1.at<float>(1) * v2.at<float>(1) + 
                        v1.at<float>(2) * v2.at<float>(2);
    float norma = sqrt(pow(v2.at<float>(0),2) + pow(v2.at<float>(1),2) + pow(v2.at<float>(2),2));
    return divi/norma;
}

void MapDrawer::DrawMapPoints()
{
    const vector<MapPoint*> &vpMPs = mpMap->GetAllMapPoints();
    const vector<MapPoint*> &vpRefMPs = mpMap->GetReferenceMapPoints();

    set<MapPoint*> spRefMPs(vpRefMPs.begin(), vpRefMPs.end());
    if(vpMPs.empty())
        return;
/* 
######################################################################################################
######################################################################################################
######################################################################################################
######################################################################################################
######################################################################################################
######################################################################################################
*/
    // cv::Mat cam;
    // mCameraPose.copyTo(cam);
    // cout << cam << endl;
    // cv::Mat x;
    // cam.col(3).copyTo(x);

/*

    if (!mCameraPose.empty()){
        cv::Mat cam = mCameraPose.rowRange(0,3).col(3);
        cout << "Camera: " << mCameraPose.rowRange(0,3).col(3) << endl << endl;
        
        /*glBegin(GL_LINE_STRIP);
        glColor3f(1.0,1.0,0.0);
        glVertex3f(cam.at<float>(0) + 1,cam.at<float>(1) + 1,0.1);
        glVertex3f(cam.at<float>(0) + 1,cam.at<float>(1) - 1,0.1);
        glVertex3f(cam.at<float>(0) - 1,cam.at<float>(1) + 1,0.1);
        glVertex3f(cam.at<float>(0) - 1,cam.at<float>(1) - 1,0.1);
        glEnd();*/
/*



        const float &w = mCameraSize;
        const float h = w*0.6;
        const float z = w*0.9;

        float x1, x2,x3;
        x1 = cam.at<float>(0);
        x2 = cam.at<float>(1);
        x3 = cam.at<float>(2);

        cv::Mat c1 = cv::Mat::zeros(3,1,CV_32F);
        c1.at<float>(0) = ((x1*3 + w*30*2)/2.0/3.0);
        c1.at<float>(1) = ((x2*3)/2.0/3.0);
        c1.at<float>(2) = ((x3*3 + z*30*2)/2.0/3.0);
        cv::Mat c2 = cv::Mat::zeros(3,1,CV_32F);
        c2.at<float>(0) = ((x1*3 - w*30*2)/2.0/3.0);
        c2.at<float>(1) = ((x2*3)/2.0/3.0);
        c2.at<float>(2) = ((x3*3 + z*30*2)/2.0/3.0);
        cv::Mat c3 = cv::Mat::zeros(3,1,CV_32F);
        c3.at<float>(0) = ((x1*3)/2.0/3.0);
        c3.at<float>(1) = ((x2*3 + h*30*2)/2.0/3.0);
        c3.at<float>(2) = ((x3*3 + z*30*2)/2.0/3.0);
        cv::Mat c4 = cv::Mat::zeros(3,1,CV_32F);
        c4.at<float>(0) = ((x1*3)/2.0/3.0);
        c4.at<float>(1) = ((x2*3 - h*30*2)/2.0/3.0);
        c4.at<float>(2) = ((x3*3 + z*30*2)/2.0/3.0);
/*

    glVertex3f(w*30,h*30,z*30);
    glVertex3f(w*30,-h*30,z*30);
    glVertex3f(0,0,0);

    glVertex3f(-w*30,h*30,z*30);
    glVertex3f(-w*30,-h*30,z*30);
    glVertex3f(0,0,0);

    glVertex3f(-w*30,h*30,z*30);
    glVertex3f(w*30,h*30,z*30);
    glVertex3f(0,0,0);

    glVertex3f(-w*30,-h*30,z*30);
    glVertex3f(w*30,-h*30,z*30);
    glVertex3f(0,0,0);

    *//*
        cv::Mat ponto = vpRefMPs[0]->GetWorldPos();
        cout << endl << endl << endl << "VPREFMPS" << endl;
        cout << "Ponto: [" << ponto.at<float>(0) << ", "
                            << ponto.at<float>(1) << ", "  
                            << ponto.at<float>(2) << "]"<< endl;
        cout << "c1: " << distance(c1, ponto) << endl;
        cout << "c2: " << distance(c2, ponto) << endl;
        cout << "c3: " << distance(c3, ponto) << endl;
        cout << "c4: " << distance(c4, ponto) << endl;


        glLineWidth(mCameraLineWidth);
        glBegin(GL_LINES);
        glColor3f(1.0,0.0,0.0);

        glVertex3f(ponto.at<float>(0),ponto.at<float>(1),ponto.at<float>(2));
        glVertex3f(c1.at<float>(0),c1.at<float>(1),c1.at<float>(2));
    
        glVertex3f(ponto.at<float>(0),ponto.at<float>(1),ponto.at<float>(2));
        glVertex3f(c2.at<float>(0),c2.at<float>(1),c2.at<float>(2));
    
        glVertex3f(ponto.at<float>(0),ponto.at<float>(1),ponto.at<float>(2));
        glVertex3f(c3.at<float>(0),c3.at<float>(1),c3.at<float>(2));
    
        glVertex3f(ponto.at<float>(0),ponto.at<float>(1),ponto.at<float>(2));
        glVertex3f(c4.at<float>(0),c4.at<float>(1),c4.at<float>(2));

        glEnd();


    }*/
    // if (vpMPs.size() > 0) {
    //     cout << endl << endl << endl << "VPMPS" << endl;
    //     for (int i = 0; i<vpMPs.size();i++) {
    //         cout << "Ponto " << i << ": " << vpMPs[i]->GetWorldPos() << endl;
    //     }
    // }
    // if (vpRefMPs.size() > 0) {
    //     cout << endl << endl << endl << "VPREFMPS" << endl;
    //     for (int i = 0; i<vpRefMPs.size();i++) {
    //         cout << "Ponto " << i << ": " << vpRefMPs[i]->GetWorldPos() << endl;
    //     }
    // }
/* 
######################################################################################################
######################################################################################################
######################################################################################################
######################################################################################################
######################################################################################################
######################################################################################################
*/

    glPointSize(mPointSize);
    glBegin(GL_POINTS);
    glColor3f(0.0,0.0,0.0);

    for(size_t i=0, iend=vpMPs.size(); i<iend;i++)
    {
        if(vpMPs[i]->isBad() || spRefMPs.count(vpMPs[i]))
            continue;
        cv::Mat pos = vpMPs[i]->GetWorldPos();
        glVertex3f(pos.at<float>(0),pos.at<float>(1),pos.at<float>(2));
    }
    glEnd();

    glPointSize(mPointSize*5);
    glBegin(GL_POINTS);
    // glLineWidth(mPointSize);
    // glBegin(GL_LINE_STRIP);
    glColor3f(1.0,0.0,0.0);

    for(set<MapPoint*>::iterator sit=spRefMPs.begin(), send=spRefMPs.end(); sit!=send; sit++)
    {
        if((*sit)->isBad())
            continue;
        cv::Mat pos = (*sit)->GetWorldPos();
        glVertex3f(pos.at<float>(0),pos.at<float>(1),pos.at<float>(2));

    }
    // line(spRefMPs);
    glEnd();
}

void MapDrawer::DrawKeyFrames(const bool bDrawKF, const bool bDrawGraph)
{
    const float &w = mKeyFrameSize;
    const float h = w*0.75;
    const float z = w*0.6;

    const vector<KeyFrame*> vpKFs = mpMap->GetAllKeyFrames();

    if(bDrawKF)
    {
        for(size_t i=0; i<vpKFs.size(); i++)
        {
            KeyFrame* pKF = vpKFs[i];
            cv::Mat Twc = pKF->GetPoseInverse().t();

            glPushMatrix();

            glMultMatrixf(Twc.ptr<GLfloat>(0));

            glLineWidth(mKeyFrameLineWidth);
            glColor3f(0.0f,0.0f,1.0f);
            glBegin(GL_LINES);
            glVertex3f(0,0,0);
            glVertex3f(w,h,z);
            glVertex3f(0,0,0);
            glVertex3f(w,-h,z);
            glVertex3f(0,0,0);
            glVertex3f(-w,-h,z);
            glVertex3f(0,0,0);
            glVertex3f(-w,h,z);

            glVertex3f(w,h,z);
            glVertex3f(w,-h,z);

            glVertex3f(-w,h,z);
            glVertex3f(-w,-h,z);

            glVertex3f(-w,h,z);
            glVertex3f(w,h,z);

            glVertex3f(-w,-h,z);
            glVertex3f(w,-h,z);
            glEnd();

            glPopMatrix();
        }
    }

    if(bDrawGraph)
    {
        glLineWidth(mGraphLineWidth);
        glColor4f(0.0f,1.0f,0.0f,0.6f);
        glBegin(GL_LINES);

        for(size_t i=0; i<vpKFs.size(); i++)
        {
            // Covisibility Graph
            const vector<KeyFrame*> vCovKFs = vpKFs[i]->GetCovisiblesByWeight(100);
            cv::Mat Ow = vpKFs[i]->GetCameraCenter();
            if(!vCovKFs.empty())
            {
                for(vector<KeyFrame*>::const_iterator vit=vCovKFs.begin(), vend=vCovKFs.end(); vit!=vend; vit++)
                {
                    if((*vit)->mnId<vpKFs[i]->mnId)
                        continue;
                    cv::Mat Ow2 = (*vit)->GetCameraCenter();
                    glVertex3f(Ow.at<float>(0),Ow.at<float>(1),Ow.at<float>(2));
                    glVertex3f(Ow2.at<float>(0),Ow2.at<float>(1),Ow2.at<float>(2));
                }
            }

            // Spanning tree
            KeyFrame* pParent = vpKFs[i]->GetParent();
            if(pParent)
            {
                cv::Mat Owp = pParent->GetCameraCenter();
                glVertex3f(Ow.at<float>(0),Ow.at<float>(1),Ow.at<float>(2));
                glVertex3f(Owp.at<float>(0),Owp.at<float>(1),Owp.at<float>(2));
            }

            // Loops
            set<KeyFrame*> sLoopKFs = vpKFs[i]->GetLoopEdges();
            for(set<KeyFrame*>::iterator sit=sLoopKFs.begin(), send=sLoopKFs.end(); sit!=send; sit++)
            {
                if((*sit)->mnId<vpKFs[i]->mnId)
                    continue;
                cv::Mat Owl = (*sit)->GetCameraCenter();
                glVertex3f(Ow.at<float>(0),Ow.at<float>(1),Ow.at<float>(2));
                glVertex3f(Owl.at<float>(0),Owl.at<float>(1),Owl.at<float>(2));
            }
        }

        glEnd();
    }
}

void MapDrawer::DrawCurrentCamera(pangolin::OpenGlMatrix &Twc)
{
    const float &w = mCameraSize;
    const float h = w*0.6;
    const float z = w*0.9;

    const vector<MapPoint*> &vpRefMPs = mpMap->GetReferenceMapPoints();

    glPushMatrix();

#ifdef HAVE_GLES
        glMultMatrixf(Twc.m);
#else
        glMultMatrixd(Twc.m);
#endif

    glLineWidth(mCameraLineWidth);
    glColor3f(0.0f,1.0f,0.0f);
    glBegin(GL_LINES);
    glVertex3f(0,0,0);
    // glVertex3f(w,h,z);
    glVertex3f(w*30,h*30,z*30);
    glVertex3f(0,0,0);
    // glVertex3f(w,-h,z);
    glVertex3f(w*30,-h*30,z*30);
    glVertex3f(0,0,0);
    // glVertex3f(-w,-h,z);
    glVertex3f(-w*30,-h*30,z*30);
    glVertex3f(0,0,0);
    // glVertex3f(-w,h,z);
    glVertex3f(-w*30,h*30,z*30);

    glVertex3f(w,h,z);
    glVertex3f(w,-h,z);

    glVertex3f(-w,h,z);
    glVertex3f(-w,-h,z);

    glVertex3f(-w,h,z);
    glVertex3f(w,h,z);

    glVertex3f(-w,-h,z);
    glVertex3f(w,-h,z);

    glEnd();

    glBegin(GL_TRIANGLES);
    // fora
    glColor3f(0.0f,1.0f,1.0f);
    
    glVertex3f(w*30,h*30,z*30);
    glVertex3f(w*30,-h*30,z*30);
    glVertex3f(0,0,0);

    glVertex3f(-w*30,h*30,z*30);
    glVertex3f(-w*30,-h*30,z*30);
    glVertex3f(0,0,0);

    glVertex3f(-w*30,h*30,z*30);
    glVertex3f(w*30,h*30,z*30);
    glVertex3f(0,0,0);

    glVertex3f(-w*30,-h*30,z*30);
    glVertex3f(w*30,-h*30,z*30);
    glVertex3f(0,0,0);
    glEnd();

    if (vpRefMPs.size() > 0) {
        cv::Mat c1 = cv::Mat::zeros(3,1,CV_32F);
        c1.at<float>(0) = (h*z-(z*-h));
        c1.at<float>(1) = (z*w-(w*z));
        c1.at<float>(2) = ((w*-h) - (h*w));
        cv::Mat c2 = cv::Mat::zeros(3,1,CV_32F);
        c2.at<float>(0) = (h*z-(z*-h));
        c2.at<float>(1) = (z*-w-(-w*z));
        c2.at<float>(2) = ((-w*-h) - (h*-w));
        cv::Mat c3 = cv::Mat::zeros(3,1,CV_32F);
        c3.at<float>(0) = (h*z-z*h);
        c3.at<float>(1) = (z*w-(-w*z));
        c3.at<float>(2) = (-w*h-h*w);
        cv::Mat c4 = cv::Mat::zeros(3,1,CV_32F);
        c4.at<float>(0) = (-h*z-z*-h);
        c4.at<float>(1) = (z*w-(-w*z));
        c4.at<float>(2) = (-w*-h - (-h*w));
        /*

        glVertex3f(w*30,h*30,z*30);
        glVertex3f(w*30,-h*30,z*30);
        glVertex3f(0,0,0);

        glVertex3f(-w*30,h*30,z*30);
        glVertex3f(-w*30,-h*30,z*30);
        glVertex3f(0,0,0);

        glVertex3f(-w*30,h*30,z*30);
        glVertex3f(w*30,h*30,z*30);
        glVertex3f(0,0,0);

        glVertex3f(-w*30,-h*30,z*30);
        glVertex3f(w*30,-h*30,z*30);
        glVertex3f(0,0,0);

        */

        // cv::Mat ponto = vpRefMPs[50]->GetWorldPos();
        // cout << endl << endl << endl << "VPREFMPS" << endl;
        // cout << "Ponto: [" << ponto.at<float>(0) << ", "
        //                     << ponto.at<float>(1) << ", "  
        //                     << ponto.at<float>(2) << "]"<< endl;
        // cout << "c1: " << distance(c1, ponto) << endl;
        // cout << "c2: " << distance(c2, ponto) << endl;
        // cout << "c3: " << distance(c3, ponto) << endl;
        // cout << "c4: " << distance(c4, ponto) << endl;


        /*glLineWidth(mCameraLineWidth);
        glBegin(GL_LINES);
        glColor3f(1.0,0.0,0.0);

        glVertex3f(ponto.at<float>(0),ponto.at<float>(1),ponto.at<float>(2));
        glVertex3f(c1.at<float>(0),c1.at<float>(1),c1.at<float>(2));

        glVertex3f(ponto.at<float>(0),ponto.at<float>(1),ponto.at<float>(2));
        glVertex3f(c2.at<float>(0),c2.at<float>(1),c2.at<float>(2));

        glVertex3f(ponto.at<float>(0),ponto.at<float>(1),ponto.at<float>(2));
        glVertex3f(c3.at<float>(0),c3.at<float>(1),c3.at<float>(2));

        glVertex3f(ponto.at<float>(0),ponto.at<float>(1),ponto.at<float>(2));
        glVertex3f(c4.at<float>(0),c4.at<float>(1),c4.at<float>(2));

        glEnd();*/
    }
    glPopMatrix();
}


void MapDrawer::SetCurrentCameraPose(const cv::Mat &Tcw)
{
    unique_lock<mutex> lock(mMutexCamera);
    mCameraPose = Tcw.clone();
}

void MapDrawer::GetCurrentOpenGLCameraMatrix(pangolin::OpenGlMatrix &M)
{
    if(!mCameraPose.empty())
    {
        cv::Mat Rwc(3,3,CV_32F);
        cv::Mat twc(3,1,CV_32F);
        {
            unique_lock<mutex> lock(mMutexCamera);
            Rwc = mCameraPose.rowRange(0,3).colRange(0,3).t();
            twc = -Rwc*mCameraPose.rowRange(0,3).col(3);
        }

        M.m[0] = Rwc.at<float>(0,0);
        M.m[1] = Rwc.at<float>(1,0);
        M.m[2] = Rwc.at<float>(2,0);
        M.m[3]  = 0.0;

        M.m[4] = Rwc.at<float>(0,1);
        M.m[5] = Rwc.at<float>(1,1);
        M.m[6] = Rwc.at<float>(2,1);
        M.m[7]  = 0.0;

        M.m[8] = Rwc.at<float>(0,2);
        M.m[9] = Rwc.at<float>(1,2);
        M.m[10] = Rwc.at<float>(2,2);
        M.m[11]  = 0.0;

        M.m[12] = twc.at<float>(0);
        M.m[13] = twc.at<float>(1);
        M.m[14] = twc.at<float>(2);
        M.m[15]  = 1.0;
    }
    else
        M.SetIdentity();
}

} //namespace ORB_SLAM
