#include "imageprovider.h"

ImageProvider::ImageProvider():QQuickImageProvider(QQuickImageProvider::Image)
{

}

//重写requestImage接口
QImage ImageProvider::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{
    qDebug() << "requestImage"<<endl;
    qDebug() << "id: " << id;
    return this->img;
}
