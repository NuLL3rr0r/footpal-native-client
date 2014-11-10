/**
 * @author  Mohammad S. Babaei <info@babaei.net>
 * @author  Majid Sadeghi Alavijeh <majid.sadeghi.alavijeh@gmail.com>
 */


#include <sstream>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <QtCore/QByteArray>
#include <QtCore/QDebug>
#include <QtCore/QFile>
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

    Http_t Http_FS_CreateDirectory;
    Http_t Http_FS_GetListOfEntity;
    Http_t Http_FS_GetAccessType;
    Http_t Http_FS_GetParentId;
    Http_t Http_FS_MoveEntity;
    Http_t Http_FS_DeleteEntity;
    Http_t Http_FS_Download;
    Http_t Http_FS_DownloadUrl;
    Http_t Http_FS_Upload;
    Http_t Http_FS_AddAccessRole;
    Http_t Http_FS_RemoveAccessRole;
    Http_t Http_FS_ChangeAccessType;
    Http_t Http_FS_GetSharedEntities;

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

    void On_FS_CreateDirectoryRequestFailed(const Http::Error &error);
    void On_FS_CreateDirectoryRequestCallback(const Ertebat::HttpStatus::HttpStatusCode &statusCode,
                                              const QString &data);
    void On_FS_GetListOfEntityRequestFailed(const Http::Error &error);
    void On_FS_GetListOfEntityRequestCallback(const Ertebat::HttpStatus::HttpStatusCode &statusCode,
                                              const QString &data);
    void On_FS_GetAccessTypeRequestFailed(const Http::Error &error);
    void On_FS_GetAccessTypeRequestCallback(const Ertebat::HttpStatus::HttpStatusCode &statusCode,
                                            const QString &data);
    void On_FS_GetParentIdRequestFailed(const Http::Error &error);
    void On_FS_GetParentIdRequestCallback(const Ertebat::HttpStatus::HttpStatusCode &statusCode,
                                          const QString &data);
    void On_FS_MoveEntityRequestFailed(const Http::Error &error);
    void On_FS_MoveEntityRequestCallback(const Ertebat::HttpStatus::HttpStatusCode &statusCode,
                                         const QString &data);
    void On_FS_DeleteEntityRequestFailed(const Http::Error &error);
    void On_FS_DeleteEntityRequestCallback(const Ertebat::HttpStatus::HttpStatusCode &statusCode,
                                           const QString &data);
    void On_FS_DownloadRequestFailed(const Http::Error &error);
    void On_FS_DownloadRequestCallback(const Ertebat::HttpStatus::HttpStatusCode &statusCode,
                                       const QString &data);
    void On_FS_DownloadUrlRequestFailed(const Http::Error &error);
    void On_FS_DownloadUrlRequestCallback(const Ertebat::HttpStatus::HttpStatusCode &statusCode,
                                          const QString &data);
    void On_FS_UploadRequestFailed(const Http::Error &error);
    void On_FS_UploadRequestCallback(const Ertebat::HttpStatus::HttpStatusCode &statusCode,
                                     const QString &data);
    void On_FS_AddAccessRoleRequestFailed(const Http::Error &error);
    void On_FS_AddAccessRoleRequestCallback(const Ertebat::HttpStatus::HttpStatusCode &statusCode,
                                            const QString &data);
    void On_FS_RemoveAccessRoleRequestFailed(const Http::Error &error);
    void On_FS_RemoveAccessRoleRequestCallback(const Ertebat::HttpStatus::HttpStatusCode &statusCode,
                                               const QString &data);
    void On_FS_ChangeAccessTypeRequestFailed(const Http::Error &error);
    void On_FS_ChangeAccessTypeRequestCallback(const Ertebat::HttpStatus::HttpStatusCode &statusCode,
                                               const QString &data);
    void On_FS_GetSharedEntitiesRequestFailed(const Http::Error &error);
    void On_FS_GetSharedEntitiesRequestCallback(const Ertebat::HttpStatus::HttpStatusCode &statusCode,
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

    tree.put(L"phonenumber", phone.toStdWString());
    tree.put(L"email", email.toStdWString());
    tree.put(L"password", pwd.toStdWString());

    boost::property_tree::write_json(stream, tree);

    QByteArray data;
    data.append(QString::fromStdWString(stream.str()));

    m_pimpl->HttpSignUp->Post(QString("%1/api/signup").arg(REST_BASE_URL), data);
}

