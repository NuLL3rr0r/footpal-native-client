/**
 * @author  Mohamad mehdi Kharatizadeh <m_kharatizadeh@yahoo.com>
 */


#include "Client.hpp"
#include "Message.hpp"
#include "Crypto.hpp"
#include <QDebug>
#include <sstream>
#include <QRegularExpression>
#include <QDateTime>
#include <QDate>
#include <QTime>
#include <QTimeZone>
#include "Mailbox.hpp"
#include "Json.hpp"

namespace Ertebat { namespace Mail {

struct Field {
    QString Field;
    QString Encoding;
    QString Name;
    QString Content;
    QString Address;
    std::vector<std::pair<QString,QString> > List;
};

QString const&  Client::GetHost() const {
    throw std::runtime_error("NOT IMPLEMENTED");
}

void Client::SetHost(QString const& host) {
    ((void)host);
    throw std::runtime_error("NOT IMPLEMENTED");
}

Mail::Port_t const& Client::GetPort() const {
    throw std::runtime_error("NOT IMPLEMENTED");
}

void Client::SetPort(Mail::Port_t const& port) {
    ((void)port);
    throw std::runtime_error("NOT IMPLEMENTED");
}

QString const& Client::GetUsername() const {
    throw std::runtime_error("NOT IMPLEMENTED");
}

void Client::SetUsername(QString const& x) {
    ((void)x);
    throw std::runtime_error("NOT IMPLEMENTED");
}

QString const& Client::GetPassword() const {
    throw std::runtime_error("NOT IMPLEMENTED");
}

void Client::SetPassword(QString const& x) {
    ((void)x);
    throw std::runtime_error("NOT IMPLEMENTED");
}

bool Client::Send(const Message &message) {
    ((void)message);
    throw std::runtime_error("NOT IMPLEMENTED");
}

bool Client::Connect() {
    throw std::runtime_error("NOT IMPLEMENTED");
}

void Client::Disconnect() {
    throw std::runtime_error("NOT IMPLEMENTED");
}

std::size_t Client::GetMessageCount() {
    throw std::runtime_error("NOT IMPLEMENTED");
}

std::vector<Message> Client::Fetch(std::size_t i, std::size_t count) {
    ((void)i);
    ((void)count);
    throw std::runtime_error("NOT IMPLEMENTED");
}

QString Client::FetchAsJson(std::size_t i, std::size_t j) {
    return Json::EncodeMessage(Fetch(i, j));
}

int Client::getMessageCount()
{
    return (int)GetMessageCount();
}

} }

#if !defined(Q_OS_ANDROID)


#include <vmime/vmime.hpp>

