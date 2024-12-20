#pragma once
#ifndef HOTHOUSE_HOTHOUSE_T_HPP_
#define HOTHOUSE_HOTHOUSE_T_HPP_

#include <string>

#include <nlohmann/json.hpp>
#include <Poco/Data/TypeHandler.h>

namespace hothouse
{

struct hothouse_t
{
    std::int64_t id;
    std::string title;
    std::string description;
    std::string crop;
};

inline void to_json(nlohmann::json& json, const hothouse_t& value)
{
    json = nlohmann::json::object(
        {
            { "id", value.id },
            { "title", value.title },
            { "description", value.description },
            { "crop", value.crop }
        });
}

inline void to_json(nlohmann::json& json, hothouse_t&& value)
{
    json = nlohmann::json::object(
        {
            { "id", value.id },
            { "title", std::move(value.title) },
            { "description", std::move(value.description) },
            { "crop", std::move(value.crop) }
        });
}

inline void from_json(const nlohmann::json& j, hothouse_t& h)
{
    auto it = j.find("id");
    h.id = it == j.end() ? 0 : it->get<std::int64_t>();
    it = j.find("title");
    h.title = it == j.end() ? std::string() : it->get<std::string>();
    it = j.find("description");
    h.description = it == j.end() ? std::string() : it->get<std::string>();
    it = j.find("crop");
    h.crop = it == j.end() ? std::string() : it->get<std::string>();
}

} // hothouse

namespace Poco {
namespace Data {

template <>
class TypeHandler<class hothouse::hothouse_t>
{
public:
    static void bind(
        std::size_t pos,
        const hothouse::hothouse_t& obj,
        AbstractBinder::Ptr pBinder,
        AbstractBinder::Direction dir)
    {
        poco_assert_dbg(!pBinder.isNull());
        TypeHandler<std::int64_t>::bind(pos++, obj.id, pBinder, dir);
        TypeHandler<std::string>::bind(pos++, obj.title, pBinder, dir);
        TypeHandler<std::string>::bind(pos++, obj.description, pBinder, dir);
        TypeHandler<std::string>::bind(pos++, obj.crop, pBinder, dir);
    }

    static std::size_t size()
    {
        return 4; // we handle four columns of the Table!
    }

    static void prepare(
        std::size_t pos,
        const hothouse::hothouse_t& obj,
        AbstractPreparator::Ptr pPrepare)
    {
        poco_assert_dbg(!pPrepare.isNull());
        TypeHandler<std::int64_t>::prepare(pos++, obj.id, pPrepare);
        TypeHandler<std::string>::prepare(pos++, obj.title, pPrepare);
        TypeHandler<std::string>::prepare(pos++, obj.description, pPrepare);
        TypeHandler<std::string>::prepare(pos++, obj.crop, pPrepare);
    }

    static void extract(
        std::size_t pos,
        hothouse::hothouse_t& obj,
        const hothouse::hothouse_t& def_val,
        AbstractExtractor::Ptr pExt)
        /// obj will contain the result, def_val contains values we should use when one column is NULL
    {
        poco_assert_dbg(!pExt.isNull());
        std::int64_t id;
        std::string title;
        std::string description;
        std::string crop;
        TypeHandler<std::int64_t>::extract(pos++, id, def_val.id, pExt);
        TypeHandler<std::string>::extract(pos++, title, def_val.title, pExt);
        TypeHandler<std::string>::extract(pos++, description, def_val.description, pExt);
        TypeHandler<std::string>::extract(pos++, crop, def_val.crop, pExt);
        obj.id = id;
        obj.title = std::move(title);
        obj.description = std::move(description);
        obj.crop = std::move(crop);
    }

private:
    TypeHandler();
    ~TypeHandler();
    TypeHandler(const TypeHandler& amp);
    TypeHandler& operator=(const TypeHandler& amp);
};

}
} // namespace Poco::Data

#endif // HOTHOUSE_HOTHOUSE_T_HPP_