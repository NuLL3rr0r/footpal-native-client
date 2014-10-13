/**
 * @author  Mohammad S. Babaei <info@babaei.net>
 */


#ifndef ERTEBAT_MAIL_BLIND_CERTIFICATE_VERIFIER_HPP
#define ERTEBAT_MAIL_BLIND_CERTIFICATE_VERIFIER_HPP


#include <string>
#include <vmime/security/cert/certificateVerifier.hpp>

namespace Ertebat {
namespace Mail {
class BlindCertificateVerifier;
}
}

class Ertebat::Mail::BlindCertificateVerifier : public vmime::security::cert::certificateVerifier {
public:
    void verify(
            vmime::shared_ptr<vmime::security::cert::certificateChain> chain,
            const std::string &hostname);
};


#endif /* ERTEBAT_MAIL_BLIND_CERTIFICATE_VERIFIER_HPP */

