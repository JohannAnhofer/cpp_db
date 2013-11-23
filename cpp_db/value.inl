#ifndef CPP_DB_VALUE_INL
#define CPP_DB_VALUE_INL

namespace cpp_db
{

template<>
struct value::concrete_holder<null_type> : public abstract_holder
{
    explicit concrete_holder(null_type)
    {
    }

    void const * get_value() const override
    {
        throw std::runtime_error("Value is null");
    }

    std::type_index get_type() const override
    {
        return typeid(null_type);
    }

    concrete_holder *clone() const override
    {
        return new concrete_holder<null_type>(null_type{});
    }
};

}

#endif // CPP_DB_VALUE_INL
