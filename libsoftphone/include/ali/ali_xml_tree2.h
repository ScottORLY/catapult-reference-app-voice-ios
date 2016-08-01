/*
 *  ali_xml_tree2.h
 *  ali Library
 *
 *  Copyright 2010 Acrobits, s.r.o. All rights reserved.
 *
 */

#pragma once

#include "ali_xml_tree2_forward.h"
#include "ali_array.h"
#include "ali_deprecated.h"
#include "ali_exception_memory.h"
#include "ali_string.h"
#include "ali_string_map.h"

namespace ali
{

namespace xml
{

// ******************************************************************
class trees
// ******************************************************************
{
public:     // Methods
    trees( void )
    {}

    ~trees( void );

    trees( trees const& b );

    trees& operator=( trees b )
    {
        swap(b);
        return *this;
    }

    int size( void ) const
    {
        return _trees.size();
    }

    bool is_empty( void ) const
    {
        return _trees.is_empty();
    }
    
    tree& add( void );
    tree& add( ali::string const& name );
    tree& add( ali::string const& name, ali::string const& data );
    tree& add( tree const& t );
    tree& add( ali::auto_ptr<tree> t );

    tree& add_swap( tree& t );

    tree& insert( int idx, ali::string const& name );
    tree& insert( int idx, ali::string const& name, ali::string const& data );
    tree& insert( int idx, tree const& t );
    tree& insert( int idx, ali::auto_ptr<tree> t );

    tree& insert_swap( int idx, tree& t );

    int remove_all( string_const_ref name );
    ali::auto_ptr<tree> remove( int i );

    trees& move( int dstIdx, int srcIdx );

    trees& clear( void );

    tree* find_with_attr(
        string_const_ref name,
        string_const_ref attr_name,
        string_const_ref attr_value );

    tree const* find_with_attr(
        string_const_ref name,
        string_const_ref attr_name,
        string_const_ref attr_value ) const;

    tree* find_first( string_const_ref name )
    {
        int const idx = find_first_index(name);
        if ( idx < 0 ) return nullptr; else return _trees[idx];
    }

    tree const* find_first( string_const_ref name ) const
    {
        int const idx = find_first_index(name);
        if ( idx < 0 ) return nullptr; else return _trees[idx];
    }

    tree* find_first(
        string_const_ref name1, string_const_ref name2 );

    tree const* find_first(
        string_const_ref name1, string_const_ref name2 ) const;

    tree* find_first(
        string_const_ref name1, string_const_ref name2,
        string_const_ref name3 );

    tree const* find_first(
        string_const_ref name1, string_const_ref name2,
        string_const_ref name3 ) const;

    tree* find_first(
        string_const_ref name1, string_const_ref name2,
        string_const_ref name3, string_const_ref name4 );

    tree const* find_first(
        string_const_ref name1, string_const_ref name2,
        string_const_ref name3, string_const_ref name4 ) const;

    tree* find_first(
        string_const_ref name1, string_const_ref name2,
        string_const_ref name3, string_const_ref name4,
        string_const_ref name5 );

    tree const* find_first(
        string_const_ref name1, string_const_ref name2,
        string_const_ref name3, string_const_ref name4,
        string_const_ref name5 ) const;

    int find_first_index( string_const_ref name ) const
    {
        return find_next_index(name);
    }

    int find_next_index(
        string_const_ref name,
        int excluded_start_index = -1 ) const;

    tree& operator[]( char const* name )
    {
        return operator[](c_string_const_ref(name));
    }

    tree& operator[]( string_const_ref name )
    {
        if ( tree* tree = find_first(name) )
            return *tree;
        else
            return add(name);
    }
    
    tree& operator[]( ali::string const& name )
    {
        if ( tree* tree = find_first(name) )
            return *tree;
        else
            return add(name);
    }

    tree const& operator[]( string_const_ref name ) const
    {
        if ( tree const* const tree = find_first(name) )
            return *tree;
        else
            return empty_tree();
    }

    tree& operator[]( int i )
    {
        return *_trees[i];
    }

    tree const& operator[]( int i ) const
    {
        return *_trees[i];
    }

    ali::ptr_array<tree>::iterator begin( void )
    {
        return _trees.begin();
    }

    friend ali::ptr_array<tree>::iterator begin( trees & t )
    {
        return t.begin();
    }

    ali::ptr_array<tree>::const_iterator begin( void ) const
    {
        return _trees.begin();
    }

    friend ali::ptr_array<tree>::const_iterator begin( trees const& t )
    {
        return t.begin();
    }

    ali::ptr_array<tree>::iterator end( void )
    {
        return _trees.end();
    }

    friend ali::ptr_array<tree>::iterator end( trees & t )
    {
        return t.end();
    }

    ali::ptr_array<tree>::const_iterator end( void ) const
    {
        return _trees.end();
    }

    friend ali::ptr_array<tree>::const_iterator end( trees const& t )
    {
        return t.end();
    }

    void swap( trees& b )
    {
        using ali::swap;
        swap(_trees, b._trees);
    }

    friend void swap( trees& a, trees& b )
    {
        a.swap(b);
    }

    bool operator==( trees const& b ) const;

    bool operator!=( trees const& b ) const
    {
        return !operator==(b);
    }

private:    //  Methods
    static ali::xml::tree const& empty_tree( void );

private:    // Data members
    ali::ptr_array<tree>    _trees;
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
struct tree
// ******************************************************************
{
    tree( void )
    {}
    
    explicit tree( ali::string const& name )
    :   name(name)
    {}
    
    tree( ali::string const& name, ali::string const& data )
    :   name(name),
        data(data)
    {}
    
    tree& operator=( tree b )
    {
        swap(b);
        return *this;
    }

    bool is_empty( void ) const
    {
        return  name.is_empty()
            &&  data.is_empty()
            &&  nodes.is_empty()
            &&  attrs.is_empty();
    }
    
    void swap( tree& b )
    {
        using ali::swap;
        swap(name, b.name);
        swap(data, b.data);
        swap(nodes, b.nodes);
        swap(attrs, b.attrs);
    }

    friend void swap( tree& a, tree& b )
    {
        a.swap(b);
    }
    
    tree& merge_with( tree const& b );
    
    bool operator==( tree const& b ) const
    {
        return name == b.name && data == b.data
            && attrs == b.attrs && nodes == b.nodes;
    }
    
    bool operator!=( tree const& b ) const
    {
        return !operator==(b);
    }
    
    ali::string name;
    ali::string data;
    trees       nodes;
    attributes  attrs;
};


//
// === Inlines ======================================================
//

// ******************************************************************
inline trees::~trees( void )
// ******************************************************************
{
    clear();
}

// ******************************************************************
inline trees::trees( trees const& b )
// ******************************************************************
{
    trees t;
    for ( int i = 0; i < b._trees.size(); ++i )
        t.add(ali::make_auto_ptr(
            ali::out_of_memory_if_null(
                new tree(*b._trees[i]), ALI_HERE)));
            
    swap(t);
}

// ******************************************************************
inline tree& trees::add( void )
// ******************************************************************
{
    return add(ali::make_auto_ptr(
        ali::out_of_memory_if_null(
            new tree(), ALI_HERE)));
}

// ******************************************************************
inline tree& trees::add( ali::string const& name )
// ******************************************************************
{
    return add(ali::make_auto_ptr(
        ali::out_of_memory_if_null(
            new tree(name), ALI_HERE)));
}

// ******************************************************************
inline tree& trees::add(
    ali::string const& name,
    ali::string const& data )
// ******************************************************************
{
    return add(ali::make_auto_ptr(
        ali::out_of_memory_if_null(new tree(name, data), ALI_HERE)));
}

// ******************************************************************
inline tree& trees::add( tree const& t )
// ******************************************************************
{
    return add(ali::make_auto_ptr(
        ali::out_of_memory_if_null(new tree(t), ALI_HERE)));
}

// ******************************************************************
inline tree& trees::add( ali::auto_ptr<tree> t )
// ******************************************************************
{
    _trees.push_back(t.get());
    t.release();
    return *_trees.back();
}

// ******************************************************************
inline tree& trees::add_swap( tree& t )
// ******************************************************************
{
    ali::auto_ptr<tree> tt(
        ali::out_of_memory_if_null(new tree, ALI_HERE));
    tt->swap(t);
    return add(tt);
}

// ******************************************************************
inline tree& trees::insert( int idx, ali::string const& name )
// ******************************************************************
{
    return insert(idx, ali::make_auto_ptr(
        ali::out_of_memory_if_null(new tree(name), ALI_HERE)));
}

// ******************************************************************
inline tree& trees::insert(
    int idx,
    ali::string const& name,
    ali::string const& data )
// ******************************************************************
{
    return insert(idx, ali::make_auto_ptr(
        ali::out_of_memory_if_null(new tree(name, data), ALI_HERE)));
}

// ******************************************************************
inline tree& trees::insert( int idx, tree const& t )
// ******************************************************************
{
    return insert(idx, ali::make_auto_ptr(
        ali::out_of_memory_if_null(new tree(t), ALI_HERE)));
}

// ******************************************************************
inline tree& trees::insert( int idx, ali::auto_ptr<tree> t )
// ******************************************************************
{
    _trees.insert(idx, t.get());
    t.release();
    return *_trees[idx];
}

// ******************************************************************
inline tree& trees::insert_swap( int idx, tree& t )
// ******************************************************************
{
    ali::auto_ptr<tree> tt(
        ali::out_of_memory_if_null(new tree, ALI_HERE));
    tt->swap(t);
    return insert(idx, tt);
}

// ******************************************************************
inline int trees::remove_all( string_const_ref name )
// ******************************************************************
{
    return _trees.erase_if(
        [name] ( tree* t ) -> bool
        {
            ali::auto_ptr<tree> temp(
                t->name == name ? t : nullptr);
            return !temp.is_null();
        });
}

// ******************************************************************
inline ali::auto_ptr<tree> trees::remove( int i )
// ******************************************************************
{
    ali::auto_ptr<tree> t(_trees[i]);
    _trees.erase(i);
    return t;
}

// ******************************************************************
inline trees& trees::move( int dstIdx, int srcIdx )
// ******************************************************************
{
    if ( dstIdx != srcIdx )
    {
        ali::auto_ptr<tree> t(_trees[srcIdx]);
        _trees.erase(srcIdx);
        _trees.insert(dstIdx, t.get());
        t.release();
    }
    return *this;
}

// ******************************************************************
inline trees& trees::clear( void )
// ******************************************************************
{
    while ( !_trees.is_empty() )
    {
        ali::auto_ptr<tree> t(_trees.back());
        _trees.erase_back();
    }
    return *this;
}

// ******************************************************************
inline tree* trees::find_with_attr(
    string_const_ref name,
    string_const_ref attr_name,
    string_const_ref attr_value )
// ******************************************************************
{
    for ( int idx = -1; (idx = find_next_index(name, idx)) >= 0; )
        if ( attribute const* const attr = _trees[idx]->attrs.find(attr_name) )
            if ( attr->value == attr_value )
                return _trees[idx];
    return nullptr;
}

// ******************************************************************
inline tree const* trees::find_with_attr(
    string_const_ref name,
    string_const_ref attr_name,
    string_const_ref attr_value ) const
// ******************************************************************
{
    for ( int idx = -1; (idx = find_next_index(name, idx)) >= 0; )
        if ( attribute const* const attr = _trees[idx]->attrs.find(attr_name) )
            if ( attr->value == attr_value )
                return _trees[idx];
    return nullptr;
}

// ******************************************************************
inline tree* trees::find_first(
    string_const_ref name1, string_const_ref name2 )
// ******************************************************************
{
    int const idx = find_first_index(name1);
    if ( idx < 0 ) return nullptr;
    return _trees[idx]->nodes.find_first(name2);
}

// ******************************************************************
inline tree const* trees::find_first(
    string_const_ref name1, string_const_ref name2 ) const
// ******************************************************************
{
    int const idx = find_first_index(name1);
    if ( idx < 0 ) return nullptr;
    return _trees[idx]->nodes.find_first(name2);
}

// ******************************************************************
inline tree* trees::find_first(
    string_const_ref name1, string_const_ref name2,
    string_const_ref name3 )
// ******************************************************************
{
    int const idx = find_first_index(name1);
    if ( idx < 0 ) return nullptr;
    return _trees[idx]->nodes.find_first(name2, name3);
}

// ******************************************************************
inline tree const* trees::find_first(
    string_const_ref name1, string_const_ref name2,
    string_const_ref name3 ) const
// ******************************************************************
{
    int const idx = find_first_index(name1);
    if ( idx < 0 ) return nullptr;
    return _trees[idx]->nodes.find_first(name2, name3);
}

// ******************************************************************
inline tree* trees::find_first(
    string_const_ref name1, string_const_ref name2,
    string_const_ref name3, string_const_ref name4 )
// ******************************************************************
{
    int const idx = find_first_index(name1);
    if ( idx < 0 ) return nullptr;
    return _trees[idx]->nodes.find_first(name2, name3, name4);
}

// ******************************************************************
inline tree const* trees::find_first(
    string_const_ref name1, string_const_ref name2,
    string_const_ref name3, string_const_ref name4 ) const
// ******************************************************************
{
    int const idx = find_first_index(name1);
    if ( idx < 0 ) return nullptr;
    return _trees[idx]->nodes.find_first(name2, name3, name4);
}

// ******************************************************************
inline tree* trees::find_first(
    string_const_ref name1, string_const_ref name2,
    string_const_ref name3, string_const_ref name4,
    string_const_ref name5 )
// ******************************************************************
{
    int const idx = find_first_index(name1);
    if ( idx < 0 ) return nullptr;
    return _trees[idx]->nodes.find_first(name2, name3, name4, name5);
}

// ******************************************************************
inline tree const* trees::find_first(
    string_const_ref name1, string_const_ref name2,
    string_const_ref name3, string_const_ref name4,
    string_const_ref name5 ) const
// ******************************************************************
{
    int const idx = find_first_index(name1);
    if ( idx < 0 ) return nullptr;
    return _trees[idx]->nodes.find_first(name2, name3, name4, name5);
}

// ******************************************************************
inline int trees::find_next_index(
    string_const_ref name, int excluded_start_index ) const
// ******************************************************************
{
    for ( int i = excluded_start_index + 1; i < _trees.size(); ++i )
        if ( _trees[i]->name == name )
            return i;
    return -1;
}

// ******************************************************************
inline bool trees::operator==( trees const& b ) const
// ******************************************************************
{
    if ( _trees.size() != b._trees.size() )
        return false;
    
    for ( int i = 0; i < _trees.size(); ++i )
        if ( *_trees[i] != *b._trees[i] )
            return false;
            
    return true;
}

// ******************************************************************
// ******************************************************************

// ******************************************************************
class string_from_tree : public ali::string
// ******************************************************************
{
public:
    string_from_tree( tree const& t );
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
class pretty_string_from_tree : public ali::string
// ******************************************************************
{
public:
    pretty_string_from_tree(
        tree const& t,
        int indent_step = 2,
        int indent = 0 );

    pretty_string_from_tree(
        trees const& t,
        int indent_step = 2,
        int indent = 0 );
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
class openxcap_string_from_tree : public ali::string
// ******************************************************************
{
public:
    openxcap_string_from_tree( tree const& t );
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
class openxcap_pretty_string_from_tree : public ali::string
// ******************************************************************
//  OpenXCAP doesn't understand short empty tags, e.g. <Empty />
// ******************************************************************
{
public:
    openxcap_pretty_string_from_tree(
        tree const& t,
        int indent_step = 2,
        int indent = 0 );

    openxcap_pretty_string_from_tree(
        trees const& t,
        int indent_step = 2,
        int indent = 0 );
};


// ******************************************************************
class encoded : public ali::string
// ******************************************************************
{
public:
    encoded(ali::string_const_ref str);
};

// ******************************************************************
class decoded : public ali::string
// ******************************************************************
{
    decoded(ali::string_const_ref str);
};


}   //  namespace xml


// ali::format
template <class Type>
struct format_t;

template <>
struct format_t<ali::xml::tree>
{
public:
    format_t()
        : _indent_step(-1)
    {}

    format_t(bool pretty)
        : _indent_step(-1)
    {}

protected:
    int _indent_step;

public:
    format_t& pretty(bool pretty = true)
    {
        this->_indent_step = pretty ? 2 : -1;
        return *this;
    }

    format_t& pretty(int indent_step)
    {
        this->_indent_step = indent_step;
        return *this;
    }

    format_t& compact()
    {
        this->_indent_step = -1;
        return *this;
    }

    string operator ()(const ali::xml::tree &value) const
    {
        if (this->_indent_step >= 0)
            return xml::pretty_string_from_tree(value, this->_indent_step);
        return xml::string_from_tree(value);
    }
};

// ali::printf
inline ali::string& format(
    ali::string& str,
    ali::xml::tree const& value,
    string_const_ref format )
{
    return str.append(ali::xml::pretty_string_from_tree(value));
}

}   //  namespace ali

