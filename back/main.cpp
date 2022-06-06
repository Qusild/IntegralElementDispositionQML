#include <QGuiApplication>
#include <QApplication>
#include <QQmlApplicationEngine>
#include "griddelegate.h"
#include <QQuickView>
#include <QQmlContext>
#include <QQuickItem>
#define  dim_x 10
#define dim_y  10
int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QApplication app(argc, argv);
    Schema test_schema(dim_x, dim_y);
    GridDelegate model(dim_y, dim_x);
    QQmlApplicationEngine engine;
    QQmlContext *context = engine.rootContext();    // Создаём корневой контекст
    /* Загружаем объект в контекст для установки соединения,
     * а также определяем имя, по которому будет происходить соединение
     * */
    test_schema.clear_map();
    integral_element* element = new integral_element(0, 0, 11,"element #1");
    element->add_connection(connection(11, 12, 10001));
    test_schema.add_element(*element);
    delete element;

    element = new integral_element(1, 1, 12, "Element #2");
    test_schema.add_element(*element);
    delete element;

    test_schema.schema_map[1][0]=10001;

    context->setContextProperty("GridModel", QVariant::fromValue(&model));
    context->setContextProperty("schema", QVariant::fromValue(&test_schema));

    // И загружаем в него исходники qml

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
