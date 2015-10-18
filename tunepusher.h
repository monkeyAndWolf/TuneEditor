#ifndef TUNEPUSHER_H
#define TUNEPUSHER_H

#include <QObject>

class TunePusher : public QObject
{
    Q_OBJECT
public:
    explicit TunePusher(QObject *parent = 0);

signals:
    void abcUpdated(QString abc);

public slots:
    void updateABC(QString abc);
    void requestABC();

private:
    QString m_abc;
};

#endif // TUNEPUSHER_H
