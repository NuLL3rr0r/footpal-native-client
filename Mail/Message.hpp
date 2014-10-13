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
    Message();
    ~Message();

public:
    const Mail::Mailbox &GetFrom() const;
    void SetFrom(const Mail::Mailbox &from);

    const Mail::Mailbox &GetReplyTo() const;
    void SetReplyTo(const Mail::Mailbox &replyTo);

    const std::vector<Mail::Recipient> &GetRecipients() const;
    void AddRecipient(const Mail::RecipientType &type, const Mail::Mailbox &recipient);
    void ClearRecipients();

    const QString &GetSubject() const;
    void SetSubject(const QString &subject);

    const QString &GetPlainBody() const;
    void SetPlainBody(const QString &plainBody);

    const QString &GetHtmlBody() const;
    void SetHtmlBody(const QString &htmlBody);

    const std::vector<QString> &GetAttachments() const;
    void AddAttachment(const QString &filePath);
    void ClearAttachments();

    void Clear();
};


#endif /* ERTEBAT_MAIL_MESSAGE_HPP */

