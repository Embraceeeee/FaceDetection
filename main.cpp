#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QApplication>

#include <QQuickStyle>

#include <showphoto.h>
#include <QQmlContext>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication app(argc, argv);

    //先在.pro里QT+=quickcontrols2 再在这里声明Style为Universal
    QQuickStyle::setStyle("Universal");

    ShowPhoto *CodePhoto = new ShowPhoto();


    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);

    //为让图片动态显示，需要设置ImageProvider
    engine.rootContext()->setContextProperty("CodePhoto",CodePhoto);
    //CodeImage成为一个元素的东西，可以在QML中使用
    engine.addImageProvider(QLatin1String("CodePhoto"),CodePhoto->m_PhotoProvider);

    engine.load(url);

    return app.exec();
}