void RestApi::signIn(const QString &user, const QString &pwd)
{
    std::wstringstream stream;
    boost::property_tree::wptree tree;

    tree.put(L"username", user.toStdWString());
    tree.put(L"password", pwd.toStdWString());

    boost::property_tree::write_json(stream, tree);

    QByteArray data;
    data.append(QString::fromStdWString(stream.str()));

    m_pimpl->HttpSignIn->Post(QString("%1/api/signin").arg(REST_BASE_URL), data);
}

void RestApi::signOut(const QString &token, const QString &user, const QString &pwd)
{
    std::wstringstream stream;
    boost::property_tree::wptree tree;

    tree.put(L"username", user.toStdWString());
    tree.put(L"password", pwd.toStdWString());

    boost::property_tree::write_json(stream, tree);

    Http::Headers_t headers;
    headers["token"] = token;

    QByteArray data;
    data.append(QString::fromStdWString(stream.str()));

    m_pimpl->HttpSignOut->Post(QString("%1/api/signout").arg(REST_BASE_URL), data, headers);
}

void RestApi::getCurrentProfile(const QString &token)
{
    Http::Headers_t headers;
    headers["token"] = token;

    m_pimpl->HttpGetCurrentProfile->Get(QString("%1/api/getCurrentProfile").arg(REST_BASE_URL), headers);
}

void RestApi::usersList(const QString &token)
{
    Http::Headers_t headers;
    headers["token"] = token;

    m_pimpl->HttpUsersList->Get(QString("%1/api/userList").arg(REST_BASE_URL), headers);
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

    m_pimpl->HttpCreateIndividualRoom->Post(QString("%1/chat/createIndividualRoom").arg(REST_BASE_URL), data, headers);
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

    m_pimpl->HttpGetIndividualContacts->Post(QString("%1/chat/getIndividualContacts").arg(REST_BASE_URL), data, headers);
}

void RestApi::fs_CreateDirectory(const QString &token, const QString &dirName, const QString &access, const QString &parentId)
{
    std::wstringstream stream;
    boost::property_tree::wptree tree;

    tree.put(L"dirName", dirName.toStdWString());
    tree.put(L"access", access.toStdWString());
    tree.put(L"parentId", parentId.toStdWString());

    boost::property_tree::write_json(stream, tree);

    Http::Headers_t headers;
    headers["token"] = token;

    QByteArray data;
    data.append(QString::fromStdWString(stream.str()));

    m_pimpl->Http_FS_CreateDirectory->Post(QString("%1/document/createDirectory").arg(REST_BASE_URL), data, headers);
}

void RestApi::fs_GetListOfEntity(const QString &token)
{
    Http::Headers_t headers;
    headers["token"] = token;

    m_pimpl->Http_FS_GetListOfEntity->Get(QString("%1/document/getListOfEntity").arg(REST_BASE_URL), headers);
}

void RestApi::fs_GetListOfEntity(const QString &token, const QString &entityId)
{
    Http::Headers_t headers;
    headers["token"] = token;

    m_pimpl->Http_FS_GetListOfEntity->Get(QString("%1/document/getListOfEntity/%2").arg(REST_BASE_URL).arg(entityId),
                                          headers);
}

void RestApi::fs_GetAccessType(const QString &token, const QString &entityId)
{
    Http::Headers_t headers;
    headers["token"] = token;

    m_pimpl->Http_FS_GetAccessType->Get(QString("%1/document/getAccessType/%2").arg(REST_BASE_URL).arg(entityId),
                                        headers);
}

