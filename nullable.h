#ifndef CPP_DB_NULLABLE_H
#define CPP_DB_NULLABLE_H

#include "value_is_null.h"

namespace cpp_db
{

template<typename T>
class nullable
{
public:
    // default
    nullable() : value_(T()), is_null_(true) {}

    // copying
    nullable(const nullable &other) : value_(other.value_), is_null_(other.is_null_) {}
    nullable &operator=(const nullable &other) {value_ = other.value_; is_null_ = other.is_null_;}

    // converting
    nullable(const T &value) : value_(value), is_null_(false) {}
    operator T() const {return value();}
    nullable &operator=(const T &value) {value_=value; is_null_=false;return *this;}

    // getter/setter
    T value() const {throw_if_this_is_null(); return value_;}
    T value(const T &defaultValue) const {return is_null_ ? defaultValue : value_;}
    void set_value(const T &value) {is_null_ = false; value_ = value;}

    // null handling
    bool is_null() const {return is_null_;}
    void set_null() {is_null_ = true;}

    // compare with value
    bool operator==(const T &value) const
    {
        throw_if_this_is_null();
        return value_ == value;
    }
    bool operator!=(const T &value) const
    {
        throw_if_this_is_null();
        return value_ != value;
    }

    bool operator<(const T &value) const
    {
        throw_if_this_is_null();
        return value_ < value;
    }

    bool operator>(const T &value) const
    {
        throw_if_this_is_null();
        return value_ > value;
    }

    bool operator>=(const T &value) const
    {
        throw_if_this_is_null();
        return value_ >= value;
    }

    bool operator<=(const T &value) const
    {
        throw_if_this_is_null();
        return value_ <= value;
    }

    // compare with other instance
    bool operator==(const nullable &other) const
    {
        throw_if_this_or_other_null();
        return value_ == other.value_;
    }

    bool operator!=(const nullable &other) const
    {
        throw_if_this_or_other_null();
        return value_ != other.value_;
    }

    bool operator<(const nullable &other) const
    {
        throw_if_this_or_other_null();
        return value_ < other.value_;
    }

    bool operator>(const nullable &other) const
    {
        throw_if_this_or_other_null();
        return value_ > other.value_;
    }

    bool operator>=(const nullable &other) const
    {
        throw_if_this_or_other_null();
        return value_ >= other.value_;
    }

    bool operator<=(const nullable &other) const
    {
        throw_if_this_or_other_null();
        return value_ <= other.value_;
    }

private:
    void throw_if_this_is_null() const
    {
        if (is_null_)
            throw value_is_null();
    }

    void throw_if_this_or_other_null(const nullable &other) const
    {
        if (is_null_ || other.is_null_)
            throw value_is_null();
    }

private:
    T value_;
    bool is_null_;
};

template<typename T>
T coalesce(const nullable<T> &first, const nullable<T> &second)
{
    if (!first.is_null())
        return first.value();
    if (!second.is_null())
        return second.value();
    return T();
}

} // namespace cpp_db

#endif // CPP_DB_NULLABLE_H
