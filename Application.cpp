#include <QtCore/QDebug>
#include <QtGui/QFont>
#include <QtGui/QFontDatabase>
#include <QtCore/QTranslator>
#include "make_unique.hpp"
#include "Application.hpp"
#include "Database.hpp"
#include "Pool.hpp"
#include "UiEngine.hpp"

using namespace Footpal;

struct Application::Impl
{
    typedef std::unique_ptr<Footpal::UiEngine> UiEngine_t;
    UiEngine_t UiEngine;
};

Application::Application(int &argc, char **argv) :
    QApplication(argc, argv),
    m_pimpl(std::make_unique<Application::Impl>())
{

}

Application::~Application() = default;

void Application::InitializeDatabase()
{
    Pool::Database()->Initialize();
}

void Application::SetupUi()
{
    int fontId = QFontDatabase::addApplicationFont(":/fnt/main.ttf");
    QString fontFamily = QFontDatabase::applicationFontFamilies(fontId).at(0);
    QApplication::setFont(fontFamily);

    Footpal::Pool::Translator()->load(QLocale::system().name(), ":/translations/");
    this->installTranslator(Footpal::Pool::Translator());

    m_pimpl->UiEngine =
            std::make_unique<Footpal::UiEngine>();
    m_pimpl->UiEngine->load(QUrl(QStringLiteral("qrc:///ui/main.qml")));

    //QObject *uiRootObject = m_pimpl->UiEngine->rootObjects().first();
}

