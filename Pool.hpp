/**
 * @author  Mohammad S. Babaei <info@babaei.net>
 * @author  Mohamad mehdi Kharatizadeh <m_kharatizadeh@yahoo.com>
 */

#ifndef ERTEBAT_POOL_HPP
#define ERTEBAT_POOL_HPP


#include <QtCore/QObject>
#include <memory>
#include "Mail/ImapClient.hpp"
#include "Mail/Pop3Client.hpp"
#include "Mail/SmtpClient.hpp"

class QTranslator;

namespace Ertebat {
#if defined(Q_OS_ANDROID)
class Android;
#endif /* defined(Q_OS_ANDROID) */
class Crypto;
class Database;
class Pool;
class RestApi;
}

namespace Ertebat {
namespace Mail {
class Mailbox;
}
}

class Ertebat::Pool
{
private:
    struct Impl;
    struct StorageStruct
    {
    };

public:
    static StorageStruct *Storage();
#if defined(Q_OS_ANDROID)
    static Ertebat::Android *Android();
#endif /* defined(Q_OS_ANDROID) */
    static Ertebat::Crypto *Crypto();
    static Ertebat::Database *Database();
    static Ertebat::RestApi *RestApi();
    static QTranslator *Translator();
    static Ertebat::Mail::SmtpClient* SmtpClient();
    static Ertebat::Mail::Pop3Client* Pop3Client();
    static Ertebat::Mail::ImapClient* ImapClient();
};


#endif /* ERTEBAT_POOL_HPP */

