#pragma once
#ifndef HOTHOUSE_HTTP_SERVER_HPP_
#define HOTHOUSE_HTTP_SERVER_HPP_

#include <Poco/Util/ServerApplication.h>

namespace hothouse
{

class http_server final : public Poco::Util::ServerApplication
{
public:
    http_server() noexcept;

    const char* name() const override { return "hothouse"; };

    void initialize(Poco::Util::Application& self) override;

    void defineOptions(Poco::Util::OptionSet& options) override;

    int main(const std::vector<std::string>& args) override;

private:

    void handle_help(const std::string&, const std::string&);
    void handle_port(const std::string&, const std::string& value);
    void handle_dbhost(const std::string&, const std::string& value);
    void handle_dbport(const std::string&, const std::string& value);
    void handle_dbuser(const std::string&, const std::string& value);
    void handle_dbpassword(const std::string&, const std::string& value);
    void handle_dbname(const std::string&, const std::string& value);

    std::uint16_t port_;
    std::string dbhost_;
    std::uint16_t dbport_;
    std::string dbuser_;
    std::string dbpassword_;
    std::string dbname_;

    bool has_help_request_;
};

} // hothouse

#endif // HOTHOUSE_HTTP_SERVER_HPP_