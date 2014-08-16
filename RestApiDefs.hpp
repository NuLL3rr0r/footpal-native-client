#ifndef FOOTPAL_REST_API_DEFS_HPP
#define FOOTPAL_REST_API_DEFS_HPP


#include <QList>
class QString;
#include "HttpStatus.hpp"

namespace Footpal {
class RestApiDefs;
}

class Footpal::RestApiDefs
{
private:
    struct User
    {
        QString Id;
        QString Username;
        QString Email;
        QString FirstName;
        QString LastName;
        QString RegisterationDate;
        QString MobileNumber;
        QString Gender;
        QString Individuals;
        QString Groups;
    };

    struct Room
    {
        QString RoomId;
        QString Code;
        QString Message;
    };

public:
    struct SignUpResult
    {
        Footpal::HttpStatus::HttpStatusCode Status;
    };

    struct SignInResult
    {
        Footpal::HttpStatus::HttpStatusCode Status;
        QString Token;
    };

    struct SignOutResult
    {
        Footpal::HttpStatus::HttpStatusCode Status;
    };

    struct GetCurrentProfileResult
    {
        Footpal::HttpStatus::HttpStatusCode Status;
        User UserInfo;
    };

    struct UsersListResult
    {
        Footpal::HttpStatus::HttpStatusCode Status;
        QList<User> Users;
    };

    struct CreateIndividualRoomResult
    {
        Footpal::HttpStatus::HttpStatusCode Status;
        Room RoomInfo;
    };
};


#endif /* FOOTPAL_REST_API_DEFS_HPP */

