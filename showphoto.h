#ifndef SHOWPHOTO_H
#define SHOWPHOTO_H

#include <stdio.h>
#include <QObject>
#include <QFileDialog>
#include <QDir>
#include <QDebug>

#include "imageprovider.h"

#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include <opencv2/dnn/shape_utils.hpp>

#include <unordered_map>

using namespace cv;

using namespace cv::dnn;
using namespace std;
const size_t inWidth = 300;
const size_t inHeight = 300;
const double inScaleFactor = 1.0;
const Scalar meanVal(104.0, 117.0, 123.0);

// 下面两个文件在opencvFile文件夹里有
const cv::String PROTOTXTPATH = "D:/Qt_file/OpenPhoto/opencvFile/deploy.prototxt";
const cv::String CAFFEMODELPATH = "D:/Qt_file/OpenPhoto/opencvFile/res10_300x300_ssd_iter_140000_fp16.caffemodel";
// 点击抠取人脸保存的文件夹路径
const string SAVEPATH = "C:/Users/Embrace/Desktop";
class ShowPhoto:public QObject
{
    //槽函数的宏
    Q_OBJECT

public:

    //显示的构造函数
    explicit ShowPhoto(QObject *parent = 0);
    //Mat--->QImage
    QImage MattoQImage(Mat cvImg);
    //dnn模型的加载
    void loadModel();
    //图片的处理
    Mat imagePro(Mat frame);
    //图片提供者
    ImageProvider *m_PhotoProvider;






public slots:
    void openPhoto();
    void detectPhoto();
    void catchROI();
    void selectRectIndex(int i);
    void openVideo();
    void setVideoImage();


signals:
    //信号命名第一个字母用小写
    void callQmlShowPhoto();

    //传数据信号试试
    void getROIDig(int x_roi,int y_roi,int width_roi,int height_roi,int width_photo,int height_photo);


private:
    //网络模型
    Net net;
    //打开的图片
    Mat photo;

    Rect rectROI;

    //处理后的图片
    Mat pro_frame;
    //photo上选中了哪个rect
    unordered_map<int,bool> selectMap;
    //photo上检测到的ROI
    vector<Rect> ROIVector;
    //ROI命名
    int count=0;
    //读取视频的播放器（与摄像头原理相似）
    VideoCapture cap;

    //视频的某一帧图片
    Mat videoImage;


};

#endif // SHOWPHOTO_H
