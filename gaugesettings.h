#ifndef GAUGESETTINGS_H
#define GAUGESETTINGS_H

#include <QObject>
#include <QtCore>
#include <QtGui>

class GaugeSettings : public QObject
{
    Q_OBJECT
public:
    explicit GaugeSettings(QObject *parent = nullptr);

    struct gaugeDef {
        float start;
        float end;
        QColor color;
    };

    std::vector<gaugeDef> definitions;
    std::vector<gaugeDef> warmupDefinitions;

    void getGaugeDef();

    void setGauge(QString gauge);

    int warmupNRange;

    QString getName() {
        return name;
    }

    int getNRange() {
        return NRange;
    }

private:
    QSettings settings;
    QString name;
    int NRange;

signals:

public slots:
};

#endif // GAUGESETTINGS_H