void RestApi::fs_GetParentId(const QString &token, const QString &entityId)
{
    Http::Headers_t headers;
    headers["token"] = token;

    m_pimpl->Http_FS_GetParentId->Get(QString("%1/document/getParentId/%2").arg(REST_BASE_URL).arg(entityId),
                                      headers);
}

void RestApi::fs_MoveEntity(const QString &token, const QString &entityId, const QString &newParentId)
{
    std::wstringstream stream;
    boost::property_tree::wptree tree;

    tree.put(L"entityId", entityId.toStdWString());
    tree.put(L"newParentId", newParentId.toStdWString());

    boost::property_tree::write_json(stream, tree);

    Http::Headers_t headers;
    headers["token"] = token;

    QByteArray data;
    data.append(QString::fromStdWString(stream.str()));

    m_pimpl->Http_FS_MoveEntity->Post(QString("%1/document/moveEntity").arg(REST_BASE_URL), data, headers);
}

void RestApi::fs_DeleteEntity(const QString &token, const QString &entityId)
{
    std::wstringstream stream;
    boost::property_tree::wptree tree;

    tree.put(L"entityId", entityId.toStdWString());

    boost::property_tree::write_json(stream, tree);

    Http::Headers_t headers;
    headers["token"] = token;

    QByteArray data;
    data.append(QString::fromStdWString(stream.str()));

    m_pimpl->Http_FS_DeleteEntity->Post(QString("%1/document/deleteEntity").arg(REST_BASE_URL), data, headers);
}

void RestApi::fs_Download(const QString &token, const QString &entityId)
{
    Http::Headers_t headers;
    headers["token"] = token;

    m_pimpl->Http_FS_Download->Get(QString("%1/document/download/%2").arg(REST_BASE_URL).arg(entityId),
                                   headers);
}

void RestApi::fs_DownloadUrl(const QString &token, const QString &entityId)
{
    Http::Headers_t headers;
    headers["token"] = token;

    m_pimpl->Http_FS_DownloadUrl->Get(QString("%1/document/downloadUrl/%2").arg(REST_BASE_URL).arg(entityId),
                                      headers);
}

void RestApi::fs_Upload(const QString &token, const QString &parentId, const QString &access, const QString &localFilePath)
{
    QFile file(localFilePath);
    if (file.exists() && file.open(QIODevice::ReadOnly)) {
        QString boundary="----ERTEBAT";

        Http::Headers_t headers;
        headers["token"] = token;
        headers["Content-Type"] = QString("multipart/form-data; boundary=%1").arg(boundary);

        QByteArray data;
        QByteArray fileData;
        fileData.append(file.readAll());

        data.append(QString("\r\n--%1\r\n").arg(boundary).toLatin1());
        data.append(QString("Content-Disposition: form-data; name=\"file\"; filename=\"%1\"\r\n").arg(file.fileName()));
        data.append("Content-Type: application/octet-stream\r\n\r\n");
        data.append(fileData);
        data.append("\r\n--" + boundary + "--\r\n");

        m_pimpl->Http_FS_Upload->Post(QString("%1/document/uploadFile/%2/%3").arg(REST_BASE_URL).arg(parentId).arg(access),
                                      data, headers);
    }
}

void RestApi::fs_AddAccessRole(const QString &token, const QString &entityId, const QString &access)
{
    std::wstringstream stream;
    boost::property_tree::wptree tree;

    boost::property_tree::write_json(stream, tree);

    Http::Headers_t headers;
    headers["token"] = token;

    QByteArray data;
    data.append(QString::fromStdWString(stream.str()));

    m_pimpl->Http_FS_AddAccessRole->Post(QString("%1/document/addAccessRole/%2/%3").arg(REST_BASE_URL).arg(entityId).arg(access),
                                         data, headers);
}