namespace Ertebat { namespace Mail {

Message& Client::ExtractMessage(Message& out, std::shared_ptr<vmime::net::message>& msg) {

    auto h = msg->getHeader();

    QString q_from_localname;
    QString q_from_domain;
    QString q_from_name;

    if(h->hasField(vmime::fields::FROM)) {
        auto from = std::static_pointer_cast<vmime::mailbox const>(h->From()->getValue());
        auto from_localname_raw = from->getEmail().getLocalName().getConvertedText(vmime::charset("utf-8"));
        auto from_domain_raw = from->getEmail().getDomainName().getConvertedText(vmime::charset("utf-8"));
        auto from_name_raw = from->getName().getConvertedText(vmime::charset("utf-8"));
        q_from_localname = QString::fromUtf8(from_localname_raw.c_str());
        q_from_domain = QString::fromUtf8(from_domain_raw.c_str());
        q_from_name = QString::fromUtf8(from_name_raw.c_str());
    }

    QString q_subject;
    if(h->hasField(vmime::fields::SUBJECT)) {
        auto subject = std::static_pointer_cast<vmime::text const>(h->Subject()->getValue());
        auto subject_raw = subject->getConvertedText(vmime::charset("utf-8"));
        q_subject = QString::fromUtf8(subject_raw.c_str());
    }

    QString q_replyto_localname = q_from_localname;
    QString q_replyto_domain = q_from_domain;
    QString q_replyto_name = q_from_name;

    if(h->hasField(vmime::fields::REPLY_TO)) {
        auto replyto = std::static_pointer_cast<vmime::mailbox const>(h->ReplyTo()->getValue());
        auto replyto_localname_raw = replyto->getEmail().getLocalName().getConvertedText(vmime::charset("utf-8"));
        auto replyto_domain_raw = replyto->getEmail().getDomainName().getConvertedText(vmime::charset("utf-8"));
        auto replyto_name_raw = replyto->getName().getConvertedText(vmime::charset("utf-8"));
        q_replyto_localname = QString::fromUtf8(replyto_localname_raw.c_str());
        q_replyto_domain = QString::fromUtf8(replyto_domain_raw.c_str());
        q_replyto_name = QString::fromUtf8(replyto_name_raw.c_str());
    }

    std::vector<QString> q_to_localname;
    std::vector<QString> q_to_domain;
    std::vector<QString> q_to_name;

    if(h->hasField(vmime::fields::TO)) {
        auto __to = std::static_pointer_cast<vmime::addressList const>(h->To()->getValue());
        for(size_t j = 0, _j = __to->getAddressCount(); j < _j; ++j) {
            auto _to = __to->getAddressAt(j);
            if(dynamic_cast<vmime::mailbox const*>(_to.get()) == nullptr) {
                auto to = std::static_pointer_cast<vmime::mailboxGroup const>(_to);
                for(size_t i = 0, _i = to->getMailboxCount(); i < _i; ++i) {
                    auto to_ = to->getMailboxAt(i);
                    auto to_localname_raw = to_->getEmail().getLocalName().getConvertedText(vmime::charset("utf-8"));
                    auto to_domain_raw = to_->getEmail().getDomainName().getConvertedText(vmime::charset("utf-8"));
                    auto to_name_raw = to_->getName().getConvertedText(vmime::charset("utf-8"));
                    q_to_localname.push_back(QString::fromUtf8(to_localname_raw.c_str()));
                    q_to_domain.push_back(QString::fromUtf8(to_domain_raw.c_str()));
                    q_to_name.push_back(QString::fromUtf8(to_name_raw.c_str()));
                }
            } else {
                auto to = std::static_pointer_cast<vmime::mailbox const>(_to);
                auto to_localname_raw = to->getEmail().getLocalName().getConvertedText(vmime::charset("utf-8"));
                auto to_domain_raw = to->getEmail().getDomainName().getConvertedText(vmime::charset("utf-8"));
                auto to_name_raw = to->getName().getConvertedText(vmime::charset("utf-8"));
                q_to_localname.push_back(QString::fromUtf8(to_localname_raw.c_str()));
                q_to_domain.push_back(QString::fromUtf8(to_domain_raw.c_str()));
                q_to_name.push_back(QString::fromUtf8(to_name_raw.c_str()));
            }
        }
    }

    std::vector<QString> q_cc_localname;
    std::vector<QString> q_cc_domain;
    std::vector<QString> q_cc_name;

    if(h->hasField(vmime::fields::CC)) {
        auto __cc = std::static_pointer_cast<vmime::addressList const>(h->Cc()->getValue());
        for(size_t j = 0, _j = __cc->getAddressCount(); j < _j; ++j) {
            auto _cc = __cc->getAddressAt(j);
            if(dynamic_cast<vmime::mailbox const*>(_cc.get()) == nullptr) {
                auto cc = std::static_pointer_cast<vmime::mailboxGroup const>(_cc);
                for(size_t i = 0, _i = cc->getMailboxCount(); i < _i; ++i) {
                    auto cc_ = cc->getMailboxAt(i);
                    auto cc_localname_raw = cc_->getEmail().getLocalName().getConvertedText(vmime::charset("utf-8"));
                    auto cc_domain_raw = cc_->getEmail().getDomainName().getConvertedText(vmime::charset("utf-8"));
                    auto cc_name_raw = cc_->getName().getConvertedText(vmime::charset("utf-8"));
                    q_cc_localname.push_back(QString::fromUtf8(cc_localname_raw.c_str()));
                    q_cc_domain.push_back(QString::fromUtf8(cc_domain_raw.c_str()));
                    q_cc_name.push_back(QString::fromUtf8(cc_name_raw.c_str()));
                }
            } else {
                auto cc = std::static_pointer_cast<vmime::mailbox const>(_cc);
                auto cc_localname_raw = cc->getEmail().getLocalName().getConvertedText(vmime::charset("utf-8"));
                auto cc_domain_raw = cc->getEmail().getDomainName().getConvertedText(vmime::charset("utf-8"));
                auto cc_name_raw = cc->getName().getConvertedText(vmime::charset("utf-8"));
                q_cc_localname.push_back(QString::fromUtf8(cc_localname_raw.c_str()));
                q_cc_domain.push_back(QString::fromUtf8(cc_domain_raw.c_str()));
                q_cc_name.push_back(QString::fromUtf8(cc_name_raw.c_str()));
            }
        }
    }

    std::vector<QString> q_bcc_localname;
    std::vector<QString> q_bcc_domain;
    std::vector<QString> q_bcc_name;

    if(h->hasField(vmime::fields::BCC)) {
        auto __bcc = std::static_pointer_cast<vmime::addressList const>(h->Bcc()->getValue());
        for(size_t j = 0, _j = __bcc->getAddressCount(); j < _j; ++j) {
            auto _bcc = __bcc->getAddressAt(j);
            if(dynamic_cast<vmime::mailbox const*>(_bcc.get()) == nullptr) {
                auto bcc = std::static_pointer_cast<vmime::mailboxGroup const>(_bcc);
                for(size_t i = 0, _i = bcc->getMailboxCount(); i < _i; ++i) {
                    auto bcc_ = bcc->getMailboxAt(i);
                    auto bcc_localname_raw = bcc_->getEmail().getLocalName().getConvertedText(vmime::charset("utf-8"));
                    auto bcc_domain_raw = bcc_->getEmail().getDomainName().getConvertedText(vmime::charset("utf-8"));
                    auto bcc_name_raw = bcc_->getName().getConvertedText(vmime::charset("utf-8"));
                    q_bcc_localname.push_back(QString::fromUtf8(bcc_localname_raw.c_str()));
                    q_bcc_domain.push_back(QString::fromUtf8(bcc_domain_raw.c_str()));
                    q_bcc_name.push_back(QString::fromUtf8(bcc_name_raw.c_str()));
                }
            } else {
                auto bcc = std::static_pointer_cast<vmime::mailbox const>(_bcc);
                auto bcc_localname_raw = bcc->getEmail().getLocalName().getConvertedText(vmime::charset("utf-8"));
                auto bcc_domain_raw = bcc->getEmail().getDomainName().getConvertedText(vmime::charset("utf-8"));
                auto bcc_name_raw = bcc->getName().getConvertedText(vmime::charset("utf-8"));
                q_bcc_localname.push_back(QString::fromUtf8(bcc_localname_raw.c_str()));
                q_bcc_domain.push_back(QString::fromUtf8(bcc_domain_raw.c_str()));
                q_bcc_name.push_back(QString::fromUtf8(bcc_name_raw.c_str()));
            }
        }
    }


    auto msg2 = msg->getParsedMessage();
    vmime::messageParser p(msg2);

    std::vector<QString> contents;
    QString single_content;

    for(std::size_t i=0, _i=p.getTextPartCount(); i < _i; ++i) {
        auto part = p.getTextPartAt(i);
        if(part->getType().getSubType() == vmime::mediaTypes::TEXT_HTML) {
            auto html = std::static_pointer_cast<vmime::htmlTextPart const>(
                            part
                        );
            // HTML tex t i s in tp−>getText ()
            // Plain tex t i s in tp−>getPlainText ()
            std::stringstream sout;
            vmime::utility::outputStreamAdapter adap(sout);
            html->getText()->extract(adap);
            std::string raw = sout.str();
            contents.push_back(QString::fromUtf8(raw.c_str()));
            single_content = single_content + contents.back();
        }
    }

    QDateTime date = QDateTime::currentDateTime();

    if(h->hasField(vmime::fields::DATE)) {
        auto d = std::static_pointer_cast<vmime::datetime const>(
                    h->Date()->getValue()
                    );
        date = QDateTime(
                    QDate(d->getYear(), d->getMonth(), d->getDay()),
                    QTime(d->getHour(), d->getMinute(), d->getSecond(), 0)
                    );
    }

    QString q_messageId;
    if(h->hasField(vmime::fields::MESSAGE_ID)) {
        auto id = std::static_pointer_cast<vmime::messageId const>(h->MessageId()->getValue())->getId();
        q_messageId = QString::fromUtf8(id.c_str());
    }

    out.SetFrom(Ertebat::Mail::Mailbox(q_from_localname + "@" + q_from_domain, q_from_name));
    out.SetReplyTo(Ertebat::Mail::Mailbox(q_replyto_localname + "@" + q_replyto_domain, q_replyto_name));

    out.ClearRecipients();

    for(std::size_t i = 0, _i = q_to_domain.size(); i < _i; ++i) {
        out.AddRecipient(Mail::RecipientType::To, Ertebat::Mail::Mailbox(q_to_localname[i] + "@" + q_to_domain[i], q_to_name[i]));
    }

    for(std::size_t i = 0, _i = q_cc_domain.size(); i < _i; ++i) {
        out.AddRecipient(Mail::RecipientType::Cc, Ertebat::Mail::Mailbox(q_cc_localname[i] + "@" + q_cc_domain[i], q_cc_name[i]));
    }

    for(std::size_t i = 0, _i = q_bcc_domain.size(); i < _i; ++i) {
        out.AddRecipient(Mail::RecipientType::Bcc, Ertebat::Mail::Mailbox(q_bcc_localname[i] + "@" + q_bcc_domain[i], q_bcc_name[i]));
    }

    out.SetSubject(q_subject);
    out.SetHtmlBody(single_content);
    out.SetTime(date);
    out.SetMessageId(q_messageId);

    return out;
}

} }

#endif




