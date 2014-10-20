/**
 * @author  Mohammad S. Babaei <info@babaei.net>
 */


#ifndef ERTEBAT_MAIL_MAILBOX_HPP
#define ERTEBAT_MAIL_MAILBOX_HPP

#include <QObject>
#include <memory>
#include "MailFolder.hpp"

class QString;

namespace Ertebat {
namespace Mail {
class Mailbox;
class MailFolder;
class Message;
}
}

class Ertebat::Mail::Mailbox : public QObject
{
    Q_OBJECT

private:
    struct Impl;

    typedef std::shared_ptr<Impl> Pimple_Type;

    Pimple_Type m_pimpl;

public:
    Q_INVOKABLE Mailbox();
    Q_INVOKABLE Mailbox(const QString &address);
    Q_INVOKABLE Mailbox(const QString &address, const QString &name);

    Mailbox(Mailbox const& x);
    Mailbox(Mailbox&& x);

    Mailbox& operator = (Mailbox const& x);
    Mailbox& operator = (Mailbox&& x);

    virtual ~Mailbox();

public:
    Q_INVOKABLE bool IsEmpty() const;
    Q_INVOKABLE const QString &GetName() const;
    Q_INVOKABLE const QString &GetAddress() const;

    /*
    Q_INVOKABLE std::size_t GetFolderCount() const;
    Q_INVOKABLE MailFolder& GetFolder(std::size_t i);
    Q_INVOKABLE void Send(Message const& msg);
    */
};


#endif /* ERTEBAT_MAIL_MAILBOX_HPP */

