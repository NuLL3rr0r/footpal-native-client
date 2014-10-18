#include "MailDatabase.hpp"
#include <map>
#include <algorithm>
#include <QStringList>
#include <sstream>
#include <QDateTime>


namespace Ertebat { namespace Mail {

    MailDatabase::singleton_type MailDatabase::m_singleton;


    class MailDatabase::Impl {
        Impl(Impl const& x) {
            ((void)x);
        }

        Impl& operator = (Impl const& x) {
            ((void)x);
            return *this;
        }

    public:
        Impl() { }

        virtual ~Impl() { }
        virtual std::vector<MailDatabase::object_t> Query(QString const& address, Mail::SortType sort = Mail::SortType::Default) const = 0;
        virtual std::vector<MailDatabase::object_t> Query(QString const& address, QString const& folder, Mail::SortType sort = Mail::SortType::Default) const = 0;
        virtual std::vector<MailDatabase::object_t> Query(QString const& address, QString const& folder, QString const& search, Mail::SortType sort = Mail::SortType::Default) const = 0;

        virtual QByteArray ReadObjectAttribute(object_t id, QString const& attribute) const = 0;
        virtual QString GetAddress(object_t obj) const = 0;
        virtual void WriteObjectAttribute(object_t id, QString const& attribute, QByteArray data) = 0;
        virtual object_t AddObject(QString const& address, QString const& folder) = 0;
        virtual bool DeleteObject(object_t obj) = 0;
        virtual bool AddFolder(QString const& address, QString const& folder) = 0;
        virtual bool DeleteFolder(QString const& address, QString const& folder) = 0;
        virtual std::vector<QString> ListAllAttributes(object_t obj) const = 0;
        virtual std::vector<QString> ListAllAttributes() const = 0;
        virtual std::vector<QString> ListAllFolders(QString const& address) const = 0;
        virtual bool AddAddress(QString const& address) = 0;
        virtual bool RemoveAddress(QString const& address) = 0;
        virtual std::vector<QString> LitsAllAddresses() const = 0;
        virtual QString GetFolder(MailDatabase::object_t obj) const = 0;
        virtual bool SetFolder(MailDatabase::object_t obj, QString const& folder) = 0;
        virtual QString GetSender(MailDatabase::object_t obj) const = 0;
        virtual void SetSender(MailDatabase::object_t obj, QString const& sender) = 0;
        virtual std::vector<QString> GetReceipientList(MailDatabase::object_t obj) const = 0;
        virtual bool RemoveReceipient(MailDatabase::object_t obj, QString const& receipient) = 0;
        virtual void SetReceipientList(MailDatabase::object_t obj, std::vector<QString> const& list) = 0;
        virtual bool AddReceipient(MailDatabase::object_t obj, QString const& receipient) = 0;
        virtual QString GetSubject(MailDatabase::object_t obj) const = 0;
        virtual void SetSubject(MailDatabase::object_t obj, QString const& subject) = 0;
        virtual QString GetPlainBody(MailDatabase::object_t obj) const = 0;
        virtual void SetPlainBody(MailDatabase::object_t obj, QString const& body) = 0;
        virtual QString GetHtmlBody(MailDatabase::object_t obj) const = 0;
        virtual void SetHtmlBody(MailDatabase::object_t obj, QString const& body) = 0;
        virtual QDateTime GetTimestamp(MailDatabase::object_t obj) const = 0;
        virtual void SetTimestamp(MailDatabase::object_t obj, QDateTime t) = 0;
        virtual std::vector<QString> GetAttachments(MailDatabase::object_t obj) = 0;
        virtual void SetAttachments(MailDatabase::object_t obj, std::vector<QString> const& list) = 0;
        virtual bool RemoveAttachment(MailDatabase::object_t obj, QString const& attach) = 0;
        virtual bool AddAttachment(MailDatabase::object_t obj, QString const& attach) = 0;
    };

