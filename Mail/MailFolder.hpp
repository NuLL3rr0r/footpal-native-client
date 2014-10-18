/**
 * @author M. Kharatizadeh  <m_kharatizadeh@yahoo.com>
 */


#ifndef ERTEBAT_MAIL_MAILFOLDER_H
#define ERTEBAT_MAIL_MAILFOLDER_H


#include <QObject>
#include "Mail.hpp"


namespace Ertebat { namespace Mail {

    class Mailbox;

    class MailFolder : public QObject {
        Q_OBJECT

    public:

        Q_INVOKABLE QString const& GetFolder() const;

    };


} }




#endif // MAILFOLDERSTORAGE_H
