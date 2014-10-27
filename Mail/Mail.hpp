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

class MailEnums : public QObject {

    Q_OBJECT

    Q_ENUMS( SecurityType )
    Q_ENUMS( RecipientType )
    Q_ENUMS( SortType )

public:

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

};


typedef MailEnums::SecurityType SecurityType;
typedef MailEnums::RecipientType RecipientType;
typedef MailEnums::SortType SortType;

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

