#ifndef CPP_DB_VALUE_INL
#define CPP_DB_VALUE_INL

namespace cpp_db
{

template<>
struct value::concrete_holder<int64_t> : public value::abstract_holder
{
    explicit concrete_holder(int64_t value_in)
        : value(value_in)
    {
    }
    explicit concrete_holder(int32_t value_in)
        : value(value_in)
    {
    }
    explicit concrete_holder(int16_t value_in)
        : value(value_in)
    {
    }
    explicit concrete_holder(int8_t value_in)
        : value(value_in)
    {
    }

    void const * get_value() const override
    {
        return &value;
    }

    std::type_index get_value_type() const override
    {
        return typeid(int64_t);
    }

    concrete_holder *clone() const override
    {
        return new concrete_holder(value);
    }

    template<typename CastType>
    CastType cast_to() const
    {
        return value;
    }

    int64_t value;
};

template<>
struct value::concrete_holder<uint64_t> : public value::abstract_holder
{
    explicit concrete_holder(uint64_t value_in)
        : value(value_in)
    {
    }
    explicit concrete_holder(uint32_t value_in)
        : value(value_in)
    {
    }
    explicit concrete_holder(uint16_t value_in)
        : value(value_in)
    {
    }
    explicit concrete_holder(uint8_t value_in)
        : value(value_in)
    {
    }

    void const * get_value() const override
    {
        return &value;
    }

    std::type_index get_value_type() const override
    {
        return typeid(uint64_t);
    }

    concrete_holder *clone() const override
    {
        return new concrete_holder(value);
    }

    template<typename CastType>
    CastType cast_to() const
    {
        return value;
    }

    uint64_t value;
};

template<>
struct value::concrete_holder<long double> : public value::abstract_holder
{
    explicit concrete_holder(long double value_in)
        : value(value_in)
    {
    }
    explicit concrete_holder(double value_in)
        : value(value_in)
    {
    }
    explicit concrete_holder(float value_in)
        : value(value_in)
    {
    }

    void const * get_value() const override
    {
        return &value;
    }

    std::type_index get_value_type() const override
    {
        return typeid(long double);
    }

    concrete_holder *clone() const override
    {
        return new concrete_holder(value);
    }

    template<typename CastType>
    CastType cast_to() const;

    long double value;
};

template<>
inline long double value::concrete_holder<long double>::cast_to() const
{
    return static_cast<long double>(value);
}

template<>
inline double value::concrete_holder<long double>::cast_to() const
{
    return static_cast<double>(value);
}

template<>
inline float value::concrete_holder<long double>::cast_to() const
{
    return static_cast<float>(value);
}

template<>
struct value::concrete_holder<std::string> : public value::abstract_holder
{
    explicit concrete_holder(std::string value_in)
        : value(value_in)
    {
    }
    explicit concrete_holder(const char *value_in)
        : value(value_in)
    {
    }
    explicit concrete_holder(char *value_in)
        : value(value_in)
    {
    }

    void const * get_value() const override
    {
        return &value;
    }

    std::type_index get_value_type() const override
    {
        return typeid(std::string);
    }

    concrete_holder *clone() const override
    {
        return new concrete_holder(value);
    }

    template<typename CastType>
    CastType cast_to() const
    {
        return value;
    }

    std::string value;
};

}

#endif // CPP_DB_VALUE_INL
