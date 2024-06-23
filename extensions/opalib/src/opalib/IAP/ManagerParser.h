#ifndef AVALON_PAYMENT_LOADER_H
#define AVALON_PAYMENT_LOADER_H

//#include <boost/noncopyable.hpp>
//#include <boost/shared_ptr.hpp>
#include "./platform.h"

namespace opalib {
namespace iap {

class Manager;

/**
 * Translates the content of the given ini-file into the right setup of
 * iap::Manager with iap::Product and/or iap::ProductConsumable.
 */
// class Loader : boost::noncopyable
class ManagerParser
{
public:
    static Manager* createManager(const char* configFile);

private:
    static std::string getProductIdKey();
};

} // namespace iap
} // namespace opalib

#endif /* AVALON_PAYMENT_LOADER_H */
