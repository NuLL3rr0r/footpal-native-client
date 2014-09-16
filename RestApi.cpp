/**
 * @author  Mohammad S. Babaei <info@babaei.net>
 */


#include <sstream>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <QtCore/QByteArray>
#include <QtCore/QDebug>
#include <QtCore/QUrlQuery>
#include "make_unique.hpp"
#include "RestApi.hpp"
#include "Crypto.hpp"
#include "Http.hpp"
#include "Pool.hpp"

using namespace std;
using namespace  Ertebat;

class RestApi::Impl : public QObject
{
    Q_OBJECT
private:
    RestApi *m_parent;

public:
    typedef std::unique_ptr<Http> Http_t;

public:
    Http_t HttpSignUp;
    Http_t HttpSignIn;
    Http_t HttpSignOut;

public:
    Impl(RestApi *parent);

public slots:
    void onSignUpCallback(const QString &response);
    void onSignInCallback(const QString &response);

public:
    void SetupEvents();
};

#include "RestApi.moc" // this is a necessitas to QObject-based-Impl

RestApi::RestApi()
    : QObject(),
      m_pimpl(std::make_unique<RestApi::Impl>(this))
{

}

RestApi::~RestApi() = default;

void RestApi::signUp(const QString &phone, const QString &email, const QString &pwd)
{
    std::wstringstream stream;
    boost::property_tree::wptree tree;
    std::string outPwd;

    tree.put(L"phonenumber", phone.toStdWString());
    tree.put(L"email", email.toStdWString());
    Pool::Crypto()->Encrypt(pwd.toStdString(), outPwd);
    tree.put(L"password", QString::fromStdString(outPwd).toStdWString());

    boost::property_tree::write_json(stream, tree);

    QByteArray data;
    data.append(QString::fromStdWString(stream.str()));
    m_pimpl->HttpSignUp->Post("http://www.13x17.org/api/signup", data);
}

void RestApi::signIn(const QString &user, const QString &pwd)
{
    std::wstringstream stream;
    boost::property_tree::wptree tree;
    std::string outPwd;

    tree.put(L"username", user.toStdWString());
    Pool::Crypto()->Encrypt(pwd.toStdString(), outPwd);
    tree.put(L"password", QString::fromStdString(outPwd).toStdWString());

    boost::property_tree::write_json(stream, tree);

    QByteArray data;
    data.append(QString::fromStdWString(stream.str()));
    m_pimpl->HttpSignIn->Post("http://www.13x17.org/api/signin", data);
}

void RestApi::signOut(const QString &user, const QString &pwd)
{

}

void RestApi::getCurrentProfile()
{

}

void RestApi::usersList()
{

}

void RestApi::createIndividualRoom()
{

}

RestApi::Impl::Impl(RestApi *parent) :
    m_parent(parent),
    HttpSignUp(std::make_unique<Http>()),
    HttpSignIn(std::make_unique<Http>()),
    HttpSignOut(std::make_unique<Http>())
{
    SetupEvents();
}

void RestApi::Impl::onSignUpCallback(const QString &response)
{
    qDebug() << 1;
    emit m_parent->signal_SignUp(response);
}

void RestApi::Impl::onSignInCallback(const QString &response)
{
    qDebug() << 2;
    emit m_parent->signal_SignIn(response);
}

void RestApi::Impl::SetupEvents()
{
    connect(HttpSignUp.get(), SIGNAL(Signal_Finished(const QString &)),
            this, SLOT(onSignUpCallback(const QString &)));
    connect(HttpSignUp.get(), SIGNAL(Signal_Error(const QString &)),
            this, SLOT(onSignUpCallback(const QString &)));

    connect(HttpSignIn.get(), SIGNAL(Signal_Finished(const QString &)),
            this, SLOT(onSignInCallback(const QString &)));
    connect(HttpSignIn.get(), SIGNAL(Signal_Error(const QString &)),
            this, SLOT(onSignInCallback(const QString &)));
}

