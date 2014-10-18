#include <QString>
#include "make_unique.hpp"
#include "Message.hpp"

using namespace Ertebat;
using namespace Ertebat::Mail;

struct Message::Impl
{
    Mail::Mailbox From;
    Mail::Mailbox ReplyTo;
    std::vector<Mail::Recipient> Recipients;

    QString Subject;
    QString PlainBody;
    QString HtmlBody;

    std::vector<QString> Attachments;
};

Message::Message() :
    m_pimpl(std::make_unique<Message::Impl>())
{

}

Message::~Message()
{

}

const Mail::Mailbox &Message::GetFrom() const
{
    return m_pimpl->From;
}

void Message::SetFrom(const Mail::Mailbox &from)
{
    m_pimpl->From = from;
}

const Mail::Mailbox &Message::GetReplyTo() const
{
    return m_pimpl->ReplyTo;
}

void Message::SetReplyTo(const Mail::Mailbox &replyTo)
{
    m_pimpl->ReplyTo = replyTo;
}

const std::vector<Mail::Recipient> &Message::GetRecipients() const
{
    return m_pimpl->Recipients;
}

void Message::ClearRecipients()
{
    m_pimpl->Recipients.clear();
}

void Message::AddRecipient(const Mail::RecipientType &type, const Mail::Mailbox &recipient)
{
    Recipient r;
    r.Type = type;
    r.Mailbox = recipient;
    m_pimpl->Recipients.push_back(r);
}

const QString &Message::GetSubject() const
{
    return m_pimpl->Subject;
}

void Message::SetSubject(const QString &subject)
{
    m_pimpl->Subject = subject;
}

const QString &Message::GetPlainBody() const
{
    return m_pimpl->PlainBody;
}

void Message::SetPlainBody(const QString &plainBody)
{
    m_pimpl->PlainBody = plainBody;
}

const QString &Message::GetHtmlBody() const
{
    return m_pimpl->HtmlBody;
}

void Message::SetHtmlBody(const QString &htmlBody)
{
    m_pimpl->HtmlBody = htmlBody;
}

const std::vector<QString> &Message::GetAttachments() const
{
    return m_pimpl->Attachments;
}

void Message::AddAttachment(const QString &filePath)
{
    m_pimpl->Attachments.push_back(filePath);
}

void Message::ClearAttachments()
{
    m_pimpl->Attachments.clear();
}

void Message::Clear()
{
    m_pimpl->From = Mailbox();
    m_pimpl->ReplyTo = Mailbox();
    m_pimpl->Recipients.clear();
    m_pimpl->Subject.clear();
    m_pimpl->PlainBody.clear();
    m_pimpl->HtmlBody.clear();
    m_pimpl->Attachments.clear();
}
