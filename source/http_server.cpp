#include "http_server.hpp"

#include <limits>

#include <Poco/Data/PostgreSQL/Connector.h>
#include <Poco/Data/Session.h>
#include <Poco/Net/HTTPServer.h>
#include <Poco/Util/HelpFormatter.h>
#include <Poco/Util/IntValidator.h>

#include "request_handler_factory.hpp"

namespace hothouse
{

http_server::http_server() noexcept
    : port_(80)
    , dbhost_("localhost")
    , dbport_(5432)
    , has_help_request_(false)
{
    setUnixOptions(true);
}

void http_server::initialize(Poco::Util::Application& self)
{
    loadConfiguration();
    ServerApplication::initialize(self);
}

void http_server::defineOptions(Poco::Util::OptionSet& options)
{
    ServerApplication::defineOptions(options);

    options.addOption(
        Poco::Util::Option("help", "h", "Display argument help information")
        .required(false)
        .repeatable(false)
        .callback(Poco::Util::OptionCallback<http_server>(
            this, &http_server::handle_help)));

    options.addOption(
        Poco::Util::Option("port", "p",
            "Specifies the TCP port socket file extension "
            "on which the server is listening for connections.")
        .required(false)
        .repeatable(false)
        .argument("<INT>", true)
        .validator(new Poco::Util::IntValidator(
            std::numeric_limits<std::uint16_t>::min(),
            std::numeric_limits<std::uint16_t>::max()))
        .callback(Poco::Util::OptionCallback<http_server>(
            this, &http_server::handle_port)));

    options.addOption(
        Poco::Util::Option("dbhost", "", "Specifies the host name of the machine on which the server is running.")
        .required(false)
        .repeatable(false)
        .argument("<STRING>", true)
        .callback(Poco::Util::OptionCallback<http_server>(
            this, &http_server::handle_dbhost)));

    options.addOption(
        Poco::Util::Option("dbport", "",
            "Specifies the TCP port or the local Unix-domain socket file extension "
            "on which the database is listening for connections.")
        .required(false)
        .repeatable(false)
        .argument("<INT>", true)
        .validator(new Poco::Util::IntValidator(
            std::numeric_limits<std::uint16_t>::min(),
            std::numeric_limits<std::uint16_t>::max()))
        .callback(Poco::Util::OptionCallback<http_server>(
            this, &http_server::handle_dbport)));

    options.addOption(
        Poco::Util::Option("dbuser", "", "Database user name.")
        .required(true)
        .repeatable(false)
        .argument("<STRING>", true)
        .callback(Poco::Util::OptionCallback<http_server>(
            this, &http_server::handle_dbuser)));
    
    options.addOption(
        Poco::Util::Option("dbpassword", "", "Database password.")
        .required(true)
        .repeatable(false)
        .argument("<STRING>", true)
        .callback(Poco::Util::OptionCallback<http_server>(
            this, &http_server::handle_dbpassword)));

    options.addOption(
        Poco::Util::Option("dbname", "", "Specifies the name of the database to connect to.")
        .required(true)
        .repeatable(false)
        .argument("<STRING>", true)
        .callback(Poco::Util::OptionCallback<http_server>(
            this, &http_server::handle_dbname)));
}

int http_server::main(const std::vector<std::string>& args)
{
    if (!has_help_request_)
    {
        Poco::Data::PostgreSQL::Connector::registerConnector();

        std::string connection_string =
            "host=" + dbhost_
            + " port=" + std::to_string(dbport_)
            + " user=" + dbuser_
            + " password=" + dbpassword_
            + " dbname=" + dbname_;
        Poco::Data::Session session("postgresql", connection_string);

        session <<
            "CREATE TABLE IF NOT EXISTS hothouses("
            "id SERIAL PRIMARY KEY,"
            "title VARCHAR(255) NOT NULL,"
            "description TEXT,"
            "crop VARCHAR(255))",
            Poco::Data::Keywords::now;

        auto params = new Poco::Net::HTTPServerParams();
        params->setMaxQueued(100);
        params->setMaxThreads(1);

        Poco::Net::HTTPServer server(new hothouse::request_handler_factory(connection_string), port_, params);

        server.start();
        waitForTerminationRequest();
        server.stop();
    }
    return Poco::Util::Application::EXIT_OK;
}

void http_server::handle_help(const std::string&, const std::string&)
{
    Poco::Util::HelpFormatter helpFormatter(options());
    helpFormatter.setCommand(commandName());
    helpFormatter.setUnixStyle(true);
    helpFormatter.setIndent(4);
    helpFormatter.setUsage("[OPTIONS]");
    helpFormatter.setHeader("A web server that serves hothouses.");
    helpFormatter.format(std::cout);
    stopOptionsProcessing();
    has_help_request_ = true;
}

void http_server::handle_port(const std::string&, const std::string& value)
{
    port_ = static_cast<std::uint16_t>(std::stoul(value));
}

void http_server::handle_dbhost(const std::string&, const std::string& value)
{
    dbhost_ = value;
}

void http_server::handle_dbport(const std::string&, const std::string& value)
{
    dbport_ = static_cast<std::uint16_t>(std::stoul(value));
}

void http_server::handle_dbuser(const std::string&, const std::string& value)
{
    dbuser_ = value;
}

void http_server::handle_dbpassword(const std::string&, const std::string& value)
{
    dbpassword_ = value;
}

void http_server::handle_dbname(const std::string&, const std::string& value)
{
    dbname_ = value;
}

} // hothouse