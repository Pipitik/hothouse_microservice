#pragma once
#ifndef HOTHOUSE_NOT_FOUND_HANDLER_HPP_
#define HOTHOUSE_NOT_FOUND_HANDLER_HPP_

#include <Poco/Net/HTTPRequestHandler.h>

namespace hothouse
{

class not_found_handler : public Poco::Net::HTTPRequestHandler
{
public:
    void handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) override;
};

} // hothouse

#endif // HOTHOUSE_NOT_FOUND_HANDLER_HPP_