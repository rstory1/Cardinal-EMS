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
        int start;
        int end;
        QColor color;
    };

    std::vector<gaugeDef> definitions;
    std::vector<gaugeDef> warmupDefinitions;

    void getGaugeDef();

    void setGauge(QString gauge);

    int NRange;
    int warmupNRange;

private:
    QSettings settings;
    void addDef(std::vector<gaugeDef>& definitions, int start, int end, QColor color);
    QString name;

signals:

public slots:
};

#endif // GAUGESETTINGS_H
