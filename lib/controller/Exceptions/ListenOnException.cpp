
#include "ListenOnException.hpp"

ListenOnException::ListenOnException(boost::system::error_code listenOnErrorCode_) : listenOnErrorCode(listenOnErrorCode_) { }

const char * ListenOnException::what() const throw () {
	return listenOnErrorCode.message().c_str();
}
