#pragma once

#include "ali_string_map_forward.h"
#include "ali_array.h"
#include "ali_string.h"
#include "ali_utility.h"

namespace ali
{

// ******************************************************************
/// \brief  Aggregates two \ref string data members `name` and
/// `value`. Also stores default comparator for comparing the
/// `name` data members.
///
/// \tparam is_less_type
///         Type of the default comparator to be used for comparing
///         the `name` data members. Typically \ref ali::less or
///         \ref ali::nocase_less. Defaults to \ref ali::less (case
///         sensitive comparison).
///
/// \pre    Object of the `is_less_type` type is a binary predicate
///         over strings implementing a strict weak ordering.
///
// ******************************************************************
template <typename is_less_type>
struct string_map_entry : private is_less_type
// ******************************************************************
{
        /// \brief  Default constructor.
        /// Constructs empty string map entry.
        ///
        /// \post   <tt>this->name.is_empty() && this->value.is_empty()</tt>
        ///
        /// \throws noexcept
        ///
    string_map_entry( void ) {}
    
        /// \brief  Initializes this string map entry with the
        /// specified name.
        ///
        /// \param[in]  name
        ///         String to initialize the `name` data member with.
        ///
        /// \post   <tt>this->name == name && this->value.is_empty()</tt>
        ///
        /// \throws noexcept <tt>(false)</tt>
        ///
    explicit string_map_entry(
        ali::string const& name )
    :   name(name)
    {}

        /// \brief  Initializes this string map entry with the
        /// specified name and value.
        ///
        /// \param[in]  name
        ///         String to initialize the `name` data member with.
        /// \param[in]  value
        ///         String to initialize the `value` data member with.
        ///
        /// \post   <tt>this->name == name && this->value == value</tt>
        ///
        /// \throws noexcept <tt>(false)</tt>
        ///
    string_map_entry(
        ali::string const& name,
        ali::string const& value )
    :   name(name),
        value(value)
    {}

        /// \brief  Determines whether the two given names are
        /// equal (m.p. equivalent) using the specified comparator.
        ///
        /// \param[in]  a
        /// \param[in]  b
        ///         Two name strings to compare.
        /// \param[in]  is_less
        ///         Comparator to be used for the comparison.
        ///
        /// \pre    `is_less` is a binary predicate over
        ///         strings implementing a strict weak ordering.
        ///
        /// \post   <tt>result == !is_less(a, b) && !is_less(b, a)</tt>
        ///
        /// \retval true
        ///         According to the specified comparator, the two
        ///         given names are equivalent.
        /// \retval false
        ///         Otherwise.
        ///
        /// \throws noexcept <tt>(noexcept(is_less(a,b)))</tt>
        ///
    template <typename other_is_less_type>
    static bool are_names_equal(
        string_const_ref a,
        string_const_ref b,
        other_is_less_type is_less )
    {
        return !is_less(a, b) && !is_less(b, a);
    }

        /// \brief  Determines whether the two given names are
        /// equal (m.p. equivalent) using the default comparator.
        ///
        /// See \ref are_names_equal(ali::string_const_ref,ali::string_const_ref,other_is_less_type).
        ///
        /// \post   <tt>result == this->are_names_equal(a, b, this->is_less())</tt>
        ///
    bool are_names_equal(
        string_const_ref a,
        string_const_ref b ) const
    {
        return are_names_equal(a, b, is_less());
    }

        /// \brief  Determines whether the `name` data member of this
        /// string map entry is equal (m.p. equivalent) to the
        /// given string using the specified comparator.
        ///
        /// \param[in]  b
        ///         String to compare the `name` data member with.
        /// \param[in]  is_less
        ///         Comparator to be used for the comparison.
        ///
        /// \pre    `is_less` is a binary predicate over
        ///         strings implementing a strict weak ordering.
        ///
        /// \post   <tt>result == this->are_names_equal(this->name, b, is_less)</tt>
        ///
        /// \retval true
        ///         According to the specified comparator, the `name`
        ///         data member is equivalent with the given string.
        /// \retval false
        ///         Otherwise.
        ///
        /// \throws noexcept <tt>(noexcept(is_less(a,b)))</tt>
        ///
    template <typename other_is_less_type>
    bool is_name_equal_to(
        string_const_ref b,
        other_is_less_type is_less ) const
    {
        return are_names_equal(name, b, is_less);
    }

        /// \brief  Determines whether the `name` data member of this
        /// string map entry is equal (m.p. equivalent) to the
        /// given string using the default comparator.
        ///
        /// \param[in]  b
        ///         String to compare the `name` data member with.
        ///
        /// \post   <tt>result == this->are_names_equal(this->name, b, this->is_less())</tt>
        ///
        /// \retval true
        ///         According to the default comparator, the `name`
        ///         data member is equivalent with the given string.
        /// \retval false
        ///         Otherwise.
        ///
        /// \throws noexcept <tt>(noexcept(is_less_type()(a,b)))</tt>
        ///
    bool is_name_equal_to(
        string_const_ref b ) const
    {
        return are_names_equal(name, b, is_less());
    }

        /// \brief  Determines whether the two given string map
        /// entries are equal.
        ///
        /// Uses the default comparator for the `name` data members
        /// and `operator==` for the default comparator and `value`
        /// data members.
        ///
        /// \param[in]  a
        /// \param[in]  b
        ///             Two string map entries to compare.
        ///
        /// \post
        ///
        ///             result
        ///                 ==  a.is_less() == b.is_less()
        ///                 &&  a.is_name_equal_to(b.name)
        ///                 &&  a.value == b.value
        ///
        /// \retval true
        ///         The given two string map entries are equal.
        /// \retval false
        ///         Otherwise.
        ///
        /// \throws noexcept <tt>(noexcept(is_less_type == is_less_type)
        /// && noexcept(is_less_type()(a,b))) && noexcept(ali::string == ali::string)</tt>
        ///
    friend bool operator==(
        string_map_entry const& a,
        string_map_entry const& b )
    {
        return  static_cast<is_less_type const&>(a)
                    == static_cast<is_less_type const&>(b)
            &&  a.is_name_equal_to(b.name)
            &&  a.value == b.value;
    }

        /// \brief  Determines whether the two given string map
        /// entries differ.
        ///
        /// Uses the default comparator for the `name` data members
        /// and `operator==` for the default comparator and `value`
        /// data members.
        ///
        /// \param[in]  a
        /// \param[in]  b
        ///             Two string map entries to compare.
        ///
        /// \post   <tt>result == !(a == b)</tt>
        ///
        /// \retval true
        ///         The given two string map entries differ.
        /// \retval false
        ///         Otherwise.
        ///
        /// \throws noexcept <tt>(noexcept(a == b))</tt>
        ///
    friend bool operator!=(
        string_map_entry const& a,
        string_map_entry const& b )
    {
        return !(a ==b);
    }

    string_map_entry& set_value( ali::string const& value )
    {
        this->value = value;
        return *this;
    }

    string_map_entry& set_value( short value );
    string_map_entry& set_value( unsigned short value );
    string_map_entry& set_value( int value );
    string_map_entry& set_value( unsigned int value );
    string_map_entry& set_value( long value );
    string_map_entry& set_value( unsigned long value );
    string_map_entry& set_value( long long value );
    string_map_entry& set_value( unsigned long long value );
    string_map_entry& set_value( float value );
    string_map_entry& set_value( double value );

        /// \brief  Updates this string map entry with a name
        /// and value extracted from the given string.
        ///
        /// The accepted format is:
        ///
        ///     str = wsp* name wsp* separator wsp* value wsp*
        ///     name = not_separator_or_wsp [not_separator* not_separator_or_wsp]
        ///     value = not_wsp [any* not_wsp]
        ///     any = {c | true}    //  any character
        ///     wsp = {c | ali::is_space(c)}
        ///     not_wsp = {c | !ali::is_space(c)}
        ///     not_separator = {c | c != separator}
        ///     not_separator_or_wsp = {c | c != separator && !ali::is_space(c)}
        ///
        /// All leading and trailing whitespace (wsp) characters
        /// are stripped from `name` and `value`.
        ///
        /// \tparam separator
        ///     The separator character.
        ///
        /// \param[in]  str
        ///     String to split into name value pair.
        ///
        /// \post
        ///
        ///             (   result
        ///
        ///             &&  !this->name.is_empty()
        ///             &&  this->name == name
        ///             &&  !ali::is_space(this->name.front())
        ///             &&  !ali::is_space(this->name.back())
        ///             &&  forall i in set {0, ..., this->name.size() - 1} &
        ///                     this->name[i] != separator
        ///
        ///             &&  !this->value.is_empty()
        ///             &&  this->value == value
        ///             &&  !ali::is_space(this->value.front())
        ///             &&  !ali::is_space(this->value.back()))
        ///
        ///         ||  (   !result
        ///             &&  *this == *pre(this))
        ///
        /// \retval true
        ///     This string map entry was updated with name and value
        ///     pair extracted from the given string.
        /// \retval false
        ///     The given string could not be split into name and
        ///     value pair. This string map entry was not modified
        ///     in any way.
        ///
        /// \throws noexcept <tt>(false)</tt>
        ///
    template <char separator>
    bool parse( string_const_ref str )
    {
        return private_parse(str, separator, false);
    }
    
        /// \brief  Updates this string map entry with a name
        /// and value extracted from the given string.
        ///
        /// The accepted format is:
        ///
        ///     str = wsp* name wsp* [separator wsp* [value wsp*]]
        ///     name = not_separator_or_wsp [not_separator* not_separator_or_wsp]
        ///     value = not_wsp [any* not_wsp]
        ///     any = {c | true}    //  any character
        ///     wsp = {c | ali::is_space(c)}
        ///     not_wsp = {c | !ali::is_space(c)}
        ///     not_separator = {c | c != separator}
        ///     not_separator_or_wsp = {c | c != separator && !ali::is_space(c)}
        ///
        /// All leading and trailing whitespace (wsp) characters
        /// are stripped from `name` and `value`.
        ///
        /// \tparam separator
        ///     The separator character.
        ///
        /// \param[in]  str
        ///     String to split into name value pair.
        ///
        /// \post
        ///
        ///             (   result
        ///
        ///             &&  !this->name.is_empty()
        ///             &&  this->name == name
        ///             &&  !ali::is_space(this->name.front())
        ///             &&  !ali::is_space(this->name.back())
        ///             &&  forall i in set {0, ..., this->name.size() - 1} &
        ///                     this->name[i] != separator
        ///
        ///             &&  (   (   str doesn't contain value
        ///                     &&  this->value.is_empty())
        ///
        ///                 ||  (   str contains value
        ///                     &&  this->value == value
        ///                     &&  !ali::is_space(this->value.front())
        ///                     &&  !ali::is_space(this->value.back()))))
        ///
        ///         ||  (   !result
        ///             &&  *this == *pre(this))
        ///
        /// \retval true
        ///     This string map entry was updated with name and value
        ///     pair extracted from the given string.
        /// \retval false
        ///     The given string could not be split into name and
        ///     value pair. This string map entry was not modified
        ///     in any way.
        ///
        /// \throws noexcept <tt>(false)</tt>
        ///
    template <char separator>
    bool parse_accepting_empty_value( string_const_ref str )
    {
        return private_parse(str, separator, true);
    }

        /// \brief  Calls this->parse<'='>(str).
        ///
    bool parse( string_const_ref str )
    {
        return parse<'='>(str);
    }

        /// \brief  Calls this->parse_accepting_empty_value<'='>(str).
        ///
    bool parse_accepting_empty_value( string_const_ref str )
    {
        return parse_accepting_empty_value<'='>(str);
    }

        /// \brief  Tries to parse the `value` data member by
        /// invoking the `parse` method of the output object.
        ///
        /// \tparam     T
        ///         Type of the output object.
        /// \param[out] t
        ///         Object of type T to be initialized from a string
        ///         value stored in the `value` data member of this
        ///         string map entry.
        ///
        /// \pre    `T` defines `parse` method accepting
        ///         \ref ali::string and returning `bool`.
        ///
        /// \post   <tt>result == t.parse(this->value) && post(t.parse(this->value))</tt>
        ///
        /// \returns Value returned by the `parse` method of `t`.
        ///
        /// \throws noexcept <tt>(noexcept(t.parse(this->value)))</tt>
        ///
    template <typename T>
    bool parse_value( T& t ) const
    {
        return t.parse(value);
    }

        /// \brief  Tries to parse the `value` data member by
        /// invoking the specified function with the output object
        /// and the `value` data member as arguments.
        ///
        /// \tparam     T
        ///         Type of the output object.
        /// \tparam     U
        ///         Type of the string object the function accepts.
        /// \param[out] t
        ///         Object of type T to be initialized from a string
        ///         value stored in the `value` data member of this
        ///         string map entry.
        /// \param[in]  parse
        ///         Parsing function to be invoked.
        ///
        /// \pre    `ali::string` object is implicitly convertible
        ///         to an object of type `U`
        ///
        /// \post   <tt>result == parse(t, this->value) && post(parse(t, this->value))</tt>
        ///
        /// \returns Value returned by the parse function.
        ///
        /// \throws noexcept <tt>(noexcept(parse(t, this->value)))</tt>
        ///
    template <typename T, typename U>
    bool parse_value( T& t, bool (*parse)( T&, U ) ) const
    {
        return parse(t, value);
    }

        /// \brief  Tries to parse the `value` data member by
        /// invoking the specified method of the output object.
        ///
        /// \tparam     T
        ///         Type of the output object.
        /// \tparam     U
        ///         Type that defines the specified method. (Might
        ///         be a base class of `T`.)
        /// \tparam     V
        ///         Type of the string object the specified method
        ///         accepts.
        /// \param[out] t
        ///         Object of type T to be initialized from a string
        ///         value stored in the `value` data member of this
        ///         string map entry.
        /// \param[in]  parse
        ///         Parsing method to be invoked.
        ///
        /// \pre    `ali::string` object is implicitly convertible
        ///         to an object of type `V`
        ///
        /// \post   <tt>result == (t.*parse)(this->value) && post((t.*parse)(this->value))</tt>
        ///
        /// \returns Value returned by the specified method of `t`.
        ///
        /// \throws noexcept <tt>(noexcept((t.*parse)(this->value)))</tt>
        ///
    template <typename T, typename U, typename V>
    bool parse_value( T& t, bool (U::*parse)( V ) ) const
    {
        return (t.*parse)(value);
    }

        /// \brief  Tries to parse the `value` data member as
        /// a signed decimal integer.
        ///
        /// \param[out]  t
        ///         Variable to store the parsed value into.
        ///
        /// \post
        ///
        ///                 (   result
        ///                 &&  the value data member contains
        ///                     decimal textual representation of
        ///                     a signed integer that fits into the
        ///                     short int data type
        ///                 &&  t is equal to the value of that integer)
        ///             ||  (   !result
        ///                 &&  t is not changed in any way)
        ///
        /// \retval true
        ///         The `value` data member contains decimal textual
        ///         representation of a signed integer that fits into
        ///         the `short int` data type.
        ///         The value of that integer was stored into the `t`
        ///         parameter.
        /// \retval false
        ///         The `value` data member doesn't contain decimal
        ///         textual representation of a signed integer or
        ///         that integer doesn't fit into the `short int`
        ///         data type.
        ///         The `t` parameter was not modified in any way.
        ///
        /// \throws noexcept
        ///
    bool parse_value( short& t ) const
    {
    	return private_parse_value_as_signed_integer(t);
    }

        /// \brief  Tries to parse the `value` data member as
        /// an unsigned decimal integer.
        ///
        /// \param[out]  t
        ///         Variable to store the parsed value into.
        ///
        /// \post
        ///
        ///                 (   result
        ///                 &&  the value data member contains
        ///                     decimal textual representation of
        ///                     an unsigned integer that fits into
        ///                     the unsigned short int data type
        ///                 &&  t is equal to the value of that integer)
        ///             ||  (   !result
        ///                 &&  t is not changed in any way)
        ///
        /// \retval true
        ///         The `value` data member contains decimal textual
        ///         representation of an unsigned integer that fits
        ///         into the `unsigned short int` data type.
        ///         The value of that integer was stored into the `t`
        ///         parameter.
        /// \retval false
        ///         The `value` data member doesn't contain decimal
        ///         textual representation of an unsigned integer or
        ///         that integer doesn't fit into the `unsigned short
        ///         int` data type.
        ///         The `t` parameter was not modified in any way.
        ///
        /// \throws noexcept
        ///
    bool parse_value( unsigned short& t ) const
    {
    	return private_parse_value_as_unsigned_integer(t);
    }

        /// \brief  Tries to parse the `value` data member as
        /// a signed decimal integer.
        ///
        /// \param[out]  t
        ///         Variable to store the parsed value into.
        ///
        /// \post
        ///
        ///                 (   result
        ///                 &&  the value data member contains
        ///                     decimal textual representation of
        ///                     a signed integer that fits into the
        ///                     int data type
        ///                 &&  t is equal to the value of that integer)
        ///             ||  (   !result
        ///                 &&  t is not changed in any way)
        ///
        /// \retval true
        ///         The `value` data member contains decimal textual
        ///         representation of a signed integer that fits into
        ///         the `int` data type.
        ///         The value of that integer was stored into the `t`
        ///         parameter.
        /// \retval false
        ///         The `value` data member doesn't contain decimal
        ///         textual representation of a signed integer or
        ///         that integer doesn't fit into the `int` data
        ///         type.
        ///         The `t` parameter was not modified in any way.
        ///
        /// \throws noexcept
        ///
    bool parse_value( int& t ) const
    {
    	return private_parse_value_as_signed_integer(t);
    }

        /// \brief  Tries to parse the `value` data member as
        /// an unsigned decimal integer.
        ///
        /// \param[out]  t
        ///         Variable to store the parsed value into.
        ///
        /// \post
        ///
        ///                 (   result
        ///                 &&  the value data member contains
        ///                     decimal textual representation of
        ///                     an unsigned integer that fits into
        ///                     the unsigned int data type
        ///                 &&  t is equal to the value of that integer)
        ///             ||  (   !result
        ///                 &&  t is not changed in any way)
        ///
        /// \retval true
        ///         The `value` data member contains decimal textual
        ///         representation of an unsigned integer that fits
        ///         into the `unsigned int` data type.
        ///         The value of that integer was stored into the `t`
        ///         parameter.
        /// \retval false
        ///         The `value` data member doesn't contain decimal
        ///         textual representation of an unsigned integer or
        ///         that integer doesn't fit into the `unsigned int`
        ///         data type.
        ///         The `t` parameter was not modified in any way.
        ///
        /// \throws noexcept
        ///
    bool parse_value( unsigned int& t ) const
    {
    	return private_parse_value_as_unsigned_integer(t);
    }

        /// \brief  Tries to parse the `value` data member as
        /// a signed decimal integer.
        ///
        /// \param[out]  t
        ///         Variable to store the parsed value into.
        ///
        /// \post
        ///
        ///                 (   result
        ///                 &&  the value data member contains
        ///                     decimal textual representation of
        ///                     a signed integer that fits into the
        ///                     long int data type
        ///                 &&  t is equal to the value of that integer)
        ///             ||  (   !result
        ///                 &&  t is not changed in any way)
        ///
        /// \retval true
        ///         The `value` data member contains decimal textual
        ///         representation of a signed integer that fits into
        ///         the `long int` data type.
        ///         The value of that integer was stored into the `t`
        ///         parameter.
        /// \retval false
        ///         The `value` data member doesn't contain decimal
        ///         textual representation of a signed integer or
        ///         that integer doesn't fit into the `long int`
        ///         data type.
        ///         The `t` parameter was not modified in any way.
        ///
        /// \throws noexcept
        ///
    bool parse_value( long& t ) const;

        /// \brief  Tries to parse the `value` data member as
        /// an unsigned decimal integer.
        ///
        /// \param[out]  t
        ///         Variable to store the parsed value into.
        ///
        /// \post
        ///
        ///                 (   result
        ///                 &&  the value data member contains
        ///                     decimal textual representation of
        ///                     an unsigned integer that fits into
        ///                     the unsigned long int data type
        ///                 &&  t is equal to the value of that integer)
        ///             ||  (   !result
        ///                 &&  t is not changed in any way)
        ///
        /// \retval true
        ///         The `value` data member contains decimal textual
        ///         representation of an unsigned integer that fits
        ///         into the `unsigned long int` data type.
        ///         The value of that integer was stored into the `t`
        ///         parameter.
        /// \retval false
        ///         The `value` data member doesn't contain decimal
        ///         textual representation of an unsigned integer or
        ///         that integer doesn't fit into the `unsigned long
        ///         int` data type.
        ///         The `t` parameter was not modified in any way.
        ///
        /// \throws noexcept
        ///
    bool parse_value( unsigned long& t ) const;

        /// \brief  Tries to parse the `value` data member as
        /// an unsigned decimal integer.
        ///
        /// \param[out]  t
        ///         Variable to store the parsed value into.
        ///
        /// \post
        ///
        ///                 (   result
        ///                 &&  the value data member contains
        ///                     decimal textual representation of
        ///                     an unsigned integer that fits into
        ///                     the unsigned long int data type
        ///                 &&  t is equal to the value of that integer)
        ///             ||  (   !result
        ///                 &&  t is not changed in any way)
        ///
        /// \retval true
        ///         The `value` data member contains decimal textual
        ///         representation of an unsigned integer that fits
        ///         into the `unsigned long int` data type.
        ///         The value of that integer was stored into the `t`
        ///         parameter.
        /// \retval false
        ///         The `value` data member doesn't contain decimal
        ///         textual representation of an unsigned integer or
        ///         that integer doesn't fit into the `unsigned long
        ///         int` data type.
        ///         The `t` parameter was not modified in any way.
        ///
        /// \throws noexcept
        ///
    bool parse_value( unsigned long long& t ) const;

        /// \brief  Tries to parse the `value` data member as
        /// a decimal real number in the fixed point notation.
        ///
        /// \param[out]  t
        ///         Variable to store the parsed value into.
        ///
        /// \post
        ///
        ///                 (   result
        ///                 &&  the value data member contains
        ///                     decimal fixed point textual
        ///                     representation of a real number that
        ///                     fits into the float data type
        ///                 &&  t is equal to "a reasonable
        ///                     approximation" of that real number)
        ///             ||  (   !result
        ///                 &&  t is not changed in any way)
        ///
        /// \retval true
        ///         The `value` data member contains decimal fixed
        ///         point textual representation of a real number
        ///         that fits into the `float` data type.
        ///         "A reasonable approximation" of that real number
        ///         was stored into the `t` parameter.
        /// \retval false
        ///         The `value` data member doesn't contain decimal
        ///         fixed point textual representation of a real
        ///         number or that number doesn't fit into the
        ///         `float` data type.
        ///         The `t` parameter was not modified in any way.
        ///
        /// \throws noexcept
        ///
    bool parse_value( float& t ) const;

        /// \brief  Tries to parse the `value` data member as
        /// a decimal real number in the fixed point notation.
        ///
        /// \param[out]  t
        ///         Variable to store the parsed value into.
        ///
        /// \post
        ///
        ///                 (   result
        ///                 &&  the value data member contains
        ///                     decimal fixed point textual
        ///                     representation of a real number that
        ///                     fits into the double data type
        ///                 &&  t is equal to "a reasonable
        ///                     approximation" of that real number)
        ///             ||  (   !result
        ///                 &&  t is not changed in any way)
        ///
        /// \retval true
        ///         The `value` data member contains decimal fixed
        ///         point textual representation of a real number
        ///         that fits into the `double` data type.
        ///         "A reasonable approximation" of that real number
        ///         was stored into the `t` parameter.
        /// \retval false
        ///         The `value` data member doesn't contain decimal
        ///         fixed point textual representation of a real
        ///         number or that number doesn't fit into the
        ///         `double` data type.
        ///         The `t` parameter was not modified in any way.
        ///
        /// \throws noexcept
        ///
    bool parse_value( double& t ) const;

        /// \brief  Tries to parse the `value` data member by
        /// invoking the `parse` method of the output object,
        /// returning either the `value` initialized object or the
        /// specified default one if the parsing fails.
        ///
        /// \tparam     T
        ///         Type of the output object.
        /// \param[in]  default_value
        ///         Object of type T to be returned by this method
        ///         in case the parsing fails.
        ///
        /// \pre
        ///
        ///                 T has default constructor
        ///             &&  T has copy constructor
        ///             &&  T has assignment operator
        ///             &&  let t is object type T
        ///                 in  pre(this->parse_value(t))
        ///
        /// \post
        ///
        ///             let t is object type T
        ///             in  post(this->parse_value(t))
        ///                 &&  (   (this->parse_value(t) && result == t)
        ///                     ||  (!this->parse_value(t) && result == default_value))
        ///
        /// \returns Either the `value` initialized object or the
        /// specified default one if the parsing fails.
        ///
        /// \throws noexcept <tt>(noexcept(T()) && noexcept(T(T))
        /// && noexcept(this->parse_value(T)) && noexcept(T = T))</tt>
        ///
    template <typename T>
    T get_value( T const& default_value = T() ) const
    {
        T temp;
        if ( !parse_value(temp) )
            temp = default_value;
        return temp;
    }

        /// \brief  Tries to parse the `value` data member by
        /// invoking the specified parsing function or method,
        /// returning either the `value` initialized object or the
        /// specified default one if the parsing fails.
        ///
        /// \tparam     T
        ///         Type of the output object.
        /// \tparam     parse_function
        ///         Type of the parsing function or method.
        /// \param[in]  parse
        ///         Parsing function or method to be invoked.
        /// \param[in]  default_value
        ///         Object of type T to be returned by this method
        ///         in case the parsing fails.
        ///
        /// \pre
        ///
        ///                 T has default constructor
        ///             &&  T has copy constructor
        ///             &&  T has assignment operator
        ///             &&  let t is object of type T
        ///                 in  pre(this->parse_value(t, parse))
        ///
        /// \post
        ///
        ///             let t is object of type T
        ///             in  post(this->parse_value(t, parse))
        ///                 &&  (   (this->parse_value(t, parse) && result == t)
        ///                     ||  (!this->parse_value(t, parse) && result == default_value))
        ///
        /// \returns Either the `value` initialized object or the
        /// specified default one if the parsing fails.
        ///
        /// \throws noexcept <tt>(noexcept(T()) && noexcept(T(T))
        /// && noexcept(this->parse_value(T, parse)) && noexcept(T = T))</tt>
        ///
    template <typename T, typename parse_function>
    T get_value(
        parse_function parse,
        T const& default_value = T() ) const
    {
        T temp;
        if ( !parse_value(temp, parse) )
            temp = default_value;
        return temp;
    }

        /// \brief  Applies the given in-place transform to the
        /// `value` data member.
        ///
        /// \param[in]  t
        ///         In-place string tranform to apply.
        ///
        /// \pre    `t` is in-place transform accepting
        ///         \ref ali::string objects.
        ///
        /// \post   <tt>post(t(this->value))</tt>
        ///
        /// \returns `*this`;
        ///
        /// \throws noexcept <tt>(noexcept(t(ali::string)))</tt>
        ///
    template <typename string_in_place_transform>
    string_map_entry& transform_value_in_place(
        string_in_place_transform t )
    {
        t(value);
        return *this;
    }

        /// \brief  Swaps contents of this string map entry with
        /// another one.
        ///
        /// \param[in,out]  b
        ///         Another string map entry to swap content with.
        ///
        /// \post
        ///
        ///                 this->name == pre(b).name
        ///             &&  this->value == pre(b).value
        ///             &&  this->is_less() == pre(b).is_less()
        ///             &&  pre(this)->name == b.name
        ///             &&  pre(this)->value == b.value
        ///             &&  pre(this)->is_less() == b.is_less()
        ///
        /// \throws noexcept
        ///
    void swap( string_map_entry& b )
    {
        using ali::swap;
        swap(
            static_cast<is_less_type&>(*this),
            static_cast<is_less_type&>(b));
        swap(name, b.name);
        swap(value, b.value);
    }

        /// \brief  See \ref string_map_entry::swap.
        ///
    friend void swap(
        string_map_entry& a,
        string_map_entry& b )
    {
        a.swap(b);
    }

        /// \brief  Returns the default comparator used for
        /// comparing the `name` data members.
        ///
        /// \returns The default comparator used for comparing the
        /// `name` data members.
        ///
        /// \throws noexcept <tt>(noexcept(is_less_type(is_less_type)))</tt>
        ///
    is_less_type is_less( void ) const
    {
        return *this;
    }

    ali::string name;
        ///<        Holds the name of this string map entry.

    ali::string value;
        ///<        Holds the value of this string map entry.

private:    //  Methods
    bool private_parse(
        string_const_ref str,
        char separator,
        bool accept_empty_value )
    {
    	int const separator_pos = str.index_of_first(separator);

		if ( !accept_empty_value && separator_pos == str.size() )
			return false;

		string_const_ref name_ref
			= str.ref_left(separator_pos).trim();

		string_const_ref value_ref
			= str.ref_right(
				ali::mini(
					str.size(),
					separator_pos + 1)).trim();

		if ( name_ref.is_empty() )
			return false;

		if ( !accept_empty_value && value_ref.is_empty() )
			return false;

		name = name_ref;

		value = value_ref;

		return true;
    }

    template <typename target_type>
    bool private_parse_value_as_signed_integer( target_type& t ) const
    {
        long const min_value = meta::integer::min_value<target_type>::result;
        long const max_value = meta::integer::max_value<target_type>::result;

        long temp =  0;

        bool const is_valid
            =   parse_value(temp)
            &&  min_value <= temp
            && 	temp <= max_value;

        if ( is_valid )
            t = static_cast<target_type>(temp);

        return is_valid;
    }

    template <typename target_type>
    bool private_parse_value_as_unsigned_integer( target_type& t ) const
    {
        unsigned long const max_value
            = meta::integer::max_value<target_type>::result;

        unsigned long temp =  0;

        bool const is_valid
            =   parse_value(temp)
            &&  temp <= max_value;

        if ( is_valid )
            t = static_cast<target_type>(temp);

        return is_valid;
    }
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <typename is_less_type>
class small_string_map : private is_less_type
// ******************************************************************
{
public:     //  Typedefs
    typedef string_map_entry<is_less_type> entry;
    typedef typename array<entry>::const_iterator const_iterator;

public:     //  Methods
    bool operator==( small_string_map const& b ) const
    {
    	if ( _entries.size() != b._entries.size() )
			return false;
		else for ( int i = _entries.size(); i != 0; )
			if ( --i, _entries[i] != b._entries[i] )
				return false;

		return true;
    }

    bool operator!=( small_string_map const& b ) const
    {
        return !operator==(b);
    }

    bool is_empty( void ) const
    {
        return _entries.is_empty();
    }

    int size( void ) const
    {
        return _entries.size();
    }

    entry& operator[]( string_const_ref name )
    {
    	int const idx = index_of(name);

		if ( idx == _entries.size() )
			_entries.push_back(entry(name));

		return _entries[idx];
    }
    
    entry const& operator[]( string_const_ref name ) const
    {
        if ( entry const* const e = find(name) )
            return *e;
        
        return _empty_entry;
    }
    
    entry& operator[]( int pos )
    {
        return _entries[pos];
    }
    
    entry const& operator[]( int pos ) const
    {
        return _entries[pos];
    }

    entry& at( int pos )
    {
        return _entries.at(pos);
    }
    
    entry const& at( int pos ) const
    {
        return _entries.at(pos);
    }

    small_string_map& set( entry const& b )
    {
    	if ( entry* const e = find(b.name) )
			e->value = b.value;
		else
			_entries.push_back(b);

    	return *this;
    }

    small_string_map& set(
        string_const_ref name,
        string const& value )
    {
        this->operator[](name).set_value(value);
        return *this;
    }

    small_string_map& set(
        string_const_ref name,
        short value )
    {
        this->operator[](name).set_value(value);
        return *this;
    }

    small_string_map& set(
        string_const_ref name,
        unsigned short value )
    {
        this->operator[](name).set_value(value);
        return *this;
    }

    small_string_map& set(
        string_const_ref name,
        int value )
    {
        this->operator[](name).set_value(value);
        return *this;
    }

    small_string_map& set(
        string_const_ref name,
        unsigned int value )
    {
        this->operator[](name).set_value(value);
        return *this;
    }

    small_string_map& set(
        string_const_ref name,
        long value )
    {
        this->operator[](name).set_value(value);
        return *this;
    }

    small_string_map& set(
        string_const_ref name,
        unsigned long value )
    {
        this->operator[](name).set_value(value);
        return *this;
    }

    small_string_map& set(
        string_const_ref name,
        long long value )
    {
        this->operator[](name).set_value(value);
        return *this;
    }

    small_string_map& set(
        string_const_ref name,
        unsigned long long value )
    {
        this->operator[](name).set_value(value);
        return *this;
    }

    small_string_map& set(
        string_const_ref name,
        float value )
    {
        this->operator[](name).set_value(value);
        return *this;
    }

    small_string_map& set(
        string_const_ref name,
        double value )
    {
        this->operator[](name).set_value(value);
        return *this;
    }

    small_string_map& erase( string_const_ref name )
    {
    	int const idx = index_of(name);

		if ( idx != _entries.size() )
			_entries.erase(idx);

		return *this;
    }

    small_string_map& erase( int pos, int n = 1 )
    {
        _entries.erase(pos, n);
        
        return *this;
    }

    small_string_map& erase( void )
    {
        _entries.erase();

        return *this;
    }

    template <typename predicate>
    int erase_if( predicate p )
    {
        return _entries.erase_if(p);
    }

    struct this_method_returns_size_if_the_name_was_not_found
    {
        this_method_returns_size_if_the_name_was_not_found( int value )
        :   value(value)
        {}

        int value;
    };

    template <typename other_is_less_type>
    int index_of(
		string_const_ref name,
		other_is_less_type is_less ) const
    {
        int idx = 0;

        for ( ; idx != _entries.size(); ++idx )
            if ( _entries[idx].is_name_equal_to(name, is_less) )
                break;

        return idx;
    }

    int index_of( string_const_ref name ) const
    {
        return index_of(name, is_less());
    }

    template <typename other_is_less_type>
    bool contains(
            string_const_ref name,
            other_is_less_type is_less ) const
    {
        return index_of(name, is_less) != size();
    }

    bool contains( string_const_ref name ) const
    {
        return contains(name, is_less());
    }

    template <typename other_is_less_type>
    entry const* find(
        string_const_ref name,
        other_is_less_type is_less ) const
    {
        int const idx = index_of(name, is_less);
        
        if ( idx != _entries.size() )
            return &_entries[idx];

        return nullptr;
    }

    template <typename other_is_less_type>
    entry* find(
        string_const_ref name,
        other_is_less_type is_less )
    {
        int const idx = index_of(name, is_less);
        
        if ( idx != _entries.size() )
            return &_entries[idx];

        return nullptr;
    }

    entry const* find( string_const_ref name ) const
    {
        return find(name, is_less());
    }

    entry* find( string_const_ref name )
    {
        return find(name, is_less());
    }

    ali::string const* find_value( string_const_ref name ) const
    {
        if ( entry const* const e = find(name) )
            return &e->value;
        else
            return nullptr;
    }

    ali::string* find_value( string_const_ref name )
    {
        if ( entry* const e = find(name) )
            return &e->value;
        else
            return nullptr;
    }

    template <
        char name_value_separator,
        char entry_separator>
    bool parse( string_const_ref str )
    {
        small_string_map temp;

        string_const_ptr ptr = str.pointer();

        while ( !ptr->is_empty() )
        {
            int const end = ptr->index_of_first_unquoted(entry_separator);

            temp._entries.push_back(entry());

            bool const success
                = temp._entries.back().template parse<
                    name_value_separator>(
                        *ptr->pointer_left(end));

            if ( !success )
                return false;

            ptr += ali::mini(ptr->size(), end + 1);

        }

        swap(temp);

        return true;
    }

    template <
        char name_value_separator,
        char entry_separator>
    bool parse_accepting_empty_values( string_const_ref str )
    {
        small_string_map temp;

        string_const_ptr ptr = str.pointer();

        while ( !ptr->is_empty() )
        {
            int const end = ptr->index_of_first_unquoted(entry_separator);

            temp._entries.push_back(entry());

            bool const success
                = temp._entries.back()
                    .template parse_accepting_empty_value<
                        name_value_separator>(
                            *ptr->pointer_left(end));

            if ( !success )
                return false;

            ptr += ali::mini(ptr->size(), end + 1);

        }

        swap(temp);

        return true;
    }

    bool parse( string_const_ref str )
    {
        return parse<'=', ';'>(str);
    }

    bool parse_accepting_empty_values( string_const_ref str )
    {
        return parse_accepting_empty_values<'=', ';'>(str);
    }

    template <char name_value_separator>
    bool parse_lines( string_const_ref str )
    {
        small_string_map temp;

        string_const_ref const crlf("\r\n");
            //  This is used as a set.

        string_const_ptr ptr = str.pointer();

        while ( ptr = ptr->pointer_right(
                    ptr->index_of_first_not_of(crlf)),
                        !ptr->is_empty() )
        {
            string_const_ptr line
                = ptr->pointer_left(
                    ptr->index_of_first_of(crlf));

            temp._entries.push_back(entry());

            bool const success
                = temp._entries.back().template parse<
                    name_value_separator>(*line);

            if ( !success )
                return false;

            ptr += line->size();
        }

        swap(temp);

        return true;
    }

    template <char name_value_separator>
    bool parse_lines_accepting_empty_values( string_const_ref str )
    {
        small_string_map temp;

        string_const_ref const crlf("\r\n");
            //  This is used as a set.

        string_const_ptr ptr = str.pointer();

        while ( ptr = ptr->pointer_right(
                    ptr->index_of_first_not_of(crlf)),
                        !ptr->is_empty() )
        {
            string_const_ptr line
                = ptr->pointer_left(
                    ptr->index_of_first_of(crlf));

            temp._entries.push_back(entry());

            bool const success
                = temp._entries.back()
                    .template parse_accepting_empty_value<
                        name_value_separator>(*line);

            if ( !success )
                return false;

            ptr += line->size();
        }

        swap(temp);

        return true;
    }

    bool parse_lines( string_const_ref str )
    {
        return parse_lines<'='>(str);
    }

    bool parse_lines_accepting_empty_values( string_const_ref str )
    {
        return parse_lines_accepting_empty_values<'='>(str);
    }

    template <typename T>
    bool parse_value( int i, T& t ) const
    {
        return  0 <= i && i < _entries.size()
            &&  _entries[i].parse_value(t);
    }

    template <typename T, typename parse_function>
    bool parse_value( int i, T& t, parse_function parse ) const
    {
        return  0 <= i && i < _entries.size()
            &&  _entries[i].parse_value(t, parse);
    }

    template <typename T>
    bool parse_value(
        string_const_ref name,
        T& t ) const
    {
        return parse_value(index_of(name), t);
    }

    template <typename T, typename parse_function>
    bool parse_value(
        string_const_ref name,
        T& t, parse_function parse ) const
    {
        return parse_value(index_of(name), t, parse);
    }

    template <typename T>
    bool parse_value_accepting_missing( int i, T& t ) const
    {
        return  i < 0 || _entries.size() <= i
            ||  _entries[i].parse_value(t);
    }

    template <typename T, typename parse_function>
    bool parse_value_accepting_missing(
        int i, T& t, parse_function parse ) const
    {
        return  i < 0 || _entries.size() <= i
            ||  _entries[i].parse_value(t, parse);
    }

    template <typename T>
    bool parse_value_accepting_missing(
        string_const_ref name,
        T& t ) const
    {
        return parse_value_accepting_missing(
            index_of(name), t);
    }

    template <typename T, typename parse_function>
    bool parse_value_accepting_missing(
        string_const_ref name,
        T& t, parse_function parse ) const
    {
        return parse_value_accepting_missing(
            index_of(name), t, parse);
    }

    ali::string get_value(
        int i,
        ali::string const& default_value = ali::string() ) const
    {
        return 0 <= i && i < _entries.size()
            ? _entries[i].value
            : default_value;
    }

    template <typename T>
    T get_value( int i, T const& default_value ) const
    {
        return 0 <= i && i < _entries.size()
            ? _entries[i].get_value(default_value)
            : default_value;
    }

    template <typename T, typename parse_function>
    T get_value(
        int i,
        parse_function parse,
        T const& default_value ) const
    {
        return 0 <= i && i < _entries.size()
            ? _entries[i].get_value(parse, default_value)
            : default_value;
    }

    ali::string get_value(
        string_const_ref name,
        ali::string const& default_value = ali::string() ) const
    {
        return get_value(index_of(name), default_value);
    }

    template <typename T>
    T get_value(
        string_const_ref name,
        T const& default_value ) const
    {
        return get_value(index_of(name), default_value);
    }

    template <typename T, typename parse_function>
    T get_value(
        string_const_ref name,
        parse_function parse,
        T const& default_value ) const
    {
        return get_value(index_of(name), parse, default_value);
    }

    template <typename string_in_place_transform>
    small_string_map& transform_values_in_place(
        string_in_place_transform t )
    {
        for ( int i = _entries.size(); i != 0; )
            _entries[--i].transform_value_in_place(t);
        return *this;
    }

    bool are_names_equal(
        string_const_ref a,
        string_const_ref b ) const
    {
        return _empty_entry.are_names_equal(a, b);
    }

    void swap( small_string_map& b )
    {
        using ali::swap;
        swap(
            static_cast<is_less_type&>(*this),
            static_cast<is_less_type&>(b));
        swap(_entries, b._entries);
        swap(_empty_entry, b._empty_entry);
    }

    friend void swap(
        small_string_map& a,
        small_string_map& b )
    {
        a.swap(b);
    }

    is_less_type is_less( void ) const
    {
        return *this;
    }

    const_iterator begin( void ) const
    {
        return _entries.begin();
    }

    friend const_iterator begin( small_string_map const& a )
    {
        return a.begin();
    }

    const_iterator end( void ) const
    {
        return _entries.end();
    }

    friend const_iterator end( small_string_map const& a )
    {
        return a.end();
    }

private:    //  Data members
    ali::array<entry>   _entries;
    entry               _empty_entry;
};

}   //  namespace ali
