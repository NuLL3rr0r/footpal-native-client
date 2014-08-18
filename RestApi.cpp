#include <QtCore/QByteArray>
#include <QtCore/QDebug>
#include <QtCore/QUrlQuery>
#include "make_unique.hpp"
#include "RestApi.hpp"
#include "Http.hpp"

using namespace std;
using namespace  Footpal;

class RestApi::Impl : public QObject
{
    Q_OBJECT
private:
    RestApi *m_parent;

public:
    typedef std::unique_ptr<Http> Http_t;

public:
    Http_t HttpSignUp;

public:
    Impl(RestApi *parent);

public slots:
    void onSignUpCallback(const QString &response);

public:
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
    connect(m_pimpl->HttpSignUp.get(), SIGNAL(Signal_Finished(const QString &)),
            m_pimpl.get(), SLOT(onSignUpCallback(const QString &)));
    connect(m_pimpl->HttpSignUp.get(), SIGNAL(Signal_Error(const QString &)),
            m_pimpl.get(), SLOT(onSignUpCallback(const QString &)));

    QUrlQuery params;
    params.addQueryItem("phonenumber", phone);
    params.addQueryItem("password", email);
    params.addQueryItem("email", pwd);
    QByteArray data;
    data.append(params.toString());
    m_pimpl->HttpSignUp->Post("http://www.13x17.org/api/signup", data);
}

void RestApi::signIn(const QString &user, const QString &pwd)
{

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
    HttpSignUp(std::make_unique<Http>())
{

}

void RestApi::Impl::onSignUpCallback(const QString &response)
{
    qDebug() << response;
    emit m_parent->signal_SignUp(response);
}


