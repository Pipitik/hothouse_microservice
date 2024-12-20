#include "hothouses_handler.hpp"

#include <deque>

#include <nlohmann/json.hpp>
#include <Poco/Data/Session.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>

#include "../hothouse_t.hpp"

namespace hothouse
{

hothouses_handler::hothouses_handler(Poco::Data::Session&& session) noexcept
    : session_(std::move(session))
{}

void hothouses_handler::handleRequest(
    Poco::Net::HTTPServerRequest& request,
    Poco::Net::HTTPServerResponse& response)
{
    if (request.getMethod() != Poco::Net::HTTPRequest::HTTP_GET)
    {
        response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_METHOD_NOT_ALLOWED);
        response.send();
        return;
    }

    try
    {
        std::deque<hothouse::hothouse_t> hothouses;
        session_ << "SELECT * FROM hothouses", Poco::Data::Keywords::into(hothouses), Poco::Data::Keywords::now;

        std::string body = nlohmann::to_string(nlohmann::json(std::move(hothouses)));
        response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_OK);
        response.setContentType("application/json");
        response.setContentLength(body.size());
        response.send() << body;
    }
    catch (const Poco::Exception& error)
    {
        std::cerr << error.displayText() << std::endl;
        response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
        response.send();
    }
    catch (const std::exception& error)
    {
        std::cerr << error.what() << std::endl;
        response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
        response.send();
    }
}

} // hothouse