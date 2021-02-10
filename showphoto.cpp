#include "showphoto.h"
/**
 * @brief ShowPhoto::ShowPhoto
 * @param parent
 * 构造函数
 */
ShowPhoto::ShowPhoto(QObject *parent):QObject(parent)
{

    //初始化图片提供者
    m_PhotoProvider = new ImageProvider();
    //提前加载模型
    loadModel();

}
/**
 * @brief ShowPhoto::loadModel
 * 加载模型
 */
void ShowPhoto::loadModel(){

    //这里会从当前应用程序所在目录的绝对路径里找opencvFile这个目录，可直接用opencvFile这个文件夹所在的绝对路径
    //opencv库里的sample下的dnn里的prototxt文件即可
    cv::String modelConfiguration = "D:/Qt_file/OpenPhoto/opencvFile/deploy.prototxt";
    cv::String modelBinary = "D:/Qt_file/OpenPhoto/opencvFile/res10_300x300_ssd_iter_140000_fp16.caffemodel";
    //! [Initialize network]
    net = readNetFromCaffe(modelConfiguration, modelBinary);
}

/**
 * @brief ShowPhoto::openPhoto
 * 槽函数：打开图片
 */
void ShowPhoto::openPhoto(){

    QString fileName =  QFileDialog::getOpenFileName(new QWidget,tr("Open Image"),
                                                     ".",tr("Image Files (*.png *.jpg *.bmp)"));
    qDebug() << "PhotoFileName:"<<fileName;

    Mat photoTemp = imread(fileName.toLocal8Bit().toStdString());

    photoTemp.copyTo(photo);



    //清空selectMap和ROIVector
    if(!selectMap.empty())
        selectMap.clear();
    if(!ROIVector.empty())
        ROIVector.clear();

    m_PhotoProvider->img = MattoQImage(photoTemp);
    //发送显示信号给QML
    emit callQmlShowPhoto();

}


/**
 * @brief ShowPhoto::MattoQImage
 * @param cvImg
 * @return qImg
 * Mat转QImage格式
 */
QImage ShowPhoto::MattoQImage(Mat cvImg)
{

    QImage qImg;
    if(cvImg.channels()==3)
    {
        qDebug()<<"to 1"<<endl;
        cvtColor(cvImg,cvImg,CV_BGR2RGB);
        //qDebug()<<"to 11"<<endl;
        qImg =QImage((const unsigned char*)(cvImg.data),
                cvImg.cols, cvImg.rows,cvImg.cols*cvImg.channels(),
                     QImage::Format_RGB888);
        //qDebug()<<"to 111"<<endl;

    }
    else if(cvImg.channels()==1){
        qDebug()<<'to 2'<<endl;
        qImg = QImage((const unsigned char*)(cvImg.data),
                      cvImg.cols,cvImg.rows,cvImg.cols*cvImg.channels(),
                       QImage::Format_Indexed8);
    }
    else{
        qDebug()<<'to 3'<<endl;
        qImg = QImage((const unsigned char*)(cvImg.data),
                      cvImg.cols,cvImg.rows,cvImg.cols*cvImg.channels(),
                       QImage::Format_RGB888);

    }
    return qImg;
}


/**
 * @brief ShowPhoto::imagePro
 * @param frame
 * @return pro_frame
 * 对frame图像检测人脸
 */
