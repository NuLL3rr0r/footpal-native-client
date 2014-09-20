/**
 * @author  Mohammad S. Babaei <info@babaei.net>
 */


#include <sstream>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <QtCore/QByteArray>
#include <QtCore/QDebug>
#include <QtCore/QUrlQuery>
#include <QtNetwork/QNetworkConfigurationManager>
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
    Http_t HttpGetCurrentProfile;
    Http_t HttpUsersList;
    Http_t HttpCreateIndividualRoom;
    Http_t HttpGetIndividualContacts;

public:
    Impl(RestApi *parent);

public slots:
    void OnSignUpRequestFailed(const Http::Error &error);
    void OnSignUpRequestCallback(const Ertebat::HttpStatus::HttpStatusCode &statusCode,
                                 const QString &data);

    void OnSignInRequestFailed(const Http::Error &error);
    void OnSignInRequestCallback(const Ertebat::HttpStatus::HttpStatusCode &statusCode,
                                 const QString &data);

    void OnSignOutRequestFailed(const Http::Error &error);
    void OnSignOutRequestCallback(const Ertebat::HttpStatus::HttpStatusCode &statusCode,
                                  const QString &data);

    void OnGetCurrentProfileRequestFailed(const Http::Error &error);
    void OnGetCurrentProfileRequestCallback(const Ertebat::HttpStatus::HttpStatusCode &statusCode,
                                            const QString &data);

    void OnUsersListRequestFailed(const Http::Error &error);
    void OnUsersListRequestCallback(const Ertebat::HttpStatus::HttpStatusCode &statusCode,
                                    const QString &data);

    void OnCreateIndividualRoomRequestFailed(const Http::Error &error);
    void OnCreateIndividualRoomRequestCallback(const Ertebat::HttpStatus::HttpStatusCode &statusCode,
                                               const QString &data);

    void OnGetIndividualContactsRequestFailed(const Http::Error &error);
    void OnGetIndividualContactsRequestCallback(const Ertebat::HttpStatus::HttpStatusCode &statusCode,
                                                const QString &data);

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

bool RestApi::isOnline()
{
    static QNetworkConfigurationManager configurationManager;

    if (!configurationManager.isOnline()) {
        return false;
    }

    return true;
}

void RestApi::signUp(const QString &phone, const QString &email, const QString &pwd)
{
    std::wstringstream stream;
    boost::property_tree::wptree tree;
    //std::string outPwd;

    tree.put(L"phonenumber", phone.toStdWString());
    tree.put(L"email", email.toStdWString());
    //Pool::Crypto()->GenerateHash(pwd.toStdString(), outPwd);
    //tree.put(L"password", QString::fromStdString(outPwd).toStdWString());
    tree.put(L"password", pwd.toStdWString());

    boost::property_tree::write_json(stream, tree);

    QByteArray data;
    data.append(QString::fromStdWString(stream.str()));

    m_pimpl->HttpSignUp->Post("http://www.13x17.org/api/signup", data);
}

void RestApi::signIn(const QString &user, const QString &pwd)
{
    std::wstringstream stream;
    boost::property_tree::wptree tree;
    //std::string outPwd;

    tree.put(L"username", user.toStdWString());
    //Pool::Crypto()->GenerateHash(pwd.toStdString(), outPwd);
    //tree.put(L"password", QString::fromStdString(outPwd).toStdWString());
    tree.put(L"password", pwd.toStdWString());

    boost::property_tree::write_json(stream, tree);

    QByteArray data;
    data.append(QString::fromStdWString(stream.str()));

    m_pimpl->HttpSignIn->Post("http://www.13x17.org/api/signin", data);
}

void RestApi::signOut(const QString &token, const QString &user, const QString &pwd)
{
    std::wstringstream stream;
    boost::property_tree::wptree tree;
    std::string outPwd;

    tree.put(L"username", user.toStdWString());
    Pool::Crypto()->GenerateHash(pwd.toStdString(), outPwd);
    tree.put(L"password", QString::fromStdString(outPwd).toStdWString());

    boost::property_tree::write_json(stream, tree);

    Http::Headers_t headers;
    headers["token"] = token;

    QByteArray data;
    data.append(QString::fromStdWString(stream.str()));

    m_pimpl->HttpSignOut->Post("http://www.13x17.org/api/signout", data, headers);
}

void RestApi::getCurrentProfile(const QString &token)
{
    Http::Headers_t headers;
    headers["token"] = token;

    m_pimpl->HttpGetCurrentProfile->Get("http://www.13x17.org/api/getCurrentProfile", headers);
}

void RestApi::usersList(const QString &token)
{
    Http::Headers_t headers;
    headers["token"] = token;

    m_pimpl->HttpUsersList->Get("http://www.13x17.org/api/userList", headers);
}

