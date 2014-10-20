#include "Client.hpp"
#include "Message.hpp"
#include <QDebug>

namespace Ertebat { namespace Mail {

struct Field {
    QString Field;
    QString Encoding;
    QString Name;
    QString Content;
    QString Address;
    std::vector<std::pair<QString,QString> > List;
};

Message& Client::ExtractMessage(Message& out, vmime::shared_ptr<vmime::net::message>& msg) {

    auto h = msg->getHeader();

    auto extractor = [](vmime::shared_ptr<const vmime::headerField> f) -> Field {
        QString raw = QString::fromStdString(f->generate());
        auto v = raw.split("?");
        QString field = v[0].split(":")[0];
        QString encoding = v[1];
        QString name = v[3];
        QString address =  v[4];
        auto VV = address.split("<")[1];
        address = VV.split(">")[0];
        Field ret;
        ret.Field = field;
        ret.Encoding = encoding;
        ret.Name = name;
        ret.Address = address;
        return ret;
    };

    auto extractor_2 = [](vmime::shared_ptr<const vmime::headerField> f) -> Field {
        Field ret;
        ret.Address = QString::fromStdString(f->generate()).split(":")[1];
        return ret;
    };

    auto extractor_3 = [&extractor,&extractor_2](vmime::shared_ptr<const vmime::headerField> f) -> Field {
        QString raw = QString::fromStdString(f->generate());
        if(raw.contains("?")) {
            return extractor(f);
        } else {
            return extractor_2(f);
        }
    };

    auto extractor_4 = [&extractor_3](vmime::shared_ptr<const vmime::headerField> f) -> Field {
        Field ret = extractor_3(f);
        auto v = ret.Address.split(",");
        for(std::size_t i=0,_i=v.size();i<_i;++i) {
            auto s = v[i].trimmed();
            if(s.contains("<")) {
                auto vv = s.split("<");
                QString name = vv[0].remove("\"").trimmed();
                QString address = vv[1].split(">")[0].trimmed();
                ret.List.push_back(std::make_pair(name, address));
            } else {
                ret.List.push_back(std::make_pair("", s));
            }
        }
        return ret;
    };

    Field from;

    if(h->hasField("From"))
        from = extractor_3(h->From());
    else if(h->hasField("Sender"))
        from = extractor_3(h->Sender());
    else
        return out;

    Field replyTo = from;
    if(h->hasField("ReplyTo"))
        replyTo = extractor_3(h->ReplyTo());

    Field subject;
    if(h->hasField("Subject"))
        subject = extractor_3(h->Subject());

    Field cc, bcc, to;
    if(h->hasField("Cc"))
        cc = extractor_4(h->Cc());
    if(h->hasField("Bcc"))
        bcc = extractor_4(h->Bcc());
    if(h->hasField("To"))
        to = extractor_4(h->To());


    QString s = QString::fromStdString(h->Date()->generate());

    return out;

}

} }


