#include "request_handler_factory.hpp"

#include <Poco/Net/HTTPServerRequest.h>

#include "handlers/hothouses_handler.hpp"
#include "handlers/hothouses_create_handler.hpp"
#include "handlers/hothouses_edit_handler.hpp"
#include "handlers/hothouses_delete_handler.hpp"
#include "handlers/not_found_handler.hpp"

namespace hothouse
{

request_handler_factory::request_handler_factory(const std::string& connection_string)
    : session_pool_("postgresql", connection_string)
{}

Poco::Net::HTTPRequestHandler*
request_handler_factory::createRequestHandler(const Poco::Net::HTTPServerRequest& request)
{
    const std::string& uri = request.getURI();

    if (uri == "/api/v1/hothouses/")
    {
        return new hothouse::hothouses_handler(session_pool_.get());
    }
    else if (uri == "/api/v1/hothouses/create/")
    {
        return new hothouse::hothouses_create_handler(session_pool_.get());
    }
    else if (uri == "/api/v1/hothouses/edit/")
    {
        return new hothouse::hothouses_edit_handler(session_pool_.get());
    }
    else if (uri == "/api/v1/hothouses/delete/")
    {
        return new hothouse::hothouses_delete_handler(session_pool_.get());
    }
    else
    {
        return new hothouse::not_found_handler();
    }
}

} // hothouse