void RestApi::createIndividualRoom(const QString &token, const QString &otherParty)
{
    std::wstringstream stream;
    boost::property_tree::wptree tree;

    tree.put(L"otherParty", otherParty.toStdWString());

    boost::property_tree::write_json(stream, tree);

    Http::Headers_t headers;
    headers["token"] = token;

    QByteArray data;
    data.append(QString::fromStdWString(stream.str()));

    m_pimpl->HttpCreateIndividualRoom->Post("http://www.13x17.org/api/createIndividualRoom", data, headers);
}

void RestApi::getIndividualContacts(const QString &token)
{
    std::wstringstream stream;
    boost::property_tree::wptree tree;

    boost::property_tree::write_json(stream, tree);

    Http::Headers_t headers;
    headers["token"] = token;

    QByteArray data;
    data.append(QString::fromStdWString(stream.str()));

    m_pimpl->HttpGetIndividualContacts->Post("http://www.13x17.org/api/getIndividualContacts", data, headers);
}

RestApi::Impl::Impl(RestApi *parent) :
    m_parent(parent),
    HttpSignUp(std::make_unique<Http>()),
    HttpSignIn(std::make_unique<Http>()),
    HttpSignOut(std::make_unique<Http>()),
    HttpGetCurrentProfile(std::make_unique<Http>()),
    HttpUsersList(std::make_unique<Http>()),
    HttpCreateIndividualRoom(std::make_unique<Http>())
{
    SetupEvents();
}

void RestApi::Impl::OnSignUpRequestFailed(const Http::Error &error)
{
    emit m_parent->signal_SignUp(
                static_cast<Ertebat::RestStatusCodes::ConnectionStatus>(error),
                Ertebat::RestStatusCodes::SignUpStatus::SignUp_None,
                QString());
}

void RestApi::Impl::OnSignUpRequestCallback(const Ertebat::HttpStatus::HttpStatusCode &statusCode,
                                            const QString &data)
{
    emit m_parent->signal_SignUp(Ertebat::RestStatusCodes::ConnectionStatus::Connection_OK,
                                 static_cast<Ertebat::RestStatusCodes::SignUpStatus>(statusCode),
                                 data);
}

void RestApi::Impl::OnSignInRequestFailed(const Http::Error &error)
{
    emit m_parent->signal_SignIn(
                static_cast<Ertebat::RestStatusCodes::ConnectionStatus>(error),
                Ertebat::RestStatusCodes::SignInStatus::SignIn_None,
                QString());
}

void RestApi::Impl::OnSignInRequestCallback(const Ertebat::HttpStatus::HttpStatusCode &statusCode,
                                            const QString &data)
{
    (void)data;
    emit m_parent->signal_SignIn(Ertebat::RestStatusCodes::ConnectionStatus::Connection_OK,
                                 static_cast<Ertebat::RestStatusCodes::SignInStatus>(statusCode),
                                 data);
}

void RestApi::Impl::OnSignOutRequestFailed(const Http::Error &error)
{
    emit m_parent->signal_SignOut(
                static_cast<Ertebat::RestStatusCodes::ConnectionStatus>(error),
                Ertebat::RestStatusCodes::SignOutStatus::SignOut_None,
                QString());
}

void RestApi::Impl::OnSignOutRequestCallback(const Ertebat::HttpStatus::HttpStatusCode &statusCode,
                                             const QString &data)
{
    emit m_parent->signal_SignOut(Ertebat::RestStatusCodes::ConnectionStatus::Connection_OK,
                                  static_cast<Ertebat::RestStatusCodes::SignOutStatus>(statusCode),
                                  data);
}

void RestApi::Impl::OnGetCurrentProfileRequestFailed(const Http::Error &error)
{
    emit m_parent->signal_GetCurrentProfile(
                static_cast<Ertebat::RestStatusCodes::ConnectionStatus>(error),
                Ertebat::RestStatusCodes::GetCurrentProfileStatus::GetCurrentProfile_None,
                QString());
}

void RestApi::Impl::OnGetCurrentProfileRequestCallback(const Ertebat::HttpStatus::HttpStatusCode &statusCode,
                                                       const QString &data)
{
    emit m_parent->signal_GetCurrentProfile(Ertebat::RestStatusCodes::ConnectionStatus::Connection_OK,
                                            static_cast<Ertebat::RestStatusCodes::GetCurrentProfileStatus>(statusCode),
                                            data);
}

void RestApi::Impl::OnUsersListRequestFailed(const Http::Error &error)
{
    emit m_parent->signal_UsersList(
                static_cast<Ertebat::RestStatusCodes::ConnectionStatus>(error),
                Ertebat::RestStatusCodes::UsersListStatus::UsersList_None,
                QString());
}

void RestApi::Impl::OnUsersListRequestCallback(const Ertebat::HttpStatus::HttpStatusCode &statusCode,
                                               const QString &data)
{
    emit m_parent->signal_UsersList(Ertebat::RestStatusCodes::ConnectionStatus::Connection_OK,
                                    static_cast<Ertebat::RestStatusCodes::UsersListStatus>(statusCode),
                                    data);
}

