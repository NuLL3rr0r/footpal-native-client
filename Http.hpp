/**
 * @author  Mohammad S. Babaei <info@babaei.net>
 */


#ifndef ERTEBAT_HTTP_HPP
#define ERTEBAT_HTTP_HPP


#include <memory>
#include <unordered_map>
#include <QtCore/QObject>
namespace Ertebat {
class Http;
}
#include <HttpStatus.hpp>

class Ertebat::Http : public QObject
{
    Q_OBJECT

public:
    typedef std::map<QString, QString> Headers_t;

public:
    enum class Error : unsigned char {
        AuthenticationRequired = 1,
        ConnectionFailed = 2,
        ProxyAuthenticationRequired = 3,
        SslError = 4
    };

private:
    class Impl;
    std::unique_ptr<Impl> m_pimpl;

public:
    Http();
    virtual ~Http();

signals:
    void Signal_Failed(const Http::Error &);
    void Signal_Finished(const Ertebat::HttpStatus::HttpStatusCode &,
                         const QString &);

public:
    void SetRawHeader(const QString &headerName, const QString &value);

    void Delete(const QString &url,
                const Headers_t &headers = Headers_t());
    void Get(const QString &url,
             const Headers_t &headers = Headers_t());
    void Post(const QString &url, const QByteArray &data,
              const Headers_t &headers = Headers_t());
    void Put(const QString &url, const QByteArray &data,
             const Headers_t &headers = Headers_t());
};


#endif /* ERTEBAT_HTTP_HPP */