void RestApi::fs_RemoveAccessRole(const QString &token, const QString &entityId, const QString &access)
{
    std::wstringstream stream;
    boost::property_tree::wptree tree;

    boost::property_tree::write_json(stream, tree);

    Http::Headers_t headers;
    headers["token"] = token;

    QByteArray data;
    data.append(QString::fromStdWString(stream.str()));

    m_pimpl->Http_FS_AddAccessRole->Post(QString("%1/document/removeAccessRole/%2/%3").arg(REST_BASE_URL).arg(entityId).arg(access),
                                         data, headers);
}

void RestApi::fs_ChangeAccessType(const QString &token, const QString &entityId, const QString &access)
{
    std::wstringstream stream;
    boost::property_tree::wptree tree;

    boost::property_tree::write_json(stream, tree);

    Http::Headers_t headers;
    headers["token"] = token;

    QByteArray data;
    data.append(QString::fromStdWString(stream.str()));

    m_pimpl->Http_FS_ChangeAccessType->Post(QString("%1/document/changeAccessType/%2/%3").arg(REST_BASE_URL).arg(entityId).arg(access),
                                            data, headers);
}

void RestApi::fs_GetSharedEntities(const QString &token, const QString &user)
{
    Http::Headers_t headers;
    headers["token"] = token;

    m_pimpl->Http_FS_GetSharedEntities->Get(QString("%1/document/getSharedEntities/%2").arg(REST_BASE_URL).arg(user), headers);
}

RestApi::Impl::Impl(RestApi *parent) :
    m_parent(parent),
    HttpSignUp(std::make_unique<Http>()),
    HttpSignIn(std::make_unique<Http>()),
    HttpSignOut(std::make_unique<Http>()),
    HttpGetCurrentProfile(std::make_unique<Http>()),
    HttpUsersList(std::make_unique<Http>()),
    HttpCreateIndividualRoom(std::make_unique<Http>()),
    HttpGetIndividualContacts(std::make_unique<Http>()),
    Http_FS_CreateDirectory(std::make_unique<Http>()),
    Http_FS_GetListOfEntity(std::make_unique<Http>()),
    Http_FS_GetAccessType(std::make_unique<Http>()),
    Http_FS_GetParentId(std::make_unique<Http>()),
    Http_FS_MoveEntity(std::make_unique<Http>()),
    Http_FS_DeleteEntity(std::make_unique<Http>()),
    Http_FS_Download(std::make_unique<Http>()),
    Http_FS_DownloadUrl(std::make_unique<Http>()),
    Http_FS_Upload(std::make_unique<Http>()),
    Http_FS_AddAccessRole(std::make_unique<Http>()),
    Http_FS_RemoveAccessRole(std::make_unique<Http>()),
    Http_FS_ChangeAccessType(std::make_unique<Http>()),
    Http_FS_GetSharedEntities(std::make_unique<Http>())
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

void RestApi::Impl::On_FS_CreateDirectoryRequestFailed(const Http::Error &error)
{
    emit m_parent->signal_FS_CreateDirectory(
                static_cast<Ertebat::RestStatusCodes::ConnectionStatus>(error),
                Ertebat::RestStatusCodes::FS_CreateDirectoryStatus::FS_CreateDirectory_None,
                QString());
}

void RestApi::Impl::On_FS_CreateDirectoryRequestCallback(const Ertebat::HttpStatus::HttpStatusCode &statusCode,
                                                         const QString &data)
{
    emit m_parent->signal_FS_CreateDirectory(Ertebat::RestStatusCodes::ConnectionStatus::Connection_OK,
                                             static_cast<Ertebat::RestStatusCodes::FS_CreateDirectoryStatus>(statusCode),
                                             data);
}

void RestApi::Impl::On_FS_GetListOfEntityRequestFailed(const Http::Error &error)
{
    emit m_parent->signal_FS_GetListOfEntity(
                static_cast<Ertebat::RestStatusCodes::ConnectionStatus>(error),
                Ertebat::RestStatusCodes::FS_GetListOfEntityStatus::FS_GetListOfEntity_None,
                QString());
}

