#include "tunepusher.h"

TunePusher::TunePusher(QObject *parent) : QObject(parent)
{

}

void TunePusher::updateABC(QString abc)
{
    if (m_abc != abc)
    {
        m_abc = abc;
        emit abcUpdated(abc);
    }
}

void TunePusher::requestABC()
{
    emit abcUpdated(m_abc);
}
