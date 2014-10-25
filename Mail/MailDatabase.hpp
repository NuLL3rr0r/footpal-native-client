/**
 * @author  Mohamad mehdi Kharatizadeh <m_kharatizadeh@yahoo.com>
 */

#ifndef ERTEBAT_MAIL_MAILDATABASE_HPP
#define ERTEBAT_MAIL_MAILDATABASE_HPP


#include <QObject>
#include <vector>
#include <memory>
#include "Mail.hpp"

class QObject;

namespace Ertebat { namespace Mail {


    class MailDatabase : public QObject {
        Q_OBJECT


        MailDatabase();


    public:

        class Impl;

    private:

        typedef std::shared_ptr<Impl> pimple_type;
        typedef std::unique_ptr<MailDatabase> singleton_type;

        pimple_type m_pimple;
        static singleton_type m_singleton;

    public:

        typedef std::size_t object_t;
        static const object_t nil = ((object_t) 1) - ((object_t)0);

        virtual ~MailDatabase();

        MailDatabase(MailDatabase const& x);
        MailDatabase(MailDatabase&& x);
        MailDatabase& operator = (MailDatabase const& x);
        MailDatabase& operator = (MailDatabase&& x);

        static MailDatabase& Current();

        std::vector<MailDatabase::object_t> Query(QString const& address, Mail::SortType sort = Mail::SortType::Default) const;
        std::vector<MailDatabase::object_t> Query(QString const& address, QString const& folder, Mail::SortType sort = Mail::SortType::Default) const;
        std::vector<MailDatabase::object_t> Query(QString const& address, QString const& folder, QString const& search, Mail::SortType sort = Mail::SortType::Default) const;

        QByteArray ReadObjectAttribute(object_t id, QString const& attribute) const;
        void WriteObjectAttribute(object_t id, QString const& attribute, QByteArray data);
        QString GetAddress(object_t obj) const;
        object_t AddObject(QString const& address, QString const& folder);
        bool DeleteObject(object_t obj);
        bool AddFolder(QString const& address, QString const& folder);
        bool DeleteFolder(QString const& address, QString const& folder);
        std::vector<QString> ListAllAttributes(object_t obj) const;
        std::vector<QString> ListAllAttributes() const;
        std::vector<QString> ListAllFolders(QString const& address) const;
        bool AddAddress(QString const& address);
        bool RemoveAddress(QString const& address);
        std::vector<QString> LitsAllAddresses() const;
        QString GetFolder(MailDatabase::object_t obj) const;
        bool SetFolder(MailDatabase::object_t obj, QString const& folder);
        QString GetSender(MailDatabase::object_t obj) const;
        void SetSender(MailDatabase::object_t obj, QString const& sender);
        std::vector<QString> GetReceipientList(MailDatabase::object_t obj) const;
        bool RemoveReceipient(MailDatabase::object_t obj, QString const& receipient);
        void SetReceipientList(MailDatabase::object_t obj, std::vector<QString> const& list);
        bool AddReceipient(MailDatabase::object_t obj, QString const& receipient);
        QString GetSubject(MailDatabase::object_t obj) const;
        void SetSubject(MailDatabase::object_t obj, QString const& subject);
        QString GetPlainBody(MailDatabase::object_t obj) const;
        void SetPlainBody(MailDatabase::object_t obj, QString const& body);
        QString GetHtmlBody(MailDatabase::object_t obj) const;
        void SetHtmlBody(MailDatabase::object_t obj, QString const& body);
        QDateTime GetTimestamp(MailDatabase::object_t obj) const;
        void SetTimestamp(MailDatabase::object_t obj, QDateTime t);
        std::vector<QString> GetAttachments(MailDatabase::object_t obj);
        void SetAttachments(MailDatabase::object_t obj, std::vector<QString> list);
        bool RemoveAttachment(MailDatabase::object_t obj, QString const& attach);
        bool AddAttachment(MailDatabase::object_t obj, QString const& attach);

    };

} }


#endif // MAILDATABASE_HPP
