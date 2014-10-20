#ifndef ERTEBAT_MAIL_MAILVIEW_HPP
#define ERTEBAT_MAIL_MAILVIEW_HPP

#include <QObject>
#include <memory>
#include "Mail.hpp"

namespace Ertebat { namespace Mail {

    class Mailbox;
    class MailFolder;
    class Message;

    class MailView : public QObject {
        Q_OBJECT

        class impl;
        typedef std::shared_ptr<impl> m_pimpl;


    public:


        MailView(Mailbox& mailBox, MailFolder& mailFolder);
        MailView(MailView const& x);
        MailView(MailView&& x);
        MailView& operator = (MailView const& x);
        MailView& operator = (MailView&& x);


        SortType GetSortType() const;
        void SetSortType(SortType s);
        std::size_t GetCount() const;
        std::size_t GetViewFrom() const;
        std::size_t GetViewTo() const;
        std::size_t GetViewCount() const;
        bool ChangeView(std::size_t from, std::size_t to);
        QString const& GetSearchKeyword() const;
        void SetSearchKeyword(QString const& s);
        Message Get(size_t i);
        MailFolder& GetFolder();
        Mailbox& GetMailbox();


    signals:

        void ViewChanged();


    };

} }


#endif // MAILVIEW_H
