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

void Terminate [[noreturn]] (int signo);

int main(int argc, char *argv[])
{
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

