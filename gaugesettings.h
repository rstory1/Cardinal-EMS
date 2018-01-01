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

    double getMin() {
        return min;
    }

    double getMax() {
        return max;
    }

private:
    QSettings settings;
    QString name;
    int NRange;

    double min;
    double max;

    double egtMin = 0;
    double egtMax = 0;

signals:

public slots:
};

#endif // GAUGESETTINGS_H
