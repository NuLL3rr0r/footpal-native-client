#ifndef FOOTPAL_REST_API_HPP
#define FOOTPAL_REST_API_HPP


#include <memory>
#include <QtCore/QObject>
#include "RestApiDefs.hpp"

namespace Footpal {
class RestApi;
}

class Footpal::RestApi : public QObject
{
    Q_OBJECT

private:
    class Impl;
    std::unique_ptr<Impl> m_pimpl;

public:
    Q_INVOKABLE RestApi();
    virtual ~RestApi();

signals:
    void signal_SignUp(const Footpal::RestApiDefs::SignUpResult &);
    void signal_SignIn(const Footpal::RestApiDefs::SignInResult &);
    void signal_SignOut(const Footpal::RestApiDefs::SignOutResult &);
    void signal_GetCurrentProfile(const Footpal::RestApiDefs::GetCurrentProfileResult &);
    void signal_UsersList(const Footpal::RestApiDefs::UsersListResult &);
    void signal_CreateIndividualRoom(const Footpal::RestApiDefs::CreateIndividualRoomResult &);

public:
    Q_INVOKABLE void signUp(const QString &phone, const QString &email, const QString &pwd);
    Q_INVOKABLE void signIn(const QString &user, const QString &pwd);
    Q_INVOKABLE void signOut(const QString &user, const QString &pwd);
    Q_INVOKABLE void getCurrentProfile();
    Q_INVOKABLE void usersList();
    Q_INVOKABLE void createIndividualRoom();
};


#endif /* FOOTPAL_REST_API_HPP */

