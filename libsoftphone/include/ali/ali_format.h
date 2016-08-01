//
//  ali_format.h
//

#pragma once

#include "ali_auto_ptr.h"
#include "ali_geometry.h"
#include "ali_location.h"
#include "ali_memory.h"
#include "ali_nullptr.h"
#include "ali_string.h"
#include "ali_typeof.h"


namespace ali {
struct format_helper;
template <typename Type>
struct format_t;

template <class Ret>
struct format_converter
{
    template <class Type>
    Ret operator ()(const Type &value)
    {
        typedef typename ali::meta::select<ali::meta::is_convertible<Type, Ret>::result,
            Ret,
            ali::format_helper>::type Converter;

        return Ret(Converter(value));
    }
};


template <class Type>
struct format_passer
    : public format_t<Type>
{
public:
    typedef typename ali::meta::effective<Type>::type Value;

    format_passer(Value value)
        : value(value)
    {}

    template <class T1>
    format_passer(Value value, T1 p1)
        : format_t<Type>(p1)
        , value(value)
    {}

    template <class T1,
              class T2>
    format_passer(Value value, T1 p1, T2 p2)
        : format_t<Type>(p1, p2)
        , value(value)
    {}

    template <class T1,
              class T2,
              class T3>
    format_passer(Value value, T1 p1, T2 p2, T3 p3)
        : format_t<Type>(p1, p2, p3)
        , value(value)
    {}

    template <class T1,
              class T2,
              class T3,
              class T4>
    format_passer(Value value, T1 p1, T2 p2, T3 p3, T4 p4)
        : format_t<Type>(p1, p2, p3, p4)
        , value(value)
    {}

    template <class T1,
              class T2,
              class T3,
              class T4,
              class T5>
    format_passer(Value value, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5)
        : format_t<Type>(p1, p2, p3, p4, p5)
        , value(value)
    {}


    Value value;
};


// Invalid passer
template <class Type>
struct format_passer<format_t<Type> >
{
private:
    format_passer();
};


template <class Type>
format_passer<Type> format(const Type &value)
{
    return format_passer<Type>(value);
}

template <class Type>
format_t<Type> format(const format_t<Type> &value)
{
    return value;
}

template <class Type>
format_passer<Type> format(const format_passer<Type> &value)
{
    return value;
}


namespace str {
    template <typename Type>
    ali::string format(const Type &value)
    {
        return format_converter<string>()(format_t<Type>()(value));
    }

    template <typename Type>
    ali::string format(const format_t<Type> &passer)
    {
        return format_converter<string>()(passer(static_cast<const format_passer<Type>&>(passer).value));
    }

    template <typename Type>
    ali::string format(const format_passer<Type> &passer)
    {
        return format_converter<string>()(passer(passer.value));
    }
}


namespace wstr {
    template <typename Type>
    ali::wstring format(const Type &value)
    {
        return format_converter<wstring>()(format_t<Type>()(value));
    }

    template <typename Type>
    ali::string format(const format_t<Type> &passer)
    {
        return format_converter<wstring>()(passer(static_cast<const format_passer<Type>&>(passer).value));
    }

