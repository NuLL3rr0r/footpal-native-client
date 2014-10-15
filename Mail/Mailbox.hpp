/**
 * @author  Mohammad S. Babaei <info@babaei.net>
 */


#ifndef ERTEBAT_MAIL_MAILBOX_HPP
#define ERTEBAT_MAIL_MAILBOX_HPP


#include <memory>
#include <QObject>

class QString;

namespace Ertebat {
namespace Mail {
class Mailbox;
}
}

class Ertebat::Mail::Mailbox
{
private:
    struct Impl;
    std::shared_ptr<Impl> m_pimpl;

public:
    Q_INVOKABLE Mailbox();
    Q_INVOKABLE Mailbox(const QString &address);
    Q_INVOKABLE Mailbox(const QString &address, const QString &name);
    ~Mailbox();

public:
    Q_INVOKABLE bool IsEmpty() const;
    Q_INVOKABLE const QString &GetName() const;
    Q_INVOKABLE const QString &GetAddress() const;
};


#endif /* ERTEBAT_MAIL_MAILBOX_HPP */

