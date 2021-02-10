#ifndef IMAGEPROVIDER_H
#define IMAGEPROVIDER_H

#include <QQuickImageProvider>
#include <QDebug>


class ImageProvider : public QQuickImageProvider
{
public:

    ImageProvider();
    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize);

    QImage img;

};

#endif // IMAGEPROVIDER_H
