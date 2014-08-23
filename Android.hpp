#ifndef ERTEBAT_ANDROID_HPP
#define ERTEBAT_ANDROID_HPP


#include <string>
#include <jni.h>
#include <QtAndroidExtras/QAndroidJniObject>

namespace Ertebat {
class Android;
}

class Ertebat::Android : public QAndroidJniObject
{
private:
    struct Impl;
    std::unique_ptr<Impl> m_pimpl;

public:
    Android();
    virtual ~Android();

public:
    bool ExceptionCheck();
    void ExceptionClear();

    void Initialize();
    void Release();

    bool IsInitialized();

public:
    QString GetScreenType();
    bool Notify(const QString &title, const QString &text, const int id = 0);
    bool ShowToast(const QString &text, const int duration = 8000);
};


#endif /* ERTEBAT_ANDROID_HPP */
