/**
 * @author  Mohammad S. Babaei <info@babaei.net>
 */


#include <algorithm>
#include <stdexcept>
#if defined ( _WIN32 )
#include <windows.h>
//#include <cryptopp/dll.h>     // msvc-shared only
#endif // defined ( _WIN32 )
#include <cryptopp/aes.h>
#include <cryptopp/ccm.h>
#include <cryptopp/cryptlib.h>
#include <cryptopp/filters.h>
#include <cryptopp/hex.h>
#include <cryptopp/sha.h>
#include <b64/decode.h>
#include <b64/encode.h>
#include "make_unique.hpp"
#include "Crypto.hpp"

#define     UNKNOWN_ERROR           "Ertebat::Crypto unknown error!"

using namespace std;
using namespace CryptoPP;
using namespace Ertebat;

struct Crypto::Impl
{
    Crypto::Byte_t *Key;
    std::size_t KeyLen;
    Crypto::Byte_t *IV;
    std::size_t IVLen;

    Impl();
    ~Impl();
};

bool Crypto::Encrypt(const std::string &plainText, std::string &out_encodedText,
                     const Byte_t *key, std::size_t keyLen, const Byte_t *iv, std::size_t ivLen)
{
    string err;
    return Crypto::Encrypt(plainText.c_str(), out_encodedText, err, key, keyLen, iv, ivLen);
}

bool Crypto::Encrypt(const std::string &plainText, std::string &out_encodedText,
                     std::string &out_error,
                     const Byte_t *key, std::size_t keyLen, const Byte_t *iv, std::size_t ivLen)
{
    try {
        string cipher;

        CBC_Mode<AES>::Encryption enc;
        enc.SetKeyWithIV(key, keyLen, iv, ivLen);

        cipher.clear();
        StringSource(plainText, true,
                     new StreamTransformationFilter(enc, new StringSink(cipher)));

        out_encodedText.clear();
        StringSource(cipher, true, new HexEncoder(new StringSink(out_encodedText)));

        return true;
    }

    catch (const CryptoPP::Exception &ex) {
        out_error.assign(ex.what());
    }

    catch (const std::exception &ex) {
        out_error.assign(ex.what());
    }

    catch (...) {
        out_error.assign(UNKNOWN_ERROR);
    }

    return false;
}

bool Crypto::Decrypt(const std::string &cipherText, std::string &out_recoveredText,
                     const Byte_t *key, std::size_t keyLen, const Byte_t *iv, std::size_t ivLen)
{
    string err;
    return Crypto::Decrypt(cipherText.c_str(), out_recoveredText, err, key, keyLen, iv, ivLen);
}

bool Crypto::Decrypt(const std::string &cipherText, std::string &out_recoveredText,
                     std::string &out_error,
                     const Byte_t *key, std::size_t keyLen, const Byte_t *iv, std::size_t ivLen)
{
    try {
        string cipher;

        CBC_Mode<AES>::Decryption dec;
        dec.SetKeyWithIV(key, keyLen, iv, ivLen);

        cipher.clear();
        StringSource(cipherText, true, new HexDecoder(new StringSink(cipher)));

        out_recoveredText.clear();
        StringSource s(cipher, true,
                       new StreamTransformationFilter(dec, new StringSink(out_recoveredText)));

        return true;
    }

    catch (const CryptoPP::Exception &ex) {
        out_error.assign(ex.what());
    }

    catch (const std::exception &ex) {
        out_error.assign(ex.what());
    }

    catch (...) {
        out_error.assign(UNKNOWN_ERROR);
    }

    return false;
}


bool Crypto::GenerateHash(const char *text, std::string &out_digest)
{
    string err;
    return GenerateHash(text, out_digest, err);
}

bool Crypto::GenerateHash(const std::string &text, std::string &out_digest)
{
    string err;
    return GenerateHash(text.c_str(), out_digest, err);
}

bool Crypto::GenerateHash(const char *text, std::string &out_digest,
                          std::string &out_error)
{
    try {
        SHA512 hash;

        out_digest.clear();
        StringSource(text, true,
                     new HashFilter(hash, new HexEncoder(new StringSink(out_digest))));

        return true;
    }

    catch (const CryptoPP::Exception &ex) {
        out_error.assign(ex.what());
    }

    catch (const std::exception &ex) {
        out_error.assign(ex.what());
    }

    catch (...) {
        out_error.assign(UNKNOWN_ERROR);
    }

    return false;
}

bool Crypto::GenerateHash(const std::string &text, std::string &out_digest,
                          std::string &out_error)
{
    return GenerateHash(text.c_str(), out_digest, out_error);
}

int Crypto::Base64Decode(char value)
{
    base64::decoder decoder;
    return decoder.decode(value);
}

int Crypto::Base64Decode(const char *code, const int length, char *out_plainText)
{
    base64::decoder decoder;
    return decoder.decode(code, length, out_plainText);
}

void Crypto::Base64Decode(std::istream &inputStream, std::ostream &outputStream)
{
    base64::decoder decoder;
    decoder.decode(inputStream, outputStream);
}

int Crypto::Base64Encode(char value)
{
    base64::encoder encoder;
    return encoder.encode(value);
}

int Crypto::Base64Encode(const char *code, const int length, char *out_plainText)
{
    base64::encoder encoder;
    return encoder.encode(code, length, out_plainText);
}

int Crypto::Base64EncodeBlockEnd(char *out_plainText)
{
    base64::encoder encoder;
    return encoder.encode_end(out_plainText);
}

void Crypto::Base64Encode(std::istream &inputStream, std::ostream &outputStream)
{
    base64::encoder encoder;
    encoder.encode(inputStream, outputStream);
}

Crypto::Crypto(const Byte_t *key, std::size_t keyLen, const Byte_t *iv, std::size_t ivLen) :
    m_pimpl(std::make_unique<Crypto::Impl>())
{
    m_pimpl->Key = new Byte_t[keyLen];
    m_pimpl->IV = new Byte_t[ivLen];

    std::copy(key, key + keyLen, m_pimpl->Key);
    std::copy(iv, iv + ivLen, m_pimpl->IV);

    m_pimpl->KeyLen = keyLen;
    m_pimpl->IVLen = ivLen;
}

Crypto::~Crypto()
{

}

bool Crypto::Encrypt(const std::string &plainText, std::string &out_encodedText)
{
    return Crypto::Encrypt(plainText, out_encodedText, m_pimpl->Key, m_pimpl->KeyLen, m_pimpl->IV, m_pimpl->IVLen);
}

bool Crypto::Encrypt(const std::string &plainText, std::string &out_encodedText,
                     std::string &out_error)
{
    return Crypto::Encrypt(plainText, out_encodedText, out_error, m_pimpl->Key, m_pimpl->KeyLen, m_pimpl->IV, m_pimpl->IVLen);
}

bool Crypto::Decrypt(const std::string &cipherText, std::string &out_recoveredText)
{
    return Crypto::Decrypt(cipherText, out_recoveredText, m_pimpl->Key, m_pimpl->KeyLen, m_pimpl->IV, m_pimpl->IVLen);
}

bool Crypto::Decrypt(const std::string &cipherText, std::string &out_recoveredText,
                     std::string &out_error)
{
    return Crypto::Encrypt(cipherText, out_recoveredText, out_error, m_pimpl->Key, m_pimpl->KeyLen, m_pimpl->IV, m_pimpl->IVLen);
}

Crypto::Impl::Impl()
{

}

Crypto::Impl::~Impl()
{
    delete IV;
    delete Key;
}

