#include "not_found_handler.hpp"

#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>

namespace hothouse
{

void not_found_handler::handleRequest(
    Poco::Net::HTTPServerRequest& request,
    Poco::Net::HTTPServerResponse& response)
{
    response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_NOT_FOUND);
    response.send();
}

} // hothouse