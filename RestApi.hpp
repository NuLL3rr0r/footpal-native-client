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

    Q_ENUMS( FS_CreateDirectoryStatus )
    Q_ENUMS( FS_GetListOfEntityStatus )
    Q_ENUMS( FS_GetListOfEntityStatus )
    Q_ENUMS( FS_GetAccessTypeStatus )
    Q_ENUMS( FS_GetParentIdStatus )
    Q_ENUMS( FS_MoveEntityStatus )
    Q_ENUMS( FS_DeleteEntityStatus )
    Q_ENUMS( FS_DownloadStatus )
    Q_ENUMS( FS_UploadStatus )

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

    enum FS_CreateDirectoryStatus {
        FS_CreateDirectory_None = 0,
        FS_CreateDirectory_Created = 201,
        FS_CreateDirectory_BadRequest = 400,
        FS_CreateDirectory_NotFound = 404
    };

    enum FS_GetListOfEntityStatus {
        FS_GetListOfEntity_None = 0,
        FS_GetListOfEntity_OK = 200,
        FS_GetListOfEntity_Forbidden = 403,
        FS_GetListOfEntity_NotFound = 404,
        FS_GetListOfEntity_InternalServerError = 500
    };

    enum FS_GetAccessTypeStatus {
        FS_GetAccessType_None = 0,
        FS_GetAccessType_OK = 200,
        FS_GetAccessType_BadRequest = 400,
        FS_GetAccessType_Forbidden = 403,
        FS_GetAccessType_InternalServerError = 500
    };

    enum FS_GetParentIdStatus {
        FS_GetParentId_None = 0,
        FS_GetParentId_OK = 200,
        FS_GetParentId_BadRequest = 400,
        FS_GetParentId_Forbidden = 403,
        FS_GetParentId_NotFound = 404,
        FS_GetParentId_InternalServerError = 500,
    };

    enum FS_MoveEntityStatus {
        FS_MoveEntity_None = 0,
        FS_MoveEntity_OK = 200,
        FS_MoveEntity_BadRequest = 400,
        FS_MoveEntity_Forbidden = 403,
        FS_MoveEntity_NotFound = 404,
        FS_MoveEntity_InternalServerError = 500,
    };

    enum FS_DeleteEntityStatus {
        FS_DeleteEntity_None = 0,
        FS_DeleteEntity_OK = 200,
        FS_DeleteEntity_BadRequest = 400,
        FS_DeleteEntity_Forbidden = 403,
        FS_DeleteEntity_NotFound = 404,
        FS_DeleteEntity_InternalServerError = 500
    };

    enum FS_DownloadStatus {
        FS_Download_None = 0,
        FS_Download_OK = 200,
        FS_Download_BadRequest = 400,
        FS_Download_Forbidden = 403,
        FS_Download_NotFound = 404,
        FS_Download_InternalServerError = 500,
    };

    enum FS_UploadStatus {
        FS_Upload_None = 0,
        FS_Upload_Created = 201,
        FS_Upload_BadRequest = 400,
        FS_Upload_Forbidden = 403,
        FS_Upload_NotFound = 404,
        FS_Upload_InternalServerError = 500,
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
                       const QString &);
    void signal_SignIn(Ertebat::RestStatusCodes::ConnectionStatus,
                       Ertebat::RestStatusCodes::SignInStatus,
                       const QString &);
    void signal_SignOut(Ertebat::RestStatusCodes::ConnectionStatus,
                        Ertebat::RestStatusCodes::SignOutStatus,
                        const QString &);

    void signal_GetCurrentProfile(Ertebat::RestStatusCodes::ConnectionStatus,
                                  Ertebat::RestStatusCodes::GetCurrentProfileStatus,
                                  const QString &);
    void signal_UsersList(Ertebat::RestStatusCodes::ConnectionStatus,
                          Ertebat::RestStatusCodes::UsersListStatus,
                          const QString &);

    void signal_CreateIndividualRoom(Ertebat::RestStatusCodes::ConnectionStatus,
                                     Ertebat::RestStatusCodes::CreateIndividualRoomStatus,
                                     const QString &);

    void signal_GetIndividualContacts(Ertebat::RestStatusCodes::ConnectionStatus,
                                      Ertebat::RestStatusCodes::GetIndividualContactsStatus,
                                      const QString &);


    void signal_FS_CreateDirectory(Ertebat::RestStatusCodes::ConnectionStatus,
                                   Ertebat::RestStatusCodes::FS_CreateDirectoryStatus,
                                   const QString &);
    void signal_FS_GetListOfEntity(Ertebat::RestStatusCodes::ConnectionStatus,
                                   Ertebat::RestStatusCodes::FS_GetListOfEntityStatus,
                                   const QString &);
    void signal_FS_GetAccessType(Ertebat::RestStatusCodes::ConnectionStatus,
                                 Ertebat::RestStatusCodes::FS_GetAccessTypeStatus,
                                 const QString &);
    void signal_FS_GetParentId(Ertebat::RestStatusCodes::ConnectionStatus,
                               Ertebat::RestStatusCodes::FS_GetParentIdStatus,
                               const QString &);
    void signal_FS_MoveEntity(Ertebat::RestStatusCodes::ConnectionStatus,
                              Ertebat::RestStatusCodes::FS_MoveEntityStatus,
                              const QString &);
    void signal_FS_DeleteEntity(Ertebat::RestStatusCodes::ConnectionStatus,
                                Ertebat::RestStatusCodes::FS_DeleteEntityStatus,
                                const QString &);
    void signal_FS_Download(Ertebat::RestStatusCodes::ConnectionStatus,
                            Ertebat::RestStatusCodes::FS_DownloadStatus,
                            const QString &);
    void signal_FS_Upload(Ertebat::RestStatusCodes::ConnectionStatus,
                          Ertebat::RestStatusCodes::FS_UploadStatus,
                          const QString &);
public:
    Q_INVOKABLE bool isOnline();

    Q_INVOKABLE void signUp(const QString &phone, const QString &email, const QString &pwd);
    Q_INVOKABLE void signIn(const QString &user, const QString &pwd);
    Q_INVOKABLE void signOut(const QString &token, const QString &user, const QString &pwd);

    Q_INVOKABLE void getCurrentProfile(const QString &token);
    Q_INVOKABLE void usersList(const QString &token);

    Q_INVOKABLE void createIndividualRoom(const QString &token, const QString &otherParty);

    Q_INVOKABLE void getIndividualContacts(const QString &token);


    Q_INVOKABLE void fs_CreateDirectory(const QString &token, const QString &dirName, const QString &access, const QString &parentId);
    Q_INVOKABLE void fs_GetListOfEntity(const QString &token);
    Q_INVOKABLE void fs_GetListOfEntity(const QString &token, const QString &entityId);
    Q_INVOKABLE void fs_GetAccessType(const QString &token, const QString &entityId);
    Q_INVOKABLE void fs_GetParentId(const QString &token, const QString &entityId);
    Q_INVOKABLE void fs_MoveEntity(const QString &token, const QString &entityId, const QString &newParentId);
    Q_INVOKABLE void fs_DeleteEntity(const QString &token, const QString &entityId);
    Q_INVOKABLE void fs_Download(const QString &token, const QString &entityId);
    Q_INVOKABLE void fs_Upload(const QString &token, const QString &parentId, const QString &access);
};


#endif /* ERTEBAT_REST_API_HPP */

