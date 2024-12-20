#include "hothouses_delete_handler.hpp"

#include <nlohmann/json.hpp>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>

namespace hothouse
{

hothouses_delete_handler::hothouses_delete_handler(Poco::Data::Session&& session) noexcept
    : session_(std::move(session))
{}

void hothouses_delete_handler::handleRequest(
    Poco::Net::HTTPServerRequest& request,
    Poco::Net::HTTPServerResponse& response)
{
    if (request.getMethod() != Poco::Net::HTTPRequest::HTTP_DELETE)
    {
        response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_METHOD_NOT_ALLOWED);
        response.send();
        return;
    }

    try
    {
        nlohmann::json body = nlohmann::json::parse(request.stream());
        session_
            << "DELETE FROM hothouses WHERE $1 = hothouses.id",
            Poco::Data::Keywords::bind(body.at("id").get<std::int64_t>()),
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