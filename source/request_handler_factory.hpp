#pragma once
#ifndef HOTHOUSE_REQUEST_HANDLER_FACTORY_HPP_
#define HOTHOUSE_REQUEST_HANDLER_FACTORY_HPP_

#include <Poco/Data/SessionPool.h>
#include <Poco/Net/HTTPRequestHandlerFactory.h>

namespace hothouse
{

class request_handler_factory : public Poco::Net::HTTPRequestHandlerFactory
{
public:
    explicit request_handler_factory(const std::string& connection_string);
    Poco::Net::HTTPRequestHandler* createRequestHandler(const Poco::Net::HTTPServerRequest& request) override;

private:
    Poco::Data::SessionPool session_pool_;
};

} // hothouse

#endif // HOTHOUSE_REQUEST_HANDLER_FACTORY_HPP_