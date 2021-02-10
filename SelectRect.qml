import QtQuick 2.0

Canvas{

    id:root
    signal clicked
    property bool isSelected: false

    onPaint: {
        var ctx = getContext("2d")
        ctx.fillStyle = "blue"
        //坐标以画布的坐标为准
        ctx.fillRect(0,0,root.width,root.height)
        ctx.clearRect(3,3,root.width-6,root.height-6)


    }
    NumberAnimation on opacity{
        from:0
        to:1
        duration: 2000
        easing.type: "InOutQuad"

    }
    Rectangle{
        id:rect



        anchors.centerIn: parent
        width: root.width-6
        height: root.height-6
        opacity: 0
        MouseArea{
            anchors.fill: parent
            onClicked: {
                if(!isSelected){
                    rect.color = "blue"
                    rect.opacity = 0.3
                    isSelected = true
                }
                else{
                    rect.color = ""
                    rect.opacity = 0
                    isSelected = false
                }
                root.clicked()
            }


       }
    }

}