void RestApi::Impl::On_FS_GetListOfEntityRequestCallback(const Ertebat::HttpStatus::HttpStatusCode &statusCode,
                                                         const QString &data)
{
    emit m_parent->signal_FS_GetListOfEntity(Ertebat::RestStatusCodes::ConnectionStatus::Connection_OK,
                                             static_cast<Ertebat::RestStatusCodes::FS_GetListOfEntityStatus>(statusCode),
                                             data);
}

void RestApi::Impl::On_FS_GetAccessTypeRequestFailed(const Http::Error &error)
{
    emit m_parent->signal_FS_GetAccessType(
                static_cast<Ertebat::RestStatusCodes::ConnectionStatus>(error),
                Ertebat::RestStatusCodes::FS_GetAccessTypeStatus::FS_GetAccessType_None,
                QString());
}

void RestApi::Impl::On_FS_GetAccessTypeRequestCallback(const Ertebat::HttpStatus::HttpStatusCode &statusCode,
                                                       const QString &data)
{
    emit m_parent->signal_FS_GetAccessType(Ertebat::RestStatusCodes::ConnectionStatus::Connection_OK,
                                           static_cast<Ertebat::RestStatusCodes::FS_GetAccessTypeStatus>(statusCode),
                                           data);
}

void RestApi::Impl::On_FS_GetParentIdRequestFailed(const Http::Error &error)
{
    emit m_parent->signal_FS_GetParentId(
                static_cast<Ertebat::RestStatusCodes::ConnectionStatus>(error),
                Ertebat::RestStatusCodes::FS_GetParentIdStatus::FS_GetParentId_None,
                QString());
}

void RestApi::Impl::On_FS_GetParentIdRequestCallback(const Ertebat::HttpStatus::HttpStatusCode &statusCode,
                                                     const QString &data)
{
    emit m_parent->signal_FS_GetParentId(Ertebat::RestStatusCodes::ConnectionStatus::Connection_OK,
                                         static_cast<Ertebat::RestStatusCodes::FS_GetParentIdStatus>(statusCode),
                                         data);
}

void RestApi::Impl::On_FS_MoveEntityRequestFailed(const Http::Error &error)
{
    emit m_parent->signal_FS_MoveEntity(
                static_cast<Ertebat::RestStatusCodes::ConnectionStatus>(error),
                Ertebat::RestStatusCodes::FS_MoveEntityStatus::FS_MoveEntity_None,
                QString());
}

void RestApi::Impl::On_FS_MoveEntityRequestCallback(const Ertebat::HttpStatus::HttpStatusCode &statusCode,
                                                    const QString &data)
{
    emit m_parent->signal_FS_MoveEntity(Ertebat::RestStatusCodes::ConnectionStatus::Connection_OK,
                                        static_cast<Ertebat::RestStatusCodes::FS_MoveEntityStatus>(statusCode),
                                        data);
}

void RestApi::Impl::On_FS_DeleteEntityRequestFailed(const Http::Error &error)
{
    emit m_parent->signal_FS_DeleteEntity(
                static_cast<Ertebat::RestStatusCodes::ConnectionStatus>(error),
                Ertebat::RestStatusCodes::FS_DeleteEntityStatus::FS_DeleteEntity_None,
                QString());
}

void RestApi::Impl::On_FS_DeleteEntityRequestCallback(const Ertebat::HttpStatus::HttpStatusCode &statusCode,
                                                      const QString &data)
{
    emit m_parent->signal_FS_DeleteEntity(Ertebat::RestStatusCodes::ConnectionStatus::Connection_OK,
                                          static_cast<Ertebat::RestStatusCodes::FS_DeleteEntityStatus>(statusCode),
                                          data);
}

void RestApi::Impl::On_FS_DownloadRequestFailed(const Http::Error &error)
{
    emit m_parent->signal_FS_Download(
                static_cast<Ertebat::RestStatusCodes::ConnectionStatus>(error),
                Ertebat::RestStatusCodes::FS_DownloadStatus::FS_Download_None,
                QString());
}

