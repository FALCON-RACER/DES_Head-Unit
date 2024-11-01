/*
 * This file was generated by the CommonAPI Generators.
 * Used org.genivi.commonapi.someip 3.2.14.v202310241606.
 * Used org.franca.core 0.13.1.201807231814.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
 * If a copy of the MPL was not distributed with this file, You can obtain one at
 * http://mozilla.org/MPL/2.0/.
 */
#include <v1/commonapi/speedSomeIPProxy.hpp>

#if !defined (COMMONAPI_INTERNAL_COMPILATION)
#define COMMONAPI_INTERNAL_COMPILATION
#define HAS_DEFINED_COMMONAPI_INTERNAL_COMPILATION_HERE
#endif

#include <CommonAPI/SomeIP/AddressTranslator.hpp>

#if defined (HAS_DEFINED_COMMONAPI_INTERNAL_COMPILATION_HERE)
#undef COMMONAPI_INTERNAL_COMPILATION
#undef HAS_DEFINED_COMMONAPI_INTERNAL_COMPILATION_HERE
#endif

namespace v1 {
namespace commonapi {

std::shared_ptr<CommonAPI::SomeIP::Proxy> createspeedSomeIPProxy(
    const CommonAPI::SomeIP::Address &_address,
    const std::shared_ptr<CommonAPI::SomeIP::ProxyConnection> &_connection) {
    return std::make_shared< speedSomeIPProxy>(_address, _connection);
}

void initializespeedSomeIPProxy() {
    CommonAPI::SomeIP::AddressTranslator::get()->insert(
        "local:commonapi.speed:v1_0:speed",
        0x1235, 0x5679, 1, 0);
    CommonAPI::SomeIP::Factory::get()->registerProxyCreateMethod(
        "commonapi.speed:v1_0",
        &createspeedSomeIPProxy);
}

INITIALIZER(registerspeedSomeIPProxy) {
    CommonAPI::SomeIP::Factory::get()->registerInterface(initializespeedSomeIPProxy);
}

speedSomeIPProxy::speedSomeIPProxy(
    const CommonAPI::SomeIP::Address &_address,
    const std::shared_ptr<CommonAPI::SomeIP::ProxyConnection> &_connection)
        : CommonAPI::SomeIP::Proxy(_address, _connection),
          speed_(*this, CommonAPI::SomeIP::eventgroup_id_t(0x8110), CommonAPI::SomeIP::event_id_t(0x8110), CommonAPI::SomeIP::method_id_t(0xfa0), true, CommonAPI::SomeIP::reliability_type_e::RT_RELIABLE, false, CommonAPI::SomeIP::method_id_t(0xfa1), true, static_cast< CommonAPI::EmptyDeployment* >(nullptr))
{
}

speedSomeIPProxy::~speedSomeIPProxy() {
}

speedSomeIPProxy::SpeedAttribute& speedSomeIPProxy::getSpeedAttribute() {
    return speed_;
}


void speedSomeIPProxy::getOwnVersion(uint16_t& ownVersionMajor, uint16_t& ownVersionMinor) const {
    ownVersionMajor = 1;
    ownVersionMinor = 0;
}

std::future<void> speedSomeIPProxy::getCompletionFuture() {
    return CommonAPI::SomeIP::Proxy::getCompletionFuture();
}

} // namespace commonapi
} // namespace v1