    namespace detail { namespace MailDatabase {

        class MemoryImpl : public Mail::MailDatabase::Impl {
            typedef Mail::MailDatabase::object_t object_t;
            std::map<object_t, std::map<QString, QByteArray> > m_objects;
            std::vector<QString> m_folders;
            object_t m_lastobject;
            std::vector<QString> m_allAttributes;

        public:

            virtual ~MemoryImpl() {

            }

            MemoryImpl() : Impl() {
                m_lastobject = 0;
                AddFolder("", "Inbox");
                AddFolder("", "Outbox");
                AddFolder("", "Sent");
                AddFolder("", "Spam");
            }

            std::vector<QString> ListAllAttributes(object_t obj) const {
                std::vector<QString> ret;

                auto i = m_objects.find(obj);
                if( i == m_objects.end())
                    return ret;

                for(auto j = i->second.begin(), _j=i->second.end(); j != _j; ++j) {
                    ret.push_back(j->first);
                }

                return ret;
            }

            QString GetAddress(object_t obj) const {
                ((void)obj);
                return "";
            }

            std::vector<QString> ListAllFolders(QString const& address) const {
                ((void)address);
                return m_folders;
            }

            std::vector<QString> ListAllAttributes() const {
                return m_allAttributes;
            }

            std::vector<object_t> Query(QString const& address, Mail::SortType sort = Mail::SortType::Default) const {
                return Query(address, "", sort);
            }

            std::vector<object_t> Query(QString const& address, QString const& folder, Mail::SortType sort = Mail::SortType::Default) const {
                return Query(address, folder, "", sort);
            }

            std::vector<object_t> Query(QString const& address, QString const& folder, QString const& search, Mail::SortType sort = Mail::SortType::Default) const {
                ((void)address);
                ((void)sort);
                std::vector<object_t> ret;
                for(auto i=m_objects.begin(), _i=m_objects.end(); i != _i; ++i) {
                    bool b = (folder == "" || QString::fromUtf8(ReadObjectAttribute(i->first, "Folder")) == folder);
                    if(!b && search != "") {
                        auto all = ListAllAttributes(i->first);
                        for(auto j=all.begin(),_j=all.end(); j!=_j && !b; ++j) {
                            auto k = i->second.find(*j);
                            if(k != i->second.end()) {
                                auto s = QString::fromUtf8(k->second);
                                if(s.contains(search)) {
                                    b = true;
                                }
                            }
                        }
                    }
                    if(b) {
                        ret.push_back(i->first);
                    }
                }
                return ret;
            }

            QByteArray ReadObjectAttribute(object_t id, QString const& attribute) const {
                auto i = m_objects.find(id);
                if( i == m_objects.end())
                    return QByteArray();
                auto j = i->second.find(attribute);
                if(j == i->second.end())
                    return QByteArray();
                return j->second;
            }

            void WriteObjectAttribute(object_t id, QString const& attribute, QByteArray data) {
                auto i = m_objects.find(id);
                if( i != m_objects.end()) {
                    i->second[attribute] = data;
                }
            }

            object_t AddObject(QString const& address, QString const& folder) {
                ((void)address);
                if(std::find(m_folders.begin(), m_folders.end(), folder) == m_folders.end())
                    return Mail::MailDatabase::nil;

                object_t ret = m_lastobject++;

                m_objects[ret] = decltype(m_objects)::mapped_type();

                auto all = ListAllAttributes();
                for(auto j = all.begin(), _j = all.end(); j != _j; ++j) {
                    WriteObjectAttribute(ret, *j, QByteArray());
                }

                WriteObjectAttribute(ret, "Folder", folder.toUtf8());
                return ret;
            }

            bool DeleteObject(object_t obj) {
                auto i = m_objects.find(obj);
                if(i == m_objects.end())
                    return false;
                m_objects.erase(i);
                return true;
            }

