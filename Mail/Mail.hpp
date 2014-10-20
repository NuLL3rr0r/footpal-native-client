/**
 * @author  Mohammad S. Babaei <info@babaei.net>
 */


#ifndef ERTEBAT_MAIL_HPP
#define ERTEBAT_MAIL_HPP


#include <memory>
#include <QObject>

namespace Ertebat {
namespace Mail {

class Mailbox;

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

enum class SortType : unsigned char {
    Time_Ascending,
    Time_Descneding,
    Default
};

struct Recipient {
    Mail::RecipientType Type;
    std::reference_wrapper<const Mail::Mailbox> Mailbox;
    Recipient(Mail::RecipientType type, Mail::Mailbox const& mailbox)
        : Type(type), Mailbox(mailbox)
    {

    }
};

typedef unsigned short Port_t;
}
}


#endif /* ERTEBAT_MAIL_HPP */