void RestApi::Impl::On_FS_DownloadRequestCallback(const Ertebat::HttpStatus::HttpStatusCode &statusCode,
                                                  const QString &data)
{
    emit m_parent->signal_FS_Download(Ertebat::RestStatusCodes::ConnectionStatus::Connection_OK,
                                      static_cast<Ertebat::RestStatusCodes::FS_DownloadStatus>(statusCode),
                                      data);
}

void RestApi::Impl::On_FS_DownloadUrlRequestFailed(const Http::Error &error)
{
    emit m_parent->signal_FS_DownloadUrl(
                static_cast<Ertebat::RestStatusCodes::ConnectionStatus>(error),
                Ertebat::RestStatusCodes::FS_DownloadUrlStatus::FS_DownloadUrl_None,
                QString());
}

void RestApi::Impl::On_FS_DownloadUrlRequestCallback(const Ertebat::HttpStatus::HttpStatusCode &statusCode,
                                                     const QString &data)
{
    emit m_parent->signal_FS_DownloadUrl(Ertebat::RestStatusCodes::ConnectionStatus::Connection_OK,
                                         static_cast<Ertebat::RestStatusCodes::FS_DownloadUrlStatus>(statusCode),
                                         data);
}

void RestApi::Impl::On_FS_UploadRequestFailed(const Http::Error &error)
{
    emit m_parent->signal_FS_Upload(
                static_cast<Ertebat::RestStatusCodes::ConnectionStatus>(error),
                Ertebat::RestStatusCodes::FS_UploadStatus::FS_Upload_None,
                QString());
}

void RestApi::Impl::On_FS_UploadRequestCallback(const Ertebat::HttpStatus::HttpStatusCode &statusCode,
                                                const QString &data)
{
    emit m_parent->signal_FS_Upload(Ertebat::RestStatusCodes::ConnectionStatus::Connection_OK,
                                    static_cast<Ertebat::RestStatusCodes::FS_UploadStatus>(statusCode),
                                    data);
}

void RestApi::Impl::On_FS_AddAccessRoleRequestFailed(const Http::Error &error)
{
    emit m_parent->signal_FS_AddAccessRole(
                static_cast<Ertebat::RestStatusCodes::ConnectionStatus>(error),
                Ertebat::RestStatusCodes::FS_AddAccessRoleStatus::FS_AddAccessRole_None,
                QString());
}

void RestApi::Impl::On_FS_AddAccessRoleRequestCallback(const Ertebat::HttpStatus::HttpStatusCode &statusCode,
                                                       const QString &data)
{
    emit m_parent->signal_FS_AddAccessRole(Ertebat::RestStatusCodes::ConnectionStatus::Connection_OK,
                                           static_cast<Ertebat::RestStatusCodes::FS_AddAccessRoleStatus>(statusCode),
                                           data);
}

void RestApi::Impl::On_FS_RemoveAccessRoleRequestFailed(const Http::Error &error)
{
    emit m_parent->signal_FS_RemoveAccessRole(
                static_cast<Ertebat::RestStatusCodes::ConnectionStatus>(error),
                Ertebat::RestStatusCodes::FS_RemoveAccessRoleStatus::FS_RemoveAccessRole_None,
                QString());
}

void RestApi::Impl::On_FS_RemoveAccessRoleRequestCallback(const Ertebat::HttpStatus::HttpStatusCode &statusCode,
                                                          const QString &data)
{
    emit m_parent->signal_FS_RemoveAccessRole(Ertebat::RestStatusCodes::ConnectionStatus::Connection_OK,
                                              static_cast<Ertebat::RestStatusCodes::FS_RemoveAccessRoleStatus>(statusCode),
                                              data);
}