            bool AddFolder(QString const& address, QString const& folder) {
                ((void)address);
                if(std::find(m_folders.begin(), m_folders.end(), folder) != m_folders.end())
                    return false;
                m_folders.push_back(folder);
                return true;
            }

            bool DeleteFolder(QString const& address, QString const& folder) {
                ((void)address);
                decltype(m_folders)::iterator i;
                if((i=std::find(m_folders.begin(), m_folders.end(), folder)) == m_folders.end())
                    return false;
                m_folders.erase(i);
                return true;
            }

            bool AddAddress(QString const& address) {
                ((void) address);
                return false;
            }

            bool RemoveAddress(QString const& address) {
                ((void) address);
                return false;
            }

            std::vector<QString> LitsAllAddresses() const {
                std::vector<QString> ret;
                ret.push_back("");
                return ret;
            }

           QString GetFolder(object_t obj) const {
               return QString::fromUtf8(ReadObjectAttribute(obj, "Folder"));
           }
           bool SetFolder(object_t obj, QString const& folder) {
               auto v = ListAllFolders(GetAddress(obj));

               if(std::find(v.begin(), v.end(), folder) == v.end()
                       || folder == GetFolder(obj))
               {
                   return false;
               }
               WriteObjectAttribute(obj, "Folder", folder.toUtf8());
               return true;
           }

           QString GetSender(object_t obj) const {
               return QString::fromUtf8(ReadObjectAttribute(obj, "Sender"));
           }
           void SetSender(object_t obj, QString const& sender) {
               WriteObjectAttribute(obj, "Sender", sender.toUtf8());
           }

           std::vector<QString> GetReceipientList(object_t obj) const {
               auto s = QString::fromUtf8(ReadObjectAttribute(obj, "ReceipientList"));
               auto v=s.split(";");
               return std::vector<QString>(v.begin(), v.end());
           }
           void SetReceipientList(object_t obj, std::vector<QString> const& list) {
               std::wstringstream sin;
               for(std::size_t i=0,_i=list.size(); i!=_i;++i) {
                   if(i > 0) sin << L";";
                   sin << list[i].toStdWString();
               }
               QString a;
               WriteObjectAttribute(obj, "ReceipientList", (QString::fromStdWString(sin.str())).toUtf8());
           }

           bool RemoveReceipient(object_t obj, QString const& receipient) {
               auto v = GetReceipientList(obj);
               auto i = std::find(v.begin(), v.end(), receipient);
               if(i == v.end()) {
                   return false;
               }
               v.erase(i);
               SetReceipientList(obj, v);
               return true;
           }

           bool AddReceipient(object_t obj, QString const& receipient) {
               auto v = GetReceipientList(obj);
               if(std::find(v.begin(), v.end(), receipient) != v.end())
                   return false;
               v.push_back(receipient);
               SetReceipientList(obj, v);
               return true;
           }

           QString GetSubject(object_t obj) const {
               return QString::fromUtf8(ReadObjectAttribute(obj, "Subject"));
           }
           void SetSubject(object_t obj, QString const& subject) {
               WriteObjectAttribute(obj, "Subject", subject.toUtf8());
           }

           QString GetPlainBody(object_t obj) const {
               return QString::fromUtf8(ReadObjectAttribute(obj, "PlainBody"));
           }
           void SetPlainBody(object_t obj, QString const& body) {
               WriteObjectAttribute(obj, "PlainBody", body.toUtf8());
           }

           QString GetHtmlBody(object_t obj) const {
               return QString::fromUtf8(ReadObjectAttribute(obj, "HtmlBody"));
           }
           void SetHtmlBody(object_t obj, QString const& body) {
               WriteObjectAttribute(obj, "HtmlBody", body.toUtf8());
           }

           QDateTime GetTimestamp(object_t obj) const {
               return QDateTime::fromString(QString::fromUtf8(ReadObjectAttribute(obj, "Timestamp")));
           }
           void SetTimestamp(object_t obj, QDateTime t) {
               WriteObjectAttribute(obj, "Timestamp", t.toString().toUtf8());
           }

