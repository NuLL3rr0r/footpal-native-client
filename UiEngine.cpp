#include <QtCore/QDebug>
#include <QtCore/QVariant>
#include <QtQml/QQmlContext>
#include <QtWidgets/QSystemTrayIcon>
#if defined ( _WIN32 )
#include <windows.h>
#endif  // defined ( _WIN32 )
#include "make_unique.hpp"
#include "uiengine.hpp"
#if defined ( Q_OS_ANDROID )
#include "Android.hpp"
#endif // defined ( Q_OS_ANDROID )
#include "Pool.hpp"
#include "RestApi.hpp"

using namespace Footpal;

struct UiEngine::Impl
{
public:
#if !defined ( Q_OS_ANDROID )
    typedef std::unique_ptr<QSystemTrayIcon> QSystemTrayIcon_t;
#endif // defined ( Q_OS_ANDROID )

private:
    UiEngine *m_parent;

public:
#if !defined ( Q_OS_ANDROID )
    QSystemTrayIcon_t Tray;
#endif // !defined ( Q_OS_ANDROID )

public:
    Impl(UiEngine *parent);
    ~Impl();

public:
    void Initialize();
    void InitializeEvents();
};

UiEngine::UiEngine(QObject *parent) :
    QQmlApplicationEngine(parent),
    m_pimpl(std::make_unique<UiEngine::Impl>(this))
{
    m_pimpl->Initialize();
}

UiEngine::UiEngine(const QUrl &url, QObject *parent) :
    QQmlApplicationEngine(url, parent),
    m_pimpl(std::make_unique<UiEngine::Impl>(this))
{
    m_pimpl->Initialize();
}

UiEngine::UiEngine(const QString &filePath, QObject *parent) :
    QQmlApplicationEngine(filePath, parent),
    m_pimpl(std::make_unique<UiEngine::Impl>(this))
{
    m_pimpl->Initialize();
}

UiEngine::~UiEngine() = default;

QString UiEngine::GetEmptyLangString() const
{
    return "";
}

bool UiEngine::notify(const QString &title, const QString &text, const int id) const
{
#if defined ( Q_OS_ANDROID )
    return Pool::Android()->Notify(title, text, id);
#else
    if (m_pimpl->Tray->isSystemTrayAvailable() || m_pimpl->Tray->supportsMessages()) {
        (void)id;
        m_pimpl->Tray->setVisible(true);
        m_pimpl->Tray->show();
        m_pimpl->Tray->showMessage(title, text);
    } else {
        qWarning() << "Notification has not been implemented on this platform, yet!";
        qDebug() << title << text << id;
    }
    return true;
#endif // defined ( Q_OS_ANDROID )
}

#if defined ( Q_OS_ANDROID )
bool UiEngine::showToast(const QString &text, const int duration) const
#else
bool UiEngine::showToast(const QString &text, const int duration)
#endif // defined ( Q_OS_ANDROID )
{
#if defined ( Q_OS_ANDROID )
    return Pool::Android()->ShowToast(text, duration);
#else
    QVariant returnedValue;
    QVariant varText = text;
    QVariant varDuration = duration;
    QMetaObject::invokeMethod(this->rootObjects().first(), "showToast",
                              Q_RETURN_ARG(QVariant, returnedValue),
                              Q_ARG(QVariant, text),
                              Q_ARG(QVariant, duration));
    return true;
#endif // defined ( Q_OS_ANDROID )
}

UiEngine::Impl::Impl(UiEngine *parent) :
    m_parent(parent)
  #if !defined ( Q_OS_ANDROID )
  ,
    Tray(std::make_unique<QSystemTrayIcon>())
  #endif // defined ( Q_OS_ANDROID )
{
#if !defined ( Q_OS_ANDROID )
    Tray->setIcon(QIcon::fromTheme(""));
#endif // !defined ( Q_OS_ANDROID )
}

UiEngine::Impl::~Impl()
{
#if !defined ( Q_OS_ANDROID )
    Tray->setVisible(false);
    Tray->hide();
#endif // !defined ( Q_OS_ANDROID )
}

void UiEngine::Impl::Initialize()
{
    QQmlContext *context = m_parent->rootContext();
    context->setContextProperty("UiEngine", m_parent);
    context->setContextProperty("RestApi", Pool::RestApi());
    context->setContextProperty("FontPath", "qrc:///fnt/main.ttf");

    InitializeEvents();
}

void UiEngine::Impl::InitializeEvents()
{

}

