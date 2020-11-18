#ifndef DXFPARSERPROCESS_H
#define DXFPARSERPROCESS_H

#include <QProcess>

class DxfParserProcess : public QObject
{
    Q_OBJECT
public:
    explicit DxfParserProcess(QObject *parent = nullptr);

signals:
    void msgFromParser(const QString &msg);
public slots:
    void onReadyRead();
    void restartProcess();

private:
    QProcess *m_dxfParserProcess;
};

#endif // DXFPARSERPROCESS_H
