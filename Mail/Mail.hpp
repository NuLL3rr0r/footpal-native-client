/**
 * @author  Mohammad S. Babaei <info@babaei.net>
 * @author  Mohamad mehdi Kharatizadeh <m_kharatizadeh@yahoo.com>
 */


#ifndef ERTEBAT_MAIL_HPP
#define ERTEBAT_MAIL_HPP


#include <memory>
#include <QObject>
#include "Mailbox.hpp"

namespace Ertebat {
namespace Mail {

class Mailbox;

enum class SecurityType : unsigned char {
    None = 0,
    STARTTLS = 1,
    SSL_TLS = 2
};

enum class RecipientType : unsigned char {
    To = 0,
    Cc = 1,
    Bcc = 2
};

enum class SortType : unsigned char {
    Time_Ascending = 0,
    Time_Descneding = 1,
    Default = 2
};

struct Recipient {
    Mail::RecipientType Type;
    Mail::Mailbox Mailbox;

    Recipient(Mail::RecipientType type, Mail::Mailbox const& mailbox)
        : Type(type), Mailbox(mailbox)
    {

    }
};

typedef unsigned int Port_t;
}
}


#endif /* ERTEBAT_MAIL_HPP */

