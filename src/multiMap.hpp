#ifndef MANDELLANDSCAPE_MULTIMAP_HPP
#define MANDELLANDSCAPE_MULTIMAP_HPP

#include <map>
#include <vector>

// simple wrapper around std::multimap, for more convenient use
template<class Key, class Value>
class MultiMap {
public:
    /*
    MultiMap(std::vector<std::pair<Key, vector<Value>> const& init)
    {
        for(auto entry : init)
        for(auto value : pair) {
            m_map.insert(entry->first, value);
        }
    }

    auto
    operator[](Key key) -> std::vector<Value>
    {
        for()
    }

private:
    std::multimap<Key, Value> m_map;
    */
}

#endif
