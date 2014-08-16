#include "make_unique.hpp"
#include "RestApi.hpp"

using namespace std;
using namespace  Footpal;

class RestApi::Impl : public QObject
{
    Q_OBJECT

public slots:

public:
};

#include "RestApi.moc" // this is a necessitas to QObject-based-Impl

RestApi::RestApi()
    : QObject(),
      m_pimpl(std::make_unique<RestApi::Impl>())
{

}

RestApi::~RestApi() = default;

void RestApi::signUp(const QString &phone, const QString &email, const QString &pwd)
{

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

