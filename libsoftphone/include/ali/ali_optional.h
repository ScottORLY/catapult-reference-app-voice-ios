/*
 *  ali_exception.h
 *  ali Library
 *
 *  Copyright (c) 2011, 2014 Acrobits s.r.o. All rights reserved.
 *
 */

#pragma once

#include "ali_exception_if.h"
#include "ali_features.h"
#include "ali_format.h" //  <-- This one is a huge weight hit!
#include "ali_meta_common.h"


namespace ali {

namespace hidden {

#if defined(ALI_FEATURE_RVALUE_REF)
template <class T,
          bool copy_constructible = ali::meta::is_copy_constructible<T>::result>
struct assigner;

// ******************************************************************
template <class T>
struct assigner<T, true>
// ******************************************************************
{
    // ******************************************************************
    static void assign(bool is_null,
                       T * store,
                       T && value)
    // ******************************************************************
    {
        if (is_null)
            new (store) T(ali::move(value));
        else
            *store = ali::move(value);
    }
};

// ******************************************************************
template <class T>
struct assigner<T, false>
// ******************************************************************
{
    // ******************************************************************
    static void assign(bool is_null,
                       T * store,
                       T && value)
    // ******************************************************************
    {
        if (!is_null)
            reinterpret_cast<T*>(store)->~T();
        new (store) T(ali::move(value));
    }
};
#endif

}

// ******************************************************************
template <typename T>
class optional_base
// ******************************************************************
// Optional (can be none) value storage
// ******************************************************************
{
public:
    typedef T stored_type;

    static const size_t size = sizeof(stored_type);
    static const size_t storage_size = (size - 1) / sizeof(size_t) + 1;
    static const size_t aligned_size = storage_size * sizeof(size_t);


    // ******************************************************************
    optional_base(nullptr_type const& = nullptr)
    // ******************************************************************
        : _valid(false)
    {}

    // ******************************************************************
    optional_base(optional_base const& other)
    // ******************************************************************
        : _valid(false)
    {
        if (other)
            set_value(*other);
    }

#if defined(ALI_FEATURE_RVALUE_REF)
    // ******************************************************************
    optional_base(optional_base && other)
    // ******************************************************************
        : _valid(false)
    {
        if (other)
            set_value(ali::move(*other));
    }

    // Explicit to make unambiguous operator ==; implicit in optional after all
    // ******************************************************************
    template <typename TT>
    explicit optional_base(TT&& data)
    // ******************************************************************
        : _valid(false)
    {
        set_value(ali::forward<TT>(data));
    }

#else

    // Explicit to make unambiguous operator ==; implicit in optional after all
    // ******************************************************************
    template <typename TT>
    explicit optional_base(TT const& data)
    // ******************************************************************
        : _valid(false)
    {
        set_value(data);
    }

#endif

    // ******************************************************************
    ~optional_base()
    // ******************************************************************
    {
        reset();
    }

    // ******************************************************************
    void reset()
    // ******************************************************************
    {
        if (is_null())
            return;
        _valid = false;
        reinterpret_cast<stored_type*>(_data)->~stored_type();
    }

    // ******************************************************************
    stored_type& set_value(stored_type const& value)
    // ******************************************************************
    {
        if (is_null())
            new (_data) stored_type(value);
        else
            *reinterpret_cast<stored_type*>(_data) = value;
        _valid = true;
        return operator *();
    }

#if defined(ALI_FEATURE_RVALUE_REF)
    // ******************************************************************
    stored_type& set_value(stored_type && value)
    // ******************************************************************
    {
        hidden::assigner<stored_type>::assign(is_null(),
                                              reinterpret_cast<stored_type*>(_data),
                                              ali::move(value));
        _valid = true;
        return operator *();
    }
#endif

    // ******************************************************************
    stored_type& set_default_value()
    // ******************************************************************
    {
        using ali::swap;

        if (is_null())
            new (_data) stored_type();
        else
        {
            stored_type def{};
                //  This initializes even built-in
                //  types to a well defined value.
            swap(operator *(), def);
        }
        _valid = true;
        return operator *();
    }

    // ******************************************************************
    bool is_null() const
    // ******************************************************************
    {
        return !_valid;
    }

    // ******************************************************************
    operator safebool() const
    // ******************************************************************
    {
        return safeboolize(!is_null());
    }

    // ******************************************************************
    safebool operator !() const
    // ******************************************************************
    {
        return safeboolize(is_null());
    }

    // ******************************************************************
    stored_type const& operator |(stored_type const& other) const
    // ******************************************************************
    {
        return is_null() ? other : operator *();
    }

    // ******************************************************************
    optional_base const& operator |(optional_base const& other) const
    // ******************************************************************
    {
        return is_null() ? other : *this;
    }

    // ******************************************************************
    optional_base& operator |=(stored_type const& other)
    // ******************************************************************
    {
        if (is_null())
            set_value(other);
        return *this;
    }

    // ******************************************************************
    optional_base& operator |=(optional_base const& other)
    // ******************************************************************
    {
        if (is_null() && other)
            set_value(*other);
        return *this;
    }

    // ******************************************************************
    stored_type * operator ->()
    // ******************************************************************
    {
        optional_is_null_if(is_null(), ALI_HERE);
        return reinterpret_cast<stored_type *>(_data);
    }

    // ******************************************************************
    stored_type const* operator ->() const
    // ******************************************************************
    {
        optional_is_null_if(is_null(), ALI_HERE);
        return reinterpret_cast<stored_type const*>(_data);
    }

    // ******************************************************************
    stored_type & operator *()
    // ******************************************************************
    {
        return *operator ->();
    }

    // ******************************************************************
    stored_type const& operator *() const
    // ******************************************************************
    {
        return *operator ->();
    }

    // ******************************************************************
    optional_base & operator =(optional_base const& other)
    // ******************************************************************
    {
        if (other)
            set_value(*other);
        else
            reset();
        return *this;
    }

    // ******************************************************************
    bool operator ==(nullptr_type const&) const
    // ******************************************************************
    {
        return is_null();
    }

    // ******************************************************************
    bool operator ==(optional_base const& other) const
    // ******************************************************************
    {
        if (is_null())
            return other.is_null();
        return operator *() == *other;
    }

    // ******************************************************************
    bool operator ==(stored_type const& other) const
    // ******************************************************************
    {
        return !is_null() && operator *() == other;
    }

    // ******************************************************************
    bool operator !=(nullptr_type const&) const
    // ******************************************************************
    {
        return !operator ==(nullptr);
    }

    // ******************************************************************
    bool operator !=(optional_base const& other) const
    // ******************************************************************
    {
        return !operator ==(other);
    }

    // ******************************************************************
    bool operator !=(stored_type const& other) const
    // ******************************************************************
    {
        return !operator ==(other);
    }

    // ******************************************************************
    void swap(optional_base & other)
    // ******************************************************************
    {
        using ali::swap;

        if ( is_null() )
        {
            if ( other.is_null() )
                // Both null, nothing to swap
                return;

            set_default_value();
            swap(operator *(), *other);
            other.reset();
        }
        else if ( other.is_null() )
        {
            other.set_default_value();
            swap(operator *(), *other);
            reset();
        }
        else
            swap(operator *(), *other);
    }

    // ******************************************************************
    friend void swap(optional_base & first,
                     optional_base & second)
    // ******************************************************************
    {
        first.swap(second);
    }

    size_t _data[storage_size];
    bool _valid;
};


// ******************************************************************
template <class T>
class optional
// ******************************************************************
    : public optional_base<T>
{
public:
    typedef optional_base<T> super;
    typedef typename super::stored_type stored_type;

    // ******************************************************************
    optional(nullptr_type const& = nullptr)
    // ******************************************************************
    {}

    // ******************************************************************
    optional(stored_type const& data)
    // ******************************************************************
        : super(data)
    {}

    // ******************************************************************
    optional(optional const& other)
    // ******************************************************************
        : super(static_cast<super const&>(other))
    {}

#if defined(ALI_FEATURE_RVALUE_REF)
    // ******************************************************************
    optional(stored_type && data)
    // ******************************************************************
        : super(ali::move(data))
    {}

    // ******************************************************************
    optional(optional && other)
    // ******************************************************************
        : super(static_cast<super &&>(other))
    {}
#endif

    // ******************************************************************
    optional& operator |=(stored_type const& other)
    // ******************************************************************
    {
        super::operator |=(other);
        return *this;
    }

    // ******************************************************************
    optional& operator |=(optional const& other)
    // ******************************************************************
    {
        super::operator |=(other);
        return *this;
    }

    // ******************************************************************
    optional& operator =(optional const& other)
    // ******************************************************************
    {
        super::operator =(other);
        return *this;
    }
};


// ******************************************************************
template <>
class optional<string>
// ******************************************************************
    : public optional_base<string>
{
public:
    typedef optional_base<string> super;

    // ******************************************************************
    optional(nullptr_type const& = nullptr)
    // ******************************************************************
    {}

    // ******************************************************************
    optional(string const& data)
    // ******************************************************************
        : super(data)
    {}

    // ******************************************************************
    optional(string_const_ptr data)
    // ******************************************************************
    {
        if (!data.is_null())
            set_value(*data);
    }

    // ******************************************************************
    optional(char const* data)
    // ******************************************************************
        : super(string(data))
    {}

    // ******************************************************************
    optional(optional const& other)
    // ******************************************************************
        : super(static_cast<super const&>(other))
    {}

#if defined(ALI_FEATURE_RVALUE_REF)
    // ******************************************************************
    optional(string && data)
    // ******************************************************************
        : super(ali::move(data))
    {}

    // ******************************************************************
    optional(optional && other)
    // ******************************************************************
        : super(static_cast<super &&>(other))
    {}
#endif

    // ******************************************************************
    operator string_const_ptr() const
    // ******************************************************************
    {
        return is_null() ? string_const_ptr() : operator *().pointer();
    }

    using super::operator ==;
    using super::operator !=;

    // ******************************************************************
    bool operator ==(string_const_ptr other) const
    // ******************************************************************
    {
        return  is_null() == other.is_null()
            &&  (is_null() || operator *() == *other);
    }

    // ******************************************************************
    bool operator !=(string_const_ptr other) const
    // ******************************************************************
    {
        return !operator ==(other);
    }

    // ******************************************************************
    optional& operator |=(string_const_ptr other)
    // ******************************************************************
    {
        if (!is_null())
            return *this;
        return operator =(other);
    }

    // ******************************************************************
    optional& operator |=(string const& other)
    // ******************************************************************
    {
        super::operator |=(other);
        return *this;
    }

    // ******************************************************************
    optional& operator |=(optional const& other)
    // ******************************************************************
    {
        super::operator |=(other);
        return *this;
    }

    // ******************************************************************
    optional& operator =(string_const_ptr other)
    // ******************************************************************
    {
        if (other.is_null())
            reset();
        else
            super::set_value(*other);
        return *this;
    }

    // ******************************************************************
    optional& operator =(optional const& other)
    // ******************************************************************
    {
        super::operator =(other);
        return *this;
    }
};


// ******************************************************************
template <>
class optional<wstring>
// ******************************************************************
    : public optional_base<wstring>
{
public:
    typedef optional_base<wstring> super;

    // ******************************************************************
    optional(nullptr_type const& = nullptr)
    // ******************************************************************
    {}

    // ******************************************************************
    optional(wstring const& data)
    // ******************************************************************
        : super(data)
    {}

    // ******************************************************************
    optional(wstring_const_ptr data)
    // ******************************************************************
    {
        if (!data.is_null())
            set_value(*data);
    }

    // ******************************************************************
    optional(wchar_t const* data)
    // ******************************************************************
        : super(wstring(data))
    {}

    // ******************************************************************
    optional(optional const& other)
    // ******************************************************************
        : super(static_cast<super const&>(other))
    {}

#if defined(ALI_FEATURE_RVALUE_REF)
    // ******************************************************************
    optional(wstring && data)
    // ******************************************************************
        : super(ali::move(data))
    {}

    // ******************************************************************
    optional(optional && other)
    // ******************************************************************
        : super(static_cast<super &&>(other))
    {}
#endif

    // ******************************************************************
    operator wstring_const_ptr() const
    // ******************************************************************
    {
        return is_null() ? wstring_const_ptr() : operator *().pointer();
    }

    using super::operator ==;
    using super::operator !=;

    // ******************************************************************
    bool operator ==(wstring_const_ptr other) const
    // ******************************************************************
    {
        return  is_null() == other.is_null()
            &&  (is_null() || operator *() == *other);
    }

    // ******************************************************************
    bool operator !=(wstring_const_ptr other) const
    // ******************************************************************
    {
        return !operator ==(other);
    }

    // ******************************************************************
    optional& operator |=(wstring_const_ptr other)
    // ******************************************************************
    {
        if (!is_null())
            return *this;
        return operator =(other);
    }

    // ******************************************************************
    optional& operator |=(wstring const& other)
    // ******************************************************************
    {
        super::operator |=(other);
        return *this;
    }

    // ******************************************************************
    optional& operator |=(optional const& other)
    // ******************************************************************
    {
        super::operator |=(other);
        return *this;
    }

    // ******************************************************************
    optional& operator =(wstring_const_ptr other)
    // ******************************************************************
    {
        if (other.is_null())
            reset();
        else
            super::set_value(*other);
        return *this;
    }

    // ******************************************************************
    optional& operator =(optional const& other)
    // ******************************************************************
    {
        super::operator =(other);
        return *this;
    }
};


// ******************************************************************
// Formatter helper classes
// ******************************************************************
template <typename T>
struct format_t;

// ******************************************************************
template <typename T>
struct format_t<optional<T> >
// ******************************************************************
{
    // ******************************************************************
    struct helper
    // ******************************************************************
    {
        // ******************************************************************
        helper(optional<T> const& opt)
        // ******************************************************************
            : opt(opt)
        {}

        optional<T> const& opt;

        // ******************************************************************
        operator string() const
        // ******************************************************************
        {
            return opt.is_null()
                ? str::format(nullptr)
                : str::format(*opt);
        }

        // ******************************************************************
        operator wstring() const
        // ******************************************************************
        {
            return opt.is_null()
                ? wstr::format(nullptr)
                : wstr::format(*opt);
        }
    };

    // ******************************************************************
    helper operator ()(optional<T> const& opt) const
    // ******************************************************************
    {
        return helper(opt);
    }
};



// ******************************************************************
// Strings
// ******************************************************************
typedef optional<string> opt_string;
typedef optional<wstring> opt_wstring;
typedef optional<tstring> opt_tstring;



/** @brief Comparison operator
  * @param first Value
  * @param second Optional value
  * @retval true The values are equal
  * @retval false The optional is different */
template <typename T,
          typename U>
// ******************************************************************
inline bool operator ==(T const& first,
                        optional<U> const& second)
// ******************************************************************
{
    return second == first;
}


/** @brief Comparison operator
  * @param first Value
  * @param second Optional value
  * @retval true The optional is different
  * @retval false The values are equal */
template <typename T,
          typename U>
// ******************************************************************
inline bool operator !=(T const& first,
                        optional<U> const& second)
// ******************************************************************
{
    return second != first;
}


namespace meta {

// ******************************************************************
// ******************************************************************
template <typename T>
struct is_optional
	: define_false_result
{};

template <typename T>
struct is_optional<optional<T> >
	: define_true_result
{};

// ******************************************************************
// ******************************************************************

template <typename T>
struct un_optional
	: define_type<T>
{};


template <typename T>
struct un_optional<optional<T> >
	: define_type<T>
{};

}

}
