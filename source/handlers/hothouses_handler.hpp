#pragma once
#ifndef HOTHOUSE_HOTHOUSES_HANDLER_HPP_
#define HOTHOUSE_HOTHOUSES_HANDLER_HPP_

#include <Poco/Data/Session.h>
#include <Poco/Net/HTTPRequestHandler.h>

namespace hothouse
{

class hothouses_handler : public Poco::Net::HTTPRequestHandler
{
public:
    explicit hothouses_handler(Poco::Data::Session&& session) noexcept;
    void handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) override;

private:
    Poco::Data::Session session_;
};

} // hothouse

#endif // HOTHOUSE_HOTHOUSES_HANDLER_HPP_