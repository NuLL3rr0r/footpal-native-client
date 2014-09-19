/**
 * @author  Mohammad S. Babaei <info@babaei.net>
 */


#ifndef ERTEBAT_REST_API_HPP
#define ERTEBAT_REST_API_HPP


#include <memory>
#include <QtCore/QObject>
class QString;

namespace Ertebat {
class RestApi;
class RestStatusCodes;
}

class Ertebat::RestStatusCodes : public QObject {
    Q_OBJECT

    Q_ENUMS( ConnectionStatus )
    Q_ENUMS( SignUpStatus )
    Q_ENUMS( SignInStatus )
    Q_ENUMS( SignOutStatus )
    Q_ENUMS( GetCurrentProfileStatus )
    Q_ENUMS( UsersListStatus )
    Q_ENUMS( CreateIndividualRoomStatus )

public:
    enum ConnectionStatus {
        Connection_OK = 0,
        Connection_AuthenticationRequired = 1,
        Connection_ConnectionFailed = 2,
        Connection_ProxyAuthenticationRequired = 3,
        Connection_SslError = 4
    };

    enum SignUpStatus {
        SignUp_None = 0,
        SignUp_Created = 201,
        SignUp_BadRequest = 400,
        SignUp_Conflict = 409
    };

    enum SignInStatus {
        SignIn_None = 0,
        SignIn_Accepted = 202,
        SignIn_BadRequest = 400,
        SignIn_Forbidden = 403,
        SignIn_NotFound = 404,
        SignIn_NotAcceptable = 406
    };

    enum SignOutStatus {
        SignOut_None = 0,
        SignOut_Accepted = 202,
        SignOut_BadRequest = 400,
        SignOut_Unauthorized = 401,
    };

    enum GetCurrentProfileStatus {
        GetCurrentProfile_None = 0,
        GetCurrentProfile_Found = 302,
        GetCurrentProfile_Unauthorized = 401,
        GetCurrentProfile_Forbidden = 403,
        GetCurrentProfile_NotFound = 404
    };

    enum UsersListStatus {
        UsersList_None = 0,
        UsersList_OK = 200,
        UsersList_Unauthorized = 401,
        UsersList_Forbidden = 403
    };

    enum CreateIndividualRoomStatus {
        CreateIndividualRoom_None = 0,
        CreateIndividualRoom_Created = 201,
        CreateIndividualRoom_BadRequest = 400,
        CreateIndividualRoom_Unauthorized = 401,
        CreateIndividualRoom_NotFound = 404
    };

    enum GetIndividualContactsStatus {
        GetIndividualContacts_None = 0,
        GetIndividualContacts_OK = 200,
        GetIndividualContacts_BadRequest = 400,
        GetIndividualContacts_Unauthorized = 401,
        GetIndividualContacts_NotFound = 404
    };
};

class Ertebat::RestApi : public QObject
{
    Q_OBJECT

private:
    class Impl;
    std::unique_ptr<Impl> m_pimpl;

public:
    Q_INVOKABLE RestApi();
    virtual ~RestApi();

signals:
    void signal_SignUp(Ertebat::RestStatusCodes::ConnectionStatus,
                       Ertebat::RestStatusCodes::SignUpStatus,
                       const QString &data);
    void signal_SignIn(Ertebat::RestStatusCodes::ConnectionStatus,
                       Ertebat::RestStatusCodes::SignInStatus,
                       const QString &data);
    void signal_SignOut(Ertebat::RestStatusCodes::ConnectionStatus,
                        Ertebat::RestStatusCodes::SignOutStatus,
                        const QString &data);

    void signal_GetCurrentProfile(Ertebat::RestStatusCodes::ConnectionStatus,
                                  Ertebat::RestStatusCodes::GetCurrentProfileStatus,
                                  const QString &data);
    void signal_UsersList(Ertebat::RestStatusCodes::ConnectionStatus,
                          Ertebat::RestStatusCodes::UsersListStatus,
                          const QString &data);

    void signal_CreateIndividualRoom(Ertebat::RestStatusCodes::ConnectionStatus,
                                     Ertebat::RestStatusCodes::CreateIndividualRoomStatus,
                                     const QString &data);

    void signal_GetIndividualContacts(Ertebat::RestStatusCodes::ConnectionStatus,
                                      Ertebat::RestStatusCodes::GetIndividualContactsStatus,
                                      const QString &data);

public:
    Q_INVOKABLE bool isOnline();

    Q_INVOKABLE void signUp(const QString &phone, const QString &email, const QString &pwd);
    Q_INVOKABLE void signIn(const QString &user, const QString &pwd);
    Q_INVOKABLE void signOut(const QString &token, const QString &user, const QString &pwd);

    Q_INVOKABLE void getCurrentProfile(const QString &token);
    Q_INVOKABLE void usersList(const QString &token);

    Q_INVOKABLE void createIndividualRoom(const QString &token, const QString &otherParty);

    Q_INVOKABLE void getIndividualContacts(const QString &token);
};


#endif /* ERTEBAT_REST_API_HPP */

