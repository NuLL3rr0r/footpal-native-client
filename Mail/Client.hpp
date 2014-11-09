/**
 * @author  Mohamad mehdi Kharatizadeh <m_kharatizadeh@yahoo.com>
 * @author  Mohammad S. Babaei <info@babaei.net>
 */


#ifndef ERTEBAT_MAIL_CLIENT_HPP
#define ERTEBAT_MAIL_CLIENT_HPP


#include <QObject>
#include "Mail.hpp"
#include "Message.hpp"
#include <memory>

class QString;

#undef SetPort

namespace vmime { namespace net {
    class message;
} }


namespace Ertebat { namespace Mail {

    class Mailbox;
    class Message;


    class Client : public QObject {

        Q_OBJECT

    private:

        struct mpi_impl;
        std::shared_ptr<mpi_impl> mpi_pimpl;

    public:

        Client();
        virtual ~Client();

    signals:

        void signal_ConnectCompleted(bool);
        void signal_DisconnectCompleted();
        void signal_SendCompleted(bool);
        void signal_GetMessageCountCompleted(int);
        void signal_FetchAsJsonCompleted(const QString&);
        void signal_FetchCompleted(const QVector<Message>&);

    public:

        //virtual Mailbox& GetMailbox() = 0;
        //virtual Mailbox const& GetMailbox() const = 0;

        virtual Q_INVOKABLE QString const&  GetHost() const;
        virtual Q_INVOKABLE void SetHost(QString const& host);
        virtual Q_INVOKABLE Mail::Port_t const& GetPort() const;
        virtual Q_INVOKABLE void SetPort(Mail::Port_t const& port);
        virtual Q_INVOKABLE QString const& GetUsername() const;
        virtual Q_INVOKABLE void SetUsername(QString const& x);
        virtual Q_INVOKABLE QString const& GetPassword() const;
        virtual Q_INVOKABLE void SetPassword(QString const& x);
        virtual Q_INVOKABLE void Connect();
        virtual Q_INVOKABLE void Disconnect();
        virtual Q_INVOKABLE void GetMessageCount();
        virtual Q_INVOKABLE void Fetch(std::size_t i, std::size_t count);
        virtual Q_INVOKABLE void FetchAsJson(std::size_t i, std::size_t count);
        virtual Q_INVOKABLE void FetchAsJsonAsync(std::size_t i, std::size_t count);

        // QML Hacks
        virtual Q_INVOKABLE void getMessageCount();
        virtual Q_INVOKABLE void fetchAsJson(int i, int count);
        virtual Q_INVOKABLE void workerThread_disconnect();
        virtual Q_INVOKABLE void workerThread_connect();
        virtual Q_INVOKABLE void workerThread_getMessageCount();
        virtual Q_INVOKABLE void workerThread_fetchAsJson(int i, int count);
        virtual Q_INVOKABLE void workerThread_autoFetchEnable(int i, int count, int totalMessageCount);
        virtual Q_INVOKABLE void workerThread_autoFetchDisable();
        virtual Q_INVOKABLE bool workerThread_autoFetchIsEnabled();
        virtual Q_INVOKABLE bool workerThread_isEnabled();
        virtual Q_INVOKABLE void workerThread_enable();
        virtual Q_INVOKABLE void workerThread_disable();

    protected:

#if !defined(Q_OS_ANDROID)
        static Message& ExtractMessage(Message& out, std::shared_ptr<vmime::net::message>& msg);
#endif

        void mpi_sendDisconnectMessage();
        void mpi_sendConnectMessage();
        void mpi_sendGetMessageCountMessage();
        void mpi_sendFetchAsJsonMessage(std::size_t i, std::size_t count);
        bool mpi_isEnabled();
        void mpi_enable();
        void mpi_disable();
        void mpi_autoFetchEnable(std::size_t i, std::size_t count, std::size_t totalMessageCount);
        void mpi_autoFetchDisable();
        void mpi_autoFetchPerform();
        bool mpi_autoFetchIsEnabled();

        void onFetchJsonMessage(const QString& message);

    };

} }


#endif // CLIENT_HPP
