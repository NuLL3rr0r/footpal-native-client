/**
 * @author  Mohammad S. Babaei <info@babaei.net>
 * @author  Mohamad mehdi Kharatizadeh <m_kharatizadeh@yahoo.com>
 */


#include <QtCore/QDebug>
#include <QtCore/QVariant>
#include <QtQml/QQmlContext>
#include <QtQml/QtQml>
#include <QtWidgets/QSystemTrayIcon>
#if defined ( _WIN32 )
#include <windows.h>
#endif  // defined ( _WIN32 )
#include "Mail/Mail.hpp"
#include "Mail/ImapClient.hpp"
#include "Mail/Mailbox.hpp"
#include "Mail/Message.hpp"
#include "Mail/Pop3Client.hpp"
#include "Mail/SmtpClient.hpp"
#include "make_unique.hpp"
#include "uiengine.hpp"
#if defined ( Q_OS_ANDROID )
#include "Android.hpp"
#endif // defined ( Q_OS_ANDROID )
#include "HttpFileTransfer.hpp"
#include "Pool.hpp"
#include "RestApi.hpp"
#include "Mail/Mail.hpp"
#include "Mail/Client.hpp"
#include "Mail/SmtpClient.hpp"
#include "Mail/Pop3Client.hpp"
#include "Mail/ImapClient.hpp"
#include "Mail/Message.hpp"

using namespace Ertebat;

