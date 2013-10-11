#ifndef TOOLS_KEY_VALUE_PAIR_H
#define TOOLS_KEY_VALUE_PAIR_H

#include <stdexcept>

namespace tools
{

template<typename K, typename V>
class key_value_pair;

class connection_option
{
public:
    virtual ~connection_option() = 0;

    template<typename K, typename V>
    K key() const
    {
        using kvp = key_value_pair<K,V>;
        if (kvp const *pkvp = dynamic_cast<kvp const *>(this))
            return pkvp->key();
        else
            throw std::invalid_argument("Invalid key or value type");
    }

    template<typename K, typename V>
    V value() const
    {
        using kvp = key_value_pair<K,V>;
        if (kvp const *pkvp = dynamic_cast<kvp const *>(this))
            return pkvp->value();
        else
            throw std::invalid_argument("Invalid key or value type");
    }
};

inline connection_option::~connection_option()
{
}

template<typename K, typename V>
class key_value_pair : public connection_option
{
public:
    typedef K key_type;
    typedef V value_type;

    key_value_pair(const K &key, const V &value) : key_(key), value_(value) {}

    K key() const {return key_;}
    V value() const {return value_;}

private:
    K key_;
    V value_;
};

template<typename K, typename V>
connection_option *make_key_value_pair(const K &key, const V &value)
{
    return new key_value_pair<K, V>(key, value);
}


} // namespace tools

#endif // TOOLS_KEY_VALUE_PAIR_H
