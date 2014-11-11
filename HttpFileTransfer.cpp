#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QStandardPaths>
#include <QtCore/QString>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkConfigurationManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>
#include "make_unique.hpp"
#include "HttpFileTransfer.hpp"

using namespace std;
using namespace Ertebat;

class HttpFileTransfer::Impl : public QObject
{
    Q_OBJECT

public:
    typedef std::unique_ptr<QNetworkAccessManager> QNetworkAccessManager_t;
    typedef std::unique_ptr<QFile> QFile_t;

public:
    QNetworkAccessManager_t NetworkAccessManager;
    QNetworkReply *NetworkReply;

    QFile_t LocalFile;

private:
    HttpFileTransfer *m_parent;

public:
    Impl(HttpFileTransfer *parent);

public slots:
    void DownloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void Error(QNetworkReply::NetworkError code);
    void Finished();
    void ReadyRead();
    void SslErrors(const QList<QSslError> &errors);

public:
    bool IsConnected();
    void SetupEvents(QNetworkReply *NetworkReply);
};

#include "HttpFileTransfer.moc" // this is a necessitas to QObject-based-Impl

HttpFileTransfer::HttpFileTransfer()
    : QObject(),
      m_pimpl(std::make_unique<HttpFileTransfer::Impl>(this))
{

}

HttpFileTransfer::~HttpFileTransfer() = default;

void HttpFileTransfer::download(const QString &url, const QString &localPath, const QString &localFileName)
{
    if (!m_pimpl->IsConnected()) {
        emit signal_Failed(m_pimpl->LocalFile->fileName());
        return;
    }

    m_pimpl->LocalFile.reset(new QFile(QDir(localPath).filePath(localFileName)));
    m_pimpl->LocalFile->open(QIODevice::WriteOnly);
    m_pimpl->NetworkAccessManager.reset(new QNetworkAccessManager());
    QNetworkRequest request { QUrl(url) };
    m_pimpl->NetworkReply = m_pimpl->NetworkAccessManager->get(request);
    m_pimpl->SetupEvents(m_pimpl->NetworkReply);
}

const QString HttpFileTransfer::getDownloadServerUrl()
{
    return REST_BASE_URL;
}

const QString HttpFileTransfer::getLocalDownloadFolderPath()
{
    return QStandardPaths::locate(QStandardPaths::DownloadLocation, QString(), QStandardPaths::LocateDirectory);
}

HttpFileTransfer::Impl::Impl(HttpFileTransfer *parent) :
    m_parent(parent)
{

}

void HttpFileTransfer::Impl::DownloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    emit m_parent->signal_DownloadProgress(LocalFile->fileName(), bytesReceived, bytesTotal);
}

void HttpFileTransfer::Impl::Error(QNetworkReply::NetworkError code)
{
    (void)code;
    if (LocalFile->isOpen()) {
        LocalFile->close();
    }
    emit m_parent->signal_Failed(LocalFile->fileName());
    NetworkReply->deleteLater();
}

void HttpFileTransfer::Impl::Finished()
{
    if (LocalFile->isOpen()) {
        LocalFile->flush();
        LocalFile->close();
    }
    emit m_parent->signal_Finished(LocalFile->fileName());
    NetworkReply->deleteLater();
}

void HttpFileTransfer::Impl::ReadyRead()
{
    emit m_parent->signal_ReadyRead(LocalFile->fileName());
    LocalFile->write(NetworkReply->readAll());
}

void HttpFileTransfer::Impl::SslErrors(const QList<QSslError> &errors)
{
    if (LocalFile->isOpen()) {
        LocalFile->close();
    }
    (void)errors;
    emit m_parent->signal_Failed(LocalFile->fileName());
    NetworkReply->deleteLater();
}

bool HttpFileTransfer::Impl::IsConnected()
{
    static QNetworkConfigurationManager configurationManager;

    if (!configurationManager.isOnline()) {
        return false;
    }

    return true;
}

void HttpFileTransfer::Impl::SetupEvents(QNetworkReply *networkReply)
{
    connect(networkReply, SIGNAL(downloadProgress(qint64, qint64)),
            this, SLOT(DownloadProgress(qint64, qint64)));
    connect(networkReply, SIGNAL(error(QNetworkReply::NetworkError)),
            this, SLOT(Error(QNetworkReply::NetworkError)));
    connect(networkReply, SIGNAL(finished()),
            this, SLOT(Finished()));
    connect(networkReply, SIGNAL(readyRead()),
            this, SLOT(ReadyRead()));
    connect(networkReply, SIGNAL(sslErrors(const QList<QSslError> &)),
            this, SLOT(SslErrors(const QList<QSslError> &)));
}


