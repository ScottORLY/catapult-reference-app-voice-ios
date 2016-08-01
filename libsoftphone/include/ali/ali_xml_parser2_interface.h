/*
 *  ali_xml_parser2.h
 *  ali Library
 *
 *  Copyright 2010 Acrobits, s.r.o. All rights reserved.
 *
 */

#pragma once

#include "ali_filesystem2_common.h"
#include "ali_xml_tree2_forward.h"
#include "ali_tstring_forward.h"

namespace ali
{

namespace xml
{

// ******************************************************************
bool parse( ali::xml::tree& root, char const* str, int size, int * processed = 0 );
// ******************************************************************

// ******************************************************************
inline bool parse( ali::xml::tree& root, ali::string_const_ref str, int * processed = 0 )
// ******************************************************************
{
    return ali::xml::parse(root, str.data(), str.size(), processed);
}

// ******************************************************************
bool load(
    ali::xml::tree& root,
    ali::tstring const& fileName );
// ******************************************************************

// ******************************************************************
inline bool load(
    ali::xml::tree& root,
    ali::filesystem2::path const& path )
// ******************************************************************
{
    return load(root, path.format_platform_string());
}

// ******************************************************************
bool save(
    ali::xml::tree const& root,
    ali::tstring const& fileName );
// ******************************************************************

// ******************************************************************
inline bool save(
    ali::xml::tree const& root,
    ali::filesystem2::path const& path )
// ******************************************************************
{
    return save(root, path.format_platform_string());
}

} // namespace xml

} // namespace ali

