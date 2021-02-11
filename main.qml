import QtQuick 2.12
import QtQuick.Window 2.12

import QtQuick.Controls 2.12
import QtQuick.Controls.Universal 2.12

ApplicationWindow {
    visible: true
    width: 640
    height: 480
    title: qsTr("图片检测样例")

    Universal.theme: Universal.Dark

    property int xr: 0
    property int yr: 0
    property int wr: 0
    property int hr: 0



    Rectangle{

        id:photoContainer
        width: 416
        height: 416
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.margins: 16
        color: "#333333"
        radius: 8




        Image {
            id: photo
            anchors.fill: parent
            anchors.margins: 8
            cache: false
            //fillMode: Image.PreserveAspectFit



        }

    }
    ListModel{
        id:selectRectModel

    }
    Repeater{
        model: selectRectModel
        SelectRect{

            x:sxr
            y:syr
            width: swr
            height: shr
            onClicked: {
                console.log("the selected rect's index:"+index)
                CodePhoto.selectRectIndex(index)

            }
        }
    }

    Column{
        width: 144
        height: 480
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.rightMargin: 32
        anchors.topMargin: 16

        spacing: 16

        Button{
            id:openPhotoButton
            width: parent.width
            height: 40


            text: qsTr("打开图片")

            onClicked: {

                CodePhoto.openPhoto();
                selectRectModel.clear()
            }
        }
        Button{
            id:detectPhotoButton
            width: parent.width
            height: 40


            text: qsTr("检测图片")

            onClicked: {

                CodePhoto.detectPhoto();
            }
        }
        Button{
            id:catchFaceButton
            width: parent.width
            height: 40
            text: qsTr("抠取人脸")
            onClicked: {

                CodePhoto.catchROI();
            }
        }
//        Button{
//            id:openVideoButton
//            width: parent.width
//            height: 40
//            text: qsTr("打开视频")

//            onClicked: {
//                CodePhoto.openVideo()
//                videoCount.running = true


//            }
//        }
//        RadioButton{
//            id:cathcVideo1
//            width: parent.width
//            height: 40
//            text: qsTr("每隔一秒抠取")

//            onClicked: {



//            }
//        }
//        RadioButton{
//            id:cathcVideo2
//            width: parent.width
//            height: 40
//            text: qsTr("每隔两秒抠取")

//            onClicked: {



//            }
//        }
//        Button{
//            id:saveROIPath
//        }
    }
    //定时器实时触发
    Timer{
        id:videoCount
        interval: 80
        running: false
        repeat: true
        onTriggered: {
            //设置Video每一帧图像，并且检测人脸
            CodePhoto.setVideoImage();
        }


    }

    Connections{
        target: CodePhoto
        onCallQmlShowPhoto:{
            console.log("load c++ opening photo...")
            photo.source = "";
            photo.source = "image://CodePhoto";

        }
        onGetROIDig:{
            console.log("load c++ ROI parameter...")
            console.log("x坐标:"+x_roi+" y坐标:"+y_roi+" ROI宽:"+width_roi+" ROI高:"+height_roi+" 图宽:"+width_photo+" 图高:"+height_photo)

            xr = (x_roi*(400/width_photo))+24
            yr = (y_roi*(400/height_photo))+24
            wr = (width_roi*(400/width_photo))
            hr = (height_roi*(400/height_photo))
            console.log("..."+"sxr_"+xr+" syr_"+yr+" swr_"+wr+" shr_"+hr)
            //传给模型，绘制矩形框
            selectRectModel.append({"sxr":xr,"syr":yr,"swr":wr,"shr":hr})

        }
    }




}
