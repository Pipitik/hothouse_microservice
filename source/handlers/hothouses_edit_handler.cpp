#include "hothouses_edit_handler.hpp"

#include <nlohmann/json.hpp>
#include <Poco/Data/Statement.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>

#include "../hothouse_t.hpp"

namespace hothouse
{

hothouses_edit_handler::hothouses_edit_handler(Poco::Data::Session&& session) noexcept
    : session_(std::move(session))
{}

void hothouses_edit_handler::handleRequest(
    Poco::Net::HTTPServerRequest& request,
    Poco::Net::HTTPServerResponse& response)
{
    if (request.getMethod() != Poco::Net::HTTPRequest::HTTP_PUT)
    {
        response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_METHOD_NOT_ALLOWED);
        response.send();
        return;
    }

    try
    {
        nlohmann::json body = nlohmann::json::parse(request.stream());

        auto hothouse_id = body.at("id").get<std::int64_t>();

        Poco::Data::Statement update(session_ << "UPDATE hothouses SET");
        std::int64_t number = 0;
        auto it = body.find("title");
        if (it != body.end())
        {
            update <<
                " title=$" << std::to_string(++number),
                Poco::Data::Keywords::bind(it->get<std::string>());
        }
        it = body.find("description");
        if (it != body.end())
        {
            if (number > 0) update << ',';
            update <<
                " description=$" << std::to_string(++number),
                Poco::Data::Keywords::bind(it->get<std::string>());
        }
        it = body.find("crop");
        if (it != body.end())
        {
            if (number > 0) update << ',';
            update <<
                " crop=$" << std::to_string(++number),
                Poco::Data::Keywords::bind(it->get<std::string>());
        }
        update <<
            " WHERE id=$" << std::to_string(++number),
            Poco::Data::Keywords::use(hothouse_id);

        if (number < body.size())
        {
            std::cerr << "Invalid input data " << body << std::endl;
            response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_UNPROCESSABLE_ENTITY);
            response.send();
            return;
        }

        update.execute();

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