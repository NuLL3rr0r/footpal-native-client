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
    void signal_Failed();
    void signal_ReadyRead();
    void signal_DownloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void signal_Finished();

public:
    Q_INVOKABLE void download(const QString &url, const QString &localPath, const QString &localFileName);
};


#endif /* ERTEBAT_HTTP_FILE_TRANSFER_HPP */