           std::vector<QString> GetAttachments(object_t obj) {
               auto v = QString::fromUtf8(ReadObjectAttribute(obj, "Attachments")).split("|");
               return std::vector<QString>(v.begin(), v.end());
           }

           void SetAttachments(object_t obj, std::vector<QString> const& list) {
               std::wstringstream sin;
               for(size_t i=0,_i=list.size();i<_i;++i) {
                    if(i > 0) {
                        sin << L"|";
                    }
                    sin << list[i].toStdWString();
               }
               WriteObjectAttribute(obj, "Attachments", QString::fromStdWString(sin.str()).toUtf8());
           }

           bool RemoveAttachment(object_t obj, QString const& attach) {
               auto v = GetAttachments(obj);
               auto i = std::find(v.begin(), v.end(), attach);
               if(i == v.end()) {
                   return false;
               }
               v.erase(i);
               SetAttachments(obj, v);
               return true;
           }

           bool AddAttachment(object_t obj, QString const& attach) {
               auto v = GetAttachments(obj);
               if(std::find(v.begin(), v.end(), attach) != v.end()) {
                   return false;
               }
               v.push_back(attach);
               SetAttachments(obj, v);
               return true;
           }



        };

    } }


    MailDatabase::MailDatabase(MailDatabase const& x)
        : QObject(), m_pimple(x.m_pimple)
    {

    }

    MailDatabase::~MailDatabase()
    {

    }


    MailDatabase::MailDatabase(MailDatabase&& x)
        : QObject(), m_pimple(std::move(x.m_pimple))
    {

    }

    MailDatabase::MailDatabase()
        : QObject(), m_pimple(new detail::MailDatabase::MemoryImpl)
    {

    }

    MailDatabase& MailDatabase::operator = (MailDatabase const& x) {
        m_pimple = x.m_pimple;
        return *this;
    }

    MailDatabase& MailDatabase::operator = (MailDatabase&& x) {
        m_pimple = std::move(x.m_pimple);
        return *this;
    }

    MailDatabase& MailDatabase::Current() {
        if(!m_singleton) {
            m_singleton = singleton_type(new singleton_type::element_type);
        }
        return *m_singleton.get();
    }

    std::vector<MailDatabase::object_t> MailDatabase::Query(QString const& address, Mail::SortType sort) const {
        return m_pimple->Query(address, sort);
    }
    std::vector<MailDatabase::object_t> MailDatabase::Query(QString const& address, QString const& folder, Mail::SortType sort) const {
        return m_pimple->Query(address, folder, sort);
    }
    std::vector<MailDatabase::object_t> MailDatabase::Query(QString const& address, QString const& folder, QString const& search, Mail::SortType sort) const {
        return m_pimple->Query(address, folder, search, sort);
    }

    QByteArray MailDatabase::ReadObjectAttribute(object_t id, QString const& attribute) const {
        return m_pimple->ReadObjectAttribute(id, attribute);
    }
    void MailDatabase::WriteObjectAttribute(object_t id, QString const& attribute, QByteArray data) {
        m_pimple->WriteObjectAttribute(id, attribute, data);
    }

    MailDatabase::object_t MailDatabase::AddObject(QString const& address, QString const& folder) {
        return m_pimple->AddObject(address, folder);
    }

    bool MailDatabase::DeleteObject(object_t obj) {
        return m_pimple->DeleteObject(obj);
    }

    bool MailDatabase::AddFolder(QString const& address, QString const& folder) {
        return m_pimple->AddFolder(address, folder);
    }

    bool MailDatabase::DeleteFolder(QString const& address, QString const& folder) {
        return m_pimple->DeleteFolder(address, folder);
    }

    QString MailDatabase::GetAddress(object_t obj) const {
        return m_pimple->GetAddress(obj);
    }

