/*
 *  ali_str_split.h
 *  ali Library
 *
 *  Copyright 2010 Acrobits, s.r.o. All rights reserved.
 *
 */

#pragma once

#include "ali_array.h"
#include "ali_meta.h"
#include "ali_tstring_forward.h"

namespace ali
{

namespace str
{

// ******************************************************************
//  post:   entries.size() - pre(entries).size() <= max_count
array<string_const_ptr>& split(
    array<string_const_ptr>& entries,
    string_const_ref str,
    string_const_ref separators,
    int max_count = meta::integer::max_value<int>::result,
    bool trim = true );
// ******************************************************************

// ******************************************************************
//  post:   entries.size() - pre(entries).size() <= max_count
array<string>& split(
    array<string>& entries,
    string_const_ref str,
    string_const_ref separators,
    int max_count = meta::integer::max_value<int>::result,
    bool trim = true );
// ******************************************************************

// ******************************************************************
//  post:   result.size() <= max_count
inline array<string> split(
    string_const_ref str,
    string_const_ref separators,
    int max_count = meta::integer::max_value<int>::result,
    bool trim = true )
// ******************************************************************
{
    array<string> entries{};
    split(entries, str, separators, max_count, trim);
    return entries;
}

namespace hidden
{

// ******************************************************************
int split(
    string_const_ptr* entries,
    string_const_ref str,
    string_const_ref separators,
    int max_count,
    bool trim );
// ******************************************************************

// ******************************************************************
int split_lines(
    string_const_ptr* lines,
    string_const_ref str,
    string_const_ref eol,
    int max_count );
// ******************************************************************

}   //  namespace hidden

// ******************************************************************
template <int max_count>
inline int split(
    string (&entries)[max_count],
    string_const_ref str,
    string_const_ref separators,
    bool trim = true )
// ******************************************************************
{
    string_const_ptr ptr_entries[max_count];
    
    int const result = hidden::split(
        ptr_entries, str, separators, max_count, trim);

    for ( int i = result; i != 0; )
        --i, entries[i] = *ptr_entries[i];

    return result;
}

// ******************************************************************
template <int max_count>
inline int split(
    string_const_ptr (&entries)[max_count],
    string_const_ref str,
    string_const_ref separators,
    bool trim = true )
// ******************************************************************
{
    return hidden::split(entries, str, separators, max_count, trim);
}

// ******************************************************************
array<string> split_lines(
    string_const_ref str,
    string_const_ref eol
        = string_literal("\r\n") );
// ******************************************************************

// ******************************************************************
template <int max_count>
inline int split_lines(
    string_const_ptr (&lines)[max_count],
    string_const_ref str,
    string_const_ref eol
        = string_literal("\r\n") )
// ******************************************************************
{
    return hidden::split_lines(lines, str, eol, max_count);
}

// ******************************************************************
bool get_line(
    string_const_ptr& line,
    string_const_ptr& lines,
    string_const_ref eol
        = string_literal("\r\n") );
// ******************************************************************

// ******************************************************************
bool get_line_robust(
    string_const_ptr& line,
    string_const_ptr& lines );
// ******************************************************************

}   //  namespace str

namespace wstr
{

// ******************************************************************
//  post:   entries.size() - pre(entries).size() <= max_count
array<wstring_const_ptr>& split(
    array<wstring_const_ptr>& entries,
    wstring_const_ref str,
    wstring_const_ref separators,
    int max_count = meta::integer::max_value<int>::result,
    bool trim = true );
// ******************************************************************

// ******************************************************************
//  post:   entries.size() - pre(entries).size() <= max_count
array<wstring>& split(
    array<wstring>& entries,
    wstring_const_ref str,
    wstring_const_ref separators,
    int max_count = meta::integer::max_value<int>::result,
    bool trim = true );
// ******************************************************************

// ******************************************************************
//  post:   result.size() <= max_count
inline array<wstring> split(
    wstring_const_ref str,
    wstring_const_ref separators,
    int max_count = meta::integer::max_value<int>::result,
    bool trim = true )
// ******************************************************************
{
    array<wstring> entries{};
    split(entries, str, separators, max_count, trim);
    return entries;
}

namespace hidden
{

// ******************************************************************
int split(
    wstring_const_ptr* entries,
    wstring_const_ref str,
    wstring_const_ref separators,
    int max_count,
    bool trim );
// ******************************************************************

// ******************************************************************
int split_lines(
    wstring_const_ptr* lines,
    wstring_const_ref str,
    wstring_const_ref eol,
    int max_count );
// ******************************************************************

}   //  namespace hidden

// ******************************************************************
template <int max_count>
inline int split(
    wstring_const_ptr (&entries)[max_count],
    wstring_const_ref str,
    wstring_const_ref separators,
    bool trim = true )
// ******************************************************************
{
    return hidden::split(entries, str, separators, max_count, trim);
}

// ******************************************************************
array<wstring> split_lines(
    wstring_const_ref str,
    wstring_const_ref eol
        = wstring_literal(L"\r\n") );
// ******************************************************************

// ******************************************************************
template <int max_count>
inline int split_lines(
    wstring_const_ptr (&lines)[max_count],
    wstring_const_ref str,
    wstring_const_ref eol
        = wstring_literal(L"\r\n") )
// ******************************************************************
{
    return hidden::split_lines(lines, str, eol, max_count);
}

// ******************************************************************
bool get_line(
    wstring_const_ptr& line,
    wstring_const_ptr& lines,
    wstring_const_ref eol
        = wstring_literal(L"\r\n") );
// ******************************************************************

// ******************************************************************
bool get_line_robust(
    wstring_const_ptr& line,
    wstring_const_ptr& lines );
// ******************************************************************

}   //  namespace wstr

namespace tstr
{

#ifdef UNICODE

using namespace wstr;

#else

using namespace str;

#endif

}   //  namespace tstr

}   //  namespace ali

