#include "tunepusher.h"

#include "abcshots.h"

#include <QDebug>
#include <QFileInfo>
#include <QTemporaryFile>

TunePusher::TunePusher(QObject *parent) : QObject(parent)
{
    m_outputFile = new QTemporaryFile();
    m_outputFile->open();
    m_outputFileName = m_outputFile->fileName();
    qDebug() << "Created a file called" << m_inputFileName;
    m_outputFile->close();

    m_inputFile = new QTemporaryFile();
    m_inputFile->open();
    m_inputFileName = m_inputFile->fileName();
    m_inputFile->close();
}

TunePusher::~TunePusher()
{
    delete (m_outputFile);
}

void TunePusher::updateABC(QString abc)
{
    if (m_abc != abc)
    {
        m_abc = abc;

        if (m_inputFile->open())
        {
            QTextStream str(m_inputFile);
            str << m_abc;
            m_inputFile->close();
        }
        runAbcToSvg(m_inputFileName.toLatin1().data(), m_outputFileName.toLatin1().data());
        int dot = m_inputFileName.indexOf(".");
        qDebug() << dot;
        dot = m_inputFileName.length() - dot;
        qDebug() << dot;
        m_inputFileName.chop(dot);
        m_inputFileName += ".svg";
        qDebug() << m_inputFileName;
        emit abcUpdated(m_inputFileName);
    }
}

void TunePusher::requestABC()
{
    emit abcUpdated(m_abc);
}
