/**
 * @author  Mohammad S. Babaei <info@babaei.net>
 */


#ifndef ERTEBAT_APPLICATION_HPP
#define ERTEBAT_APPLICATION_HPP


#include <memory>
#include <QtWidgets/QApplication>

namespace Ertebat {
class Application;
}

class Ertebat::Application : public QApplication
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
    void InitializeDatabase();
    void SetupUi();
};

#endif /* APPLICATION_HPP */
