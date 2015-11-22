#ifndef TUNEPUSHER_H
#define TUNEPUSHER_H

#include <QObject>

class QTemporaryFile;

class TunePusher : public QObject
{
    Q_OBJECT
public:
    explicit TunePusher(QObject *parent = 0);
    virtual ~TunePusher();

signals:
    void abcUpdated(QString abc);

public slots:
    void updateABC(QString abc);
    void requestABC();

private:
    QString m_abc;
    QTemporaryFile *m_inputFile;
    QTemporaryFile *m_outputFile;
    QString m_inputFileName;
    QString m_outputFileName;
};

#endif // TUNEPUSHER_H
