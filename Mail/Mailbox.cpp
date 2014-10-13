#include <QString>
#include "make_unique.hpp"
#include "Mailbox.hpp"

using namespace Ertebat;
using namespace Ertebat::Mail;

struct Mailbox::Impl
{
    QString Name;
    QString Address;
};

Mailbox::Mailbox() :
    m_pimpl(std::make_unique<Mailbox::Impl>())
{

}

Mailbox::Mailbox(const QString &address) :
    m_pimpl(std::make_unique<Mailbox::Impl>())
{
    m_pimpl->Address = address;
}

Mailbox::Mailbox(const QString &address, const QString &name) :
    m_pimpl(std::make_unique<Mailbox::Impl>())
{
    m_pimpl->Name = name;
    m_pimpl->Address = address;
}

Mailbox::~Mailbox()
{

}

bool Mailbox::IsEmpty() const
{
    if (m_pimpl->Address != "") {
        return false;
    }

    return true;
}

const QString &Mailbox::GetName() const
{
    return m_pimpl->Name;
}

const QString &Mailbox::GetAddress() const
{
    return m_pimpl->Address;
}

