/**
 * @author  Mohamad mehdi Kharatizadeh <m_kharatizadeh@yahoo.com>
 */


#include "Json.hpp"

#include "boost/property_tree/ptree.hpp"
#include "boost/property_tree/json_parser.hpp"
#include "boost/foreach.hpp"
#include <QDebug>
#include <QDateTime>

namespace Ertebat { namespace Mail {

namespace detail { namespace Json {

    QString ToString(boost::property_tree::ptree const& pt) {
        std::stringstream ss;
        boost::property_tree::write_json(ss, pt);
        std::string _ss = ss.str();

        return QString::fromUtf8(_ss.c_str());
    }

    boost::property_tree::ptree EncodeMessage(Message const& message) {
        boost::property_tree::ptree pt;
        boost::property_tree::ptree from, to, headers;
        //pt.put("message_id", message.GetMessageId().toStdString());
        //from.put("email", message.GetFrom().GetAddress().toStdString());
        //from.put("name", message.GetFrom().GetName().toStdString());
        //pt.add_child("from", from);
        pt.put("subject", message.GetSubject().toStdString());
        auto recep = message.GetRecipients();
        for(auto i = recep.begin(), _i = recep.end(); i != _i; ++i) {
            Mail::Recipient iv = *i;
            boost::property_tree::ptree p;
            p.put("name", iv.Mailbox.GetName().toStdString());
            p.put("email", iv.Mailbox.GetAddress().toStdString());
            if(iv.Type == Mail::RecipientType::To) {
                p.put("type", "TO");
            } else if(iv.Type == Mail::RecipientType::Cc) {
                p.put("type", "CC");
            } else if(iv.Type == Mail::RecipientType::Bcc) {
                p.put("type", "BCC");
            }
            to.push_back(std::make_pair("", p));
        }
        pt.add_child("to", to);
        headers.put("Reply-To", message.GetReplyTo().GetAddress().toStdString());
        pt.add_child("headers", headers);
        pt.put("text", message.GetHtmlBody().toStdString());

        //qDebug() << "-------------" << ToString(pt);

        return pt;
    }



} }

QString Json::EncodeMessage(Message const& message) {
    return detail::Json::ToString(detail::Json::EncodeMessage(message));
}

QString Json::EncodeMessage(std::vector<Message> const& message) {
    boost::property_tree::ptree root;
    boost::property_tree::ptree data;
    boost::property_tree::ptree pt;
    boost::property_tree::ptree from, to, headers;

    for(auto i=message.begin(),_i=message.end(); i!=_i; ++i) {
        data.push_back(std::make_pair("", detail::Json::EncodeMessage((*i))));
    }

    root.add_child("data", data);

    return detail::Json::ToString(root);
}

Message Json::DecodeSingleMessage(QString const& json) {
    std::stringstream ss;
    ss << json.toStdString();
    boost::property_tree::ptree v;
    boost::property_tree::read_json(ss, v);

    Ertebat::Mail::Message msg;


    if(v.find("from") != v.not_found()) {
        std::string _from_email;
        std::string _from_name;

        BOOST_FOREACH(boost::property_tree::ptree::value_type &vv, v.get_child("from")) {
            _from_email = vv.second.get<std::string>("email");
            if(vv.second.find("name") != vv.second.not_found()) {
                _from_name = vv.second.get<std::string>("name");
            }
            break;
        }

        QString from_email = QString::fromUtf8(_from_email.c_str());
        QString from_name = QString::fromUtf8(_from_name.c_str());

        msg.SetFrom(Mail::Mailbox(from_email, from_name));
    }

    auto _subject = v.get<std::string>("subject");
    if(v.find("headers.Reply-To") != v.not_found()) {
        auto _replyto = v.get<std::string>("headers.Reply-To");
        QString ReplyTo = QString::fromUtf8(_replyto.c_str());
        msg.SetReplyTo(Mail::Mailbox(ReplyTo));
    }
    auto _text = v.get<std::string>("text");

    QString subject = QString::fromUtf8(_subject.c_str());
    QString text = QString::fromUtf8(_text.c_str());

    msg.SetSubject(subject);
    msg.SetHtmlBody(text);


    std::vector<QString> to_name,
            to_email, cc_name, cc_email, bcc_name, bcc_email;

    BOOST_FOREACH(boost::property_tree::ptree::value_type &v2, v.get_child("to"))
    {
        auto _email = v2.second.get<std::string>("email");
        std::string _name = "";
        if(v2.second.find("name") != v2.second.not_found()) {
            _name = v2.second.get<std::string>("name");
        }
        auto _type = v2.second.get<std::string>("type");
        QString email = QString::fromUtf8(_email.c_str());
        QString name = QString::fromUtf8(_name.c_str());
        QString type = QString::fromUtf8(_type.c_str());
        if(type.toLower() == "to") {
            to_email.push_back(email);
            to_name.push_back(name);
        } else if(type.toLower() == "cc") {
            cc_email.push_back(email);
            cc_name.push_back(name);
        } else if(type.toLower() == "bcc") {
            bcc_email.push_back(email);
            bcc_name.push_back(name);
        }
    }

    for(std::size_t i=0,_i=to_name.size();i<_i;++i) {
        msg.AddRecipient(Mail::RecipientType::To, Mail::Mailbox(to_email[i], to_name[i]));
    }
    for(std::size_t i=0,_i=cc_name.size();i<_i;++i) {
        msg.AddRecipient(Mail::RecipientType::Cc, Mail::Mailbox(cc_email[i], cc_name[i]));
    }
    for(std::size_t i=0,_i=bcc_name.size();i<_i;++i) {
        msg.AddRecipient(Mail::RecipientType::Bcc, Mail::Mailbox(bcc_email[i], bcc_name[i]));
    }

    return msg;
}

std::vector<Message> Json::DecodeMessage(QString const& raw) {
    std::stringstream ss;
    ss << raw.toStdString();
    boost::property_tree::ptree pt;
    boost::property_tree::read_json(ss, pt);

    std::vector<Message> ret;

    BOOST_FOREACH(boost::property_tree::ptree::value_type &v, pt.get_child("data"))
    {
        auto _message_id = v.second.get<std::string>("message_id");
        auto _date = v.second.get<std::string>("date");

        std::string _from_email;
        std::string _from_name;

        BOOST_FOREACH(boost::property_tree::ptree::value_type &vv, v.second.get_child("from")) {
            _from_email = vv.second.get<std::string>("email");
            _from_name = vv.second.get<std::string>("name");
            break;
        }

        auto _subject = v.second.get<std::string>("subject");
        auto _replyto = v.second.get<std::string>("headers.Reply-To");
        auto _text = v.second.get<std::string>("text");

        QString message_id = QString::fromUtf8(_message_id.c_str());
        auto v_date = QString::fromUtf8(_date.c_str()).split(":");
        QDateTime date;
        date.setDate(QDate(v_date[3].toInt(), v_date[2].toInt(), v_date[1].toInt()));
        date.setTime(QTime(v_date[4].toInt(), v_date[5].toInt(), v_date[6].toInt()));
        QString from_email = QString::fromUtf8(_from_email.c_str());
        QString from_name = QString::fromUtf8(_from_name.c_str());
        QString subject = QString::fromUtf8(_subject.c_str());
        QString text = QString::fromUtf8(_text.c_str());

        Ertebat::Mail::Message msg;
        msg.SetMessageId(message_id);
        msg.SetTime(date);
        msg.SetFrom(Mail::Mailbox(from_email, from_name));
        msg.SetSubject(subject);
        msg.SetHtmlBody(text);


        std::vector<QString> to_name,
                to_email, cc_name, cc_email, bcc_name, bcc_email;

        BOOST_FOREACH(boost::property_tree::ptree::value_type &v2, v.second.get_child("to"))
        {
            auto _email = v2.second.get<std::string>("email");
            auto _name = v2.second.get<std::string>("name");
            auto _type = v2.second.get<std::string>("type");
            QString email = QString::fromUtf8(_email.c_str());
            QString name = QString::fromUtf8(_name.c_str());
            QString type = QString::fromUtf8(_type.c_str());
            if(type.toLower() == "to") {
                to_email.push_back(email);
                to_name.push_back(name);
            } else if(type.toLower() == "cc") {
                cc_email.push_back(email);
                cc_name.push_back(name);
            } else if(type.toLower() == "bcc") {
                bcc_email.push_back(email);
                bcc_name.push_back(name);
            }
        }

        for(std::size_t i=0,_i=to_name.size();i<_i;++i) {
            msg.AddRecipient(Mail::RecipientType::To, Mail::Mailbox(to_email[i], to_name[i]));
        }
        for(std::size_t i=0,_i=cc_name.size();i<_i;++i) {
            msg.AddRecipient(Mail::RecipientType::Cc, Mail::Mailbox(cc_email[i], cc_name[i]));
        }
        for(std::size_t i=0,_i=bcc_name.size();i<_i;++i) {
            msg.AddRecipient(Mail::RecipientType::Bcc, Mail::Mailbox(bcc_email[i], bcc_name[i]));
        }

        ret.push_back(msg);
    }

    return ret;

}

} }