    template <typename Type>
    ali::string format(const format_passer<Type> &passer)
    {
        return format_converter<wstring>()(passer(passer.value));
    }
}


namespace tstr {
#if defined(UNICODE)
    using wstr::format;
#else
    using str::format;
#endif
}


template <class Type>
struct format_t<const Type>
	: public format_t<Type>
{};


template <class Type,
          unsigned size>
struct format_t<Type[size]>
	: public format_t<const Type*>
{};


template <class Type>
struct format_t
{
	inline format_helper operator ()(const Type &value) const;
};

// Nesting is not allowed
template <class Type>
struct format_t<format_t<Type> >
{
private:
    format_t();
};


//-----------------------------------------------------------------------------
// Formatters for basic types -- forwarded definitions
//-----------------------------------------------------------------------------


struct format_number_flags_data
{
	int _digits;
	bool _hex;
};

struct format_number_flags
	: public format_number_flags_data
{
	format_number_flags()
	{
		this->_digits = 0;
		this->_hex = false;
	}

	format_number_flags(int digits)
	{
		this->_digits = digits;
		this->_hex = false;
	}
};


template <class T>
struct format_number_flags_template
	: public format_number_flags
{
	format_number_flags_template()
	{}

	format_number_flags_template(int digits)
		: format_number_flags(digits)
	{}

	format_t<T>& digits(int digits)
	{
		this->_digits = digits;
		return static_cast<format_t<T>&>(*this);
	}

	format_t<T>& hex(bool hex = true)
	{
		this->_hex = hex;
		return static_cast<format_t<T>&>(*this);
	}

	inline format_helper operator ()(T value) const;
};


template <>
struct format_t<signed char>
	: format_number_flags_template<signed char>
{
	format_t()
	{}

protected:
	format_t(int digits)
		: format_number_flags_template<signed char>(digits)
	{}
};


template <>
struct format_t<unsigned char>
	: format_number_flags_template<unsigned char>
{
	format_t()
	{}

protected:
	format_t(int digits)
		: format_number_flags_template<unsigned char>(digits)
	{}
};


template <>
struct format_t<short>
	: format_number_flags_template<short>
{
	format_t()
	{}

protected:
	format_t(int digits)
		: format_number_flags_template<short>(digits)
	{}
};


template <>
struct format_t<unsigned short>
	: format_number_flags_template<unsigned short>
{
	format_t()
	{}

protected:
	format_t(int digits)
		: format_number_flags_template<unsigned short>(digits)
	{}
};


template <>
struct format_t<int>
	: format_number_flags_template<int>
{
	format_t()
	{}

protected:
	format_t(int digits)
		: format_number_flags_template<int>(digits)
	{}
};


template <>
struct format_t<unsigned int>
	: format_number_flags_template<unsigned int>
{
	format_t()
	{}

protected:
	format_t(int digits)
		: format_number_flags_template<unsigned int>(digits)
	{}
};


template <>
struct format_t<long>
	: format_number_flags_template<long>
{
	format_t()
	{}

protected:
	format_t(int digits)
		: format_number_flags_template<long>(digits)
	{}
};


template <>
struct format_t<unsigned long>
	: format_number_flags_template<unsigned long>
{
	format_t()
	{}

protected:
	format_t(int digits)
		: format_number_flags_template<unsigned long>(digits)
	{}
};


struct format_bool_flags_data
{
	bool _lexical;
};


template <>
struct format_t<bool>
	: public format_bool_flags_data
{
	format_t()
	{
		this->_lexical = false;
	}

protected:
	format_t(bool lexical)
	{
		this->_lexical = lexical;
	}

public:
	format_t& lexical(bool lexical = true)
	{
		this->_lexical = lexical;
		return *this;
	}

	inline format_helper operator ()(bool value) const;
};

template <class T>
struct format_t<T*>
{
	format_helper operator ()(const T *pointer) const;
};


template <class T>
struct format_t<ali::auto_ptr<T> >
{
	format_helper operator ()(const ali::auto_ptr<T> &pointer) const;
};


//-----------------------------------------------------------------------------
// Format helper
//-----------------------------------------------------------------------------
struct format_helper
{
	format_helper(char character,
	              const format_t<char>& = format_t<char>())
		: type(format_helper::character)
	{
		this->value.character = character;
	}


	format_helper(wchar_t wcharacter,
	              const format_t<wchar_t>& = format_t<wchar_t>())
		: type(format_helper::wcharacter)
	{
		this->value.wcharacter = wcharacter;
	}


    format_helper(const char* characters,
                  const format_t<char*>& = format_t<char*>())
        : type(format_helper::characters)
    {
        this->value.characters.data = characters;
        this->value.characters.size = str_len(characters);
    }


    template <int n>
    format_helper(const char (&characters)[n],
                  const format_t<char*>& = format_t<char*>())
        : type(format_helper::characters)
    {
        this->value.characters.data = characters;
        this->value.characters.size = n;
    }


    format_helper(string_const_ref characters,
                  const format_t<string_const_ref>& = format_t<string_const_ref>())
        : type(format_helper::characters)
    {
        this->value.characters.data = characters.data();
        this->value.characters.size = characters.size();
    }


    format_helper(const wchar_t* wcharacters,
                  const format_t<wchar_t*>& = format_t<wchar_t*>())
        : type(format_helper::wcharacters)
    {
        this->value.wcharacters.data = wcharacters;
        this->value.wcharacters.size = str_len(wcharacters);
    }


    template <int n>
    format_helper(const wchar_t (&wcharacters)[n],
                  const format_t<wchar_t*>& = format_t<wchar_t*>())
        : type(format_helper::wcharacters)
    {
        this->value.wcharacters.data = wcharacters;
        this->value.wcharacters.size = n;
    }


