/**
 * @author  Mohammad S. Babaei <info@babaei.net>
 */


#include <boost/algorithm/string.hpp>
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
#include "Http.hpp"

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

    HttpFileTransferOperations::Operation CurrentOperation;

private:
    HttpFileTransfer *m_parent;

public:
    Impl(HttpFileTransfer *parent);

public slots:
    void DownloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void UploadProgress(qint64 bytesReceived, qint64 bytesTotal);
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
    m_pimpl->CurrentOperation = HttpFileTransferOperations::None;
}

HttpFileTransfer::~HttpFileTransfer() = default;

void HttpFileTransfer::download(const QString &url, const QString &localPath, const QString &localFileName)
{
    m_pimpl->CurrentOperation = HttpFileTransferOperations::Download;

    m_pimpl->LocalFile.reset(new QFile(QDir(localPath).filePath(localFileName)));

    if (!m_pimpl->IsConnected()) {
        emit signal_Failed(m_pimpl->CurrentOperation, m_pimpl->LocalFile->fileName());
        return;
    }

    if (m_pimpl->LocalFile->open(QIODevice::WriteOnly)) {
        m_pimpl->NetworkAccessManager.reset(new QNetworkAccessManager());
        QNetworkRequest request { QUrl(url) };
        QSslConfiguration sslConfiguration(QSslConfiguration::defaultConfiguration());
        request.setSslConfiguration(sslConfiguration);
        m_pimpl->NetworkReply = m_pimpl->NetworkAccessManager->get(request);
        m_pimpl->SetupEvents(m_pimpl->NetworkReply);
    } else {
        emit signal_Failed(m_pimpl->CurrentOperation, m_pimpl->LocalFile->fileName());
    }
}

void HttpFileTransfer::upload(const QString &token, const QString &parentId, const QString &access, const QString &localFilePath)
{
    m_pimpl->CurrentOperation = HttpFileTransferOperations::Upload;

    if (!m_pimpl->IsConnected()) {
        emit signal_Failed(m_pimpl->CurrentOperation, m_pimpl->LocalFile->fileName());
        return;
    }

    m_pimpl->LocalFile.reset(new QFile(localFilePath));

    if (m_pimpl->LocalFile->exists() && m_pimpl->LocalFile->open(QIODevice::ReadOnly)) {
        QString boundary="----ERTEBAT";

        Http::Headers_t headers;
        headers["token"] = token;
        headers["Content-Type"] = QString("multipart/form-data; boundary=%1").arg(boundary);

        QByteArray data;
        QByteArray fileData;
        fileData.append(m_pimpl->LocalFile->readAll());

        data.append(QString("\r\n--%1\r\n").arg(boundary).toLatin1());
        data.append(QString("Content-Disposition: form-data; name=\"file\"; filename=\"%1\"\r\n").arg(m_pimpl->LocalFile->fileName()));
        data.append("Content-Type: application/octet-stream\r\n\r\n");
        data.append(fileData);
        data.append("\r\n--" + boundary + "--\r\n");

        m_pimpl->NetworkAccessManager.reset(new QNetworkAccessManager());
        QNetworkRequest request { QUrl(QString("%1/document/uploadFile/%2/%3").arg(REST_BASE_URL).arg(parentId).arg(access)) };
        request.setRawHeader("Accept", "application/json");
        request.setRawHeader("Accept-Charset", "utf-8");
        request.setRawHeader("Content-Type", "application/json");
        for (Http::Headers_t::const_iterator it= headers.cbegin();
             it != headers.cend(); ++it) {
            request.setRawHeader(it->first.toUtf8(), it->second.toUtf8());
        }
        QSslConfiguration sslConfiguration(QSslConfiguration::defaultConfiguration());
        request.setSslConfiguration(sslConfiguration);
        m_pimpl->NetworkReply = m_pimpl->NetworkAccessManager->post(request, data);
        m_pimpl->SetupEvents(m_pimpl->NetworkReply);
    } else {
        emit signal_Failed(m_pimpl->CurrentOperation, m_pimpl->LocalFile->fileName());
    }
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
    emit m_parent->signal_Progress(CurrentOperation, LocalFile->fileName(), bytesReceived, bytesTotal);
}

void HttpFileTransfer::Impl::UploadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    emit m_parent->signal_Progress(CurrentOperation, LocalFile->fileName(), bytesReceived, bytesTotal);
}

void HttpFileTransfer::Impl::Error(QNetworkReply::NetworkError code)
{
    (void)code;
    if (LocalFile->isOpen()) {
        LocalFile->close();
    }
    emit m_parent->signal_Failed(CurrentOperation, LocalFile->fileName());
    NetworkReply->deleteLater();
}

void HttpFileTransfer::Impl::Finished()
{
    if (LocalFile->isOpen()) {
        if (CurrentOperation == HttpFileTransferOperations::Download) {
            LocalFile->flush();
        }
        LocalFile->close();
    }
    emit m_parent->signal_Finished(CurrentOperation, LocalFile->fileName());
    NetworkReply->deleteLater();
}

void HttpFileTransfer::Impl::ReadyRead()
{
    emit m_parent->signal_ReadyRead(CurrentOperation, LocalFile->fileName());
    if (CurrentOperation == HttpFileTransferOperations::Download) {
        LocalFile->write(NetworkReply->readAll());
    }
}

void HttpFileTransfer::Impl::SslErrors(const QList<QSslError> &errors)
{
    if (LocalFile->isOpen()) {
        LocalFile->close();
    }
    (void)errors;
    emit m_parent->signal_Failed(CurrentOperation, LocalFile->fileName());
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
    connect(networkReply, SIGNAL(uploadProgress(qint64, qint64)),
            this, SLOT(UploadProgress(qint64, qint64)));
    connect(networkReply, SIGNAL(error(QNetworkReply::NetworkError)),
            this, SLOT(Error(QNetworkReply::NetworkError)));
    connect(networkReply, SIGNAL(finished()),
            this, SLOT(Finished()));
    connect(networkReply, SIGNAL(readyRead()),
            this, SLOT(ReadyRead()));
    connect(networkReply, SIGNAL(sslErrors(const QList<QSslError> &)),
            this, SLOT(SslErrors(const QList<QSslError> &)));
}


