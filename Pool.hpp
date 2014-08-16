#ifndef FOOTPAL_POOL_HPP
#define FOOTPAL_POOL_HPP


#include <QtCore/QObject>
#include <memory>

class QTranslator;

namespace Footpal {
#if defined(Q_OS_ANDROID)
class Android;
#endif /* defined(Q_OS_ANDROID) */
class Database;
class Pool;
class RestApi;
}

class Footpal::Pool
{
private:
    struct Impl;
    struct StorageStruct
    {
    };

public:
    static StorageStruct *Storage();
#if defined(Q_OS_ANDROID)
    static Footpal::Android *Android();
#endif /* defined(Q_OS_ANDROID) */
    static Footpal::Database *Database();
    static Footpal::RestApi *RestApi();
    static QTranslator *Translator();
};


#endif /* FOOTPAL_POOL_HPP */

