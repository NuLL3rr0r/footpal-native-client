/**
 * @author  Mohammad S. Babaei <info@babaei.net>
 */


#include <QtCore/QDebug>
#include <QtGui/QFont>
#include <QtGui/QFontDatabase>
#include <QtCore/QTranslator>
#include "make_unique.hpp"
#include "Application.hpp"
#include "Database.hpp"
#include "Pool.hpp"
#include "UiEngine.hpp"

using namespace Ertebat;

struct Application::Impl
{
    typedef std::unique_ptr<Ertebat::UiEngine> UiEngine_t;
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

void Application::OnEnglishLanguageSelected()
{
    this->removeTranslator(Pool::Translator());
    Pool::Translator()->load("en", ":/translations/");
    this->installTranslator(Pool::Translator());
    emit m_pimpl->UiEngine->signal_LanguageChanged();
}

void Application::OnFarsiLanguageSelected()
{
    this->removeTranslator(Pool::Translator());
    Pool::Translator()->load("fa", ":/translations/");
    this->installTranslator(Pool::Translator());
    emit m_pimpl->UiEngine->signal_LanguageChanged();
}

void Application::SetupUi()
{
    int fontId = QFontDatabase::addApplicationFont(":/fnt/main.ttf");
    QString fontFamily = QFontDatabase::applicationFontFamilies(fontId).at(0);
    QApplication::setFont(fontFamily);

    Ertebat::Pool::Translator()->load(QLocale::system().name(), ":/translations/");
    this->installTranslator(Ertebat::Pool::Translator());

    m_pimpl->UiEngine =
            std::make_unique<Ertebat::UiEngine>();
    m_pimpl->UiEngine->load(QUrl(QStringLiteral("qrc:///ui/Main.qml")));

    QObject *uiRootObject = m_pimpl->UiEngine->rootObjects().first();

    QObject::connect(uiRootObject, SIGNAL(signal_englishLanguageSelected()),
                     this, SLOT(OnEnglishLanguageSelected()));
    QObject::connect(uiRootObject, SIGNAL(signal_farsiLanguageSelected()),
                     this, SLOT(OnFarsiLanguageSelected()));
}

