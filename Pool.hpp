#ifndef FOOTPAL_POOL_HPP
#define FOOTPAL_POOL_HPP


#include <QObject>
#include <memory>

namespace Footpal {
#if defined(Q_OS_ANDROID)
class Android;
#endif /* defined(Q_OS_ANDROID) */
class Database;
class Pool;
}

class Footpal::Pool
{
private:
    struct Impl;
    struct StorageStruct
    {
        const std::string DATABASE_FILE = "database.db";
    };

public:
    static StorageStruct *Storage();
#if defined(Q_OS_ANDROID)
    static Footpal::Android *Android();
#endif /* defined(Q_OS_ANDROID) */
    static Footpal::Database *Database();
};


#endif /* FOOTPAL_POOL_HPP */

