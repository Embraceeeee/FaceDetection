# Face Detection
Description: The repository is used to store face detection project written by C++ and QML.





## 基于OpenCV 与QML的人脸检测与抠图程序

> Qt Project File名称：OpenPhoto



#### 程序概况

* 采用的是OpenCv里的DNN模块中已训练好的残差网络模型来检测人脸。
* 使用QML绘制程序界面。
* 虽然该程序并没有完善得很好，但作为一个*学习样例*还算okay的，内有C++与QML的交互，检测模型的使用等code。

#### 环境配置

* 编译软件
  
    * Qt Creator 5.14.2（MingW版本）
* 编译环境
    * OpenCv 3.4.9（需编译）
    * Qt 5.14.2
    * MingW73 64位
    
#### 运行事项
1. 需修改OpenPhoto.pro文件下的INCLUDEPATH和LIBS的路径，改成自己电脑下OpenCv库include和libs所在的绝对路径。

2. 需修改showphoto.h中的

   * PROTOTXTPATH（deploy.prototxt路径）

   * CAFFEMODELPATH（res10_300x300_ssd_iter_140000_fp16.caffemodel路径）

   * SAVEPATH（人脸ROI区域图片保存的文件夹路径）

     这三个变量指向的绝对路径。


#### 运行效果

![效果图](/resultImage/resultImage.png)

#### 参考的学习链接
* [OpenCV基于残差网络实现人脸检测](https://blog.51cto.com/gloomyfish/2094611)
* [QML Book](https://cwc1987.gitbooks.io/qmlbook-in-chinese/content/)
