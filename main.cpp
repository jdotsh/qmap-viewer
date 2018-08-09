#include <QDebug>
#include <QFile>
#include <QGuiApplication>
#include <QJsonDocument>
#include <QQmlApplicationEngine>
#include <QVariantMap>
#include <QQmlContext>
#include <QtLocation/private/qgeojson_p.h>
#include <QGeoCircle>
#include <QGeoPath>
#include <QGeoPolygon>


const QString QGeoJsonPrettyPrint(const QVariant &variantItem)
{
    QJsonDocument jsonMapDoc;
    jsonMapDoc= QJsonDocument::fromVariant(variantItem);
    QString geoJsonString = jsonMapDoc.toJson(QJsonDocument::Indented);
    return geoJsonString;

    /*QString res;
    int i;

    if (variantItem.canConvert<QVariantMap>()) {
        QVariantMap map = variantItem.value<QVariantMap>();

        QVariantMap::const_iterator itr = map.begin();

        res.append("{");
        for (; itr != map.end(); ++itr) {
            res.append(QString(QStringLiteral(" «%1» : %2,\n")).arg(itr.key(), QGeoJsonPrettyPrint(itr.value(),indent)));
        }
        res.append(QStringLiteral("}\n"));
    };

    if (variantItem.canConvert<QVariantList>()){
        QVariantList list = variantItem.value<QVariantList>();

        res.append("[");
        QVariantList::const_iterator itr = list.begin();
        for (; itr != list.end(); ++itr) {
             res.append(QString(QGeoJsonPrettyPrint(*itr,indent)));
             res.append(QString(" - "));
        }
        res.append(QStringLiteral("]\n"));
    };
    if (variantItem.canConvert<QString>()){
        res.append(QStringLiteral("\""));
        res.append(variantItem.value<QString>());
        res.append(QStringLiteral("\""));
    }
    if (variantItem.canConvert<QGeoCircle>()){
            res.append(QString(QStringLiteral("<QGeoCircle>")));

       }
    if (variantItem.canConvert<QGeoPath>()){
        res.append(QString(QStringLiteral("<QGeoPath>")));

    }
    if (variantItem.canConvert<QGeoPolygon>()){
        res.append(QString(QStringLiteral("<QGeoPolygon>")));

    }
    for(i=0 ; i<indent-2 ; i++ ) res.append(QStringLiteral(" ")); res.append("}");
    return res;*/
}

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);
    if (argc < 2) {
        printf("Usage: %s file.json", argv[0]);
        return -1;
    }

    QFile loadFile(argv[1]);
    if (!loadFile.open(QIODevice::ReadOnly)) {
        qWarning() << "Couldn't open" << argv[1];
        return -2;
    }
    QFile saveFile("/home/jdotsh/Projects/mix-map-master/testdata/savedmap.txt");
    if (!saveFile.open(QIODevice::WriteOnly)) {
        qWarning() << "Couldn't open to write";
        return -3;
    }

    QJsonDocument loadDoc(QJsonDocument::fromJson(loadFile.readAll()));
    QVariantMap model = QGeoJson::importGeoJson(loadDoc);
    QVariant prettyfied = QVariant::fromValue(model);

    QTextStream out(&saveFile);

    out << QGeoJsonPrettyPrint(prettyfied);
    saveFile.close();

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("annotations", model);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
