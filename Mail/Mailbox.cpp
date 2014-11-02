/**
 * @author  Mohammad S. Babaei <info@babaei.net>
 * @author  Mohamad mehdi Kharatizadeh <m_kharatizadeh@yahoo.com>
 */


#include <QString>
#include "make_unique.hpp"
#include "Mailbox.hpp"

using namespace Ertebat;
using namespace Ertebat::Mail;

struct Mailbox::Impl
{
    QString Name;
    QString Address;

    Impl() {

    }
};

Mailbox::Mailbox() :
    m_pimpl(Pimple_Type(new Mailbox::Impl()))
{

}

Mailbox::Mailbox(const QString &address) :
    m_pimpl(Pimple_Type(new Mailbox::Impl()))
{
    m_pimpl->Address = address;
}

Mailbox::Mailbox(const QString &address, const QString &name) :
    m_pimpl(Pimple_Type(new Mailbox::Impl()))
{
    m_pimpl->Name = name;
    m_pimpl->Address = address;
}

Mailbox::~Mailbox()
{

}

Mailbox::Mailbox(Mailbox const& x)
    : QObject(), m_pimpl(x.m_pimpl)
{
}

Mailbox::Mailbox(Mailbox&& x)
    : QObject(), m_pimpl(std::move(x.m_pimpl))
{

}

Mailbox& Mailbox::operator = (Mailbox const& x) {
    m_pimpl = x.m_pimpl;
    return *this;
}

Mailbox& Mailbox::operator = (Mailbox&& x) {
    m_pimpl = std::move(x.m_pimpl);
    return *this;
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

