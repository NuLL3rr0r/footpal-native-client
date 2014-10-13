/**
 * @author  Mohammad S. Babaei <info@babaei.net>
 */


#ifndef ERTEBAT_MAIL_HPP
#define ERTEBAT_MAIL_HPP


#include <memory>
#include <QObject>
#include "Mailbox.hpp"

namespace Ertebat {
namespace Mail {

enum class SecurityType : unsigned char {
    None,
    STARTTLS,
    SSL_TLS
};

enum class RecipientType : unsigned char {
    To,
    Cc,
    Bcc
};

struct Recipient {
    Mail::RecipientType Type;
    Mail::Mailbox Mailbox;
};

typedef unsigned short Port_t;
}
}


#endif /* ERTEBAT_MAIL_HPP */

