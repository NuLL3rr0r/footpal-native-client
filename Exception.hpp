/**
 * @author  Mohammad S. Babaei <info@babaei.net>
 */


#ifndef ERTEBAT_EXCEPTION_HPP
#define ERTEBAT_EXCEPTION_HPP


#include <stdexcept>
#include <string>

namespace Ertebat {
class Exception;
}

class Ertebat::Exception : public std::runtime_error
{
public:
    explicit Exception(const std::string &message);
};


#endif /* ERTEBAT_EXCEPTION_HPP */

