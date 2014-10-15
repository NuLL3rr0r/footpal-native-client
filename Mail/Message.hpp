/**
 * @author  Mohammad S. Babaei <info@babaei.net>
 */


#ifndef ERTEBAT_MAIL_MESSAGE_HPP
#define ERTEBAT_MAIL_MESSAGE_HPP


#include <memory>
#include <QObject>
#if !defined ( Q_OS_ANDROID )
#include <vmime/vmime.hpp>
#endif // !defined ( Q_OS_ANDROID )
#include "Mail.hpp"

class QString;

namespace Ertebat {
namespace Mail {
class Mailbox;
class Message;
}
}

class Ertebat::Mail::Message
{
private:
    struct Impl;
    std::unique_ptr<Impl> m_pimpl;

public:
    Q_INVOKABLE Message();
    ~Message();

public:
    Q_INVOKABLE const Mail::Mailbox &GetFrom() const;
    Q_INVOKABLE void SetFrom(const Mail::Mailbox &from);

    Q_INVOKABLE const Mail::Mailbox &GetReplyTo() const;
    Q_INVOKABLE void SetReplyTo(const Mail::Mailbox &replyTo);

    Q_INVOKABLE const std::vector<Mail::Recipient> &GetRecipients() const;
    Q_INVOKABLE void AddRecipient(const Mail::RecipientType &type, const Mail::Mailbox &recipient);
    Q_INVOKABLE void ClearRecipients();

    Q_INVOKABLE const QString &GetSubject() const;
    Q_INVOKABLE void SetSubject(const QString &subject);

    Q_INVOKABLE const QString &GetPlainBody() const;
    Q_INVOKABLE void SetPlainBody(const QString &plainBody);

    Q_INVOKABLE const QString &GetHtmlBody() const;
    Q_INVOKABLE void SetHtmlBody(const QString &htmlBody);

    Q_INVOKABLE const std::vector<QString> &GetAttachments() const;
    Q_INVOKABLE void AddAttachment(const QString &filePath);
    Q_INVOKABLE void ClearAttachments();

    Q_INVOKABLE void Clear();
};


#endif /* ERTEBAT_MAIL_MESSAGE_HPP */

