#ifndef Footpal_APPLICATION_HPP
#define Footpal_APPLICATION_HPP


#include <memory>
#include <QApplication>

namespace Footpal {
class Application;
}

class Footpal::Application : public QApplication
{
    Q_OBJECT

private:
    struct Impl;
    std::unique_ptr<Impl> m_pimpl;

public:
    Application(int &argc, char **argv);
    virtual ~Application();

signals:

public slots:

public:
    void SetupUi();
};

#endif // APPLICATION_HPP
