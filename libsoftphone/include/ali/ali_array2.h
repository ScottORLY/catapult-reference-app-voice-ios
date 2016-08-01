#pragma once
#include "ali_array2_forward.h"
#include "ali_addressof.h"
#include "ali_array_utils.h"
#include "ali_exception_if.h"
#include "ali_initializer_list.h"
#include "ali_integer.h"
#include "ali_memory.h"
#include "ali_range.h"

namespace ali
{

namespace hidden
{

// ******************************************************************
template <typename T, int _capacity>
class array_common;
// ******************************************************************

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <typename T, int _capacity>
class array_storage
// ******************************************************************
//  Storage for array with fixed capacity.
// ******************************************************************
{
public:     //  Methods
        /// \brief Returns the maximum number of
        /// elements this array can potentially hold.
        ///
        /// \returns The maximum number of elements
        /// this array can potentially hold.
        ///
        /// \throws noexcept
        ///
    static int max_size( void )
    {
        return _capacity;
    }

        /// \brief  Returns the number of elements this
        /// array can accommodate without triggering
        /// memory allocation.
        ///
        /// \note   The _no memory allocation_ guarantee is
        /// valid only for the memory managed by the array
        /// object itself. Memory might still be allocated
        /// internally by the contained elements, though.
        ///
        /// The capacity of an array object can be
        /// increased using the \ref reserve method.
        ///
        /// \returns The number of elements this array
        /// can accommodate without triggering memory
        /// allocation.
        ///
        /// \throws noexcept
        ///
        /// \see    \ref reserve
        ///
    int capacity( void ) const
    {
        return _capacity;
    }

private:    //  Methods
    array_storage( void ) {}
    array_storage( array_storage const& ) = delete;
    array_storage( array_storage&& b ) = delete;
    
    ~array_storage( void )
    {
        ali_assert(this->_size == 0);
    }

    array_storage& operator=( array_storage const& ) = delete;
    array_storage& operator=( array_storage&& b ) = delete;

private:    //  Data members
    int                                 _size{};
    class in_place_storage
    {
    public:
        operator T*( void )
        {
            return reinterpret_cast<T*>(_storage);
        }

        operator T const*( void ) const
        {
            return reinterpret_cast<T const*>(_storage);
        }

    private:    //  Data members
        alignas(alignof(T)) ali::uint8  _storage[_capacity * sizeof(T)];

    }                                   _begin{};
    
    template <typename, int>
    friend class array_common;
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <typename T>
class array_storage<T, 0>
// ******************************************************************
//  Storage for array with dynamic capacity.
// ******************************************************************
{
public:     //  Methods
        /// \brief Returns the maximum number of
        /// elements this array can potentially hold.
        ///
        /// \returns The maximum number of elements
        /// this array can potentially hold.
        ///
        /// \throws noexcept
        ///
    static int max_size( void )
    {
        return meta::integer::max_value<
            int>::result / sizeof(T);
    }

        /// \brief  Returns the number of elements this
        /// array can accommodate without triggering
        /// memory allocation.
        ///
        /// \note   The _no memory allocation_ guarantee is
        /// valid only for the memory managed by the array
        /// object itself. Memory might still be allocated
        /// internally by the contained elements, though.
        ///
        /// The capacity of an array object can be
        /// increased using the \ref reserve method.
        ///
        /// \returns The number of elements this array
        /// can accommodate without triggering memory
        /// allocation.
        ///
        /// \throws noexcept
        ///
        /// \see    \ref reserve
        ///
    int capacity( void ) const
    {
        return this->_capacity;
    }

private:    //  Methods
    array_storage( void ) {}

    explicit array_storage( int capacity )
    :   _begin{static_cast<T*>(allocate(
            capacity * sizeof(T)))},
        _capacity{capacity}
    {}

    array_storage( array_storage const& ) = delete;
    array_storage( array_storage&& b ) = delete;
    
    ~array_storage( void )
    {
        ali_assert(this->_size == 0);

        deallocate(this->_begin);
    }

    array_storage& operator=( array_storage const& ) = delete;
    array_storage& operator=( array_storage&& b ) = delete;

private:    //  Data members
    T*  _begin{};
    int _size{};
    int _capacity{};

    template <typename, int>
    friend class array_common;
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
/// \brief Manages a contiguous sequence of elements
/// of type `T`.
///
/// \tparam T Type of contained elements.
///
/// \pre    `T` supports copying using copy constructor
/// and assignment operator and the copy is equivalent
/// to the original source object.
///
/// \invariant
///
///             0 <= this->size()
///
///         &&  this->size() <= this->capacity()
///
///         &&  this->capacity() <= this->max_size()
///
///         &&  this->is_empty() == (this->size() == 0)
///
// ******************************************************************
template <typename T, int capacity_>
class array_common : public array_storage<T, capacity_>
// ******************************************************************
{
    typedef array_storage<T, capacity_>    this_array_storage;
    typedef array<T, capacity_>            this_array;

    typedef meta::define_bool_result<true>  pod_version;
    typedef meta::define_bool_result<false> non_pod_version;
    typedef meta::is_pod<T>                 is_pod;

    typedef meta::define_bool_result<true>  fixed_capacity_version;
    typedef meta::define_bool_result<false> dynamic_capacity_version;
    typedef meta::define_bool_result<
        capacity_ != 0>                     has_fixed_capacity;

public:     //  Typedefs
    typedef typename array_const_ref<
        T>::read_only_T                     read_only_T;
    typedef meta::is_same_type<
        read_only_T, T const&>              read_only_T_is_const_ref;

    typedef T                               value_type;
        ///<        The type of contained elements.

    typedef T*                              iterator;
        ///<\brief  Provides pointer-like read-write
        /// access to individual elements contained
        /// in an `array` object.

    typedef T const*                        const_iterator;
        ///<\brief  Provides pointer-like read-only
        /// access to individual elements contained
        /// in an `array` object.

    typedef ali::range<T*>                  range_type;
    typedef ali::range<T const*>            const_range_type;

public:     //  Methods
        /// \brief  Determines whether this array contains any
        /// elements.
        ///
        /// \retval true
        ///         This array contains no elements.
        /// \retval false
        ///         Otherwise.
        ///
        /// \throws noexcept
        ///
    bool is_empty( void ) const
    {
        return this->_size == 0;
    }

        /// \brief  Returns the number of elements
        /// currently stored in this array.
        ///
        /// \returns The number of elements currently
        /// stored in this array.
        ///
        /// \throws noexcept
        ///
    int size( void ) const
    {
        return this->_size;
    }

    int back_capacity( void ) const
    {
        return this->capacity() - this->_size;
    }

    //  reserve

        /// \brief  Increases capacity of this array.
        ///
        /// Ensures that this array will be able to
        /// accommodate at least `cap` elements without
        /// triggering memory allocation.
        ///
        /// \note   The _no memory allocation_ guarantee
        /// is valid only for the memory managed by the array
        /// object itself. Memory might still be allocated
        /// internally by the contained elements, though.
        ///
        /// \note   This method affects only the \ref capacity
        /// of this array, not its \ref size.
        ///
        /// \note   If `cap <= this->capacity()` then this
        /// method has no effect.
        ///
        /// \param  cap
        ///         The desired number of elements this
        ///         array will reserve space for.
        ///
        /// \pre    <tt>0 <= cap</tt>
        ///
        /// \post   <tt>cap <= this->capacity()
        /// && this->capacity() <= this->max_size()</tt>
        ///
        /// \returns `*this`
        ///
        /// \throws noexcept <tt>(false)</tt>
        ///
    this_array& reserve( int min_capacity )
    {
        return this->reserve(min_capacity, has_fixed_capacity{});
    }

    this_array& reserve_back( int min_back_capacity )
    {
        return this->reserve_back(min_back_capacity, has_fixed_capacity{});
    }

    T const* data( void ) const
    {
        return this->_begin;
    }

    T* data( void )
    {
        return this->_begin;
    }

        /// \brief  Provides read-only access
        /// to the first element of this array.
        ///
        /// \pre    <tt>!this->is_empty()</tt>
        ///
        /// \returns Const reference to the
        /// first element of this array.
        ///
        /// \throws noexcept <tt>(false)</tt>
        ///
    T const& front( void ) const
    {
        ali_assert(!this->is_empty());

        return this->_begin[0];
    }

        /// \brief  Provides read-write access
        /// to the first element of this array.
        ///
        /// \pre    <tt>!this->is_empty()</tt>
        ///
        /// \returns Reference to the first
        /// element of this array.
        ///
        /// \throws noexcept <tt>(false)</tt>
        ///
    T& front( void )
    {
        ali_assert(!this->is_empty());

        return this->_begin[0];
    }

        /// \brief  Provides read-only access
        /// to the last element of this array.
        ///
        /// \pre    <tt>!this->is_empty()</tt>
        ///
        /// \returns Const reference to the
        /// last element of this array.
        ///
        /// \throws noexcept <tt>(false)</tt>
        ///
    T const& back( void ) const
    {
        ali_assert(!this->is_empty());

        return this->_begin[this->_size - 1];
    }

        /// \brief  Provides read-write access
        /// to the last element of this array.
        ///
        /// \pre    <tt>!this->is_empty()</tt>
        ///
        /// \returns Reference to the last
        /// element of this array.
        ///
        /// \throws noexcept <tt>(false)</tt>
        ///
    T& back( void )
    {
        ali_assert(!this->is_empty());

        return this->_begin[this->_size - 1];
    }

        /// \brief  Provides read-only access to
        /// individual elements of this array.
        ///
        /// \param  i
        ///         Index of the desired element.
        ///
        /// \pre    <tt>0 <= i && i < this->size()</tt>
        ///
        /// \returns Constant reference to
        /// the element at the given index.
        ///
        /// \throws noexcept
        ///
        /// \see    \ref at
        ///
    T const& operator[]( int pos1 ) const
    {
        ali_assert(0 <= pos1);
        ali_assert(pos1 < this->_size);

        return this->_begin[pos1];
    }

        /// \brief  Provides read-write access to
        /// individual elements of this array.
        ///
        /// \param  i
        ///         Index of the desired element.
        ///
        /// \pre    <tt>0 <= i && i < this->size()</tt>
        ///
        /// \returns Reference to the element at
        /// the given index.
        ///
        /// \throws noexcept
        ///
        /// \see    \ref at
        ///
    T& operator[]( int pos1 )
    {
        ali_assert(0 <= pos1);
        ali_assert(pos1 < this->_size);

        return this->_begin[pos1];
    }

        /// \brief  Provides read-only access to
        /// individual elements of this array.
        ///
        /// \param  i
        ///         Index of the desired element.
        ///
        /// \pre    <tt>0 <= i && i < this->size()</tt>
        ///
        /// \returns Constant reference to
        /// the element at the given index.
        ///
        /// \throws noexcept <tt>(false)</tt>
        ///
        /// \see    \ref operator[]
        ///
    T const& at( int pos1 ) const
    {
        ali_assert(0 <= pos1);
        ali_assert(pos1 < this->_size);

        return this->_begin[pos1];
    }

        /// \brief  Provides read-write access to
        /// individual elements of this array.
        ///
        /// \param  i
        ///         Index of the desired element.
        ///
        /// \pre    <tt>0 <= i && i < this->size()</tt>
        ///
        /// \returns Reference to the element at
        /// the given index.
        ///
        /// \throws noexcept <tt>(false)</tt>
        ///
        /// \see    \ref operator[]
        ///
    T& at( int pos1 )
    {
        ali_assert(0 <= pos1);
        ali_assert(pos1 < this->_size);

        return this->_begin[pos1];
    }

    //  push_back

        /// \brief  Appends copies of all elements in
        /// another array to the end of this array.
        ///
        /// \param  b
        ///         Array of objects to initialize the
        ///         new elements with.
        ///
        /// \post
        ///
        ///             this->size() == pre(this)->size() + b.size()
        ///
        ///         &&  this->size() <= this->max_size()
        ///
        ///         &&  forall i in set {0, ..., pre(this)->size() - 1} &
        ///                     this->at(i) is not changed in any way
        ///                 ||  this->at(i) is moved from pre(this)->at(i)
        ///
        ///         &&  forall i in set {pre(this)->size(), ..., this->size() - 1} &
        ///                 this->at(i) is copy of b.at(i - pre(this)->size())
        ///
        /// \returns `*this`
        ///
        /// \throws noexcept <tt>(false)</tt>
        ///
    this_array& push_back( array_const_ref<T> b )
    {
        if ( this->back_capacity() >= b.size() )
        {
            this->uninitialized_copy_back_n(b);
        }
        else
        {
            this->reallocating_copy_back_n(b);
        }
        
        return this->self();
    }

    this_array& append( array_const_ref<T> b )
    {
        return this->push_back(b);
    }

        /// \brief  Appends copies of all elements in
        /// an initializer list to the end of this array.
        ///
        /// \param  b
        ///         Initializer list of objects to append.
        ///
        /// \post
        ///
        ///             this->size() == pre(this)->size() + b.size()
        ///
        ///         &&  this->size() <= this->max_size()
        ///
        /// \returns `*this`
        ///
        /// \throws noexcept <tt>(false)</tt>
        ///
    this_array& push_back( ali::initializer_list<T> b )
    {
        return this->push_back(make_ref(b));
    }

    this_array& append( ali::initializer_list<T> b )
    {
        return this->push_back(b);
    }

    this_array& push_back( read_only_T value, int n2 )
    {
        ali_assert(0 <= n2);

        if ( this->back_capacity() >= n2 )
        {
            this->uninitialized_fill_back(value, n2);
        }
        else
        {
            this->reallocating_replace_1(this->_size, 0, value, n2);
        }

        return this->self();
    }

    this_array& append( read_only_T value, int n2 )
    {
        return this->push_back(value, n2);
    }

        /// \brief  Appends a copy of the given
        /// element to the end of this array.
        ///
        /// \param  value
        ///         Object to initialize the new element with.
        ///
        /// \post
        ///
        ///             this->size() == pre(this)->size() + 1
        ///
        ///         &&  this->size() <= this->max_size()
        ///
        ///         &&  forall i in set {0, ..., this->size() - 2} &
        ///                     this->at(i) is not changed in any way
        ///                 ||  this->at(i) is moved from pre(this)->at(i)
        ///
        ///         &&  this->back() is copy of value
        ///
        /// \returns `*this`
        ///
        /// \throws noexcept <tt>(false)</tt>
        ///
    this_array& push_back( read_only_T value )
    {
        if ( this->_size != this->capacity() )
        {
            this->uninitialized_copy_back_1(value);
        }
        else
        {
            this->reallocating_replace_1(this->_size, 0, value, 1);
        }

        return this->self();
    }

    this_array& append( read_only_T value )
    {
        return this->push_back(value);
    }

        /// \brief  Appends the given element to the end of this array
        /// using move semantic.
        ///
        /// \param  value
        ///         Object to initialize the new element with.
        ///
        /// \post
        ///
        ///             this->size() == pre(this)->size() + 1
        ///
        ///         &&  this->size() <= this->max_size()
        ///
        ///         &&  forall i in set {0, ..., this->size() - 2} &
        ///                     this->at(i) is not changed in any way
        ///                 ||  this->at(i) is moved from pre(this)->at(i)
        ///
        ///         &&  this->back() is moved from value
        ///
        /// \returns `*this`
        ///
        /// \throws noexcept <tt>(false)</tt>
        ///
    template <typename return_type = this_array&>
    typename meta::enable_if<
        read_only_T_is_const_ref::result,
        return_type>::type  //  :-/
            push_back( T&& value )
    {
        if ( this->_size != this->capacity() )
        {
            this->uninitialized_move_back_1(value);
        }
        else
        {
            this->reallocating_replace_1(this->_size, 0, ali::move(value));
        }
        
        return this->self();
    }

    template <typename return_type = this_array&>
    typename meta::enable_if<
        read_only_T_is_const_ref::result,
        return_type>::type  //  :-/
            append( T&& value )
    {
        return this->push_back(ali::move(value));
    }

    //  replace

    this_array& replace( int pos1, int n1, array_const_ref<T> b )
    {
        ali_assert(0 <= pos1);
        ali_assert(pos1 <= this->_size);
        ali_assert(0 <= n1);
        ali_assert(n1 <= this->_size - pos1);

        if ( n1 >= b.size() )
        {
            this->shrinking_replace_n(pos1, n1, b);
        }
        else if ( this->back_capacity() < b.size() - n1 )
        {
            this->reallocating_replace_n(pos1, n1, b);
        }
        else
        {
            this->growing_replace_n(pos1, n1, b);
        }

        return this->self();
    }

    this_array& replace( int pos1, int n1, ali::initializer_list<T> b )
    {
        return this->replace(pos1, n1, make_ref(b));
    }

    this_array& replace( int pos1, int n1, read_only_T value, int n2 = 1 )
    {
        ali_assert(0 <= pos1);
        ali_assert(pos1 <= this->_size);
        ali_assert(0 <= n1);
        ali_assert(n1 <= this->_size - pos1);
        ali_assert(0 <= n2);

        if ( n1 >= n2 )
        {
            this->shrinking_replace_1(pos1, n1, value, n2);
        }
        else if ( this->back_capacity() < n2 - n1 )
        {
            this->reallocating_replace_1(pos1, n1, value, n2);
        }
        else
        {
            this->growing_replace_1(pos1, n1, value, n2);
        }

        return this->self();
    }

    //  :-/
    template <typename return_type = this_array&>
    typename meta::enable_if<
        read_only_T_is_const_ref::result,
        return_type>::type
            replace( int pos1, int n1, T&& value )
    {
        ali_assert(0 <= pos1);
        ali_assert(pos1 <= this->_size);
        ali_assert(0 <= n1);
        ali_assert(n1 <= this->_size - pos1);

        if ( n1 >= 1 )
        {
            this->shrinking_replace_1(pos1, n1, ali::move(value));
        }
        else if ( this->back_capacity() < 1 - n1 )
        {
            this->reallocating_replace_1(pos1, n1, ali::move(value));
        }
        else
        {
            this->growing_replace_1(pos1, n1, ali::move(value));
        }

        return this->self();
    }

    //  erase

        /// \brief  Erases elements in the given range from this array.
        ///
        /// \param  i
        ///         Index of the first element to erase.
        /// \param  n
        ///         Number of elements to erase.
        ///
        /// \pre
        ///
        ///             0 <= i && i <= this->size()
        ///         &&  0 <= n && n <= this->size() - i
        ///
        /// \post
        ///
        ///             this->size() == pre(this)->size() - n
        ///
        ///         &&  forall j in set {0, ..., i - 1} &
        ///                 this->at(j) is not changed in any way
        ///
        ///         &&  forall j in set {i, ..., this->size() - 1} &
        ///                 this->at(j) is moved from pre(this)->at(j + n)
        ///
        ///         &&  forall j in set {this->size(), ..., pre(this)->size() - 1} &
        ///                 pre(this)->at(j) is destroyed
        ///
        /// \returns `*this`
        ///
        /// \throws noexcept <tt>(noexcept(T = T) && noexcept(T::~T()))</tt>
        ///
    this_array& erase( int pos1, int n1 = 1 )
    {
        ali_assert(0 <= pos1);
        ali_assert(pos1 <= this->_size);
        ali_assert(0 <= n1);
        ali_assert(n1 <= this->_size - pos1);

        if ( n1 != 0 )
        {
            this->mutable_ref().move_if_noexcept(
                pos1, this->mutable_ref_right(pos1 + n1));
                
            this->private_erase_back(n1);
        }

        return this->self();
    }

        /// \brief  Erases all elements contained in this array.
        ///
        /// \note   This method does not necessarily deallocate
        /// the internal buffer. If you need to ensure that the
        /// internal buffer of `arr` is deallocated, then write:
        /// `ali::array<T>().swap(arr)`.
        ///
        /// \post
        ///
        ///             this->is_empty() 
        ///
        ///         &&  forall i in set {0, ..., pre(this)->size() - 1} 
        ///                 pre(this)->at(i) is destroyed
        ///
        /// \returns `*this`
        ///
        /// \throws noexcept <tt>(noexcept(T::~T()))</tt>
        ///
    this_array& erase( void )
    {
        this->private_erase_back(this->_size);

        return this->self();
    }
    
    this_array& clear( void )
    {
        return this->erase();
    }

        /// \brief  Erases elements from the end of this array.
        ///
        /// \param  n
        ///         Number of elements to erase.
        ///
        /// \pre    <tt>0 <= n && n <= this->size()</tt>
        ///
        /// \post
        ///
        ///             this->size() == pre(this)->size() - n
        ///
        ///         &&  forall i in set {0, ..., this->size() - 1} &
        ///                 this->at(i) is not changed in any way
        ///
        ///         &&  forall i in set {this->size(), ..., pre(this)->size() - 1} &
        ///                 pre(this)->at(i) is destroyed
        ///
        /// \returns `*this`
        ///
        /// \throws noexcept <tt>(noexcept(T::~T()))</tt>
        ///
    this_array& erase_back( int n1 = 1 )
    {
        ali_assert(0 <= n1);
        ali_assert(n1 <= this->_size);

        this->private_erase_back(n1);

        return this->self();
    }

        /// \brief  Erases elements from the beginning of this array.
        ///
        /// \param  n
        ///         Number of elements to erase.
        ///
        /// \pre    <tt>0 <= n && n <= this->size()</tt>
        ///
        /// \post
        ///
        ///             this->size() == pre(this)->size() - n
        ///
        ///         &&  forall i in set {0, ..., this->size() - 1} &
        ///                 this->at(i) is moved from pre(this)->at(i + n)
        ///
        ///         &&  forall i in set {this->size(), ..., pre(this)->size() - 1} &
        ///                 pre(this)->at(i) is destroyed
        ///
        /// \returns `*this`
        ///
        /// \throws noexcept <tt>(noexcept(T = T) && noexcept(T::~T()))</tt>
        ///
    this_array& erase_front( int n1 = 1 )
    {
        return this->erase(0, n1);
    }

    this_array& erase_left( int pos1 )
    {
        return this->erase(0, pos1);
    }

    this_array& erase_right( int pos1 )
    {
        ali_assert(0 <= pos1);
        ali_assert(pos1 <= this->_size);

        this->private_erase_back(this->_size - pos1);

        return this->self();
    }

        /// \brief  Erases elements matching the specified predicate.
        ///
        /// \tparam predicate
        ///         The predicate's type.
        /// \param  p
        ///         The predicate; an element `t` is erased
        ///         if `p(t)` evaluates to `true`.
        ///
        /// \pre    For an object `t` of type `T`, `p(t)` is valid
        ///         expression convertible to `bool`.
        ///
        /// \post
        ///
        ///             result == pre(this)->size() - this->size()
        ///
        ///         &&  let original_indexes_of_retained_elements
        ///                     = [i | 0 <= i && i < pre(this)->size() && !p(pre(this)->at(i))]
        ///                         //  This is a sequence, not a set -- order is important.
        ///
        ///             in      this->size() == len original_indexes_of_retained_elements
        ///
        ///                 &&  forall i in set {0, ..., this->size() - 1} &
        ///                             i == original_indexes_of_retained_elements(i)
        ///                         ||  this->at(i) is moved from pre(this)->at(
        ///                                 original_indexes_of_retained_elements(i))
        ///
        ///         &&  forall i in set {this->size(), ..., pre(this)->size() - 1} &
        ///                 pre(this)->at(i) is destroyed
        ///
        /// \returns The number of erased elements.
        ///
        /// \throws noexcept <tt>(noexcept(p(T) && noexcept(T = T) && noexcept(T::~T()))</tt>
        ///
    template <typename predicate>
    int erase_if( predicate p )
    {
        int const result{
            this->_size - this->mutable_ref().stable_partition_left_only(
                [p] ( read_only_T value ) { return !p(value); })};

        this->private_erase_back(result);
        
        return result;
    }

    //  resize

        /// \brief  Appends/erases elements to/from this
        /// array such that its \ref size is eual to the
        /// specified value.
        ///
        /// Appended elements are initialized with `value`.
        ///
        /// \param  size
        ///         The desired size of this array.
        /// \param  value
        ///         Object to initialize appended elements with.
        ///
        /// \pre    <tt>0 <= size</tt>
        ///
        /// \post
        ///
        ///                 this->size() == size
        ///
        ///             &&  this->size() <= this->max_size()
        ///
        ///             &&  (    this->size() == pre(this)->size()
        ///
        ///                 ||  (   this->size() < pre(this)->size()
        ///
        ///                     &&  forall i in set {0, ..., this->size() - 1} &
        ///                             this->at(i) is not changed in any way
        ///
        ///                     &&  forall i in set {this->size(), ..., pre(this)->size() - 1} &
        ///                             pre(this)->at(i) is destroyed)
        ///
        ///                 ||  (   this->size() > pre(this)->size()
        ///
        ///                     &&  forall i in set {0, ..., pre(this)->size() - 1} &
        ///                                 this->at(i) is not changed in any way
        ///                             ||  this->at(i) is moved from pre(this)->at(i)
        ///
        ///                     &&  forall i in set {pre(this)->size(), ..., this->size() - 1} &
        ///                             this->at(i) is copy of value))
        ///
        /// \returns `*this`
        ///
        /// \throws noexcept <tt>(false)</tt>
        ///
    this_array& resize( int size, read_only_T value )
    {
        ali_assert(0 <= size);

        if ( size < this->_size )
        {
            this->private_erase_back(this->_size - size);
        }
        else if ( size > this->_size )
        {
            this->push_back(value, size - this->_size);
        }

        return this->self();
    }

    this_array& resize( int size )
    {
        ali_assert(0 <= size);

        if ( size < this->_size )
        {
            this->private_erase_back(this->_size - size);
        }
        else if ( size > this->_size )
        {
            this->reserve(size);
            
            this->default_construct_back(size - this->_size);
        }

        return this->self();
    }

    //  assign

    this_array& assign( array_const_ref<T> b )
    {
        if ( this->_size >= b.size() )
        {
            this->shrinking_assign(b);
        }
        else if ( this->back_capacity() < b.size() - this->_size )
        {
            this->reallocating_assign(b);
        }
        else
        {
            this->growing_assign(b);
        }

        return this->self();
    }

    this_array& assign( ali::initializer_list<T> b )
    {
        return this->assign(make_ref(b));
    }

    this_array& assign( read_only_T value, int n2 = 1 )
    {
        return this->replace(0, this->_size, value, n2);
    }

    //  :-/
    template <typename return_type = this_array&>
    typename meta::enable_if<
        read_only_T_is_const_ref::result,
        return_type>::type
            assign( T&& value )
    {
        return this->replace(0, this->_size, ali::move(value));
    }

    //  push_front

    this_array& push_front( array_const_ref<T> b )
    {
        return this->replace(0, 0, b);
    }

    this_array& push_front( ali::initializer_list<T> b )
    {
        return this->push_front(make_ref(b));
    }

        /// \brief  Inserts a copy of the given element at the
        /// beginning of this array.
        ///
        /// \param  value
        ///         Object to initialize the new element with.
        ///
        /// \post
        ///
        ///             this->size() == pre(this)->size() + 1
        ///
        ///         &&  this->size() <= this->max_size()
        ///
        ///         &&  this->front() is copy of value
        ///
        ///         &&  forall i in set {1, ..., this->size() - 1} &
        ///                 this->at(i) is moved from pre(this)->at(i - 1)
        ///
        /// \returns `*this`
        ///
        /// \throws noexcept <tt>(false)</tt>
        ///
    this_array& push_front( read_only_T value, int n2 = 1 )
    {
        return this->replace(0, 0, value, n2);
    }

    //  :-/
    template <typename return_type = this_array&>
    typename meta::enable_if<
        read_only_T_is_const_ref::result,
        return_type>::type
            push_front( T&& value )
    {
        return this->replace(0, 0, ali::move(value));
    }

    //  insert

        /// \brief  Inserts a copy of all elements in another
        /// array into this array at the specified index.
        ///
        /// \param  i
        ///         Index where to place the first new element.
        /// \param  b
        ///         Array of objects to initialize the new elements with.
        ///
        /// \pre    <tt>0 <= i && i <= this->size()</tt>
        ///
        /// \post
        ///
        ///             this->size() == pre(this)->size() + b.size()
        ///
        ///         &&  this->size() <= this->max_size()
        ///
        ///         &&  forall j in set {0, ..., i - 1} &
        ///                     this->at(j) is not changed in any way
        ///                 ||  this->at(j) is moved from pre(this)->at(j)
        ///
        ///         &&  forall j in set {i, ..., i + b.size() - 1} &
        ///                 this->at(j) is copy of b.at(j - i)
        ///
        ///         &&  forall j in set {i + b.size(), ..., this->size() - 1} &
        ///                 this->at(j) is moved from pre(this)->at(j - b.size())
        ///
        /// \returns `*this`
        ///
        /// \throws noexcept <tt>(false)</tt>
        ///
    this_array& insert( int pos1, array_const_ref<T> b )
    {
        return this->replace(pos1, 0, b);
    }

    this_array& insert( int pos1, ali::initializer_list<T> b )
    {
        return this->insert(pos1, make_ref(b));
    }

        /// \brief  Inserts a copy of the given element into
        /// this array at the specified index.
        ///
        /// \param  i
        ///         Index where to place the new element.
        /// \param  value
        ///         Object to initialize the new element with.
        ///
        /// \pre    <tt>0 <= i && i <= this->size()</tt>
        ///
        /// \post
        ///
        ///             this->size() == pre(this)->size() + 1
        ///
        ///         &&  this->size() <= this->max_size()
        ///
        ///         &&  forall j in set {0, ..., i - 1} &
        ///                     this->at(j) is not changed in any way
        ///                 ||  this->at(j) is moved from pre(this)->at(j)
        ///
        ///         &&  this->at(i) is copy of value
        ///
        ///         &&  forall j in set {i + 1, ..., this->size() - 1} &
        ///                 this->at(j) is moved from pre(this)->at(j - 1)
        ///
        /// \returns `*this`
        ///
        /// \throws noexcept <tt>(false)</tt>
        ///
    this_array& insert( int pos1, read_only_T value, int n2 = 1 )
    {
        return this->replace(pos1, 0, value, n2);
    }

        /// \brief  Inserts the given element into this array at the
        //  specified index using move semantic.
        ///
        /// \param  i
        ///         Index where to place the new element.
        /// \param  value
        ///         Object to initialize the new element with.
        ///
        /// \pre    <tt>0 <= i && i <= this->size()</tt>
        ///
        /// \post
        ///
        ///             this->size() == pre(this)->size() + 1
        ///
        ///         &&  this->size() <= this->max_size()
        ///
        ///         &&  forall j in set {0, ..., i - 1} &
        ///                     this->at(j) is not changed in any way
        ///                 ||  this->at(j) is moved from pre(this)->at(j)
        ///
        ///         &&  this->at(i) is moved from value
        ///
        ///         &&  forall j in set {i + 1, ..., this->size() - 1} &
        ///                 this->at(j) is moved from pre(this)->at(j - 1)
        ///
        /// \returns `*this`
        ///
        /// \throws noexcept <tt>(false)</tt>
        ///
    template <typename return_type = this_array&>
    typename meta::enable_if<
        read_only_T_is_const_ref::result,
        return_type>::type  //  :-/
            insert( int pos1, T&& value )
    {
        return this->replace(pos1, 0, ali::move(value));
    }

    //  conversion operators

    operator array_const_ref<T>( void ) const
    {
        return array_const_ref<T>{
            this->_begin, this->_size};
    }

    operator array_const_ref<T>( void )
    {
        return array_const_ref<T>{
            this->_begin, this->_size};
    }

    operator array_ref<T>( void )
    {
        return array_ref<T>{
            this->_begin, this->_size};
    }

    //  ref

    array_const_ref<T> ref( void ) const
    {
        return *this;
    }

    array_const_ref<T> ref( int pos, int n ) const
    {
        return this->ref().ref(pos, n);
    }

    array_const_ref<T> ref_left( int pos ) const
    {
        return this->ref().ref_left(pos);
    }

    array_const_ref<T> ref_right( int pos ) const
    {
        return this->ref().ref_right(pos);
    }

    array_const_ref<T> ref_front( int n ) const
    {
        return this->ref().ref_front(n);
    }

    array_const_ref<T> ref_back( int n ) const
    {
        return this->ref().ref_back(n);
    }

    array_const_ref<T> ref_not_front( int n ) const
    {
        return this->ref().ref_not_front(n);
    }

    array_const_ref<T> ref_not_back( int n ) const
    {
        return this->ref().ref_not_back(n);
    }

    //  mutable_ref

    array_ref<T> mutable_ref( void )
    {
        return *this;
    }

    array_ref<T> mutable_ref( int pos, int n )
    {
        return this->mutable_ref().mutable_ref(pos, n);
    }

    array_ref<T> mutable_ref_left( int pos )
    {
        return this->mutable_ref().mutable_ref_left(pos);
    }

    array_ref<T> mutable_ref_right( int pos )
    {
        return this->mutable_ref().mutable_ref_right(pos);
    }

    array_ref<T> mutable_ref_front( int n )
    {
        return this->mutable_ref().mutable_ref_front(n);
    }

    array_ref<T> mutable_ref_back( int n )
    {
        return this->mutable_ref().mutable_ref_back(n);
    }

    array_ref<T> mutable_ref_not_front( int n )
    {
        return this->mutable_ref().mutable_ref_not_front(n);
    }

    array_ref<T> mutable_ref_not_back( int n )
    {
        return this->mutable_ref().mutable_ref_not_back(n);
    }

    //  pointer

    array_const_ptr<T> pointer( void ) const
    {
        return this->ref().pointer();
    }

    array_const_ptr<T> pointer( int pos, int n ) const
    {
        return this->ref(pos, n).pointer();
    }

    array_const_ptr<T> pointer_left( int pos ) const
    {
        return this->ref_left(pos).pointer();
    }

    array_const_ptr<T> pointer_right( int pos ) const
    {
        return this->ref_right(pos).pointer();
    }

    array_const_ptr<T> pointer_front( int n ) const
    {
        return this->ref_front(n).pointer();
    }

    array_const_ptr<T> pointer_back( int n ) const
    {
        return this->ref_back(n).pointer();
    }

    array_const_ptr<T> pointer_not_front( int n ) const
    {
        return this->ref_not_front(n).pointer();
    }

    array_const_ptr<T> pointer_not_back( int n ) const
    {
        return this->ref_not_back(n).pointer();
    }

    //  mutable_pointer

    array_ptr<T> mutable_pointer( void )
    {
        return this->mutable_ref().mutable_pointer();
    }

    array_ptr<T> mutable_pointer( int pos, int n )
    {
        return this->mutable_ref(pos, n).mutable_pointer();
    }

    array_ptr<T> mutable_pointer_left( int pos )
    {
        return this->mutable_ref_left(pos).mutable_pointer();
    }

    array_ptr<T> mutable_pointer_right( int pos )
    {
        return this->mutable_ref_right(pos).mutable_pointer();
    }

    array_ptr<T> mutable_pointer_front( int n )
    {
        return this->mutable_ref_front(n).mutable_pointer();
    }

    array_ptr<T> mutable_pointer_back( int n )
    {
        return this->mutable_ref_back(n).mutable_pointer();
    }

    array_ptr<T> mutable_pointer_not_front( int n )
    {
        return this->mutable_ref_not_front(n).mutable_pointer();
    }

    array_ptr<T> mutable_pointer_not_back( int n )
    {
        return this->mutable_ref_not_back(n).mutable_pointer();
    }

    bool invariant( void  ) const
    {
        return  0 <= this->size()
            &&  this->size() <= this->capacity()
            &&  this->capacity() <= this->max_size()
            &&  this->is_empty() == (this->size() == 0);
    }

        /// \brief  Determines whether the given object resides
        /// inside this array.
        ///
        /// \param  value
        ///         Object to test.
        ///
        /// \returns Index into this array where the given object
        /// is located, if it indeed resides in this array.
        /// Value less than zero otherwise.
        ///
        /// \throws noexcept
        ///
    is_internal_element_result is_internal_element( T const& value ) const
    {
        return this->ref().is_internal_element(value);
    }

    is_internal_pointer_result is_internal_pointer( T const* b ) const
    {
        return this->ref().is_internal_pointer(b);
    }
    
    is_internal_pointer_result is_internal_pointer( array_const_ref<T> b ) const
    {
        return this->ref().is_internal_pointer(b);
    }

    is_internal_pointer_result is_internal_pointer( array_const_ptr<T> b ) const
    {
        return this->ref().is_internal_pointer(b);
    }

    //  iterators

        /// \brief  Provides read-only iterator
        /// pointing to the beginning of this array.
        ///
        /// \returns \ref const_iterator object
        /// pointing to the beginning of this array.
        ///
        /// \throws noexcept
        ///
        /// \deprecated Use indexes instead.
        ///
    const_iterator begin( void ) const
    {
        return this->_begin;
    }

    friend const_iterator begin( array_common const& a )
    {
        return a.begin();
    }

        /// \brief  Provides read-write iterator
        /// pointing to the beginning of this array.
        ///
        /// \returns \ref iterator object pointing
        /// to the beginning of this array.
        ///
        /// \throws noexcept
        ///
        /// \deprecated Use indexes instead.
        ///
    iterator begin( void )
    {
        return this->_begin;
    }

    friend iterator begin( array_common& a )
    {
        return a.begin();
    }

        /// \brief  Provides read-only iterator pointing to
        /// the one past the last element of this array.
        ///
        /// \returns \ref const_iterator object pointing to
        /// the one past the last element of this array.
        ///
        /// \throws noexcept
        ///
        /// \deprecated Use indexes instead.
        ///
    const_iterator end( void ) const
    {
        return this->_begin + this->_size;
    }

    friend const_iterator end( array_common const& a )
    {
        return a.end();
    }

        /// \brief  Provides read-write iterator pointing to
        /// the one past the last element of this array.
        ///
        /// \returns \ref iterator object pointing to
        /// the one past the last element of this array.
        ///
        /// \throws noexcept
        ///
        /// \deprecated Use indexes instead.
        ///
    iterator end( void )
    {
        return this->_begin + this->_size;
    }

    friend iterator end( array_common& a )
    {
        return a.end();
    }

    //  range

    const_range_type range( void ) const
    {
        return const_range_type(this->begin(), this->end());
    }

    range_type range( void )
    {
        return range_type(this->begin(), this->end());
    }

        /// \brief  Determines whether both arrays are of
        /// the same size and contain equal elements.
        ///
        /// \param  b
        ///         Another array to compare this array with.
        ///
        /// \pre    There are `operator==` and `operator!=`
        /// that accept objects of type `T` and
        /// `(a != b) == !(a == b)` for all `a`, `b` of type `T`.
        ///
        /// \post
        ///
        ///             result
        ///                 ==  this->size() == b.size()
        ///                 &&  forall i in set {0, ..., this->size() - 1} &
        ///                         this->at(i) == b.at(i)
        ///
        /// \retval true
        ///         Both arrays are of the same size
        ///         and contain equal elements.
        /// \retval false
        ///         Otherwise.
        ///
        /// \throws noexcept <tt>(noexcept(T == T) && noexcept(T != T))</tt>
        ///
    bool operator==( array_const_ref<T> b ) const
    {
        return this->ref() == b;
    }

        /// \brief  Determines whether both arrays are of
        /// different size or contain non-equal elements.
        ///
        /// \param  b
        ///         Another array to compare this array with.
        ///
        /// \pre    There are `operator==` and `operator!=`
        /// that accept objects of type `T` and
        /// `(a != b) == !(a == b)` for all `a`, `b` of type `T`.
        ///
        /// \post
        ///
        ///             result
        ///                 ==  this->size() != b.size()
        ///                 ||  exists i in set {0, ..., this->size() - 1} &
        ///                         this->at(i) != b.at(i)
        ///
        /// \retval true
        ///         The arrays differ in size or
        ///         in the contained elements.
        /// \retval false
        ///         Otherwise.
        ///
        /// \throws noexcept <tt>(noexcept(T == T) && noexcept(T != T))</tt>
        ///
    bool operator!=( array_const_ref<T> b ) const
    {
        return !this->operator==(b);
    }

protected:  //  Methods

        /// \brief  Default constructor. Constructs empty array.
        /// \post   <tt>this->is_empty()</tt>
        /// \throws noexcept
        ///
    array_common( void ) {}

        /// \brief  Copy constructor.
        /// Initializes this array by copying all elements
        /// from another array.
        ///
        /// \param  b
        ///         Another instance of the `array` class.
        ///
        /// \post
        ///
        ///             this->size() == b.size()
        ///
        ///         &&  forall i in set {0, ..., this->size() - 1} &
        ///                 this->at(i) is copy of b.at(i)
        ///
        /// \throws noexcept <tt>(false)</tt>
        ///
    array_common( array_common const& b )
    :   array_common{b.ref()}
    {}
    
        /// \brief  Move constructor.
        /// Initializes this array by moving all elements
        /// from another array.
        ///
        /// \param  b
        ///         Another instance of the `array` class.
        ///
        /// \post
        ///
        ///             this->size() == pre(b)->size()
        ///
        ///         &&  forall i in set {0, ..., this->size() - 1} &
        ///                 &this->at(i) == &pre(b)->at(i)
        ///                     //  Note the address-of operators (&) above.
        ///
        /// \throws noexcept
        ///
    array_common( array_common&& b )
    {
        this->swap(b);
    }

    array_common( array_const_ref<T> b )
    {
        this->push_back(b);
    }

        /// \brief Initializes this array with the specified
        /// number of elements.
        ///
        /// \param  size
        ///         The desired number of elements in the array.
        ///
        /// \pre    <tt>0 <= size</tt>
        ///
        /// \post
        ///
        ///             this->size() == size
        ///
        ///         &&  forall i in set {0, ..., this->size() - 1} &
        ///                 this->at(i) is default constructed element
        ///
        /// \throws noexcept <tt>(false)</tt>
        ///
    explicit array_common( int size )
    {
        this->resize(size);
    }
    
        /// \brief Initializes this array with the specified
        /// number of elements.
        ///
        /// \param  size
        ///         The desired number of elements in the array.
        /// \param  value
        ///         Object to initialize elements of this array with.
        ///
        /// \pre    <tt>0 <= size</tt>
        ///
        /// \post
        ///
        ///             this->size() == size
        ///
        ///         &&  forall i in set {0, ..., this->size() - 1} &
        ///                 this->at(i) is copy of value
        ///
        /// \throws noexcept <tt>(false)</tt>
        ///
    array_common( int size, read_only_T value )
    {
        this->resize(size, value);
    }

    //  array_common( ali::initializer_list<T> b );
            //  Intentionally postponed to derived classes as
            //  this constructor is a nasty little game changer.

        /// \brief  Destructor.
        /// Destroys all contained elements and
        /// frees all memory used by this array.
        ///
        /// \post
        ///
        ///         forall i in set {0, ..., pre(this)->size() - 1} &
        ///             pre(this)->at(i) is destroyed
        ///
        /// \throws noexcept <tt>(noexcept(T::~T()))</tt>
        ///
    ~array_common( void )
    {
        this->erase();
    }

        /// \brief  Assignment operator.
        /// Copies all elements from another array into this array.
        ///
        /// \param  b
        ///         Array of elements to copy into this array.
        ///
        /// \post
        ///
        ///             this->size() == b.size()
        ///
        ///         &&  forall i in set {0, ..., this->size() - 1} &
        ///                 this->at(i) is copy of b.at(i)
        ///
        /// \returns `*this`
        ///
        /// \throws noexcept <tt>(false)</tt>
        ///
    array_common& operator=( array_common const& b )
    {
        return this->operator=(b.ref());
    }

        /// \brief  Move assignment operator.
        /// Moves all elements from another array into this array.
        ///
        /// \param  b
        ///         Array of elements to move into this array.
        ///
        /// \post
        ///
        ///             this->size() == pre(b)->size()
        ///
        ///         &&  forall i in set {0, ..., this->size() - 1} &
        ///                 &this->at(i) == &pre(b)->at(i)
        ///                     //  Note the address-of operators (&) above.
        ///
        /// \returns `*this`
        ///
        /// \throws noexcept <tt>(false)</tt>
        ///
    array_common& operator=( array_common&& b )
    {
        if ( this != &b )
        {
            this->swap(b);
            
            array_common{}.swap(b);
        }
        
        return *this;
    }

    array_common& operator=( array_const_ref<T> b )
    {
        return this->assign(b);
    }

    array_common& operator=( ali::initializer_list<T> b )
    {
        return this->operator=(make_ref(b));
    }

    int calculate_auto_capacity( int min_back_capacity ) const
        //  pre:    0 < n
        //  post:       n <= result - this->_size
        //          &&  result <= this->max_size()
    {
        ali_assert(0 < min_back_capacity);

        int const max_size{this->max_size()};

        this->raise_requested_capacity_too_large_if(
            min_back_capacity > max_size - this->_size,
            ALI_HERE);

        int const capacity{ali::maxi(4, this->_size + ali::maxi(
            ali::mini(max_size - this->_size, this->_size / 2),
            min_back_capacity))};

        ali_assert(min_back_capacity <= capacity - this->_size);
        ali_assert(capacity <= max_size);

        return capacity;
    }

    this_array& reserve( int min_capacity, fixed_capacity_version )
    {
        ali_assert(0 <= min_capacity);

        this->raise_requested_capacity_too_large_if(
            min_capacity > this->max_size(), ALI_HERE);

        return this->self();
    }
    
    this_array& reserve( int min_capacity, dynamic_capacity_version )
    {
        ali_assert(0 <= min_capacity);
    
        if ( this->capacity() < min_capacity )
        {
            this->raise_requested_capacity_too_large_if(
                min_capacity > this->max_size(), ALI_HERE);
    
            this->private_reserve(min_capacity);
        }
    
        ali_assert(min_capacity <= this->capacity());
    
        return this->self();
    }

    this_array& reserve_back( int min_back_capacity, fixed_capacity_version )
    {
        ali_assert(0 <= min_back_capacity);

        this->raise_requested_capacity_too_large_if(
            min_back_capacity > this->max_size() - this->_size,
            ALI_HERE);

        return this->self();
    }

    this_array& reserve_back( int min_back_capacity, dynamic_capacity_version )
    {
        ali_assert(0 <= min_back_capacity);

        if ( this->back_capacity() < min_back_capacity )
        {
            this->raise_requested_capacity_too_large_if(
                min_back_capacity > this->max_size() - this->_size,
                ALI_HERE);

            this->private_reserve(this->_size + min_back_capacity);
        }

        ali_assert(min_back_capacity <= this->back_capacity());

        return this->self();
    }

    void private_reserve( int capacity )
    {
        ali_assert(this->capacity() < capacity);
        ali_assert(capacity <= this->max_size());
        
        array_common temp{private_version{}, capacity};

        ali_assert(temp._size == 0);
        ali_assert(temp.capacity() == capacity);
        ali_assert(temp._begin != nullptr);

        temp.uninitialized_move_back_n(*this);
            
        temp.swap(*this);

        ali_assert(this->capacity() == capacity);
    }

    void default_construct_back( int const n )
    {
        this->default_construct_back(n, is_pod{});
    }

        void default_construct_back( int const n, pod_version )
        {
            ali_assert(0 <= n);
            ali_assert(n <= this->back_capacity());
            
            this->_size += n;

            this->mutable_ref_back(n).fill(T());
        }

        void default_construct_back( int const n, non_pod_version )
        {
            ali_assert(0 <= n);
            ali_assert(n <= this->back_capacity());
            
            for ( int i{}; i != n; ++i, ++this->_size )
                new (this->_begin + this->_size) T();
        }

    template <typename constructor>
    void construct_back(
        int const n,
        constructor construct )
            //  construct( T* storage, int i )
            //  i goes from 0 upto n - 1, inclusive.
    {
        ali_assert(0 <= n);
        ali_assert(n <= this->back_capacity());
        
        for ( int i{}; i != n; ++i, ++this->_size )
            construct(this->_begin + this->_size, i);
    }

    void uninitialized_copy_back_n( array_const_ref<T> b )
    {
        this->uninitialized_copy_back_n(b, is_pod{});
    }

        void uninitialized_copy_back_n( array_const_ref<T> b, pod_version )
        {
            ali_assert(b.size() <= this->back_capacity());

            this->_size += b.size();

            this->mutable_ref().copy_back(b);
        }

        void uninitialized_copy_back_n( array_const_ref<T> b, non_pod_version )
        {
            this->construct_back(
                b.size(),
                [b]( T* storage, int i )
                { new (storage) T(b[i]); });
        }

    void uninitialized_copy_back_1( read_only_T value )
    {
        this->uninitialized_copy_back_1(value, is_pod{});
    }
        
        void uninitialized_copy_back_1( read_only_T value, pod_version )
        {
            ali_assert(1 <= this->back_capacity());

            this->_begin[this->_size] = value;

            ++this->_size;
        }

        void uninitialized_copy_back_1( read_only_T value, non_pod_version )
        {
            ali_assert(1 <= this->back_capacity());

            new (this->_begin + this->_size) T(value);

            ++this->_size;
        }

    void uninitialized_move_back_n( array_ref<T> b )
    {
        this->uninitialized_move_back_n(b, is_pod{});
    }

        void uninitialized_move_back_n( array_ref<T> b, pod_version version )
        {
            this->uninitialized_copy_back_n(b, version);
        }

        void uninitialized_move_back_n( array_ref<T> b, non_pod_version )
        {
            this->construct_back(
                b.size(),
                [&b]( T* storage, int i )
                { new (storage) T(ali::move_if_noexcept(b.at(i))); });
        }

    void uninitialized_move_back_1( T& value )
    {
        this->uninitialized_move_back_1(value, is_pod{});
    }

        void uninitialized_move_back_1( T& value, pod_version version )
        {
            this->uninitialized_copy_back_1(value, version);
        }

        void uninitialized_move_back_1( T& value, non_pod_version )
        {
            ali_assert(1 <= this->back_capacity());

            new (this->_begin + this->_size) T(ali::move_if_noexcept(value));

            ++this->_size;
        }

    void uninitialized_fill_back( read_only_T value, int n2 )
    {
        this->uninitialized_fill_back(value, n2, is_pod{});
    }

        void uninitialized_fill_back( read_only_T value, int n2, pod_version )
        {
            ali_assert(0 <= n2);
            ali_assert(n2 <= this->back_capacity());

            this->_size += n2;

            this->mutable_ref_back(n2).fill(value);
        }
        
        void uninitialized_fill_back( read_only_T value, int n2, non_pod_version )
        {
            this->construct_back(
                n2,
                [&value]( T* storage, int )
                { new (storage) T(value); });
        }

    void reallocating_copy_back_n( array_const_ref<T> b )
    {
        this->reallocating_copy_back_n(b, has_fixed_capacity{});
    }

        void reallocating_copy_back_n(
            array_const_ref<T> b,
            fixed_capacity_version )
        {
            this->raise_requested_capacity_too_large(ALI_HERE);
        }

        void reallocating_copy_back_n(
            array_const_ref<T> b,
            dynamic_capacity_version )
        {
            array_common temp{
                private_version{},
                this->calculate_auto_capacity(b.size())};

            {
                temp.uninitialized_move_back_n(*this);

                temp.swap(*this);
            }

            //  Now temp holds the original vector.
            //  Thus, regarding aliasing, we need
            //  to check against temp, not this.

            auto const internal_element
                = temp.is_internal_element(b.front());

            if ( internal_element.index_or_negative >= 0 )
            {
                //  Aliasing.

                ali_assert(temp.is_internal_element(
                    b.back()).index_or_negative >= 0);

                int const pos2{internal_element.index_or_negative};
                    
                int const n2{b.size()};
                    //  b's elements were moved to temp along
                    //  with ours and we then swapped this and temp.
                    //  Here, we calculate the coordinates we
                    //  will use to obtain a new "b" from this.

                this->uninitialized_copy_back_n(this->ref(pos2, n2));
            }
            else
            {
                ali_assert(temp.is_internal_element(
                    b.back()).index_or_negative < 0);

                this->uninitialized_copy_back_n(b);
            }
        }

    void reallocating_replace_n( int pos1, int n1, array_const_ref<T> b )
    {
        this->reallocating_replace_n(pos1, n1, b, has_fixed_capacity{});
    }

        void reallocating_replace_n(
            int pos1, int n1, array_const_ref<T> b,
            fixed_capacity_version )
        {
            ali_assert(0 <= pos1);
            ali_assert(pos1 <= this->_size);
            ali_assert(0 <= n1);
            ali_assert(n1 <= this->_size - pos1);
            ali_assert(n1 < b.size());

            this->raise_requested_capacity_too_large(ALI_HERE);
        }

        void reallocating_replace_n(
            int pos1, int n1, array_const_ref<T> b,
            dynamic_capacity_version )
        {
            ali_assert(0 <= pos1);
            ali_assert(pos1 <= this->_size);
            ali_assert(0 <= n1);
            ali_assert(n1 <= this->_size - pos1);
            ali_assert(n1 < b.size());

            array_common temp{
                private_version{},
                this->calculate_auto_capacity(b.size() - n1)};

            {
                temp.uninitialized_move_back_n(*this);

                temp.swap(*this);
            }

            //  Now temp holds the original vector.
            //  Thus, regarding aliasing, we need
            //  to check against temp, not this.

            auto const internal_element
                = temp.is_internal_element(b.front());

            if ( internal_element.index_or_negative >= 0 )
            {
                //  Aliasing.

                ali_assert(temp.is_internal_element(
                    b.back()).index_or_negative >= 0);

                int const pos2{internal_element.index_or_negative};
                    
                int const n2{b.size()};
                    //  b's elements were moved to temp along
                    //  with ours and we then swapped this and temp.
                    //  Here, we calculate the coordinates we
                    //  will use to obtain a new "b" from this.

                this->growing_replace_n(pos1, n1, this->ref(pos2, n2));
            }
            else
            {
                ali_assert(temp.is_internal_element(
                    b.back()).index_or_negative < 0);

                this->growing_replace_n(pos1, n1, b);
            }
        }

    void reallocating_replace_1( int pos1, int n1, read_only_T value, int n2 )
    {
        this->reallocating_replace_1(
            pos1, n1, value, n2,
            has_fixed_capacity{});
    }

        void reallocating_replace_1(
            int pos1, int n1, read_only_T value, int n2,
            fixed_capacity_version )
        {
            ali_assert(0 <= pos1);
            ali_assert(pos1 <= this->_size);
            ali_assert(0 <= n1);
            ali_assert(n1 <= this->_size - pos1);
            ali_assert(n1 < n2);

            this->raise_requested_capacity_too_large(ALI_HERE);
        }

        void reallocating_replace_1(
            int pos1, int n1, read_only_T value, int n2,
            dynamic_capacity_version )
        {
            ali_assert(0 <= pos1);
            ali_assert(pos1 <= this->_size);
            ali_assert(0 <= n1);
            ali_assert(n1 <= this->_size - pos1);
            ali_assert(n1 < n2);
        
            array_common temp{
                private_version{},
                this->calculate_auto_capacity(n2 - n1)};

            {
                temp.uninitialized_move_back_n(*this);

                temp.swap(*this);
            }

            //  Now temp holds the original vector.
            //  Thus, regarding aliasing, we need
            //  to check against temp, not this.

            auto const internal_element
                = temp.is_internal_element(value);

            if ( internal_element.index_or_negative >= 0 )
            {
                //  Aliasing.

                int const pos2{internal_element.index_or_negative};
                    //  value was moved to temp along with our
                    //  elements and we then swapped this and temp.
                    //  Here, we calculate the coordinates we
                    //  will use to obtain a new "value" from this.

                this->growing_replace_1(pos1, n1, this->_begin[pos2], n2);
            }
            else
            {
                this->growing_replace_1(pos1, n1, value, n2);
            }
        }

    void reallocating_replace_1( int pos1, int n1, T&& value )
    {
        this->reallocating_replace_1(
            pos1, n1, ali::move(value),
            has_fixed_capacity{});
    }

        void reallocating_replace_1(
            int pos1, int n1, T&& value,
            fixed_capacity_version )
        {
            ali_assert(0 <= pos1);
            ali_assert(pos1 <= this->_size);
            ali_assert(n1 == 0);

            this->raise_requested_capacity_too_large(ALI_HERE);
        }

        void reallocating_replace_1(
            int pos1, int n1, T&& value,
            dynamic_capacity_version )
        {
            ali_assert(0 <= pos1);
            ali_assert(pos1 <= this->_size);
            ali_assert(n1 == 0);

            array_common temp{
                private_version{},
                this->calculate_auto_capacity(1)};

            {
                temp.uninitialized_move_back_n(*this);

                temp.swap(*this);
            }

            //  Now temp holds the original vector.
            //  Thus, regarding aliasing, we need
            //  to check against temp, not this.

            auto const internal_element
                = temp.is_internal_element(value);

            if ( internal_element.index_or_negative >= 0 )
            {
                //  Aliasing.

                int const pos2{internal_element.index_or_negative};
                    //  value was moved to temp along with our
                    //  elements and we then swapped this and temp.
                    //  Here, we calculate the coordinates we
                    //  will use to obtain a new "value" from this.

                this->growing_replace_1(
                    pos1, n1, ali::move(this->_begin[pos2]));
            }
            else
            {
                this->growing_replace_1(
                    pos1, n1, ali::move(value));
            }
        }

    void growing_replace_n( int pos1, int n1, array_const_ref<T> b )
    {
        ali_assert(0 <= pos1);
        ali_assert(pos1 <= this->_size);
        ali_assert(0 <= n1);
        ali_assert(n1 <= this->_size - pos1);
        ali_assert(n1 < b.size());
        
        int const delta{b.size() - n1};

        ali_assert(this->back_capacity() >= delta);

        int const end1{pos1 + n1};

        if ( pos1 == this->_size )
        {
            //  push_back

            this->uninitialized_copy_back_n(b);
        }
        else if ( end1 == this->_size )
        {
            //  replace_right

            this->uninitialized_copy_back_n(b.ref_right(n1));
                    
            this->mutable_ref().copy(pos1, b.ref_left(n1));
        }
        else
        {
            auto const internal_element
                = this->is_internal_element(b.front());

            if ( internal_element.index_or_negative >= 0 )
            {
                ali_assert(this->is_internal_element(
                    b.back()).index_or_negative >= 0);

                int const pos2{internal_element.index_or_negative};

                this->uninitialized_copy_back_n(this->ref(pos2 + n1, delta));

                this->mutable_ref().copy(pos1, this->ref(pos2, n1));

                this->mutable_ref_right(pos1 + n1).rotate_right(delta);
            }
            else
            {
                ali_assert(this->is_internal_element(
                    b.back()).index_or_negative < 0);

                if ( pos1 + delta > this->_size )
                {
                    int const n_tail{this->_size - end1};

                    int const pos2{this->_size - pos1};
                
                    this->uninitialized_copy_back_n(b.ref_right(pos2));
                        //  also: b.ref_back(delta - (this->_size - end1)));

                    this->uninitialized_move_back_n(
                        this->mutable_ref(end1, n_tail));

                    this->mutable_ref().copy(pos1, b.ref_left(pos2));
                }
                else
                {
                    this->uninitialized_move_back_n(
                        this->mutable_ref_back(delta));

                    if ( this->_size - end1 > 2 * delta )
                    {
                        this->mutable_ref().move_if_noexcept(
                            pos1 + b.size(),
                            this->mutable_ref(
                                end1,
                                this->_size - 2 * delta - end1));
                    }
                    
                    this->mutable_ref().copy(pos1, b);
                }
            }
        }
    }

    void growing_replace_1( int pos1, int n1, read_only_T value, int n2 )
    {
        ali_assert(0 <= pos1);
        ali_assert(pos1 <= this->_size);
        ali_assert(0 <= n1);
        ali_assert(n1 <= this->_size - pos1);
        ali_assert(n1 < n2);
        
        int const delta{n2 - n1};

        ali_assert(this->back_capacity() >= delta);

        int const end1{pos1 + n1};
        
        if ( end1 == this->_size )
        {
            this->uninitialized_fill_back(value, delta);

            this->mutable_ref(pos1, n1).fill(value);
        }
        else
        {
            this->uninitialized_copy_back_n(this->ref_back(delta));

            if ( this->_size - end1 > 2 * delta )
            {
                this->mutable_ref().copy(
                    pos1 + n2,
                    this->ref(
                        end1,
                        this->_size - 2 * delta - end1));

                auto const internal_element
                    = this->is_internal_element(value);

                if ( internal_element.index_or_negative >= 0 )
                {
                    int pos2 = internal_element.index_or_negative;

                    if ( pos2 >= pos1 + n2 )
                        pos2 += delta;

                    this->mutable_ref(pos1, n2).fill(this->_begin[pos2]);
                }
                else
                {
                    this->mutable_ref(pos1, n2).fill(value);
                }
            }
            else
            {
                this->mutable_ref(pos1, n2).fill(value);
            }
        }
    }

    void growing_replace_1( int pos1, int n1, T&& value )
    {
        ali_assert(0 <= pos1);
        ali_assert(pos1 <= this->_size);
        ali_assert(n1 == 0);

        ali_assert(this->back_capacity() >= 1);

        if ( pos1 == this->_size )
        {
            this->uninitialized_move_back_1(value);
        }
        else
        {
            T temp(ali::move_if_noexcept(value));
                //  Move the value to a temporary first,
                //  because the following call may change
                //  the value if &value == &this->back().

            this->uninitialized_move_back_1(this->back());
                //  This may change the value
                //  if &value == &this->back().

            if ( this->_size - pos1 > 2 )
            {
                this->mutable_ref().move_if_noexcept(
                    pos1 + 1,
                    this->mutable_ref(
                        pos1, this->_size - 2 - pos1));
            }

            this->_begin[pos1] = ali::move_if_noexcept(temp);
        }
    }

    void shrinking_replace_n( int pos1, int n1, array_const_ref<T> b )
    {
        ali_assert(0 <= pos1);
        ali_assert(pos1 <= this->_size);
        ali_assert(0 <= n1);
        ali_assert(n1 <= this->_size - pos1);
        ali_assert(b.size() <= n1);

        this->mutable_ref().copy(pos1, b);
            
        if ( b.size() < n1 )
        {
            this->mutable_ref().move_if_noexcept(
                pos1 + b.size(),
                this->mutable_ref_right(
                    pos1 + n1));
                
            this->private_erase_back(n1 - b.size());
        }
    }

    void shrinking_replace_1( int pos1, int n1, read_only_T value, int n2 )
    {
        ali_assert(0 <= pos1);
        ali_assert(pos1 <= this->_size);
        ali_assert(0 <= n1);
        ali_assert(n1 <= this->_size - pos1);
        ali_assert(n2 <= n1);

        this->mutable_ref(pos1, n2).fill(value);
            
        if ( n2 < n1 )
        {
            this->mutable_ref().move_if_noexcept(
                pos1 + n2,
                this->mutable_ref_right(
                    pos1 + n1));
                
            this->private_erase_back(n1 - n2);
        }
    }
    
    void shrinking_replace_1( int pos1, int n1, T&& value )
    {
        ali_assert(0 <= pos1);
        ali_assert(pos1 <= this->_size);
        ali_assert(1 <= n1);
        ali_assert(n1 <= this->_size - pos1);

        this->_begin[pos1] = ali::move_if_noexcept(value);
            
        if ( 1 < n1 )
        {
            this->mutable_ref().move_if_noexcept(
                pos1 + 1,
                this->mutable_ref_right(
                    pos1 + n1));
                
            this->private_erase_back(n1 - 1);
        }
    }

    //  The *_assign methods are specializations of *_replace methods
    //  with the first two parameters fixed at 0 and this->_size.
    //  The motivation is to eliminate unnecessary call to rotate_right
    //  which requires T to define swap function.

    void reallocating_assign( array_const_ref<T> b )
    {
        this->reallocating_assign(b, has_fixed_capacity{});
    }

        void reallocating_assign(
            array_const_ref<T> b,
            fixed_capacity_version )
        {
            this->raise_requested_capacity_too_large(ALI_HERE);
        }

        void reallocating_assign(
            array_const_ref<T> b,
            dynamic_capacity_version )
        {
            ali_assert(this->_size < b.size());

            array_common temp{private_version{}, b.size()};

            temp.uninitialized_copy_back_n(b);

            temp.swap(*this);
        }

    void growing_assign( array_const_ref<T> b )
    {
        ali_assert(this->_size < b.size());
        ali_assert(this->back_capacity() >= b.size() - this->_size);

        this->uninitialized_copy_back_n(b.ref_right(this->_size));
                    
        this->mutable_ref().copy(0, b.ref_left(this->_size));
    }

    void shrinking_assign( array_const_ref<T> b )
    {
        ali_assert(b.size() <= this->_size);

        this->mutable_ref().copy(0, b);
            
        this->private_erase_back(this->_size - b.size());
    }

    void private_erase_back( int n1 )
    {
        this->private_erase_back(n1, is_pod{});
    }
    
        void private_erase_back( int n1, pod_version )
        {
            ali_assert(0 <= n1);
            ali_assert(n1 <= this->_size);

            this->_size -= n1;
        }
    
        void private_erase_back( int n1, non_pod_version )
        {
            ali_assert(0 <= n1);
            ali_assert(n1 <= this->_size);

            for ( ; n1 != 0; --n1 )
            {
                --this->_size;
            
                this->_begin[this->_size].~T();
            }
        }

        /// \brief  Swaps contents of this array with another array.
        ///
        /// \param  b
        ///         Another array to swap content with.
        ///
        /// \post
        ///
        ///                 this->size() == pre(b).size()
        ///
        ///             &&  forall i in set {0, ..., this->size() - 1} &
        ///                     &this->at(i) == &pre(b).at(i)
        ///
        ///             &&  b.size() == pre(this)->size()
        ///
        ///             &&  forall i in set {0, ..., b.size() - 1} &
        ///                     &b.at(i) == &pre(this)->at(i)
        ///
        /// \throws noexcept
        ///
    void swap( array_common& b )
    {
        this->swap(b, has_fixed_capacity{});
    }

        void swap( array_common& b, fixed_capacity_version )
        {
            //  Swap elements.
        
            int const m{ali::maxi(this->_size, b._size)};
            int const this_delta{m - this->_size};
            int const b_delta{m - b._size};
            
            this->default_construct_back(this_delta);
            b.default_construct_back(b_delta);
                //  Make the arrays the same size.
            
            ali_assert(this->_size == b._size);
            
            this->mutable_ref()
                .swap_block_front(
                    b.mutable_ref());
                        //  Swap individual elements.
            
            this->private_erase_back(b_delta);
                //  Intentionally erasing b_delta elements from this.
            b.private_erase_back(this_delta);
                //  Intentionally erasing this_delta elements from b.
        }
        
        void swap( array_common& b, dynamic_capacity_version )
        {
            //  Swap pointers.

            using ali::swap;
            swap(this->_begin, b._begin);
            swap(this->_size, b._size);
            swap(this->_capacity, b._capacity);
        }

    this_array& self( void )
    {
        return static_cast<this_array&>(*this);
    }

    this_array const& self( void ) const
    {
        return static_cast<this_array const&>(*this);
    }

    static void raise_requested_capacity_too_large(
        ali::location const& location )
    {
        general_error(_T("Requested capacity too large."), location);
    }

    static void raise_requested_capacity_too_large_if(
        bool condition, ali::location const& location )
    {
        if ( condition )
            raise_requested_capacity_too_large(location);
    }

private:    //  Methods
    struct private_version{};

    array_common( private_version, int capacity )
    :   this_array_storage{capacity}
    {}

    template <typename, int>
    friend class hidden::array_storage;
};

// ******************************************************************
// ******************************************************************

}   //  namespace hidden


// ******************************************************************
template <typename T, int capacity_>
class array : public hidden::array_common<T, capacity_>
// ******************************************************************
{
    typedef hidden::array_common<T, capacity_> array_common;

public:
    typedef typename array_common::read_only_T read_only_T;

public:
    array( void ) {}

    array( array const& b )
    :   array_common{b}
    {}

    array( array&& b )
    :   array_common{ali::move(b)}
    {}

    array( array_const_ref<T> b )
    :   array_common{b}
    {}

    explicit array( int size )
    :   array_common(size)
            //  Intentional use of ( ) initializer to
            //  disambiguate from the initializer_list version.
    {}
    
    array( int size, read_only_T value )
    :   array_common(size, value)
            //  Intentional use of ( ) initializer to
            //  disambiguate from the initializer_list version.
    {}

        /// \brief  Initializer list constructor.
        /// Initializes this array from an initializer list.
        ///
        /// \param  b
        ///         The initializer list.
        ///
        /// \post
        ///
        ///             this->size() == pre(b)->size()
        ///
        /// \throws noexcept <tt>(false)</tt>
        ///
    array( ali::initializer_list<T> b )
    :   array_common{make_ref(b)}
    {}

    array& operator=( array const& b )
    {
        array_common::operator=(b);

        return *this;
    }

    array& operator=( array&& b )
    {
        array_common::operator=(ali::move(b));

        return *this;
    }

    array& operator=( array_const_ref<T> b )
    {
        array_common::operator=(b);

        return *this;
    }

    array& operator=( ali::initializer_list<T> b )
    {
        array_common::operator=(b);

        return *this;
    }

    void swap( array& b )
    {
        array_common::swap(b);
    }

    friend void swap( array& a, array& b )
    {
        a.swap(b);
    }
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <int capacity_>
class array<ali::uint8, capacity_>
// ******************************************************************
    : public hidden::array_common<ali::uint8, capacity_>
{
    typedef ali::uint8 T;
    typedef hidden::array_common<T, capacity_> array_common;

public:
    typedef typename array_common::read_only_T read_only_T;

public:
    array( void ) {}

    array( array const& b )
    :   array_common{b}
    {}

    array( array&& b )
    :   array_common{ali::move(b)}
    {}

    array( array_const_ref<T> b )
    :   array_common{b}
    {}

    array( void const* data, int size )
    :   array_common{blob_const_ref{reinterpret_cast<
            ali::uint8 const*>(data), size}}
    {}

    array( string_const_ref b )
    :   array_common{b.as_blob()}
    {}

    explicit array( int size )
    :   array_common(size)
            //  Intentional use of ( ) initializer to
            //  disambiguate from the initializer_list version.
    {}
    
    array( int size, read_only_T value )
    :   array_common(size, value)
            //  Intentional use of ( ) initializer to
            //  disambiguate from the initializer_list version.
    {}

        /// \brief  Initializer list constructor.
        /// Initializes this array from an initializer list.
        ///
        /// \param  b
        ///         The initializer list.
        ///
        /// \post
        ///
        ///             this->size() == pre(b)->size()
        ///
        /// \throws noexcept <tt>(false)</tt>
        ///
    array( ali::initializer_list<T> b )
    :   array_common{make_ref(b)}
    {}

    array& operator=( array const& b )
    {
        array_common::operator=(b);

        return *this;
    }

    array& operator=( array&& b )
    {
        array_common::operator=(ali::move(b));

        return *this;
    }

    array& operator=( array_const_ref<T> b )
    {
        array_common::operator=(b);

        return *this;
    }

    array& operator=( ali::initializer_list<T> b )
    {
        array_common::operator=(b);

        return *this;
    }

    array& operator=( string_const_ref b )
    {
        array_common::operator=(b.as_blob());

        return *this;
    }

    //  replace

    using array_common::replace;

    array& replace( int pos1, int n1, void const* data, int size )
    {
        return this->replace(pos1, n1, blob_const_ref{
            reinterpret_cast<ali::uint8 const*>(data), size});
    }

    array& replace( int pos1, int n1, string_const_ref b )
    {
        return this->replace(pos1, n1, b.as_blob());
    }

    //  assign

    using array_common::assign;

    array& assign( void const* data, int size )
    {
        return this->assign(blob_const_ref{
            reinterpret_cast<ali::uint8 const*>(data), size});
    }

    array& assign( string_const_ref value )
    {
        return this->assign(value.as_blob());
    }

    //  append

    using array_common::append;

    array& append( void const* data, int size )
    {
        return this->append(blob_const_ref{
            reinterpret_cast<ali::uint8 const*>(data), size});
    }

    array& append( string_const_ref value )
    {
        return this->append(value.as_blob());
    }

    // pre: byte_count in set {1, 2, 3, 4}
    array& append_int_be( ali::uint32 value, int byte_count )
    {
        int const begin{this->size()};
        return this->append(ali::uint8{}, byte_count)
            .set_int_be_at(begin, value, byte_count);
    }
    
    // pre: byte_count in set {1, 2, 3, 4}
    array& append_int_le( ali::uint32 value, int byte_count )
    {
        int const begin{this->size()};
        return this->append(ali::uint8{}, byte_count)
            .set_int_le_at(begin, value, byte_count);
    }

    // pre: byte_count in set {1, 2, 3, 4, 5, 6, 7, 8}
    array& append_long_be( ali::uint64 value, int byte_count )
    {
        int const begin{this->size()};
        return this->append(ali::uint8{}, byte_count)
            .set_long_be_at(begin, value, byte_count);
    }
        
    // pre: byte_count in set {1, 2, 3, 4, 5, 6, 7, 8}
    array& append_long_le_at( ali::uint64 value, int byte_count )
    {
        int const begin{this->size()};
        return this->append(ali::uint8{}, byte_count)
            .set_long_le_at(begin, value, byte_count);
    }
    
    array& append_int8( ali::uint8 value )
    {
        return this->append(value);
    }

    array& append_int8( ali::uint8 value, int n2 )
    {
        return this->append(value, n2);
    }
    
    array& append_int16_be( ali::uint16 value )
    {
        return this->append_int_be(value, 2);
    }
    
    array& append_int16_le( ali::uint16 value )
    {
        return this->append_int_le(value, 2);
    }
    
    array& append_int16_ne( ali::uint16 value )
    {
        return this->append_int_le(to_little_endian(value), 2);
    }

    array& append_int32_be( ali::uint32 value )
    {
        return this->append_int_be(value, 4);
    }

    array& append_int32_le( ali::uint32 value )
    {
        return this->append_int_le(value, 4);
    }
    
    array& append_int32_ne( ali::uint32 value )
    {
        return this->append_int_le(to_little_endian(value), 4);
    }

    array& append_int64_be( ali::uint64 value )
    {
        return this->append_long_be(value, 8);
    }

    array& append_int64_le( ali::uint64 value )
    {
        return this->append_long_le(value, 8);
    }
    
    array& append_int64_ne( ali::uint64 value )
    {
        return this->append_long_le(to_little_endian(value), 8);
    }

    //  insert

    using array_common::insert;
    
    array& insert( int pos1, void const* data, int size )
    {
        return this->insert(pos1, blob_const_ref{
            reinterpret_cast<ali::uint8 const*>(data), size});
    }

    array& insert( int pos1, string_const_ref b )
    {
        return this->insert(pos1, b.as_blob());
    }

    // pre: byte_count in set {1, 2, 3, 4}
    array& insert_int_be( int pos1, ali::uint32 value, int byte_count )
    {
        return this->insert(pos1, ali::uint8{}, byte_count)
            .set_int_be_at(pos1, value, byte_count);
    }
    
    // pre: byte_count in set {1, 2, 3, 4}
    array& insert_int_le( int pos1, ali::uint32 value, int byte_count )
    {
        return this->insert(pos1, ali::uint8{}, byte_count)
            .set_int_le_at(pos1, value, byte_count);
    }

    // pre: byte_count in set {1, 2, 3, 4, 5, 6, 7, 8}
    array& insert_long_be( int pos1, ali::uint64 value, int byte_count )
    {
        return this->insert(pos1, ali::uint8{}, byte_count)
            .set_long_be_at(pos1, value, byte_count);
    }
        
    // pre: byte_count in set {1, 2, 3, 4, 5, 6, 7, 8}
    array& insert_long_le_at( int pos1, ali::uint64 value, int byte_count )
    {
        return this->insert(pos1, ali::uint8{}, byte_count)
            .set_long_le_at(pos1, value, byte_count);
    }
    
    array& insert_int8( int pos1, ali::uint8 value, int n2 = 1 )
    {
        return this->insert(pos1, value, n2);
    }
    
    array& insert_int16_be( int pos1, ali::uint16 value )
    {
        return this->insert_int_be(pos1, value, 2);
    }
    
    array& insert_int16_le( int pos1, ali::uint16 value )
    {
        return this->insert_int_le(pos1, value, 2);
    }
    
    array& insert_int16_ne( int pos1, ali::uint16 value )
    {
        return this->insert_int_le(pos1, to_little_endian(value), 2);
    }

    array& insert_int32_be( int pos1, ali::uint32 value )
    {
        return this->insert_int_be(pos1, value, 4);
    }

    array& insert_int32_le( int pos1, ali::uint32 value )
    {
        return this->insert_int_le(pos1, value, 4);
    }
    
    array& insert_int32_ne( int pos1, ali::uint32 value )
    {
        return this->insert_int_le(pos1, to_little_endian(value), 4);
    }

    array& insert_int64_be( int pos1, ali::uint64 value )
    {
        return this->insert_long_be(pos1, value, 8);
    }

    array& insert_int64_le( int pos1, ali::uint64 value )
    {
        return this->insert_long_le(pos1, value, 8);
    }
    
    array& insert_int64_ne( int pos1, ali::uint64 value )
    {
        return this->insert_long_le(pos1, to_little_endian(value), 8);
    }

    //  int

    template <typename integer_type>
    integer_type int_be_at_( int pos, int byte_count ) const
        // pre: byte_count in set {1 ... sizeof(integer_type)}
    {
        return this->ref().template int_be_at_<integer_type>(pos, byte_count);
    }

    template <typename integer_type>
    integer_type int_le_at_( int pos, int byte_count ) const
        // pre: byte_count in set {1 ... sizeof(integer_type)}
    {
        return this->ref().template int_le_at_<integer_type>(pos, byte_count);
    }

    ali::uint32 int_be_at( int pos, int byte_count ) const
        // pre: byte_count in set {1, 2, 3, 4}
    {
        return this->ref().int_be_at(pos, byte_count);
    }

    ali::uint32 int_le_at( int pos, int byte_count ) const
        // pre: byte_count in set {1, 2, 3, 4}
    {
        return this->ref().int_le_at(pos, byte_count);
    }

    ali::uint8 int8_at( int pos ) const
    {
        return this->at(pos);
    }
    
    ali::uint16 int16_be_at( int pos ) const
    {
        return this->ref().int16_be_at(pos);
    }

    ali::uint16 int16_le_at( int pos ) const
    {
        return this->ref().int16_le_at(pos);
    }

    ali::uint16 int16_ne_at( int pos ) const
        //  Native endian.
    {
        return this->ref().int16_ne_at(pos);
    }
    
    ali::uint32 int32_be_at( int pos ) const
    {
        return this->ref().int32_be_at(pos);
    }

    ali::uint32 int32_le_at( int pos ) const
    {
        return this->ref().int32_le_at(pos);
    }
    
    ali::uint32 int32_ne_at( int pos ) const
        //  Native endian.
    {
        return this->ref().int32_ne_at(pos);
    }

    ali::uint64 int64_be_at( int pos ) const
    {
        return this->ref().int64_be_at(pos);
    }

    ali::uint64 int64_le_at( int pos ) const
    {
        return this->ref().int64_le_at(pos);
    }
    
    ali::uint64 int64_ne_at( int pos ) const
        //  Native endian.
    {
        return this->ref().int64_ne_at(pos);
    }

    // pre: byte_count in set {1, 2, 3, 4}
    array& set_int_be_at(
        int pos, ali::uint32 value, int byte_count )
    {
        this->mutable_ref().set_int_be_at(pos, value, byte_count);
        return *this;
    }
        
    // pre: byte_count in set {1, 2, 3, 4}
    array& set_int_le_at(
        int pos, ali::uint32 value, int byte_count )
    {
        this->mutable_ref().set_int_le_at(pos, value, byte_count);
        return *this;
    }

    // pre: byte_count in set {1, 2, 3, 4, 5, 6, 7, 8}
    array& set_long_be_at(
        int pos, ali::uint64 value, int byte_count )
    {
        this->mutable_ref().set_long_be_at(pos, value, byte_count);
        return *this;
    }
        
    // pre: byte_count in set {1, 2, 3, 4, 5, 6, 7, 8}
    array& set_long_le_at(
        int pos, ali::uint64 value, int byte_count )
    {
        this->mutable_ref().set_long_le_at(pos, value, byte_count);
        return *this;
    }

    array& set_int8_at( int pos, ali::uint8 value )
    {
        this->at(pos) = value;
        return *this;
    }

    array& set_int16_be_at( int pos, ali::uint16 value )
    {
        return this->set_int_be_at(pos, value, 2);
    }

    array& set_int16_le_at( int pos, ali::uint16 value )
    {
        return this->set_int_le_at(pos, value, 2);
    }
    
    array& set_int16_ne_at( int pos, ali::uint16 value )
        //  Native endian.
    {
        return this->set_int_le_at(pos, to_little_endian(value), 2);
    }

    array& set_int32_be_at( int pos, ali::uint32 value )
    {
        return this->set_int_be_at(pos, value, 4);
    }

    array& set_int32_le_at( int pos, ali::uint32 value )
    {
        return this->set_int_le_at(pos, value, 4);
    }
    
    array& set_int32_ne_at( int pos, ali::uint32 value )
        //  Native endian.
    {
        return this->set_int_le_at(pos, to_little_endian(value), 4);
    }

    array& set_int64_be_at( int pos, ali::uint64 value )
    {
        return this->set_long_be_at(pos, value, 8);
    }
    
    array& set_int64_le_at( int pos, ali::uint64 value )
    {
        return this->set_long_le_at(pos, value, 8);
    }
    
    array& set_int64_ne_at( int pos, ali::uint64 value )
        //  Native endian.
    {
        return this->set_long_le_at(pos, to_little_endian(value), 8);
    }
    
    //  set_bin_at
    
    array& set_bin_at( int pos, void const* data, int size )
    {
        this->mutable_ref().copy(pos, blob_const_ref{
            reinterpret_cast<ali::uint8 const*>(data), size});
        return *this;
    }

    array& set_bin_at( int pos, blob_const_ref value )
    {
        this->mutable_ref().copy(pos, value);
        return *this;
    }

    //  swap

    void swap( array& b )
    {
        array_common::swap(b);
    }

    friend void swap( array& a, array& b )
    {
        a.swap(b);
    }

    string_const_ref as_string( void ) const
    {
        return this->ref().as_string();
    }
};

// ******************************************************************
// ******************************************************************

array<bool> test_array( void );

}   //  namespace ali