    std::vector<QString> MailDatabase::ListAllAttributes(object_t obj) const {
        return m_pimple->ListAllAttributes(obj);
    }
    std::vector<QString> MailDatabase::ListAllAttributes() const {
        return m_pimple->ListAllAttributes();
    }
    std::vector<QString> MailDatabase::ListAllFolders(QString const& address) const {
        return m_pimple->ListAllFolders(address);
    }
    bool MailDatabase::AddAddress(QString const& address) {
        return m_pimple->AddAddress(address);
    }

    bool MailDatabase::RemoveAddress(QString const& address) {
        return m_pimple->RemoveAddress(address);
    }

    std::vector<QString> MailDatabase::LitsAllAddresses() const {
        return m_pimple->LitsAllAddresses();
    }
    QString MailDatabase::GetFolder(MailDatabase::object_t obj) const {
        return m_pimple->GetFolder(obj);
    }
    bool MailDatabase::SetFolder(MailDatabase::object_t obj, QString const& folder) {
        return m_pimple->SetFolder(obj, folder);
    }

    QString MailDatabase::GetSender(MailDatabase::object_t obj) const {
        return m_pimple->GetSender(obj);
    }
    void MailDatabase::SetSender(MailDatabase::object_t obj, QString const& sender) {
        m_pimple->SetSender(obj, sender);
    }
    std::vector<QString> MailDatabase::GetReceipientList(MailDatabase::object_t obj) const {
        return m_pimple->GetReceipientList(obj);
    }
    bool MailDatabase::RemoveReceipient(MailDatabase::object_t obj, QString const& receipient) {
        return m_pimple->RemoveReceipient(obj, receipient);
    }

    void MailDatabase::SetReceipientList(MailDatabase::object_t obj, std::vector<QString> const& list) {
        m_pimple->SetReceipientList(obj, list);
    }

    bool MailDatabase::AddReceipient(MailDatabase::object_t obj, QString const& receipient) {
        return m_pimple->AddReceipient(obj, receipient);
    }

    QString MailDatabase::GetSubject(MailDatabase::object_t obj) const {
        return m_pimple->GetSubject(obj);
    }
    void MailDatabase::SetSubject(MailDatabase::object_t obj, QString const& subject) {
        m_pimple->SetSubject(obj, subject);
    }

    QString MailDatabase::GetPlainBody(MailDatabase::object_t obj) const {
        return m_pimple->GetPlainBody(obj);
    }
    void MailDatabase::SetPlainBody(MailDatabase::object_t obj, QString const& body) {
        m_pimple->SetPlainBody(obj, body);
    }

    QString MailDatabase::GetHtmlBody(MailDatabase::object_t obj) const {
        return m_pimple->GetHtmlBody(obj);
    }
    void MailDatabase::SetHtmlBody(MailDatabase::object_t obj, QString const& body) {
        m_pimple->SetHtmlBody(obj, body);
    }

    QDateTime MailDatabase::GetTimestamp(MailDatabase::object_t obj) const {
        return m_pimple->GetTimestamp(obj);
    }
    void MailDatabase::SetTimestamp(MailDatabase::object_t obj, QDateTime t) {
        m_pimple->SetTimestamp(obj, t);
    }


    std::vector<QString> MailDatabase::GetAttachments(MailDatabase::object_t obj) {
        return m_pimple->GetAttachments(obj);
    }

    void MailDatabase::SetAttachments(MailDatabase::object_t obj, std::vector<QString> list) {
        m_pimple->SetAttachments(obj, list);
    }

    bool MailDatabase::RemoveAttachment(MailDatabase::object_t obj, QString const& attach) {
        return m_pimple->RemoveAttachment(obj, attach);
    }

    bool MailDatabase::AddAttachment(MailDatabase::object_t obj, QString const& attach) {
        return m_pimple->AddAttachment(obj, attach);
    }


} }



