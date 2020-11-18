#include "dxfparserprocess.h"
#include <QDebug>

DxfParserProcess::DxfParserProcess(QObject *parent) : QObject(parent)
{
    m_dxfParserProcess = new QProcess(this);
    QObject::connect(m_dxfParserProcess, &QProcess::readyRead,
                    this,  &DxfParserProcess::onReadyRead);
    QObject::connect(m_dxfParserProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
                     this, &DxfParserProcess::restartProcess);
    m_dxfParserProcess->setProcessChannelMode(QProcess::MergedChannels);
    m_dxfParserProcess->setWorkingDirectory("/home/woodenprint/git/dxfio/");
    QStringList args;
    args << "run";
    m_dxfParserProcess->start("cargo", args);
    if (!m_dxfParserProcess->waitForStarted()) {
        qDebug() << "Dxfio process start fail";
    }
}

void DxfParserProcess::onReadyRead()
{
    while(m_dxfParserProcess->canReadLine()){
        emit msgFromParser(m_dxfParserProcess->readLine());
        qDebug() << "DXF output: " << m_dxfParserProcess->readLine();
    }
}

void DxfParserProcess::restartProcess()
{
    QStringList args;
    args << "run";
    m_dxfParserProcess->start("cargo", args);
    if (!m_dxfParserProcess->waitForStarted()) {
        qDebug() << "Dxfio process start fail";
    }
}