void RestApi::Impl::OnCreateIndividualRoomRequestFailed(const Http::Error &error)
{
    emit m_parent->signal_CreateIndividualRoom(
                static_cast<Ertebat::RestStatusCodes::ConnectionStatus>(error),
                Ertebat::RestStatusCodes::CreateIndividualRoomStatus::CreateIndividualRoom_None,
                QString());
}

void RestApi::Impl::OnCreateIndividualRoomRequestCallback(const Ertebat::HttpStatus::HttpStatusCode &statusCode,
                                                          const QString &data)
{
    emit m_parent->signal_CreateIndividualRoom(Ertebat::RestStatusCodes::ConnectionStatus::Connection_OK,
                                               static_cast<Ertebat::RestStatusCodes::CreateIndividualRoomStatus>(statusCode),
                                               data);
}

void RestApi::Impl::OnGetIndividualContactsRequestFailed(const Http::Error &error)
{
    emit m_parent->signal_GetIndividualContacts(
                static_cast<Ertebat::RestStatusCodes::ConnectionStatus>(error),
                Ertebat::RestStatusCodes::GetIndividualContactsStatus::GetIndividualContacts_None,
                QString());
}

void RestApi::Impl::OnGetIndividualContactsRequestCallback(const Ertebat::HttpStatus::HttpStatusCode &statusCode,
                                                           const QString &data)
{
    emit m_parent->signal_GetIndividualContacts(Ertebat::RestStatusCodes::ConnectionStatus::Connection_OK,
                                                static_cast<Ertebat::RestStatusCodes::GetIndividualContactsStatus>(statusCode),
                                                data);
}

void RestApi::Impl::SetupEvents()
{
    connect(HttpSignUp.get(), SIGNAL(Signal_Failed(const Http::Error &)),
            this, SLOT(OnSignUpRequestFailed(const Http::Error &)));
    connect(HttpSignUp.get(), SIGNAL(Signal_Finished(const Ertebat::HttpStatus::HttpStatusCode &, const QString &)),
            this, SLOT(OnSignUpRequestCallback(const Ertebat::HttpStatus::HttpStatusCode &, const QString &)));

    connect(HttpSignIn.get(), SIGNAL(Signal_Failed(const Http::Error &)),
            this, SLOT(OnSignInRequestFailed(const Http::Error &)));
    connect(HttpSignIn.get(), SIGNAL(Signal_Finished(const Ertebat::HttpStatus::HttpStatusCode &, const QString &)),
            this, SLOT(OnSignInRequestCallback(const Ertebat::HttpStatus::HttpStatusCode &, const QString &)));

    connect(HttpSignOut.get(), SIGNAL(Signal_Failed(const Http::Error &)),
            this, SLOT(OnSignOutRequestFailed(const Http::Error &)));
    connect(HttpSignOut.get(), SIGNAL(Signal_Finished(const Ertebat::HttpStatus::HttpStatusCode &, const QString &)),
            this, SLOT(OnSignOutRequestCallback(const Ertebat::HttpStatus::HttpStatusCode &, const QString &)));

    connect(HttpGetCurrentProfile.get(), SIGNAL(Signal_Failed(const Http::Error &)),
            this, SLOT(OnGetCurrentProfileRequestFailed(const Http::Error &)));
    connect(HttpGetCurrentProfile.get(), SIGNAL(Signal_Finished(const Ertebat::HttpStatus::HttpStatusCode &, const QString &)),
            this, SLOT(OnGetCurrentProfileRequestCallback(const Ertebat::HttpStatus::HttpStatusCode &, const QString &)));

    connect(HttpUsersList.get(), SIGNAL(Signal_Failed(const Http::Error &)),
            this, SLOT(OnUsersListRequestFailed(const Http::Error &)));
    connect(HttpUsersList.get(), SIGNAL(Signal_Finished(const Ertebat::HttpStatus::HttpStatusCode &, const QString &)),
            this, SLOT(OnUsersListRequestCallback(const Ertebat::HttpStatus::HttpStatusCode &, const QString &)));

    connect(HttpCreateIndividualRoom.get(), SIGNAL(Signal_Failed(const Http::Error &)),
            this, SLOT(OnCreateIndividualRoomRequestFailed(const Http::Error &)));
    connect(HttpCreateIndividualRoom.get(), SIGNAL(Signal_Finished(const Ertebat::HttpStatus::HttpStatusCode &, const QString &)),
            this, SLOT(OnCreateIndividualRoomRequestCallback(const Ertebat::HttpStatus::HttpStatusCode &, const QString &)));

    connect(HttpGetIndividualContacts.get(), SIGNAL(Signal_Failed(const Http::Error &)),
            this, SLOT(OnGetIndividualContactsRequestFailed(const Http::Error &)));
    connect(HttpGetIndividualContacts.get(), SIGNAL(Signal_Finished(const Ertebat::HttpStatus::HttpStatusCode &, const QString &)),
            this, SLOT(OnGetIndividualContactsRequestCallback(const Ertebat::HttpStatus::HttpStatusCode &, const QString &)));
}

