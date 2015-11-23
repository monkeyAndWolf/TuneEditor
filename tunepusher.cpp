#include "tunepusher.h"

#include "abcshots.h"

#include <QDebug>
#include <QFileInfo>
#include <QTemporaryFile>

TunePusher::TunePusher(QObject *parent) : QObject(parent)
{

}

TunePusher::~TunePusher()
{
    qDebug() << "Tidy shut down, removing files";
    QFile f(m_actualSVGName);
    f.remove();
}

void TunePusher::updateABC(QString abc)
{
    if (m_abc != abc)
    {
        m_abc = abc;

        QTemporaryFile inputFile;
        if (inputFile.open())
        {
            QTextStream str(&inputFile);
            str << m_abc;
            inputFile.close();
        }
        // The input and output file names are the same. This is because abc2mps
        // changes the output name to end .svg anyway - so a separate output file
        // would never ever be used.
        QString inputFileName = inputFile.fileName();
        runAbcToSvg(inputFileName.toLatin1().data(), inputFileName.toLatin1().data());
        int dot = inputFileName.indexOf(".");
        dot = inputFileName.length() - dot;
        inputFileName.chop(dot);
        inputFileName += ".svg";
        if (!m_actualSVGName.isNull())
        {
            if (m_actualSVGName != inputFileName)
            {
                // Will this introduce a delay to the UI?
                QFile f(m_actualSVGName);
                f.remove();
                // Probably not, I don't think that the
                // file name changes very often.
            }
        }
        m_actualSVGName = inputFileName;
        emit abcUpdated(m_actualSVGName);
    }
}

void TunePusher::requestABC()
{
    emit abcUpdated(m_abc);
}
