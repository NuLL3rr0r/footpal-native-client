#ifndef ERTEBAT_REST_API_HPP
#define ERTEBAT_REST_API_HPP


#include <memory>
#include <QtCore/QObject>
class QString;

namespace Ertebat {
class RestApi;
}

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
    void signal_SignUp(const QString &);
    void signal_SignIn(const QString &);
    void signal_SignOut(const QString &);
    void signal_GetCurrentProfile(const QString &);
    void signal_UsersList(const QString &);
    void signal_CreateIndividualRoom(const QString &);

public:
    Q_INVOKABLE void signUp(const QString &phone, const QString &email, const QString &pwd);
    Q_INVOKABLE void signIn(const QString &user, const QString &pwd);
    Q_INVOKABLE void signOut(const QString &user, const QString &pwd);
    Q_INVOKABLE void getCurrentProfile();
    Q_INVOKABLE void usersList();
    Q_INVOKABLE void createIndividualRoom();
};


#endif /* ERTEBAT_REST_API_HPP */

