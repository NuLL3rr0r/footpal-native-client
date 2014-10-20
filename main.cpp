/**
 * @author  Mohammad S. Babaei <info@babaei.net>
 */


#include <iostream>
#include <memory>
#include <stdexcept>
#include <csignal>
#include <cstdlib>
#include <boost/algorithm/string.hpp>
#include <boost/exception/diagnostic_information.hpp>
#include <boost/filesystem/exception.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/format.hpp>
#include "make_unique.hpp"
#include "Application.hpp"
#include "Exception.hpp"
#include "Log.hpp"

#include "Mail/SmtpClient.hpp"
#include "Mail/ImapClient.hpp"
#include "Mail/Message.hpp"
#include "Mail/Mailbox.hpp"

#include <QDebug>

void Terminate [[noreturn]] (int signo);

int main(int argc, char *argv[])
{
    qDebug() << "Heelo!!";

    try {
        /*! Gracefully handling SIGTERM */
        void (*prev_fn)(int);
        prev_fn = signal(SIGTERM, Terminate);
        if (prev_fn == SIG_IGN)
            signal(SIGTERM, SIG_IGN);

        Ertebat::Log::Initialize(std::cout);

#if !defined ( Q_OS_ANDROID )
        /*! Change to executable path */
        boost::filesystem::path path(boost::filesystem::initial_path<boost::filesystem::path>());
        if (argc > 0 && argv[0] != NULL)
            path = boost::filesystem::system_complete(boost::filesystem::path(argv[0]));
        std::string appId(path.filename().string());
        std::string appPath(boost::algorithm::replace_last_copy(path.string(), appId, ""));
        boost::filesystem::current_path(appPath);
#endif // !defined ( Q_OS_ANDROID )

        std::unique_ptr<Ertebat::Application> app =
                std::make_unique<Ertebat::Application>(argc, argv);
        app->InitializeDatabase();
        app->SetupUi();


        Ertebat::Mail::Mailbox from_box("arcananemous@gmail.com","mehdi_gmail");
        Ertebat::Mail::Mailbox to_box("m_kharatizadeh@yahoo.com","mehdi_yahoo");

        Ertebat::Mail::ImapClient c;
        c.SetHost("imap.gmail.com");
        c.SetPort(993);
        //c.SetUsername("morteza.sabetraftar@gmail.com");
        //c.SetPassword("zjwdmoszjtinyogs");
        c.SetUsername("arcananemous@gmail.com");
        c.SetPassword("konohanosennin1x");
        c.SetSecurityType(Ertebat::Mail::SecurityType::SSL_TLS);
        if(!c.Connect()) {
            qDebug() << "[CAN NOT CONNECT]";
        }
        else {
            qDebug() << "CONNECTED!";
        }

        {
            Ertebat::Mail::Message toSend;
            toSend.SetFrom(from_box);
            toSend.SetReplyTo(from_box);
            toSend.SetHtmlBody("<HTML><BODY><b>This</b> Is Just A Test</BODY></HTML>");
            toSend.SetSubject("This is Just a Test");
            toSend.AddRecipient(Ertebat::Mail::RecipientType::To, to_box);
            //if(!c.Send(toSend)) {
            //    qDebug() << "[CAN NOT SEND]";
            //} else {
            //    qDebug() << "SEND SUCCESSFUL";
            //}
            auto v = c.Fetch();

        }

        c.Disconnect();
        return 0;


        return app->exec();
    }

    catch (const Ertebat::Exception &ex) {
        LOG_ERROR(ex.what());
    }

    catch (const boost::exception &ex) {
        LOG_ERROR(boost::diagnostic_information(ex));
    }

    catch (const std::exception &ex) {
        LOG_ERROR(ex.what());
    }

    catch (...) {
        LOG_ERROR("main: Unknown error!");
    }

    return EXIT_FAILURE;
}

void Terminate(int signo)
{
    LOG_WARNING((boost::format("Terminating by signal %1%...") % signo).str());
    exit(1);
}