    format_helper(wstring_const_ref wcharacters,
                  const format_t<wstring_const_ref>& = format_t<wstring_const_ref>())
        : type(format_helper::wcharacters)
    {
        this->value.wcharacters.data = wcharacters.data();
        this->value.wcharacters.size = wcharacters.size();
    }


	format_helper(const ali::string &string,
	              const format_t<ali::string>& = format_t<ali::string>())
		: type(format_helper::string)
	{
		this->value.string = &string;
	}


	format_helper(const ali::wstring &wstring,
	              const format_t<ali::wstring>& = format_t<ali::wstring>())
		: type(format_helper::wstring)
	{
		this->value.wstring = &wstring;
	}


	format_helper(signed char integer,
	              const format_number_flags &flags = format_number_flags())
		: type(format_helper::integer)
	{
		this->value.number.flags = flags;
		this->value.number.value.integer = integer;
	}


	format_helper(unsigned char natural,
	              const format_number_flags &flags = format_number_flags())
		: type(format_helper::natural)
	{
		this->value.number.flags = flags;
		this->value.number.value.natural = natural;
	}


	format_helper(short integer,
	              const format_number_flags &flags = format_number_flags())
		: type(format_helper::integer)
	{
		this->value.number.flags = flags;
		this->value.number.value.integer = integer;
	}


	format_helper(unsigned short natural,
	              const format_number_flags &flags = format_number_flags())
		: type(format_helper::natural)
	{
		this->value.number.flags = flags;
		this->value.number.value.natural = natural;
	}


	format_helper(int integer,
	              const format_number_flags &flags = format_number_flags())
		: type(format_helper::integer)
	{
		this->value.number.flags = flags;
		this->value.number.value.integer = integer;
	}


	format_helper(unsigned int natural,
	              const format_number_flags &flags = format_number_flags())
		: type(format_helper::natural)
	{
		this->value.number.flags = flags;
		this->value.number.value.natural = natural;
	}


	format_helper(long integer,
	              const format_number_flags &flags = format_number_flags())
		: type(format_helper::integer)
	{
		this->value.number.flags = flags;
		this->value.number.value.integer = integer;
	}


	format_helper(unsigned long natural,
	              const format_number_flags &flags = format_number_flags())
		: type(format_helper::natural)
	{
		this->value.number.flags = flags;
		this->value.number.value.natural = natural;
	}


    format_helper(long long integer,
                  const format_number_flags &flags = format_number_flags())
        : type(format_helper::integer)
    {
        this->value.number.flags = flags;
        this->value.number.value.integer = integer;
    }


    format_helper(unsigned long long natural,
                  const format_number_flags &flags = format_number_flags())
        : type(format_helper::natural)
    {
        this->value.number.flags = flags;
        this->value.number.value.natural = natural;
    }


	format_helper(float real,
	              const format_t<float>& = format_t<float>())
		: type(format_helper::real)
	{
		this->value.real = real;
	}


	format_helper(double real,
	              const format_t<double>& = format_t<double>())
		: type(format_helper::real)
	{
		this->value.real = real;
	}


    format_helper(long double real,
                  const format_t<double>& = format_t<double>())
        : type(format_helper::real)
    {
        this->value.real = real;
    }


	format_helper(const void *pointer,
	              const format_t<void*>& = format_t<void*>())
		: type(format_helper::pointer)
	{
		this->value.pointer = pointer;
	}


	format_helper(bool boolean,
	              const format_t<bool> &flags = format_t<bool>())
		: type(format_helper::boolean)
	{
		this->value.boolean.flags = flags;
		this->value.boolean.value = boolean;
	}

	enum {
		character,
		wcharacter,
		characters,
		wcharacters,
		string,
		wstring,
		integer,
		natural,
		real,
		pointer,
		boolean
	} type;

	union {
		char character;
		wchar_t wcharacter;
        struct
        {
            const char *data;
            int size;
        } characters;
		struct
		{
            const wchar_t *data;
            int size;
        } wcharacters;
		const ali::string *string;
		const ali::wstring *wstring;
		struct {
			format_number_flags_data flags;
			union {
				long long integer;
				unsigned long long natural;
			} value;
		} number;
		long double real;
		const void *pointer;
		struct {
			format_bool_flags_data flags;
			bool value;
		} boolean;
	} value;

