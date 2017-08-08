#ifndef PCRE2PP_PRIVATE_TABLE_MAP_H
#define PCRE2PP_PRIVATE_TABLE_MAP_H

#include <cstdint>
#include <cstdlib>
#include <map>
#include <string>

namespace pcre2 {
namespace details {

class table_map {
public:
    static table_map& instance()
    {
        static table_map self;
        return self;
    }

    ~table_map()
    {
        for (auto& x : this->m_map) {
            // By default, pcre2_maketables() uses malloc()
            std::free(x.second);
        }

        this->m_map.clear();
    }

    std::uint8_t* get(const std::string& key) const
    {
        map_t::const_iterator it = this->m_map.find(key);
        if (it != this->m_map.end()) {
            return it->second;
        }

        return nullptr;
    }

    void set(const std::string& key, std::uint8_t* tbl)
    {
        map_t::const_iterator it = this->m_map.find(key);
        if (it == this->m_map.end()) {
            this->m_map.insert(std::make_pair(key, tbl));
        }
    }

private:
    using map_t = std::map<std::string, std::uint8_t*>;
    map_t m_map;

    table_map()
        : m_map()
    {
    }

    table_map(const table_map&) = delete;
    table_map(table_map&&) = delete;
    table_map& operator=(const table_map&) = delete;
    table_map& operator=(table_map&&) = delete;
};

}
}



#endif // PCRE2PP_PRIVATE_TABLE_MAP_H
