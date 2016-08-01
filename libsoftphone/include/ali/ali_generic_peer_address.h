//
//  ali_generic_peer_address.h
//  ali-new
//
//  Created by Jiri Kral on 04/02/15.
//
//

#pragma once
#include "ali_string.h"

namespace ali
{

// ******************************************************************
class generic_peer_address
// ******************************************************************
{
public:     //  Enum
    struct kind
    {
        enum Type {Empty, Number, Uri};
        
        kind( Type value )
        :   _value(value)
        {}
        
        friend bool operator==( kind a, kind b )
        {
            return a._value == b._value;
        }
        
        friend bool operator!=( kind a, kind b )
        {
            return !(a == b);
        }
        
        bool is_empty( void ) const
        {
            return _value == Empty;
        }
        
        bool is_number( void ) const
        {
            return _value == Number;
        }
        
        bool is_uri( void ) const
        {
            return _value == Uri;
        }
        
        Type    _value;
    };
    
public:
    generic_peer_address( void )
    :   _kind(kind::Empty)
    {}
    
    explicit generic_peer_address(string contact);
    
    bool is_empty( void ) const
    {
        return _kind.is_empty();
    }
    
    kind get_kind( void ) const
    {
        return _kind;
    }
    
    ali::string const& get( void ) const
    {
        return _contact;
    }
    
    bool operator==( generic_peer_address const& b ) const
    {
        return  _kind == b._kind
        &&  _contact == b._contact;
    }
    
    bool operator!=( generic_peer_address const& b ) const
    {
        return !operator==(b);
    }
    
    ali::string_const_ref scheme_from_uri( void ) const;
    
    ali::string_const_ref username_from_uri( void ) const;
    
    ali::string_const_ref domain_from_uri( void ) const;
    
    ali::string full_uri(ali::string_const_ref default_scheme,
                         ali::string_const_ref default_domain ) const;
    
    bool is_equivalent_to(generic_peer_address const& b ) const;
    
private:    //  Methods
    static bool is_numerical_only( ali::string const& contact );
    
private:    //  Data members
    kind                _kind;
    string              _contact;
};

}   //  namespace ali
