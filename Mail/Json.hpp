/**
 * @author  Mohamad mehdi Kharatizadeh <m_kharatizadeh@yahoo.com>
 */

#ifndef ERTEBAT_MAIL_JSON_HPP
#define ERTEBAT_MAIL_JSON_HPP

#include "Mail.hpp"
#include "Message.hpp"
#include <vector>

namespace Ertebat { namespace Mail {

    class Json {
        Json();
        Json(Json const&);
        Json& operator = (Json const&);

    public:

        static QString EncodeMessage(Message const& message);
        static QString EncodeMessage(std::vector<Message> const& message);
        static std::vector<Message> DecodeMessage(QString const& json);
    };

} }

#endif // JSON_HPP
