#include "make_unique.hpp"
#include "Application.hpp"
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

void Application::SetupUi()
{
    m_pimpl->UiEngine =
            std::make_unique<Footpal::UiEngine>();
    m_pimpl->UiEngine->load(QUrl(QStringLiteral("qrc:///ui/main.qml")));
}

