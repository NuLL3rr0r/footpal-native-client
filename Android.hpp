#ifndef FOOTPAL_ANDROID_HPP
#define FOOTPAL_ANDROID_HPP


#include <string>
#include <jni.h>
#include <QtAndroidExtras/QAndroidJniObject>

namespace Footpal {
class Android;
}

class Footpal::Android : public QAndroidJniObject
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
    bool Notify(const QString &title, const QString &text, const int id = 0);
    bool ShowToast(const QString &text, const int duration = 8000);
};


#endif /* FOOTPAL_ANDROID_HPP */
