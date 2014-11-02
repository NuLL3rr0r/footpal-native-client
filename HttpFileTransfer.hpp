/**
 * @author  Mohammad S. Babaei <info@babaei.net>
 */


#ifndef ERTEBAT_HTTP_FILE_TRANSFER_HPP
#define ERTEBAT_HTTP_FILE_TRANSFER_HPP


#include <memory>
#include <QtCore/QObject>
class QString;
namespace Ertebat {
class HttpFileTransfer;
}

class Ertebat::HttpFileTransfer : public QObject
{
    Q_OBJECT

private:
    class Impl;
    std::unique_ptr<Impl> m_pimpl;

public:
    Q_INVOKABLE HttpFileTransfer();
    virtual ~HttpFileTransfer();

signals:
    void Signal_Failed();
    void Signal_ReadyRead();
    void Signal_DownloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void Signal_Finished();

public:
    Q_INVOKABLE void Download(const QString &url, const QString &localPath, const QString &localFileName);
};


#endif /* ERTEBAT_HTTP_FILE_TRANSFER_HPP */

