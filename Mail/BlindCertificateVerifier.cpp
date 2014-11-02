/**
 * @author  Mohammad S. Babaei <info@babaei.net>
 */

#include <qsystemdetection.h>

#if !defined(Q_OS_ANDROID)

#include "BlindCertificateVerifier.hpp"

using namespace Ertebat;
using namespace Ertebat::Mail;

void BlindCertificateVerifier::verify(
        vmime::shared_ptr<vmime::security::cert::certificateChain> chain,
        const std::string &hostname)
{
    (void)chain;
    (void)hostname;
}

#endif