struct UiEngine::Impl
{
public:
#if !defined ( Q_OS_ANDROID )
    typedef std::unique_ptr<QSystemTrayIcon> QSystemTrayIcon_t;
#endif // !defined ( Q_OS_ANDROID )

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
#if defined ( Q_OS_ANDROID )
    void OpenFileDialogAccepted(QString path);
    void OpenFileDialogRejected();
#endif // defined ( Q_OS_ANDROID )

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

Screen::Type UiEngine::GetTargetScreenType()
{
#if defined ( Q_OS_ANDROID )
    QString screenType(Pool::Android()->GetScreenType());
    if (screenType == "phone") {
        return Screen::Phone;
    } else if (screenType == "7-inch-tablet") {
        return Screen::Tablet7;
    } else if (screenType == "10-inch-tablet") {
        return Screen::Tablet10;
    }
#endif // defined ( Q_OS_ANDROID )

    return Screen::PC;
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
bool UiEngine::openFileDialog() const
#else
bool UiEngine::openFileDialog()
#endif // defined ( Q_OS_ANDROID )
{
#if defined ( Q_OS_ANDROID )
    return Pool::Android()->OpenFileDialog();
#else
    QMetaObject::invokeMethod(this->rootObjects().first(), "openFileDialog");
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

#if defined ( Q_OS_ANDROID )
void UiEngine::Impl::OpenFileDialogAccepted(QString path)
{
    QVariant varPath = path;
    QMetaObject::invokeMethod(m_parent->rootObjects().first(), "onOpenFileDialogAccepted",
                              Q_ARG(QVariant, path));
}

void UiEngine::Impl::OpenFileDialogRejected()
{
    QMetaObject::invokeMethod(m_parent->rootObjects().first(), "onOpenFileDialogRejected");
}
#endif // defined ( Q_OS_ANDROID )

void UiEngine::Impl::Initialize()
{
    qRegisterMetaType<Ertebat::RestStatusCodes::ConnectionStatus>("Ertebat::RestStatusCodes::ConnectionStatus");
    qRegisterMetaType<Ertebat::RestStatusCodes::SignUpStatus>("Ertebat::RestStatusCodes::SignUpStatus");
    qRegisterMetaType<Ertebat::RestStatusCodes::SignInStatus>("Ertebat::RestStatusCodes::SignInStatus");
    qRegisterMetaType<Ertebat::RestStatusCodes::SignOutStatus>("Ertebat::RestStatusCodes::SignOutStatus");
    qRegisterMetaType<Ertebat::RestStatusCodes::GetCurrentProfileStatus>("Ertebat::RestStatusCodes::GetCurrentProfileStatus");
    qRegisterMetaType<Ertebat::RestStatusCodes::UsersListStatus>("Ertebat::RestStatusCodes::UsersListStatus");
    qRegisterMetaType<Ertebat::RestStatusCodes::CreateIndividualRoomStatus>("Ertebat::RestStatusCodes::CreateIndividualRoomStatus");

    qRegisterMetaType<Ertebat::RestStatusCodes::FS_CreateDirectoryStatus>("Ertebat::RestStatusCodes::FS_CreateDirectoryStatus");
    qRegisterMetaType<Ertebat::RestStatusCodes::FS_GetListOfEntityStatus>("Ertebat::RestStatusCodes::FS_GetListOfEntityStatus");
    qRegisterMetaType<Ertebat::RestStatusCodes::FS_GetListOfEntityStatus>("Ertebat::RestStatusCodes::FS_GetListOfEntityStatus");
    qRegisterMetaType<Ertebat::RestStatusCodes::FS_GetAccessTypeStatus>("Ertebat::RestStatusCodes::FS_GetAccessTypeStatus");
    qRegisterMetaType<Ertebat::RestStatusCodes::FS_GetParentIdStatus>("Ertebat::RestStatusCodes::FS_GetParentIdStatus");
    qRegisterMetaType<Ertebat::RestStatusCodes::FS_MoveEntityStatus>("Ertebat::RestStatusCodes::FS_MoveEntityStatus");
    qRegisterMetaType<Ertebat::RestStatusCodes::FS_DeleteEntityStatus>("Ertebat::RestStatusCodes::FS_DeleteEntityStatus");
    qRegisterMetaType<Ertebat::RestStatusCodes::FS_DownloadStatus>("Ertebat::RestStatusCodes::FS_DownloadStatus");
    qRegisterMetaType<Ertebat::RestStatusCodes::FS_DownloadUrlStatus>("Ertebat::RestStatusCodes::FS_DownloadUrlStatus");
    qRegisterMetaType<Ertebat::RestStatusCodes::FS_UploadStatus>("Ertebat::RestStatusCodes::FS_UploadStatus");
    qRegisterMetaType<Ertebat::RestStatusCodes::FS_AddAccessRoleStatus>("Ertebat::RestStatusCodes::FS_AddAccessRoleStatus");
    qRegisterMetaType<Ertebat::RestStatusCodes::FS_RemoveAccessRoleStatus>("Ertebat::RestStatusCodes::FS_RemoveAccessRoleStatus");
    qRegisterMetaType<Ertebat::RestStatusCodes::FS_ChangeAccessTypeStatus>("Ertebat::RestStatusCodes::FS_ChangeAccessTypeStatus");
    qRegisterMetaType<Ertebat::RestStatusCodes::FS_GetSharedEntitiesStatus>("Ertebat::RestStatusCodes::FS_GetSharedEntitiesStatus");

    qRegisterMetaType<Ertebat::RestStatusCodes::Profile_UploadProfilePicStatus>("Ertebat::RestStatusCodes::Profile_UploadProfilePicStatus");
    qRegisterMetaType<Ertebat::RestStatusCodes::Profile_GetUserProfileStatus>("Ertebat::RestStatusCodes::Profile_GetUserProfileStatus");
    qRegisterMetaType<Ertebat::RestStatusCodes::Profile_SaveProfileStatus>("Ertebat::RestStatusCodes::Profile_SaveProfileStatus");

    qRegisterMetaType<Ertebat::HttpFileTransferOperations::Operation>("Ertebat::HttpFileTransferOperations::Operation");

    qmlRegisterType<Ertebat::RestStatusCodes>("RestStatusCodes", 1, 0, "RestStatusCode");
    qmlRegisterType<Ertebat::Screen>("ScreenTypes", 1, 0, "ScreenType");
    qmlRegisterType<Ertebat::HttpFileTransferOperations>("HttpFileTransferOperations", 1, 0, "HttpFileTransferOperation");

    qmlRegisterType<Ertebat::Mail::ImapClient>("ImapClient", 1, 0, "ImapClient");
    qmlRegisterType<Ertebat::Mail::Pop3Client>("Pop3Client", 1, 0, "Pop3Client");
    qmlRegisterType<Ertebat::Mail::SmtpClient>("SmtpClient", 1, 0, "SmtpClient");

    QQmlContext *context = m_parent->rootContext();
    context->setContextProperty("UiEngine", m_parent);
    context->setContextProperty("RestApi", Pool::RestApi());
    context->setContextProperty("HttpFileTransfer", Pool::HttpFileTransfer());
    context->setContextProperty("FontPath", "qrc:///fnt/main.ttf");

    context->setContextProperty("ImapClient", Pool::ImapClient());
    context->setContextProperty("Pop3Client", Pool::Pop3Client());
    context->setContextProperty("SmtpClient", Pool::SmtpClient());

    InitializeEvents();
}

void UiEngine::Impl::InitializeEvents()
{
#if defined ( Q_OS_ANDROID )
    Pool::Android()->OpenFileDialogAccepted(
                std::bind(&UiEngine::Impl::OpenFileDialogAccepted,
                          this,
                          std::placeholders::_1)
                );
    Pool::Android()->OpenFileDialogRejected(
                std::bind(&UiEngine::Impl::OpenFileDialogRejected,
                          this)
                );
#endif // defined ( Q_OS_ANDROID )
}

