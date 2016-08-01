/*
 *  ali_string.h
 *  ali Library
 *
 *  Copyright 2010 Acrobits, s.r.o. All rights reserved.
 *
 */

#pragma once

#include "ali_string_forward.h"
#include "ali_array_forward.h"

// ******************************************************************
#define ALI_STRING_VERSION 2
// ******************************************************************


#if ALI_STRING_VERSION == 1

#include "ali_string1.h"

#elif ALI_STRING_VERSION == 2

#include "ali_string2.h"

#else

#error "Bad ali::string version"

#endif // ALI_STRING_VERSION


#include "ali_str_utils.h"

namespace ali
{

namespace functor_types
{

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <typename is_quoted_type>
struct unquote_in_place_ : private is_quoted_type
// ******************************************************************
{
    unquote_in_place_( is_quoted_type const& is_quoted )
    :   is_quoted_type(is_quoted)
    {}

    template <typename T>
    T& operator()( T& str ) const
    {
        this->transform(str);
        return str;
    }

private:    //  Methods
    void transform( string& str ) const
    {
        if ( is_quoted_type::operator()(str) )
            str.resize(str.size() - 1).erase(0, 1);
    }

    void transform( wstring& str ) const
    {
        if ( is_quoted_type::operator()(str) )
            str.resize(str.size() - 1).erase(0, 1);
    }

    void transform( string_const_ptr& str ) const
    {
        if ( is_quoted_type::operator()(*str) )
            str = str->pointer(1, str->size() - 2);
    }

    void transform( wstring_const_ptr& str ) const
    {
        if ( is_quoted_type::operator()(*str) )
            str = str->pointer(1, str->size() - 2);
    }
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
struct unquote_in_place : unquote_in_place_<functor_types::is_quoted>
// ******************************************************************
{
    unquote_in_place( void )
    :   unquote_in_place_<functor_types::is_quoted>(ali::is_quoted)
    {}
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
struct quote_in_place
// ******************************************************************
{
    template <typename T>
    T& operator()( T& str ) const
    {
        this->transform(str);
        return str;
    }

private:    //  Methods
    void transform( string& str ) const
    {
        str.insert(0, 1, '\"').append(1, '\"');
    }

    void transform( wstring& str ) const
    {
        str.insert(0, 1, L'\"').append(1, L'\"');
    }
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
struct quote_in_place_1
// ******************************************************************
{
    quote_in_place_1( char quote )
    :   quote(quote),
        wquote(quote)
    {}

    quote_in_place_1( char quote, wchar wquote )
    :   quote(quote),
        wquote(wquote)
    {}

    template <typename T>
    T& operator()( T& str ) const
    {
        this->transform(str);
        return str;
    }

private:
    void transform( string& str ) const
    {
        str.insert(0, 1, quote).append(1, quote);
    }

    void transform( wstring& str ) const
    {
        str.insert(0, 1, wquote).append(1, wquote);
    }

    char        quote;
    wchar       wquote;
};

// ******************************************************************
// ******************************************************************

}   //  namespace functor_types

// ******************************************************************
extern functor_types::unquote_in_place const unquote_in_place;
// ******************************************************************

// ******************************************************************
template <typename is_quoted_type>
inline functor_types::unquote_in_place_<is_quoted_type>
    make_unquote_in_place( is_quoted_type const& is_quoted )
// ******************************************************************
{
    return functor_types::unquote_in_place_<is_quoted_type>(is_quoted);
}

// ******************************************************************
// ******************************************************************

// ******************************************************************
extern functor_types::quote_in_place const quote_in_place;
// ******************************************************************

// ******************************************************************
inline functor_types::quote_in_place_1 make_quote_in_place(
    char quote )
// ******************************************************************
{
    return functor_types::quote_in_place_1(quote);
}

// ******************************************************************
inline functor_types::quote_in_place_1 make_quote_in_place(
    char quote, wchar wquote )
// ******************************************************************
{
    return functor_types::quote_in_place_1(quote, wquote);
}

// ******************************************************************
// ******************************************************************

// ******************************************************************
ALI_DEPRECATED(
inline string& to_upper_case( string& str )
// ******************************************************************
{
    return to_upper_in_place(str);
});

// ******************************************************************
ALI_DEPRECATED(
inline string& to_lower_case( string& str )
// ******************************************************************
{
    return to_lower_in_place(str);
});

// ******************************************************************
inline string operator+( string s1, string const& s2 )
// ******************************************************************
{
    return s1.append(s2);
}

// ******************************************************************
inline string operator+( string s1, string::value_type const* s2 )
// ******************************************************************
{
    return s1.append(s2);
}

// ******************************************************************
inline string operator+( string s, string::value_type c )
// ******************************************************************
{
    return s.push_back(c);
}

// ******************************************************************
inline string operator+( string::value_type const* s1, string const& s2 )
// ******************************************************************
{
    return string(s1).append(s2);
}

// ******************************************************************
inline string operator+( string::value_type c, string const& s )
// ******************************************************************
{
    return string().push_back(c).append(s);
}

// ******************************************************************
inline wstring operator+( wstring s1, wstring const& s2 )
// ******************************************************************
{
    return s1.append(s2);
}

// ******************************************************************
inline wstring operator+( wstring s1, wstring::value_type const* s2 )
// ******************************************************************
{
    return s1.append(s2);
}

// ******************************************************************
inline wstring operator+( wstring s, wstring::value_type c )
// ******************************************************************
{
    return s.push_back(c);
}

// ******************************************************************
inline wstring operator+( wstring::value_type const* s1, wstring const& s2 )
// ******************************************************************
{
    return wstring(s1).append(s2);
}

// ******************************************************************
inline wstring operator+( wstring::value_type c, wstring const& s )
// ******************************************************************
{
    return wstring().push_back(c).append(s);
}
/*
// ******************************************************************
inline bool operator==( wstring const& s1, wstring const& s2 )
// ******************************************************************
{
    return s1.compare(s2) == 0;
}

// ******************************************************************
inline bool operator==( wstring::value_type const* s1, wstring const& s2 )
// ******************************************************************
{
    return s2.compare(s1) == 0;
}

// ******************************************************************
inline bool operator==( wstring const& s1, wstring::value_type const* s2 )
// ******************************************************************
{
    return s1.compare(s2) == 0;
}

// ******************************************************************
inline bool operator!=( wstring const& s1, wstring const& s2 )
// ******************************************************************
{
    return !(s1 == s2);
}

// ******************************************************************
inline bool operator!=( wstring::value_type const* s1, wstring const& s2 )
// ******************************************************************
{
    return !(s1 == s2);
}

// ******************************************************************
inline bool operator!=( wstring const& s1, wstring::value_type const* s2 )
// ******************************************************************
{
    return !(s1 == s2);
}

// ******************************************************************
inline bool operator<( wstring const& s1, wstring const& s2 )
// ******************************************************************
{
    return s1.compare(s2) < 0;
}

// ******************************************************************
inline bool operator<( wstring::value_type const* s1, wstring const& s2 )
// ******************************************************************
{
    return s2.compare(s1) > 0;
}

// ******************************************************************
inline bool operator<( wstring const& s1, wstring::value_type const* s2 )
// ******************************************************************
{
    return s1.compare(s2) < 0;
}

// ******************************************************************
inline bool operator>( wstring const& s1, wstring const& s2 )
// ******************************************************************
{
    return s1.compare(s2) > 0;
}

// ******************************************************************
inline bool operator>( wstring::value_type const* s1, wstring const& s2 )
// ******************************************************************
{
    return s2.compare(s1) < 0;
}

// ******************************************************************
inline bool operator>( wstring const& s1, wstring::value_type const* s2 )
// ******************************************************************
{
    return s1.compare(s2) > 0;
}

// ******************************************************************
inline bool operator<=( wstring const& s1, wstring const& s2 )
// ******************************************************************
{
    return !(s1 > s2);
}

// ******************************************************************
inline bool operator<=( wstring::value_type const* s1, wstring const& s2 )
// ******************************************************************
{
    return !(s1 > s2);
}

// ******************************************************************
inline bool operator<=( wstring const& s1, wstring::value_type const* s2 )
// ******************************************************************
{
    return !(s1 > s2);
}

// ******************************************************************
inline bool operator>=( wstring const& s1, wstring const& s2 )
// ******************************************************************
{
    return !(s1 < s2);
}

// ******************************************************************
inline bool operator>=( wstring::value_type const* s1, wstring const& s2 )
// ******************************************************************
{
    return !(s1 < s2);
}

// ******************************************************************
inline bool operator>=( wstring const& s1, wstring::value_type const* s2 )
// ******************************************************************
{
    return !(s1 < s2);
}
*/

// ******************************************************************
ali::wstring& prepad(ali::wstring &s,int n,
    ali::wstring::value_type c);
// ******************************************************************

// ******************************************************************
ali::string& prepad(ali::string &s,int n,
    ali::string::value_type c);
// ******************************************************************

// ******************************************************************
inline ali::string& trim_right( ali::string& str )
// ******************************************************************
{
    return str.resize(str.ref().trim_right().size());
}

// ******************************************************************
inline ali::string& trim_left( ali::string& str )
// ******************************************************************
{
    return str.erase(0, str.size() - str.ref().trim_left().size());
}

// ******************************************************************
inline ali::string& trim( ali::string& str )
// ******************************************************************
{
    return trim_left(trim_right(str));
}

// ******************************************************************
inline ali::wstring& trim_right( ali::wstring& str )
// ******************************************************************
{
    return str.resize(str.ref().trim_right().size());
}

// ******************************************************************
inline ali::wstring& trim_left( ali::wstring& str )
// ******************************************************************
{
    return str.erase(0, str.size() - str.ref().trim_left().size());
}

// ******************************************************************
inline ali::wstring& trim( ali::wstring& str )
// ******************************************************************
{
    return trim_left(trim_right(str));
}
	
// ******************************************************************
inline ali::string& trim_right(
    ali::string& str,
    string_const_ref chars )
// ******************************************************************
{
    return str.resize(str.ref().trim_right(chars).size());
}

// ******************************************************************
inline ali::string& trim_left(
    ali::string& str,
    string_const_ref chars )
// ******************************************************************
{
    return str.erase(0, str.size() - str.ref().trim_left(chars).size());
}

// ******************************************************************
inline ali::string& trim(
    ali::string& str,
    ali::string const& chars )
// ******************************************************************
{
	return trim_left(trim_right(str, chars), chars);
}

// ******************************************************************
inline ali::wstring& trim_right(
    ali::wstring& str,
    wstring_const_ref chars )
// ******************************************************************
{
    return str.resize(str.ref().trim_right(chars).size());
}

// ******************************************************************
inline ali::wstring& trim_left(
    ali::wstring& str,
    wstring_const_ref chars )
// ******************************************************************
{
    return str.erase(0, str.size() - str.ref().trim_left(chars).size());
}

// ******************************************************************
inline ali::wstring& trim(
    ali::wstring& str,
    wstring_const_ref chars )
// ******************************************************************
{
	return trim_left(trim_right(str, chars), chars);
}
    
ali::string & strip_diacritics_in_place(ali::string & str);
ali::string & strip_diacritics(ali::string & output, ali::string_const_ref input);

ali::wstring & strip_diacritics_in_place(ali::wstring & str);
ali::wstring & strip_diacritics(ali::wstring & output, ali::wstring_const_ref input);

ali::string & transliterate_to_latin_in_place(ali::string & str);
ali::string & transliterate_to_latin(ali::string & output, ali::string_const_ref input);

ali::wstring & transliterate_to_latin_in_place(ali::wstring & str);
ali::wstring & transliterate_to_latin(ali::wstring & output, ali::wstring_const_ref input);


// ******************************************************************
inline int find_non_quoted(
    ali::string const& str,
    char c,
    int pos )
// ******************************************************************
{
    return pos + str.pointer_right(pos)->index_of_first_unquoted(c);
}

// ******************************************************************
inline int find_non_quoted(
    ali::wstring const& str,
    wchar c,
    int pos )
// ******************************************************************
{
    return pos + str.pointer_right(pos)->index_of_first_unquoted(c);
}

// ******************************************************************
array<bool> test_string_in_place_transforms( void );
// ******************************************************************

}   //  namespace ali