void RestApi::Impl::On_FS_ChangeAccessTypeRequestFailed(const Http::Error &error)
{
    emit m_parent->signal_FS_ChangeAccessType(
                static_cast<Ertebat::RestStatusCodes::ConnectionStatus>(error),
                Ertebat::RestStatusCodes::FS_ChangeAccessTypeStatus::FS_ChangeAccessType_None,
                QString());
}

void RestApi::Impl::On_FS_ChangeAccessTypeRequestCallback(const Ertebat::HttpStatus::HttpStatusCode &statusCode,
                                                          const QString &data)
{
    emit m_parent->signal_FS_ChangeAccessType(Ertebat::RestStatusCodes::ConnectionStatus::Connection_OK,
                                              static_cast<Ertebat::RestStatusCodes::FS_ChangeAccessTypeStatus>(statusCode),
                                              data);
}

void RestApi::Impl::On_FS_GetSharedEntitiesRequestFailed(const Http::Error &error)
{
    emit m_parent->signal_FS_GetSharedEntities(
                static_cast<Ertebat::RestStatusCodes::ConnectionStatus>(error),
                Ertebat::RestStatusCodes::FS_GetSharedEntitiesStatus::FS_GetSharedEntities_None,
                QString());
}

void RestApi::Impl::On_FS_GetSharedEntitiesRequestCallback(const Ertebat::HttpStatus::HttpStatusCode &statusCode,
                                                           const QString &data)
{
    emit m_parent->signal_FS_GetSharedEntities(Ertebat::RestStatusCodes::ConnectionStatus::Connection_OK,
                                               static_cast<Ertebat::RestStatusCodes::FS_GetSharedEntitiesStatus>(statusCode),
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

    connect(Http_FS_CreateDirectory.get(), SIGNAL(Signal_Failed(const Http::Error &)),
            this, SLOT(On_FS_CreateDirectoryRequestFailed(const Http::Error &)));
    connect(Http_FS_CreateDirectory.get(), SIGNAL(Signal_Finished(const Ertebat::HttpStatus::HttpStatusCode &, const QString &)),
            this, SLOT(On_FS_CreateDirectoryRequestCallback(const Ertebat::HttpStatus::HttpStatusCode &, const QString &)));

    connect(Http_FS_GetListOfEntity.get(), SIGNAL(Signal_Failed(const Http::Error &)),
            this, SLOT(On_FS_GetListOfEntityRequestFailed(const Http::Error &)));
    connect(Http_FS_GetListOfEntity.get(), SIGNAL(Signal_Finished(const Ertebat::HttpStatus::HttpStatusCode &, const QString &)),
            this, SLOT(On_FS_GetListOfEntityRequestCallback(const Ertebat::HttpStatus::HttpStatusCode &, const QString &)));

    connect(Http_FS_GetAccessType.get(), SIGNAL(Signal_Failed(const Http::Error &)),
            this, SLOT(On_FS_GetAccessTypeRequestFailed(const Http::Error &)));
    connect(Http_FS_GetAccessType.get(), SIGNAL(Signal_Finished(const Ertebat::HttpStatus::HttpStatusCode &, const QString &)),
            this, SLOT(On_FS_GetAccessTypeRequestCallback(const Ertebat::HttpStatus::HttpStatusCode &, const QString &)));

    connect(Http_FS_GetParentId.get(), SIGNAL(Signal_Failed(const Http::Error &)),
            this, SLOT(On_FS_GetParentIdRequestFailed(const Http::Error &)));
    connect(Http_FS_GetParentId.get(), SIGNAL(Signal_Finished(const Ertebat::HttpStatus::HttpStatusCode &, const QString &)),
            this, SLOT(On_FS_GetParentIdRequestCallback(const Ertebat::HttpStatus::HttpStatusCode &, const QString &)));

    connect(Http_FS_MoveEntity.get(), SIGNAL(Signal_Failed(const Http::Error &)),
            this, SLOT(On_FS_MoveEntityRequestFailed(const Http::Error &)));
    connect(Http_FS_MoveEntity.get(), SIGNAL(Signal_Finished(const Ertebat::HttpStatus::HttpStatusCode &, const QString &)),
            this, SLOT(On_FS_MoveEntityRequestCallback(const Ertebat::HttpStatus::HttpStatusCode &, const QString &)));

    connect(Http_FS_DeleteEntity.get(), SIGNAL(Signal_Failed(const Http::Error &)),
            this, SLOT(On_FS_DeleteEntityRequestFailed(const Http::Error &)));
    connect(Http_FS_DeleteEntity.get(), SIGNAL(Signal_Finished(const Ertebat::HttpStatus::HttpStatusCode &, const QString &)),
            this, SLOT(On_FS_DeleteEntityRequestCallback(const Ertebat::HttpStatus::HttpStatusCode &, const QString &)));

    connect(Http_FS_Download.get(), SIGNAL(Signal_Failed(const Http::Error &)),
            this, SLOT(On_FS_DownloadRequestFailed(const Http::Error &)));
    connect(Http_FS_Download.get(), SIGNAL(Signal_Finished(const Ertebat::HttpStatus::HttpStatusCode &, const QString &)),
            this, SLOT(On_FS_DownloadRequestCallback(const Ertebat::HttpStatus::HttpStatusCode &, const QString &)));

    connect(Http_FS_Upload.get(), SIGNAL(Signal_Failed(const Http::Error &)),
            this, SLOT(On_FS_UploadRequestFailed(const Http::Error &)));
    connect(Http_FS_Upload.get(), SIGNAL(Signal_Finished(const Ertebat::HttpStatus::HttpStatusCode &, const QString &)),
            this, SLOT(On_FS_UploadRequestCallback(const Ertebat::HttpStatus::HttpStatusCode &, const QString &)));

    connect(Http_FS_AddAccessRole.get(), SIGNAL(Signal_Failed(const Http::Error &)),
            this, SLOT(On_FS_AddAccessRoleRequestFailed(const Http::Error &)));
    connect(Http_FS_AddAccessRole.get(), SIGNAL(Signal_Finished(const Ertebat::HttpStatus::HttpStatusCode &, const QString &)),
            this, SLOT(On_FS_AddAccessRoleRequestCallback(const Ertebat::HttpStatus::HttpStatusCode &, const QString &)));

    connect(Http_FS_RemoveAccessRole.get(), SIGNAL(Signal_Failed(const Http::Error &)),
            this, SLOT(On_FS_RemoveAccessRoleRequestFailed(const Http::Error &)));
    connect(Http_FS_RemoveAccessRole.get(), SIGNAL(Signal_Finished(const Ertebat::HttpStatus::HttpStatusCode &, const QString &)),
            this, SLOT(On_FS_RemoveAccessRoleRequestCallback(const Ertebat::HttpStatus::HttpStatusCode &, const QString &)));

    connect(Http_FS_ChangeAccessType.get(), SIGNAL(Signal_Failed(const Http::Error &)),
            this, SLOT(On_FS_ChangeAccessTypeRequestFailed(const Http::Error &)));
    connect(Http_FS_ChangeAccessType.get(), SIGNAL(Signal_Finished(const Ertebat::HttpStatus::HttpStatusCode &, const QString &)),
            this, SLOT(On_FS_ChangeAccessTypeRequestCallback(const Ertebat::HttpStatus::HttpStatusCode &, const QString &)));

    connect(Http_FS_GetSharedEntities.get(), SIGNAL(Signal_Failed(const Http::Error &)),
            this, SLOT(On_FS_GetSharedEntitiesRequestFailed(const Http::Error &)));
    connect(Http_FS_GetSharedEntities.get(), SIGNAL(Signal_Finished(const Ertebat::HttpStatus::HttpStatusCode &, const QString &)),
            this, SLOT(On_FS_GetSharedEntitiesRequestCallback(const Ertebat::HttpStatus::HttpStatusCode &, const QString &)));
}