	operator ali::string() const;
	operator ali::wstring() const;
};

//-----------------------------------------------------------------------------
// Formatters for basic types -- function definitions
//-----------------------------------------------------------------------------

template <class Type>
format_helper format_t<Type>::operator()(const Type &value) const
{
	return format_helper(value);
}


template <class T>
format_helper format_number_flags_template<T>::operator()(T value) const
{
	return format_helper(value, *this);
}


format_helper format_t<bool>::operator()(bool value) const
{
	return format_helper(value, *this);
}

template <class T>
format_helper format_t<T*>::operator ()(const T *pointer) const
{
	return format_helper(pointer);
}


template <class T>
format_helper format_t<ali::auto_ptr<T> >::operator ()(const ali::auto_ptr<T> &pointer) const
{
	return format_helper(pointer.get());
}


//-----------------------------------------------------------------------------
// Formatters for other basic types
//-----------------------------------------------------------------------------

template <>
struct format_t<ali::safebool>
	: public format_t<bool>
{
protected:
    format_t(bool lexical)
        : format_t<bool>(lexical)
    {}

public:
	format_helper operator ()(ali::safebool b) const
	{
		return format_helper(static_cast<bool>(b), *this);
	}
};


template <>
struct format_t<nullptr_type>
{
    format_helper operator ()(const nullptr_type&) const
	{
		return format_helper(static_cast<void*>(nullptr), format_t<void*>());
	}
};


template <>
struct format_t<ali::location>
{
	ali::string operator ()(const ali::location &loc) const;
};


template <class T>
struct format_t<ali::point<T> >
{
	format_t<T> _coordinates;
	format_t& coordinates(const format_t<T> &coordinates)
	{
		this->_coordinates = coordinates;
        return *this;
	}

	format_t()
	{}

protected:
	format_t(const format_t<T> &coordinates)
		: _coordinates(coordinates)
	{}

public:
	struct helper {
		helper(const ali::point<T> &point,
		       const format_t<T> &flags)
			: point(point)
			, flags(flags)
		{}

		const ali::point<T> &point;
		const ali::format_t<T> flags;

		operator ali::string() const
		{
			return str::format(this->flags(this->point.x()))
					+ ":"
					+ str::format(this->flags(this->point.y()));
		}

		operator ali::wstring() const
		{
            return wstr::format(this->flags(this->point.x()))
                    + L":"
                    + wstr::format(this->flags(this->point.y()));
		}
	};

	helper operator ()(const ali::point<T> &point) const
	{
		return helper(point, this->_coordinates);
	}
};


template <class T>
struct format_t<ali::size<T> >
{
	format_t<T> _coordinates;
	format_t& coordinates(const format_t<T> &coordinates)
	{
		this->_coordinates = coordinates;
        return *this;
	}

	format_t()
	{}

protected:
	format_t(const format_t<T> &coordinates)
		: _coordinates(coordinates)
	{}

public:
	struct helper {
		helper(const ali::size<T> &size,
		       const format_t<T> &flags)
			: size(size)
			, flags(flags)
		{}

		const ali::size<T> &size;
		const ali::format_t<T> flags;

        operator ali::string() const
        {
            return str::format(this->flags(this->size.w()))
                    + "x"
                    + str::format(this->flags(this->size.h()));
        }

        operator ali::wstring() const
        {
            return wstr::format(this->flags(this->size.w()))
                    + (wchar_t)0xd7 // MULTIPLICATION SIGN
                    + wstr::format(this->flags(this->size.h()));
        }
	};

	helper operator ()(const ali::size<T> &size) const
	{
		return helper(size, this->_coordinates);
	}
};


template <class T>
struct format_t<ali::rect<T> >
{
	format_t<T> _coordinates;
	format_t& coordinates(const format_t<T> &coordinates)
	{
		this->_coordinates = coordinates;
        return *this;
	}

	format_t()
	{}

protected:
	format_t(const format_t<T> &coordinates)
		: _coordinates(coordinates)
	{}

public:
	struct helper {
		helper(const ali::rect<T> &rect,
		       const format_t<T> &flags)
			: rect(rect)
			, flags(flags)
		{}

		const ali::rect<T> &rect;
		const ali::format_t<T> flags;

        operator ali::string() const
        {
            return str::format(format(this->rect.origin()).coordinates(this->flags))
                    + "+"
                    + str::format(format(this->rect.size()).coordinates(this->flags));
        }

        operator ali::wstring() const
        {
            return wstr::format(format(this->rect.origin()).coordinates(this->flags))
                    + (wchar_t)0x2192 // RIGHTWARDS ARROW
                    + wstr::format(format(this->rect.size()).coordinates(this->flags));
        }
	};

	helper operator ()(const ali::rect<T> &rect) const
	{
		return helper(rect, this->_coordinates);
	}
};

}