Mat ShowPhoto::imagePro(Mat frame)
{
    Mat inputBlob = blobFromImage(frame, inScaleFactor,
        Size(inWidth, inHeight), meanVal, false, false); //Convert Mat to batch of images

    net.setInput(inputBlob, "data"); //set the network input

    Mat detection = net.forward("detection_out"); //compute output

    //获取网络的总用时以及每层的用时
    //vector<double> layersTimings;
    //double freq = getTickFrequency() / 1000;
    //double time = net.getPerfProfile(layersTimings) / freq;

    Mat detectionMat(detection.size[2], detection.size[3], CV_32F, detection.ptr<float>());

    float confidenceThreshold = 0.45;

    for (int i = 0; i < detectionMat.rows; i++)
    {
        float confidence = detectionMat.at<float>(i, 2);

        if (confidence > confidenceThreshold)
        {
            int xLeftTop = static_cast<int>(detectionMat.at<float>(i, 3) * frame.cols);
            int yLeftTop = static_cast<int>(detectionMat.at<float>(i, 4) * frame.rows);
            int xRightBottom = static_cast<int>(detectionMat.at<float>(i, 5) * frame.cols);
            int yRightBottom = static_cast<int>(detectionMat.at<float>(i, 6) * frame.rows);

            Rect object((int)xLeftTop, (int)yLeftTop,
                (int)(xRightBottom - xLeftTop),
                (int)(yRightBottom - yLeftTop));

            //存储这张图片上的ROI的Rect
            ROIVector.push_back(object);

            emit getROIDig((int)xLeftTop, (int)yLeftTop,
                           (int)(xRightBottom - xLeftTop),
                           (int)(yRightBottom - yLeftTop),frame.cols,frame.rows);

            //Rect(左上角的x,左上角的y,宽,高)
            rectROI = Rect((int)xLeftTop, (int)yLeftTop,
                           (int)(xRightBottom - xLeftTop),
                           (int)(yRightBottom - yLeftTop));
            if(rectROI==object)
                qDebug() <<".............yes..............."<<endl;
            else
                qDebug() <<".............no..............."<<endl;


            rectangle(frame, rectROI, Scalar(0,0,255),2);

        }
     }

     pro_frame = frame.clone();
     return pro_frame;
}

/**
 * @brief ShowPhoto::detectPhoto
 * 槽函数：检测人脸（图片）
 */
void ShowPhoto::detectPhoto()
{
    //设置暂时图片，避免photo被影响
    Mat photoTemp;
    photo.copyTo(photoTemp);

    Mat detectPhoto = imagePro(photoTemp);
    //因为已经在界面上画出了人脸框了，所以这里就不用再传过去了
    //m_PhotoProvider->img = MattoQImage(photoTemp);
    //emit callQmlShowPhoto();
}

/**
 * @brief ShowPhoto::selectRectIndex
 * @param i
 * 发送选中矩形框的序号
 */
void ShowPhoto::selectRectIndex(int i)
{
    if(selectMap[i]==true)
        selectMap[i] = false;
    else
        selectMap[i] = true;
}

/**
 * @brief ShowPhoto::catchROI
 * 抠取选中的人脸，保存至指定路径
 */
void ShowPhoto::catchROI()
{


    for(int i=0;i<ROIVector.size();i++)
    {

        if(selectMap[i]){

            //保存的路径
            stringstream st;
            st<<"C:/Users/Embrace/Desktop/MaskedFace/";
            st<<count;
            st<<".png";
            string saveName = st.str();
            cout<<saveName<<endl;


            qDebug()<<"抓取的roi有："<<i<<endl;

            Mat imageROI;

            photo(ROIVector[i]).copyTo(imageROI);

            bool saveROIBool = imwrite(saveName,imageROI);

            if(saveROIBool)
                qDebug() << "save success!"<<endl;
            else
                qDebug() << "save fail!"<<endl;

            count++;
        }
        else
            qDebug()<<"你没有选中任何一个人脸框!!!"<<endl;
    }

}
/**
 * @brief ShowPhoto::openVideo
 * 打开视频文件操作
 */
void ShowPhoto::openVideo()
{
    QString fileName =  QFileDialog::getOpenFileName(new QWidget,tr("Open Video"),
                                                     ".",tr("Video Files (*.mp4 *.avi *.wmv)"));
    qDebug() << "VideoFileName:"<<fileName;

    //打开这个视频文件
    cap.open(fileName.toLocal8Bit().toStdString());

    if(cap.isOpened())
        qDebug() << "Video资源正处于开放状态！"<<endl;
    else
        qDebug() << "Video资源不开放！"<<endl;

}
/**
 * @brief ShowPhoto::setVideoImage
 * 放置视频中的其中一帧图像
 */
void ShowPhoto::setVideoImage()
{
    //存储视频中的每一帧图片
    Mat frame;
    //保存下来
    frame.copyTo(videoImage);
    //摄像头实时读取每一帧图片
    cap>>frame;
    if(frame.empty()){

        qDebug() << "视频播放完毕！"<<endl;
        //释放资源
        cap.release();

    }else{

        //直接在视频中检测人脸
        m_PhotoProvider->img = MattoQImage(imagePro(frame));
        //发送显示信号给QML
        emit callQmlShowPhoto();
    }
}


