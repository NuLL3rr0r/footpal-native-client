#include <mutex>
#include <cassert>
#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtCore/QTranslator>
#include "make_unique.hpp"
#include "Pool.hpp"
#if defined ( Q_OS_ANDROID )
#include "Android.hpp"
#endif // defined ( Q_OS_ANDROID )
#include "Database.hpp"

#if defined ( _WIN32 )
#define     DATABASE_RELATIVE_PATH          "resources\\db"
#else
#define     DATABASE_RELATIVE_PATH          "resources/db"
#endif // defined ( _WIN32 )
#define     DATABASE_FILE_NAME              "database.db"

using namespace std;
using namespace Footpal;

struct Pool::Impl
{
    typedef std::unique_ptr<Pool::StorageStruct> Storage_t;
#if defined ( Q_OS_ANDROID )
    typedef std::unique_ptr<Footpal::Android> Android_t;
#endif // defined ( Q_OS_ANDROID )
    typedef std::unique_ptr<Footpal::Database> Database_t;
    typedef std::unique_ptr<QTranslator> QTranslator_t;

    static std::mutex StorageMutex;
    static Storage_t StorageInstance;

#if defined ( Q_OS_ANDROID )
    static std::mutex AndroidMutex;
    static Android_t AndroidInstance;
#endif // defined ( Q_OS_ANDROID )

    static std::mutex DatabaseMutex;
    static Database_t DatabaseInstance;

    static std::mutex TranslatorMutex;
    static QTranslator_t TranslatorInstance;
};

std::mutex Pool::Impl::StorageMutex;
Pool::Impl::Storage_t Pool::Impl::StorageInstance = nullptr;

#if defined ( Q_OS_ANDROID )
std::mutex Pool::Impl::AndroidMutex;
Pool::Impl::Android_t Pool::Impl::AndroidInstance = nullptr;
#endif // defined ( Q_OS_ANDROID )

std::mutex Pool::Impl::DatabaseMutex;
Pool::Impl::Database_t Pool::Impl::DatabaseInstance = nullptr;

std::mutex Pool::Impl::TranslatorMutex;
Pool::Impl::QTranslator_t Pool::Impl::TranslatorInstance = nullptr;

Pool::StorageStruct *Pool::Storage()
{
    lock_guard<mutex> lock(Impl::StorageMutex);
    (void)lock;

    if (Impl::StorageInstance == nullptr) {
        Impl::StorageInstance =
                std::make_unique<Pool::StorageStruct>();
    }

    return Impl::StorageInstance.get();
}

#if defined ( Q_OS_ANDROID )
Footpal::Android *Pool::Android()
{
    lock_guard<mutex> lock(Impl::AndroidMutex);
    (void)lock;

    if (Impl::AndroidInstance == nullptr) {
        Impl::AndroidInstance =
                std::make_unique<Footpal::Android>();
    }

    return Impl::AndroidInstance.get();
}
#endif

Footpal::Database *Pool::Database()
{
    lock_guard<mutex> lock(Impl::DatabaseMutex);
    (void)lock;

    if (Impl::DatabaseInstance == nullptr) {
        QString dbPath(QDir::currentPath());
        if (!dbPath.endsWith(QDir::separator()))
            dbPath += QDir::separator();
        dbPath += DATABASE_RELATIVE_PATH;
        if (!dbPath.endsWith(QDir::separator()))
            dbPath += QDir::separator();
        QString dbFile(dbPath + DATABASE_FILE_NAME);

        if (!QFileInfo(dbFile).exists()) {
            if (!QDir(dbPath).exists()) {
                assert(QDir().mkpath(dbPath));
            }
            Database::Vacuum(dbFile.toStdString());
            assert(QFile::setPermissions(dbFile, QFile::ReadOwner | QFile::WriteOwner));
        }

#if defined ( STATIC_LINK_DEPENDENCIES )
        if (!Database::IsSQLite3DriverLoaded())
            Database::LoadSQLite3Driver();
#endif  // defined ( STATIC_LINK_DEPENDENCIES )

        Impl::DatabaseInstance =
                std::make_unique<Footpal::Database>(QDir::toNativeSeparators(dbFile).toStdString());
    }

    return Impl::DatabaseInstance.get();
}

QTranslator *Pool::Translator()
{
    lock_guard<mutex> lock(Impl::TranslatorMutex);
    (void)lock;

    if (Impl::TranslatorInstance == nullptr) {
        Impl::TranslatorInstance =
                std::make_unique<QTranslator>();
    }

    return Impl::TranslatorInstance.get();
}

