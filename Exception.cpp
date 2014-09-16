/**
 * @author  Mohammad S. Babaei <info@babaei.net>
 */


#include "Exception.hpp"

using namespace std;
using namespace Ertebat;

Exception::Exception(const std::string &message) :
    runtime_error(message)
{

}

