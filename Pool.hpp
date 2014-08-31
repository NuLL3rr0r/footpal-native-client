/**
 * @author  Mohammad S. Babaei <info@babaei.net>
 */


#ifndef ERTEBAT_POOL_HPP
#define ERTEBAT_POOL_HPP


#include <QtCore/QObject>
#include <memory>

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
};


#endif /* ERTEBAT_POOL_HPP */

