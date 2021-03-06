/**
 * @author  Mohammad S. Babaei <info@babaei.net>
 * @author  Mohamad mehdi Kharatizadeh <m_kharatizadeh@yahoo.com>
 */

#ifndef ERTEBAT_ANDROID_HPP
#define ERTEBAT_ANDROID_HPP


#include <functional>
#include <string>
#include <jni.h>
#include <QtAndroidExtras/QAndroidJniObject>
#include "Mail/Mail.hpp"

namespace Ertebat {
class Android;
}

class Ertebat::Android : public QAndroidJniObject
{
public:
    typedef std::function<void(QString)> OpenFileDialogAcceptedHandler_t;
    typedef std::function<void()> OpenFileDialogRejectedHandler_t;

private:
    struct Impl;
    std::unique_ptr<Impl> m_pimpl;

public:
    Android();
    virtual ~Android();

public:
    void OpenFileDialogAccepted(OpenFileDialogAcceptedHandler_t handler);
    void OpenFileDialogRejected(OpenFileDialogRejectedHandler_t handler);

public:
    bool ExceptionCheck();
    void ExceptionClear();

    void Initialize();
    void Release();

    bool IsInitialized();

public:
    QString GetScreenType();
    bool Notify(const QString &title, const QString &text, const int id = 0);
    bool OpenFileDialog();
    bool ShowToast(const QString &text, const int duration = 4000);

    void MailProfile_init();
    void MailProfile_setHost(QString const& host, QString const& protocol);
    void MailProfile_setSmtpHost(QString const& host);
    void MailProfile_setImapHost(QString const& host);
    void MailProfile_setPort(short port, QString const& protocol);
    void MailProfile_setSmtpPort(short port);
    void MailProfile_setImapPort(short port);
    void MailProfile_setUsername(QString const& username);
    void MailProfile_setUsername(QString const& username, QString const& protcol);
    void MailProfile_setSmtpUsername(QString const& username);
    void MailProfile_setImapUsername(QString const& username);
    void MailProfile_setPassword(QString const& password);
    void MailProfile_setPassword(QString const& password, QString const& protocol);
    void MailProfile_setSmtpPassword(QString const& password);
    void MailProfile_setImapPassword(QString const& password);
    void MailProfile_setSecurity(Mail::SecurityType securityType);
    void MailProfile_setSecurity(Mail::SecurityType securityType, QString const& protocol);
    void MailProfile_setSmtpSecurity(Mail::SecurityType securityType);
    void MailProfile_setImapSecurity(Mail::SecurityType securityType);
    bool MailProfile_connect(QString const& protocol);
    void MailProfile_connectSmtp();
    void MailProfile_disconnect(QString const& protocol);
    void MailProfile_disconnectSmtp();
    void MailProfile_send(QString const& jsonMessage);
    QString MailProfile_fetchMessage(int from, int count);
    std::size_t MailProfile_getMessageCount();
};


#endif /* ERTEBAT_ANDROID_HPP */
