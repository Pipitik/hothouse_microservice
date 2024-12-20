#include "hothouses_create_handler.hpp"

#include <nlohmann/json.hpp>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>

#include "../hothouse_t.hpp"

namespace hothouse
{

hothouses_create_handler::hothouses_create_handler(Poco::Data::Session&& session) noexcept
    : session_(std::move(session))
{ }

void hothouses_create_handler::handleRequest(
    Poco::Net::HTTPServerRequest& request,
    Poco::Net::HTTPServerResponse& response)
{
    if (request.getMethod() != Poco::Net::HTTPRequest::HTTP_POST)
    {
        response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_METHOD_NOT_ALLOWED);
        response.send();
        return;
    }

    try
    {
        nlohmann::json body = nlohmann::json::parse(request.stream());
        hothouse::hothouse_t new_hothouse(body.get<hothouse::hothouse_t>());

        session_
            << "INSERT INTO hothouses(title, description, crop) VALUES($1, $2, $3)",
            Poco::Data::Keywords::use(new_hothouse.title),
            Poco::Data::Keywords::use(new_hothouse.description),
            Poco::Data::Keywords::use(new_hothouse.crop),
            Poco::Data::Keywords::now;

        response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_OK);
        response.send();
    }
    catch (const nlohmann::json::exception& error)
    {
        std::cerr << error.what() << std::endl;
        response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_UNPROCESSABLE_ENTITY);
        response.send();
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