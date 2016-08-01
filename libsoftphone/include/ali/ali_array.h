#pragma once
#include "ali_array_forward.h"

#ifdef  ALI_STREAMLINED_ARRAYS

#include "ali_array2.h"

// ******************************************************************
//  THE FOLLOWING CLASSES AND FUNCTIONS ARE TO BE GRADUALLY REMOVED.
// ******************************************************************

#include "ali_auto_ptr.h"
#include "ali_buffer.h"
#include "ali_comma.h"
#include "ali_string.h" //  Used by format_t<ali::array<T> >

namespace ali
{

#if 1

// ******************************************************************
template <typename T, int size>
using fixed_size_array = buffer<T, size>;
// ******************************************************************

#else

/** @brief Array of a fixed size
  * @tparam T Type stored in the array
  * @tparam SIZE Array size */
// ******************************************************************
template <typename T,
          int SIZE>
class fixed_size_array
// ******************************************************************
{
public:
    /** @brief Type stored in the array */
    typedef T   value_type;

public:
    /** @brief Get array size
      * @return Array size */
    static int size()
    {
        return SIZE;
    }

    /** @brief Convert to a constant pointer
      * @return This array converted to a constant pointer */
    operator T const*() const
    {
        return _arr;
    }

    /** @brief Convert to a pointer
      * @return This array converted to a pointer */
    operator T*()
    {
        return _arr;
    }

    /** @brief Constant index operator
      * @param pos Index of the requested constant element; @c 0 ? @p pos < @ref SIZE
      * @return The constant element at the specified position */
    const T& operator [](int pos) const
    {
        ali_assert(0 <= pos && pos < SIZE);
        return _arr[pos];
    }

    /** @brief Index operator
      * @param pos Index of the requested element; @c 0 ? @p pos < @ref SIZE
      * @return The element at the specified position */
    T& operator [](int pos)
    {
        ali_assert(0 <= pos && pos < SIZE);
        return _arr[pos];
    }

    /** @brief Get as a constant pointer
      * @return This array as a constant pointer */
    T const* data() const
    {
        return _arr;
    }

    /** @brief Get as a pointer
      * @return This array as a pointer */
    T* data()
    {
        return _arr;
    }


private:
    T   _arr[SIZE]; ///< The array itself
};

#endif


#if 1

template <typename T, int capacity>
using fixed_capacity_array = array<T, capacity>;

#else

/** @brief Array with a fixed capacity
  * @tparam T Type stored in the array
  * @tparam CAP Array capacity */
// ******************************************************************
template <typename T,
          int CAP>
class fixed_capacity_array
// ******************************************************************
{
public:
    /** @brief Type stored in the array */
    typedef T           value_type;

ALI_DEPRECATED(
    /** @brief Size type */
    typedef int         size_type;
)

    /** @brief Reference type */
    typedef T&          reference;

    /** @brief Constant reference type */
    typedef T const&    const_reference;

    /** @brief Iterator type */
    typedef T*          iterator;

    /** @brief Constant iterator type */
    typedef T const*    const_iterator;


    /** @brief Constructor */
    fixed_capacity_array()
        : _size(0)
    {}

    /** @brief Get array capacity
      * @return Array capacity */
    int capacity() const
    {
        return CAP;
    }

    /** @brief Get array size
      * @return Array size */
    int size() const
    {
        return _size;
    }

    bool is_empty( void ) const
    {
        return _size == 0;
    }

    /** @brief Erase the array
      * @return Self */
    fixed_capacity_array& erase()
    {
        _size = 0;
        return *this;
    }

ALI_DEPRECATED(
    /** @brief Clear the array
      * @return Self
      * @deprecated Use @ref erase instead */
    fixed_capacity_array& clear()
    {
        return erase();
    }
)

    /** @brief Resize the array
      * @param size New size; @c 0 ? @p size ? @ref CAP
      * @param t Initializer for new elements
      * @return Self */
    fixed_capacity_array& resize(int size,
                                 T const& t = T())
    {
        ali_assert(0 <= size && size <= CAP);
        for (int i = _size; i < size; ++i)
            _arr[i] = t;
        _size = size;
        return *this;
    }

    /** @brief Push an item at the back
      * @param t Item to push at the back
      * @return Self */
    fixed_capacity_array& push_back(T const& t)
    {
        ali_assert(_size < CAP);
        _arr[_size] = t;
        ++_size;
        return *this;
    }

#ifdef ALI_FEATURE_RVALUE_REF
    
    /** @brief Push an item at the back
      * @param t Item to push at the back
      * @return Self */
    fixed_capacity_array& push_back(T&& t)
    {
        ali_assert(_size < CAP);
        _arr[_size] = ali::move(t);
        ++_size;
        return *this;
    }

#endif  //  ALI_FEATURE_RVALUE_REF

    /** @brief Pop an item from the back
      * @return Item popped from the back */
    fixed_capacity_array& pop_back()
    {
        ali_assert(_size > 0);
        --_size;
        return *this;
    }

    /** @brief Get element at the front
      * @return Element at the front */
    reference front()
    {
        ali_assert(_size > 0);
        return _arr[0];
    }

    /** @brief Get constant element at the front
      * @return Constant element at the front */
    const_reference front() const
    {
        ali_assert(_size > 0);
        return _arr[0];
    }

    /** @brief Get element at the back
      * @return Element at the back */
    reference back()
    {
        ali_assert(_size > 0);
        return _arr[_size - 1];
    }

    /** @brief Get constant element at the back
      * @return Constant element at the back */
    const_reference back() const
    {
        ali_assert(_size > 0);
        return _arr[_size - 1];
    }

    /** @brief Get iterator at the begin
      * @return Iterator at the begin */
    iterator begin()
    {
        return _arr;
    }

    friend iterator begin(fixed_capacity_array & a)
    {
        return a.begin();
    }

    /** @brief Get constant iterator at the begin
      * @return Constant iterator at the begin */
    const_iterator begin() const
    {
        return _arr;
    }

    friend const_iterator begin(fixed_capacity_array const& a)
    {
        return a.begin();
    }

    /** @brief Get iterator at the end
      * @return Iterator at the end (beyond the last item) */
    iterator end()
    {
        return _arr + _size;
    }

    friend iterator end(fixed_capacity_array & a)
    {
        return a.end();
    }

    /** @brief Get constant iterator at the end
      * @return Constant iterator at the end (beyond the last item) */
    const_iterator end() const
    {
        return _arr + _size;
    }

    friend const_iterator end(fixed_capacity_array const& a)
    {
        return a.end();
    }

    /** @brief Index operator
      * @param i Index of the requested element; @c 0 ? @p i < @ref size
      * @return The element at the specified index */
    reference operator [](int i)
    {
        ali_assert(0 <= i && i < _size);
        return _arr[i];
    }

    /** @brief Constant index operator
      * @param i Index of the requested constant element; @c 0 ? @p i < @ref size
      * @return The constant element at the specified index */
    const_reference operator [](int i) const
    {
        ali_assert(0 <= i && i < _size);
        return _arr[i];
    }

    void swap( fixed_capacity_array& b )
    {
        using ali::swap;
        swap(_size, b._size);
        swap(_arr, b._arr);
    }

    friend void swap(
        fixed_capacity_array& a,
        fixed_capacity_array& b )
    {
        a.swap(b);
    }

    operator array_const_ref<T>( void ) const
    {
        return array_const_ref<T>(this->_arr, this->_size);
    }

    operator array_const_ref<T>( void )
    {
        return array_const_ref<T>(this->_arr, this->_size);
    }

    operator array_ref<T>( void )
    {
        return array_ref<T>(this->_arr, this->_size);
    }

    array_const_ref<T> ref( void ) const
    {
        return *this;
    }

    array_ref<T> mutable_ref( void )
    {
        return *this;
    }

private:
    int _size; ///< Used array size
    T   _arr[CAP]; ///< The array
};

#endif

#if 1

template <typename T>
using ptr_array = array<T*>;

#else

/** @brief Dynamic array storing pointers
  * @param T Type of the pointer stored in the array */
// ******************************************************************
template <typename T>
class ptr_array
// ******************************************************************
{
public:
    /** @brief Pointer type stored in the array */
    typedef T*          value_type;

ALI_DEPRECATED(
    /** @brief Size type */
    typedef int         size_type;
)

    /** @brief Reference type */
    typedef T*&         reference;

    /** @brief Constant reference type */
    typedef T* const&   const_reference;

    /** @brief Iterator type */
    typedef T**         iterator;

    /** @brief Constant iterator type */
    typedef T* const*   const_iterator;


    /** @brief Default constructor */
    ptr_array()
        : _size(0)
        , _cap(0)
        , _arr(nullptr)
    {}

    /** @brief Constructor of a pre-filled array
      * @param size Size of the newly created array
      * @param val Initializer for each new element */
    explicit ptr_array(int size,
                       T* val = nullptr)
        : _size(0)
        , _cap(0)
        , _arr(nullptr)
    {
        resize(size, val);
    }

    /** @brief Copy constructor
      * @param b Pointer array to copy
      * @note This makes only a shallow copy, i.e. the pointed data are not
      *       copied */
    ptr_array(ptr_array const& b)
        : _size(0)
        , _cap(0)
        , _arr(nullptr)
    {
        if (b._size == 0)
            return;

        reserve(b._size);

        _size = b._size;

        for (int i = 0; i < _size; ++i)
            _arr[i] = b._arr[i];
    }

    /** @brief Destructor */
    ~ptr_array()
    {
        delete[] _arr;
    }

    /** @brief Assignment operator
      * @param b Array to copy from
      * @return Self */
    ptr_array& operator =(ptr_array b)
    {
        swap(b);
        return *this;
    }

    /** @brief Get maximal size
      * @return Maximal size */
    static int max_size()
    {
        return ali::meta::integer::max_value<int>::result / sizeof(T*);
    }

    /** @brief Get current size
      * @return Current size */
    int size() const
    {
        return _size;
    }

    /** @brief Get capacity
      * @return Capacity of the array that it can grow to without reallocating */
    int capacity() const
    {
        return _cap;
    }

    /** @brief Check if the array is empty
      * @retval true The array is empty
      * @retval false The array contains some elements */
    bool is_empty() const
    {
        return _size == 0;
    }

ALI_DEPRECATED(
    /** @brief Check if the array is empty
      * @retval true The array is empty
      * @retval false The array contains some elements
      * @deprecated Use @ref is_empty instead */
    bool empty() const
    {
        return is_empty();
    }
)

    /** @brief Reserve capacity
      * @param cap Capacity to reserve; @c 0 ? @p cap ? @ref max_size
      * @return Self
      *
      * If @p cap ? @ref capacity, nothing happens. */
    ptr_array& reserve(int cap)
    {
        ali_assert(0 <= cap && cap <= max_size());
        if (_cap >= cap)
            return *this;

        T** arr = ali::out_of_memory_if_null(new T*[cap], ALI_HERE);

        for (int i = 0; i < _size; ++i)
            arr[i] = _arr[i];

        using ali::swap;
        swap(_arr, arr);
        swap(_cap, cap);

        delete[] arr;
        return *this;
    }

    /** @brief Resize the array
      * @param size New array size; @c 0 ? @p size ? @ref max_size
      * @param val Constructor for newly created elements
      * @return Self */
    ptr_array& resize(int size, T* val = nullptr)
    {
        ali_assert(0 <= size && size <= max_size());

        if (size < _size)
        {
            _size = size;
        }
        else if (size > _size)
        {
            reserve(size);

            for (; _size < size; ++_size)
                _arr[_size] = val;
        }
        
        return *this;
    }

    /** @brief Erase elements from the array's back
      * @param n Number of elements to erase; @c 0 ? @p n ? @ref size
      * @return Self */
    ptr_array& erase_back(int n = 1)
    {
        ali_assert(0 <= n && n <= _size);

        _size -= n;
        return *this;
    }

    /** @brief Erase elements from the array
      * @param i Index of the first element to erase; @c 0 ? @p i ? @ref size
      * @param n Number of elements to erase; @c 0 ? @p n ? @ref size - @p i
      * @return Self */
    ptr_array& erase(int i, int n = 1)
    {
        ali_assert(0 <= i && i <= _size);
        ali_assert(0 <= n && n <= _size - i);

        for (i += n; i < _size; ++i)
            _arr[i - n] = _arr[i];

        return erase_back(n);
    }

    /** @brief Erase elements matching a predicate
      * @tparam predicate The predicate's type
      * @param p The predicate; the element is erased if @p p returns @c true
      * @return Number of erased elements */
    template <typename predicate>
    int erase_if(predicate p)
    {
        int iw = 0;

        for (int ir = 0; ir < _size; ++ir)
            if (!p(_arr[ir]))
                _arr[iw++] = _arr[ir];

        const int result = _size - iw;
        erase_back(result);
        return result;
    }

    /** @brief Erase an element from the array
      * @param p Iterator to the element to erase; @ref begin ? @p p < @ref end
      * @return Iterator to the next element; @ref end if the erased element
      *         was at the back */
    iterator erase(iterator p)
    {
        erase(p - begin());
        return p;
    }

    /** @brief Erase the array
      * @return Self */
    ptr_array& erase()
    {
        return erase_back(_size);
    }

    /** @brief Insert an element into the array
      * @param i Index on which to insert the new element; @c 0 ? @p i ? @ref size
      * @param val The element to insert
      * @return Self */
    ptr_array& insert(int i, T* val = nullptr)
    {
        if (i == _size)
            return push_back(val);

        ali_assert(0 <= i && i < _size);
        ali_assert(_size <= max_size() - 1);

        auto_reserve_free_capacity(1);
        ali_assert(_cap - _size >= 1);

        ++_size;

        for (int n = _size - 1; n > i; --n)
            _arr[n] = _arr[n - 1];

        _arr[i] = val;

        return *this;
    }

    /** @brief Insert an array into the array
      * @param i Index on which to insert the array; @c 0 ? @p i ? @ref size
      * @param vals The array to insert
      * @return Self */
    ptr_array& insert(int i, ptr_array const& vals)
    {
        if (i == _size)
            return push_back(vals);

        ali_assert(0 <= i && i < _size);
        ali_assert(_size <= max_size() - vals._size);

        auto_reserve_free_capacity(vals._size);

        for (int n = _size - 1; n >= i; --n)
            _arr[n + vals._size] = _arr[n];

        for (int n = 0; n < vals._size; ++n)
            _arr[i + n] = vals._arr[n];

        _size += vals._size;
        return *this;
    }

ALI_DEPRECATED(
    /** @brief Clear the array
      * @return Self
      * @deprecated Use @ref erase instead */
    ptr_array& clear()
    {
        return erase();
    }
)

    /** @brief Push an item at the back
      * @param val The item to push
      * @return Self */
    ptr_array& push_back(T* val)
    {
        ali_assert(_size <= max_size() - 1);

        auto_reserve_free_capacity(1);
        ali_assert(_cap - _size >= 1);

        _arr[_size] = val;
        ++_size;

        return *this;
    }

    /** @brief Push an array at the back
      * @param vals Array to push
      * @return Self */
    ptr_array& push_back(ptr_array const& vals)
    {
        ali_assert(_size <= max_size() - vals._size);

        auto_reserve_free_capacity(vals._size);
        ali_assert(_cap - _size >= vals._size);

        for (int i = 0; i < vals._size; ++i, ++_size)
            _arr[_size] = vals._arr[i];

        return *this;
    }

    /** @brief Pop an item from the back
      * @return The popped element */
    T* pop_back()
    {
        ali_assert(_size > 0);
        return _arr[--_size];
    }

    /** @brief Get constant element at the front
      * @return Constant element at the front */
    const_reference front() const
    {
        ali_assert(_size > 0);
        return _arr[0];
    }

    /** @brief Get element at the front
      * @return Element at the front */
    reference front()
    {
        ali_assert(_size > 0);
        return _arr[0];
    }

    /** @brief Get constant element at the back
      * @return Constant element at back */
    const_reference back() const
    {
        ali_assert(_size > 0);
        return _arr[_size - 1];
    }

    /** @brief Get element at the back
      * @return Element at back */
    reference back()
    {
        ali_assert(_size > 0);
        return _arr[_size - 1];
    }

    /** @brief Get constant iterator at the begin
      * @return Constant iterator at the begin */
    const_iterator begin() const
    {
        return _arr;
    }

    /** @brief Get iterator at the begin
      * @return Iterator at the begin */
    iterator begin()
    {
        return _arr;
    }

    /** @brief Get constant iterator at the end
      * @return Constant iterator at the end */
    const_iterator end() const
    {
        return _arr + _size;
    }

    /** @brief Get iterator at the end
      * @return Iterator at the end */
    iterator end()
    {
        return _arr + _size;
    }

    /** @brief Constant index operator
      * @param i Index of the requested constant element; @c 0 ? @p i < @ref size
      * @return The constant element at the specified index */
    const_reference operator [](int i) const
    {
        ali_assert(0 <= i && i < _size);
        return _arr[i];
    }

    /** @brief Index operator
      * @param i Index of the requested element; @c 0 ? @p i < @ref size
      * @return The element at the specified index */
    reference operator [](int i)
    {
        ali_assert(0 <= i && i < _size);
        return _arr[i];
    }

    /** @brief Get a constant element at an index
      * @param i Index of the requested constant element; @c 0 ? @p i < @ref size
      * @return The constant element at the specified index */
    const_reference at(int i) const
    {
        return operator [](i);
    }

    /** @brief Get an element at an index
      * @param i Index of the requested element; @c 0 ? @p i < @ref size
      * @return The element at the specified index */
    reference at(int i)
    {
        return operator [](i);
    }

    /** @brief Swap content with another array
      * @param b Another array to swap content with
      * @return Self */
    ptr_array& swap(ptr_array& b)
    {
        using ali::swap;
        swap(_size, b._size);
        swap(_cap, b._cap);
        swap(_arr, b._arr);
        return *this;
    }

    /** @brief Swap content of two arrays
      * @param a Array to swap content with @p b
      * @param b Array to swap content with @p a */
    friend void swap(ptr_array& a,
                     ptr_array& b)
    {
        a.swap(b);
    }

    /** @brief Swap two items
      * @param idx1 Index of the first item to swap; @c 0 ? @p idx1 < @ref size
      * @param idx2 Index of the second item to swap; @c 0 ? @p idx2 < @ref size
      * @return Self */
    ptr_array& swap_items(int idx1, int idx2)
    {
        ali_assert(0 <= idx1 && idx1 < _size);
        ali_assert(0 <= idx2 && idx2 < _size);

        using ali::swap;
        if (idx1 != idx2)
            swap(_arr[idx1], _arr[idx2]);

        return *this;
    }

    /** @brief Sort the array
      * @return Self */
    ptr_array& sort()
    {
        return sort(0, size());
    }

    /** @brief Sort a region of the array
      * @param from Index of the first element to sort; @c 0 ? @p from ? @ref size
      * @param n Number of elements to sort; @c 0 ? @p n ? @ref size - @p from
      * @return Self */
    ptr_array& sort(int from, int n)
    {
        if (n == 0)
            return *this;
        ali_assert(0 <= from && from < _size);
        ali_assert(0 < n && n <= _size - from);
        ali::generic_sort(_arr, _size, from, n, ali::deref_less());
        return *this;
    }

    /** @brief Sort the array using a comparator
      * @param cmp The comparator; returns:
      *            - < @c 0 if the first element is smaller than the second
      *            - @c 0 if both elements are equal
      *            - > @c 0 if the first element is greater than the second
      * @return Self */
    ptr_array& sort(int (*cmp)(T const*,
                               T const*))
    {
        return sort(cmp, 0, size());
    }

    /** @brief Sort the array using a comparator
      * @param cmp The comparator; returns:
      *            - < @c 0 if the first element is smaller than the second
      *            - @c 0 if both elements are equal
      *            - > @c 0 if the first element is greater than the second
      * @return Self */
    template <typename comparator>
    ptr_array& sort( comparator cmp )
    {
        return sort(cmp, 0, size());
    }

    /** @brief Sort a region of the array using a comparator
      * @param cmp The comparator; returns:
      *            - < @c 0 if the first element is smaller than the second
      *            - @c 0 if both elements are equal
      *            - > @c 0 if the first element is greater than the second
      * @param from Index of the first element to sort; @c 0 ? @p from ? @ref size
      * @param n Number of elements to sort; @c 0 ? @p n ? @ref size - @p from
      * @return Self */
    ptr_array& sort(int (*cmp)(T const*,
                               T const*),
                    int from,
                    int n)
    {
        if (n == 0)
            return *this;
        ali_assert(0 <= from && from < _size);
        ali_assert(0 < n && n <= _size - from);
        ali::generic_sort(
            _arr, _size, from, n,
            ali::less_from_compare_ptr<T>(cmp));
        return *this;
    }

    /** @brief Sort a region of the array using a comparator
      * @param cmp The comparator; returns:
      *            - < @c 0 if the first element is smaller than the second
      *            - @c 0 if both elements are equal
      *            - > @c 0 if the first element is greater than the second
      * @param from Index of the first element to sort; @c 0 ? @p from ? @ref size
      * @param n Number of elements to sort; @c 0 ? @p n ? @ref size - @p from
      * @return Self */
    template <typename comparator>
    ptr_array& sort( comparator cmp, int from, int n )
    {
        if (n == 0)
            return *this;
        ali_assert(0 <= from && from < _size);
        ali_assert(0 < n && n <= _size - from);
        ali::generic_sort(
            _arr, _size, from, n,
            ali::less_from_comparator<comparator>(cmp));
        return *this;
    }

    operator array_const_ref<T*>( void ) const
    {
        return array_const_ref<T*>(this->_arr, this->_size);
    }

    operator array_const_ref<T*>( void )
    {
        return array_const_ref<T*>(this->_arr, this->_size);
    }

    operator array_ref<T*>( void )
    {
        return array_ref<T*>(this->_arr, this->_size);
    }

    array_const_ref<T*> ref( void ) const
    {
        return *this;
    }

    array_ref<T*> mutable_ref( void )
    {
        return *this;
    }

private:    //  Methods
    /** @brief Automatically reserve capacity to maintain specified minimum
      * @param free_cap Minimal free capacity to maintain */
    void auto_reserve_free_capacity(int free_cap)
    {
        ali_assert(0 <= free_cap && free_cap <= max_size() - _cap);

        if (_cap - _size < free_cap)
            reserve(ali::maxi(4, ali::maxi(
                _cap <= max_size() - _cap / 2
                    ? _cap + _cap / 2
                    : max_size(),
                _cap + free_cap)));
    }

private:    //  Data members
    int   _size; ///< Used size
    int   _cap; ///< Allocated capacity
    T**   _arr; ///< Memory
};

#endif

/** @brief Dynamic pointer array with automatic deletion
  * @tparam T Type of the pointer stored in the array */
// ******************************************************************
template <typename T>
class auto_ptr_array
// ******************************************************************
{
public:
    /** @brief Pointer type stored in the array */
    typedef T*                      value_type;

ALI_DEPRECATED(
    /** @brief Size type */
    typedef int                     size_type;
)

    /** @brief Reference type */
    typedef ali::auto_ptr_lvalue<T> reference;

    /** @brief Constant reference type */
    typedef T* const&               const_reference;

    /** @brief Constant iterator type */
    typedef T* const*               const_iterator;


    /** @brief Iterator class */
    class iterator
        : public ali::iterator_base<ali::random_access_iterator_tag, T*>
    {
    public:
        /** @brief Default constructor */
        iterator()
            : _p(nullptr)
        {}

        /** @brief Constructor from a pointer
          * @param p The pointer */
        iterator(T** p)
            : _p(p)
        {}

        /** @brief Dereference operator
          * @return Value reference */
        reference operator *() const
        {
            return *_p;
        }

        /** @brief Prefix increment operator
          * @return Self */
        iterator& operator ++()
        {
            ++_p;
            return *this;
        }

        /** @brief Postfix increment operator
          * @return Original (before increment) iterator */
        iterator operator++(int)
        {
            iterator t(*this);
            ++*this;
            return t;
        }

        /** @brief Prefix decrement operator
          * @return Self */
        iterator& operator--( void )
        {
            --_p;
            return *this;
        }

        /** @brief Postfix decrement operator
          * @return Original (before decrement) iterator */
        iterator operator--( int )
        {
            iterator t(*this);
            --*this;
            return t;
        }

        /** @brief Addition-augmented assignment operator
          * @param d Value to add
          * @return Self */
        iterator& operator +=(int d)
        {
            _p += d;
            return *this;
        }

        /** @brief Addition operator
          * @param d Value to add
          * @return Iterator with the specified value added */
        iterator operator +(int d) const
        {
            return iterator(*this) += d;
        }

        /** @brief Subtraction-augmented assignment operator
          * @param d Value to subtract
          * @return Self */
        iterator& operator -=(int d)
        {
            _p -= d;
            return *this;
        }

        /** @brief Subtraction operator
          * @param d Value to subtract
          * @return Iterator with the specified value subtracted */
        iterator operator -(int d) const
        {
            return iterator(*this) -= d;
        }

        /** @brief Difference operator
          * @param b Iterator to compute difference from
          * @return The difference between this and the specified iterator */
        int operator -(iterator const& b) const
        {
            return _p - b._p;
        }

        /** @brief Comparison operator
          * @param b Iterator to compare with
          * @retval true This iterator is equal to the specified iterator
          * @retval false This iterator is not equal to the specified iterator */
        bool operator ==(iterator const& b) const
        {
            return _p == b._p;
        }

        /** @brief Comparison operator
          * @param b Iterator to compare with
          * @retval false This iterator is equal to the specified iterator
          * @retval true This iterator is not equal to the specified iterator */
        bool operator !=(iterator const& b) const
        {
            return !(*this == b);
        }

        /** @brief Operator for conversion to constant iterator
          * @return The constant iterator */
        operator const_iterator() const
        {
            return _p;
        }

    private:
        T** _p; ///< Value
    };


    /** @brief Default constructor */
    auto_ptr_array()
    {}

    /** @brief Constructor of a pre-filled array
      * @param size Size of the newly created array
      * @param val Initializer for each new element
      *
      * The array is preffiled with <code>nullptr</code>s */
    explicit auto_ptr_array(int size)
    :   _arr(size)
    {}

    /** @brief Destructor */
    ~auto_ptr_array()
    {
        erase();
    }

    /** @brief Move constructor
      * @param b Array to move  */
    auto_ptr_array( auto_ptr_array&& b )
    {
        _arr.swap(b._arr);
    }

    /** @brief Move assignment operator
      * @param b Array to move from
      * @return Self */
    auto_ptr_array& operator=( auto_ptr_array&& b )
    {
        if ( this != &b )
        {
            swap(b);
            auto_ptr_array{}.swap(b);
        }
        return *this;
    }

    /** @brief Get maximal size
      * @return Maximal size */
    static int max_size()
    {
        return ptr_array<T>::max_size();
    }

    /** @brief Get current size
      * @return Current size */
    int size() const
    {
        return _arr.size();
    }

    /** @brief Get capacity
     * @return Capacity of the array that it can grow to without reallocating */
    int capacity() const
    {
        return _arr.capacity();
    }

    /** @brief Check if the array is empty
      * @retval true The array is empty
      * @retval false The array contains some elements */
    bool is_empty() const
    {
        return _arr.is_empty();
    }

ALI_DEPRECATED(
    /** @brief Check if the array is empty
      * @retval true The array is empty
      * @retval false The array contains some elements
      * @deprecated Use @ref is_empty instead */
    bool empty() const
    {
        return is_empty();
    }
)

    /** @brief Reserve capacity
      * @param cap Capacity to reserve; @c 0 ? @p cap ? @ref max_size
      * @return Self
      *
      * If @p cap ? @ref capacity, nothing happens. */
    auto_ptr_array& reserve(int cap)
    {
        _arr.reserve(cap);
        return *this;
    }

    /** @brief Resize the array
      * @param size New array size; @c 0 ? @p size ? @ref max_size
      * @param val Constructor for newly created elements
      * @return Self */
    auto_ptr_array& resize(int size)
    {
        ali_assert(0 <= size && size <= max_size());
        for (int i = size; i < _arr.size(); ++i)
        {
            delete _arr[i];
            _arr[i] = nullptr;
        }
        _arr.resize(size, nullptr);
        return *this;
    }

    /** @brief Erase elements from the array's back
      * @param n Number of elements to erase; @c 0 ? @p n ? @ref size
      * @return Self */
    auto_ptr_array& erase_back(int n = 1)
    {
        ali_assert(0 <= n && n <= _arr.size());

        return erase(_arr.size() - n, n);
    }

    /** @brief Erase elements from the array
      * @param i Index of the first element to erase; @c 0 ? @p i ? @ref size
      * @param n Number of elements to erase; @c 0 ? @p n ? @ref size - @p i
      * @return Self */
    auto_ptr_array& erase(int i, int n = 1)
    {
        ali_assert(0 <= i && i <= _arr.size());
        ali_assert(0 <= n && n <= _arr.size() - i);

        for (int j = i + n; j > i;)
        {
            delete _arr[--j];
            _arr[j] = nullptr;
        }

        _arr.erase(i, n);

        return *this;
    }

    /** @brief Erase an element from the array
      * @param p Iterator to the element to erase; @ref begin ? @p p < @ref end
      * @return Iterator to the next element; @ref end if the erased element
      *         was at the back */
    iterator erase(iterator p)
    {
        erase(p - begin());
        return p;
    }

    /** @brief Erase elements matching a predicate
      * @tparam predicate The predicate's type
      * @param p The predicate; the element is erased if @p p returns @c true
      * @return Number of erased elements */
    template <typename predicate>
    int erase_if(predicate p)
    {
        int iw = 0;

        for (int ir = 0; ir < _arr.size(); ++ir)
            if (!p(_arr[ir]))
            {
                if (iw != ir)
                {
                    ali_assert(_arr[iw] == nullptr);
                    _arr[iw] = _arr[ir];
                    _arr[ir] = nullptr;
                }
                ++iw;
            }
            else
            {
                delete _arr[ir];
                _arr[ir] = nullptr;
            }

        const int result = _arr.size() - iw;
        _arr.erase_back(result);
        return result;
    }

    /** @brief Erase the array
      * @return Self */
    auto_ptr_array& erase()
    {
        return erase_back(_arr.size());
    }

    /** @brief Insert an element into the array
      * @param i Index on which to insert the new element; @c 0 ? @p i ? @ref size
      * @param val The element to insert
      * @return Self */
    auto_ptr_array& insert(int i,
                           ali::auto_ptr<T> val)
    {
        ali_assert(0 <= i && i <= _arr.size());

        _arr.insert(i, val.get());
        ali_assert(i != _arr.size() && _arr[i] == val.get());

        val.release();
        return *this;
    }

    /** @brief Insert an array into the array
      * @param i Index on which to insert the array; @c 0 ? @p i ? @ref size
      * @param vals The array to insert
      * @return Self */
    auto_ptr_array& insert(int i,
                           auto_ptr_array& vals)
    {
        ali_assert(0 <= i && i <= _arr.size());
        _arr.insert(i, vals._arr);

        ali_assert(vals.is_empty()
            || (i != _arr.size() && _arr[i] == vals.front()));

        vals._arr.erase();
        return *this;
    }

    /** @brief Reset an element
      * @param i Index of the element to reset; @c 0 ? @p i < @ref size
      * @param val The value to set
      * @return Self */
    auto_ptr_array& reset(int i,
                          T* val = nullptr)
    {
        return reset(i, ali::auto_ptr<T>(val));
    }

    /** @brief Reset an element
      * @param i Index of the element to reset; @c 0 ? @p i < @ref size
      * @param val The value to set
      * @return Self */
    auto_ptr_array& reset(int i,
                          ali::auto_ptr<T> val)
    {
        ali_assert(0 <= i && i < _arr.size());
        delete _arr[i];
        _arr[i] = val.release();
        return *this;
    }

    /** @brief Release an element
      * @param i Index of the element to release; @c 0 ? @p i < @ref size
      * @return The released element
      *
      * The released element's position is erased */
    ali::auto_ptr<T> release(int i)
    {
        ali_assert(0 <= i && i < _arr.size());
        ali::auto_ptr<T> temp(_arr[i]);
        _arr.erase(i);
        return temp;
    }

    /** @brief Release an element without erasing its place
      * @param i Index of the element to release; @c 0 ? @p i < @ref size
      * @return The released element
      *
      * The released element's position is reset to @c nullptr */
    ali::auto_ptr<T> release_no_erase(int i)
    {
        ali_assert(0 <= i && i < _arr.size());
        ali::auto_ptr<T> temp(_arr[i]);
        _arr[i] = nullptr;
        return temp;
    }

ALI_DEPRECATED(
    /** @brief Clear the array
      * @return Self
      * @deprecated Use @ref erase instread */
    auto_ptr_array& clear()
    {
        return erase();
    }
)

    /** @brief Push an item at the back
      * @param val The item to push
      * @return Self */
    auto_ptr_array& push_back(ali::auto_ptr<T> val)
    {
        _arr.push_back(val.get());

        ali_assert(!_arr.is_empty() && _arr.back() == val.get());

        val.release();
        return *this;
    }

    /** @brief Push an array at the back
      * @param vals Array to push; the array is cleared
      * @return Self */
    auto_ptr_array& push_back(auto_ptr_array& vals)
    {
        _arr.push_back(vals._arr);

        ali_assert(vals.is_empty()
            || (!_arr.is_empty() && _arr.back() == vals.back()));

        vals._arr.erase();
        return *this;
    }

    template <typename clone_function>
    auto_ptr_array& push_back_cloning(
        auto_ptr_array const& vals,
        clone_function clone )
    {
        if ( vals.is_empty() )
            return *this;

        reserve(size() + vals.size());

        for ( int i = 0; i < vals.size(); ++i )
            if ( vals[i] == nullptr )
                push_back(ali::auto_ptr<T>());
            else
                push_back(clone(*vals[i]));

        return *this;
    }

    auto_ptr_array& push_back_cloning(
        auto_ptr_array const& vals )
    {
        return push_back_cloning(vals,
            []( T const& b )
            {
                return ali::make_auto_ptr(
                    ali::out_of_memory_if_null(
                        new T(b),
                        ALI_HERE));
            });
    }

    /** @brief Pop an item from the back
      * @return The popped element */
    ali::auto_ptr<T> pop_back()
    {
        ali_assert(!_arr.is_empty());

        auto_ptr<T> p(_arr.back());
        _arr.erase_back();
        return p;
    }

    /** @brief Get constant element at the front
      * @return Constant element at the front */
    const_reference front() const
    {
        return _arr.front();
    }

    /** @brief Get element at the front
      * @return Element at the front */
    reference front()
    {
        return _arr.front();
    }

    /** @brief Get constant element at the back
      * @return Constant element at back */
    const_reference back() const
    {
        return _arr.back();
    }

    /** @brief Get element at the back
      * @return Element at back */
    reference back()
    {
        return _arr.back();
    }

    /** @brief Get constant iterator at the begin
      * @return Constant iterator at the begin */
    const_iterator begin() const
    {
        return _arr.begin();
    }

    /** @brief Get iterator at the begin
      * @return Iterator at the begin */
    iterator begin()
    {
        return _arr.begin();
    }

    /** @brief Get constant iterator at the end
      * @return Constant iterator at the end */
    const_iterator end() const
    {
        return _arr.end();
    }

    /** @brief Get iterator at the end
      * @return Iterator at the end */
    iterator end()
    {
        return _arr.end();
    }

    /** @brief Constant index operator
      * @param i Index of the requested constant element; @c 0 ? @p i < @ref size
      * @return The constant element at the specified index */
    const_reference operator [](int i) const
    {
        return _arr[i];
    }

    /** @brief Index operator
      * @param i Index of the requested element; @c 0 ? @p i < @ref size
      * @return The element at the specified index */
    reference operator [](int i)
    {
        return _arr[i];
    }

    /** @brief Get a constant element at an index
      * @param i Index of the requested constant element; @c 0 ? @p i < @ref size
      * @return The constant element at the specified index */
    const_reference at(int i) const
    {
        return operator [](i);
    }

    /** @brief Get an element at an index
      * @param i Index of the requested element; @c 0 ? @p i < @ref size
      * @return The element at the specified index */
    reference at(int i)
    {
        return operator [](i);
    }

    /** @brief Swap content with another array
      * @param b Another array to swap content with
      * @return Self */
    auto_ptr_array& swap(auto_ptr_array& b)
    {
        _arr.swap(b._arr);
        return *this;
    }

    /** @brief Swap content of two arrays
      * @param a Array to swap content with @p b
      * @param b Array to swap content with @p a */
    friend void swap(auto_ptr_array& a,
                     auto_ptr_array& b)
    {
        a.swap(b);
    }

    /** @brief Swap two items
      * @param idx1 Index of the first item to swap; @c 0 ? @p idx1 < @ref size
      * @param idx2 Index of the second item to swap; @c 0 ? @p idx2 < @ref size
      * @return Self */
    auto_ptr_array<T>& swap_items(int idx1,
                                  int idx2)
    {
        _arr.swap_items(idx1, idx2);
        return *this;
    }

    /** @brief Sort the array
      * @return Self */
    auto_ptr_array& sort()
    {
        return sort(0, size());
    }

    /** @brief Sort a region of the array
      * @param from Index of the first element to sort; @c 0 ? @p from ? @ref size
      * @param n Number of elements to sort; @c 0 ? @p n ? @ref size - @p from
      * @return Self */
    auto_ptr_array& sort(int from,
                         int n)
    {
        _arr.mutable_ref(from, n).sort();
        return *this;
    }

    /** @brief Sort the array using a comparator
      * @param cmp The comparator; returns:
      *            - < @c 0 if the first element is smaller than the second
      *            - @c 0 if both elements are equal
      *            - > @c 0 if the first element is greater than the second
      * @return Self */
    auto_ptr_array& sort(int (*cmp)(T const*,
                                    T const*))
    {
        _arr.mutable_ref().sort(cmp);
        return *this;
    }

    /** @brief Sort the array using a comparator
      * @param cmp The comparator; returns:
      *            - < @c 0 if the first element is smaller than the second
      *            - @c 0 if both elements are equal
      *            - > @c 0 if the first element is greater than the second
      * @return Self */
    template <typename comparator>
    auto_ptr_array& sort( comparator cmp )
    {
        _arr.mutable_ref().sort(cmp);
        return *this;
    }

    /** @brief Sort a region of the array using a comparator
      * @param cmp The comparator; returns:
      *            - < @c 0 if the first element is smaller than the second
      *            - @c 0 if both elements are equal
      *            - > @c 0 if the first element is greater than the second
      * @param from Index of the first element to sort; @c 0 ? @p from ? @ref size
      * @param n Number of elements to sort; @c 0 ? @p n ? @ref size - @p from
      * @return Self */
    auto_ptr_array& sort(int (*cmp)(T const*, T const*),
                         int from,
                         int n)
    {
        _arr.mutable_ref(from, n).sort(cmp);
        return *this;
    }

    /** @brief Sort a region of the array using a comparator
      * @param cmp The comparator; returns:
      *            - < @c 0 if the first element is smaller than the second
      *            - @c 0 if both elements are equal
      *            - > @c 0 if the first element is greater than the second
      * @param from Index of the first element to sort; @c 0 ? @p from ? @ref size
      * @param n Number of elements to sort; @c 0 ? @p n ? @ref size - @p from
      * @return Self */
    template <typename comparator>
    auto_ptr_array& sort( comparator cmp, int from, int n )
    {
        _arr.mutable_ref(from, n).sort(cmp);
        return *this;
    }

    operator array_const_ref<T*>( void ) const
    {
        return _arr;
    }

    operator array_const_ref<T*>( void )
    {
        return _arr;
    }

    array_const_ref<T*> ref( void ) const
    {
        return _arr;
    }

private:    //  Methods
    /** @brief Copy constructor
      * @param b Array to copy
      * @note This makes a deep copy, i.e. each element of the provided array is
      *       copy-constructed */
    auto_ptr_array( auto_ptr_array const& b );

    /** @brief Assignment operator
      * @param b Array to copy from
      * @return Self */
    auto_ptr_array& operator=( auto_ptr_array const& );

private:
    ali::ptr_array<T>   _arr; ///< Storage
};

#if 1

template <typename T>
using pod_array = array<T>;

#else

/** @brief Array of a POD type
  * @tparam T The POD type */
// ******************************************************************
template <typename T>
class pod_array
// ******************************************************************
{
public:
    /** @brief Type stored in the array */
    typedef T           value_type;

ALI_DEPRECATED(
    /** @brief Size type */
    typedef int         size_type;
)

    /** @brief Reference type */
    typedef T&          reference;

    /** @brief Constant reference type */
    typedef T const&    const_reference;

    /** @brief Iterator type */
    typedef T*          iterator;

    /** @brief Constant iterator type */
    typedef T const*    const_iterator;


    /** @brief Default constructor */
    pod_array()
        : _size(0)
        , _cap(0)
        , _arr(nullptr)
    {}

    /** @brief Constructor of a pre-filled array
      * @param size Size of the newly created array
      * @param val Initializer for each new element */
    explicit pod_array(int size,
                       T const& val = T())
        : _size(0)
        , _cap(0)
        , _arr(nullptr)
    {
        resize(size, val);
    }

    /** @brief Copy constructor
      * @param b Array to copy */
    pod_array(pod_array const& b)
        : _size(0)
        , _cap(0)
        , _arr(nullptr)
    {
        if (b._size == 0)
            return;

        reserve(b._size);
        ali_assert(b._size <= _cap);

        _size = b._size;
        for (int i = 0; i < _size; ++i)
            _arr[i] = b._arr[i];
    }

    /** @brief Destructor */
    ~pod_array()
    {
        delete[] _arr;
    }

    /** @brief Copy operator
      * @param b Array to copy from
      * @return Self */
    pod_array& operator =(pod_array b)
    {
        swap(b);
        return *this;
    }

    /** @brief Get maximal size
      * @return Maximal size */
    static int max_size()
    {
        return ali::meta::integer::max_value<int>::result / sizeof(T);
    }

    /** @brief Get current size
      * @return Current size */
    int size() const
    {
        return _size;
    }

    /** @brief Get capacity
      * @return Capacity of the array that it can grow to without reallocating */
    int capacity() const
    {
        return _cap;
    }

    /** @brief Check if the array is empty
      * @retval true The array is empty
      * @retval false The array contains some elements */
    bool is_empty() const
    {
        return _size == 0;
    }

ALI_DEPRECATED(
    /** @brief Check if the array is empty
      * @retval true The array is empty
      * @retval false The array contains some elements
      * @deprecated Use @ref empty instead */
    bool empty() const
    {
        return is_empty();
    }
)

    /** @brief Reserve capacity
      * @param cap Capacity to reserve; @c 0 ? @p cap ? @ref max_size
      * @return Self */
    pod_array& reserve(int cap)
    {
        ali_assert(0 <= cap && cap <= max_size());
        if (_cap >= cap)
            return *this;

        T* arr = ali::out_of_memory_if_null(new T[cap], ALI_HERE);

        for (int i = 0; i < _size; ++i)
            arr[i] = _arr[i];

        using ali::swap;
        swap(_arr, arr);
        swap(_cap, cap);

        delete[] arr;
        return *this;
    }

    /** @brief Resize the array
      * @param size New size
      * @param val Value for initializing new elements
      * @return Self */
    pod_array& resize(int size, T const& val = T())
    {
        ali_assert(0 <= size && size <= max_size());
        if (size > _size)
        {
            reserve(size);

            ali_assert(size <= _cap);

            for (int i = _size; i < size; ++i)
                _arr[i] = val;
        }
        _size = size;
        return *this;
    }

    /** @brief Erase elements from the array's back
      * @param n Number of elements to erase; @c 0 ? @p n ? @ref size
      * @return Self */
    pod_array& erase_back(int n = 1)
    {
        ali_assert(0 <= n && n <= _size);

        _size -= n;

        return *this;
    }

    /** @brief Erase elements from the array
      * @param i Index of the first element to erase; @c 0 ? @p i ? @ref size
      * @param n Number of elements to erase; @c 0 ? @p n ? @ref size - @p i
      * @return Self */
    pod_array& erase(int i, int n = 1)
    {
        ali_assert(0 <= i && i <= _size);
        ali_assert(0 <= n && n <= _size - i);

        for (i += n; i < _size; ++i)
            _arr[i - n] = _arr[i];

        return erase_back(n);
    }

    /** @brief Erase an element from the array
      * @param p Iterator to the element to erase; @ref begin ? @p p < @ref end
      * @return Iterator to the next element; @ref end if the erased element
      *         was at the back */
    iterator erase(iterator p)
    {
        erase(p - _arr);
        return p;
    }

    /** @brief Erase elements matching a predicate
      * @tparam predicate The predicate's type
      * @param p The predicate; the element is erased if @p p returns @c true
      * @return Number of erased elements */
    template <typename predicate>
    int erase_if(predicate p)
    {
        int iw = 0;

        for (int ir = 0; ir < _size; ++ir)
            if (!p(_arr[ir]))
                _arr[iw++] = _arr[ir];

        const int result = _size - iw;
        erase_back(result);
        return result;
    }

    /** @brief Erase the array
      * @return Self */
    pod_array& erase()
    {
        return erase_back(_size);
    }

    /** @brief Insert an element into the array
      * @param i Index on which to insert the new element; @c 0 ? @p i ? @ref size
      * @param val The element to insert
      * @return Self */
    pod_array& insert(int i, T const& val)
    {
        if (i == _size)
            return push_back(val);

        ali_assert(0 <= i && i < _size);
        ali_assert(_size < max_size());

        auto_reserve_free_capacity(1);

        ali_assert(_cap - _size >= 1);

        ++_size;

        for (int n = _size - 1; n > i; --n)
            _arr[n] = _arr[n - 1];

        _arr[i] = val;

        return *this;
    }

ALI_DEPRECATED(
    /** @brief Clear the array
      * @return Self
      * @deprecated Use @ref erase instead */
    pod_array& clear()
    {
        return erase();
    }
)

    /** @brief Push an item at the back
      * @param val The item to push
      * @return Self */
    pod_array& push_back(T const& val)
    {
        ali_assert(_size <= max_size() - 1);

        auto_reserve_free_capacity(1);
        ali_assert(_cap - _size >= 1);

        _arr[_size] = val;
        ++_size;

        return *this;
    }

    /** @brief Pop an item from the back
      * @return Self */
    pod_array& pop_back()
    {
        return erase_back();
    }

    /** @brief Get constant element at the front
      * @return Constant element at the front */
    const_reference front() const
    {
        ali_assert(!is_empty());
        return _arr[0];
    }

    /** @brief Get element at the front
      * @return Element at the front */
    reference front()
    {
        ali_assert(!is_empty());
        return _arr[0];
    }

    /** @brief Get constant element at the back
      * @return Constant element at the back */
    const_reference back() const
    {
        ali_assert(!is_empty());
        return _arr[_size - 1];
    }

    /** @brief Get element at the back
      * @return Element at the back */
    reference back()
    {
        ali_assert(!is_empty());
        return _arr[_size - 1];
    }

    /** @brief Get constant iterator at the begin
      * @return Constant iterator at the begin */
    const_iterator begin() const
    {
        return _arr;
    }

    /** @brief Get iterator at the begin
      * @return Iterator at the begin */
    iterator begin()
    {
        return _arr;
    }


    /** @brief Get constant iterator at the end
      * @return Constant iterator at the end */
    const_iterator end() const
    {
        return _arr + _size;
    }

    /** @brief Get iterator at the end
      * @return Iterator at the end */
    iterator end()
    {
        return _arr + _size;
    }

    /** @brief Constant index operator
      * @param i Index of the requested constant element; @c 0 ? @p i < @ref size
      * @return The constant element at the specified index */
    const_reference operator [](int i) const
    {
        ali_assert(0 <= i && i < _size);
        return _arr[i];
    }

    /** @brief Index operator
      * @param i Index of the requested element; @c 0 ? @p i < @ref size
      * @return The element at the specified index */
    reference operator [](int i)
    {
        ali_assert(0 <= i && i < _size);
        return _arr[i];
    }

    /** @brief Get a constant element at an index
      * @param i Index of the requested constant element; @c 0 ? @p i < @ref size
      * @return The constant element at the specified index */
    const_reference at(int i) const
    {
        return operator [](i);
    }

    /** @brief Get an element at an index
      * @param i Index of the requested element; @c 0 ? @p i < @ref size
      * @return The element at the specified index */
    reference at(int i)
    {
        return operator [](i);
    }

    /** @brief Swap content with another array
      * @param b Another array to swap content with
      * @return Self */
    pod_array& swap(pod_array& b)
    {
        using ali::swap;
        swap(_size, b._size);
        swap(_cap, b._cap);
        swap(_arr, b._arr);
        return *this;
    }

    /** @brief Swap content of two arrays
      * @param a Array to swap content with @p b
      * @param b Array to swap content with @p a */
    friend void swap(pod_array& a,
                     pod_array& b)
    {
        a.swap(b);
    }

    /** @brief Sort the array
      * @return Self */
    pod_array& sort()
    {
        return sort(0, size());
    }

    /** @brief Sort a region of the array
      * @param from Index of the first element to sort; @c 0 ? @p from ? @ref size
      * @param n Number of elements to sort; @c 0 ? @p n ? @ref size - @p from
      * @return Self */
    pod_array& sort(int from, int n)
    {
        if (n == 0)
            return *this;
        ali_assert(0 <= from && from < _size);
        ali_assert(0 < n && n <= _size - from);
        ali::sort(_arr, from, n);
        return *this;
    }

    /** @brief Sort the array using a comparator
      * @param cmp The comparator; returns:
      *            - < @c 0 if the first element is smaller than the second
      *            - @c 0 if both elements are equal
      *            - > @c 0 if the first element is greater than the second
      * @return Self */
    pod_array& sort(int (*cmp)(T const&,
                               T const&))
    {
        return sort(cmp, 0, size());
    }

    /** @brief Sort a region of the array using a comparator
      * @param cmp The comparator; returns:
      *            - < @c 0 if the first element is smaller than the second
      *            - @c 0 if both elements are equal
      *            - > @c 0 if the first element is greater than the second
      * @param from Index of the first element to sort; @c 0 ? @p from ? @ref size
      * @param n Number of elements to sort; @c 0 ? @p n ? @ref size - @p from
      * @return Self */
    pod_array& sort(int (*cmp)(T const&,
                               T const&),
                    int from,
                    int n)
    {
        if (n == 0)
            return *this;
        ali_assert(0 <= from && from < _size);
        ali_assert(0 < n && n <= _size - from);
        ali::generic_sort(_arr, size(), from, n, less_from_compare<T>(cmp));
        return *this;
    }

    /** @brief Sort the array using a method for element swapping
      * @param swp The method for element swapping
      * @return Self */
    pod_array& sort(void (*swp)(T&,
                                T&))
    {
        return sort(0, size(), swp);
    }

    /** @brief Sort a region of the array using a method for element swapping
      * @param from Index of the first element to sort; @c 0 ? @p from ? @ref size
      * @param n Number of elements to sort; @c 0 ? @p n ? @ref size - @p from
      * @param swp The method for element swapping
      * @return Self */
    pod_array& sort(int from,
                    int n,
                    void (*swp)(T&,T&))
    {
        if (n == 0)
            return *this;
        ali_assert(0 <= from && from < _size);
        ali_assert(0 < n && n <= _size - from);
        ali::generic_sort(_arr, size(), from, n, ali::less(), swp);
        return *this;
    }

    /** @brief Sort the array using a comparator and a method for element swapping
      * @param cmp The comparator; returns:
      *            - < @c 0 if the first element is smaller than the second
      *            - @c 0 if both elements are equal
      *            - > @c 0 if the first element is greater than the second
      * @param swp The method for element swapping
      * @return Self */
    pod_array& sort(int (*cmp)(T const&,
                               T const&),
                    void (*swp)(T&,
                                T&))
    {
        return sort(cmp, 0, size(), swp);
    }

    /** @brief Sort a region of the array using a comparator and a method for element swapping
     * @param cmp The comparator; returns:
     *            - < @c 0 if the first element is smaller than the second
     *            - @c 0 if both elements are equal
     *            - > @c 0 if the first element is greater than the second
     * @param from Index of the first element to sort; @c 0 ? @p from ? @ref size
     * @param n Number of elements to sort; @c 0 ? @p n ? @ref size - @p from
     * @param swp The method for element swapping
     * @return Self */
    pod_array& sort(int (*cmp)(T const&,
                               T const&),
                    int from,
                    int n,
                    void (*swp)(T&,
                                T&))
    {
        if (n == 0)
            return *this;
        ali_assert(0 <= from && from < _size);
        ali_assert(0 < n && n <= _size - from);
        ali::generic_sort(_arr, size(), from, n, less_from_compare<T>(cmp), swp);
        return *this;
    }

private:    //  Methods
    /** @brief Automatically reserve capacity to maintain specified minimum
      * @param free_cap Minimal free capacity to maintain */
    void auto_reserve_free_capacity(int free_cap)
    {
        ali_assert(0 <= free_cap && free_cap <= max_size() - _cap);

        if (_cap - _size < free_cap)
            reserve(ali::maxi(4, ali::maxi(
                _cap <= max_size() - _cap / 2
                    ? _cap + _cap / 2
                    : max_size(),
                _cap + free_cap)));
    }

private:    //  Data members
    int   _size; ///< Used size
    int   _cap; ///< Allocated capacity
    T*    _arr; ///< Memory
};

#endif

#if 0

/** @brief Find element in an array
  * @tparam T Type stored in the array
  * @tparam U Type of the item to compare
  * @param arr Array to search in
  * @param u Item that should match the searched element
  * @return Index of the first matched element
  * @retval @p arr.size if not found */
// ******************************************************************
template <class T,
          class U>
int find(ali::array<T> const& arr,
         U const& u)
// ******************************************************************
{
    int i = 0;
    for (; i < arr.size(); ++i)
        if (arr[i] == u)
            break;
    return i;
}

/** @brief Find element in an array using that element's member value
  * @tparam S Type stored in the array
  * @tparam T Element's member containing class
  * @tparam U Element's member type
  * @tparam V Type of the item to compare
  * @param arr Array to search in
  * @param member Member to compare
  * @param v Item that should match the @p member of the searched element
  * @return Index of the first matched element
  * @retval @p arr.size if not found */
// ******************************************************************
template <class S,
          class T,
          class U,
          class V>
int find(ali::array<S> const& arr,
         U T::*member,
         V const& v)
// ******************************************************************
{
    int i = 0;
    for (; i < arr.size(); ++i)
        if (arr[i].*member == v)
            break;
    return i;
}

/** @brief Find element in a pointer array (by pointer)
  * @tparam T Type stored in the pointer array
  * @param arr Pointer array to search in
  * @param t Pointer that should match the searched element
  * @return Index of the first matched element
  * @retval @p arr.size if not found */
// ******************************************************************
template <class T>
int find_ptr(ali::ptr_array<T> const& arr,
             T const* t)
// ******************************************************************
{
    int i = 0;
    for (; i < arr.size(); ++i)
        if (arr[i] == t)
            break;
    return i;
}

/** @brief Find element in a pointer array (by value)
  * @tparam T Type stored in the pointer array
  * @tparam U Type of the item to compare
  * @param arr Pointer array to search in
  * @param u Item that should match the searched element
  * @return Index of the first matched element
  * @retval @p arr.size if not found */
// ******************************************************************
template <class T,
          class U>
int find(ali::ptr_array<T> const& arr,
         U const& u)
// ******************************************************************
{
    int i = 0;
    for (; i < arr.size(); ++i)
        if (*arr[i] == u)
            break;
    return i;
}

/** @brief Find element in a pointer array using that element's member value
  * @tparam S Type stored in the pointer array
  * @tparam T Element's member containing class
  * @tparam U Element's member type
  * @tparam V Type of the item to compare
  * @param arr Pointer array to search in
  * @param member Member to compare
  * @param v Item that should match the @p member of the searched element
  * @return Index of the first matched element
  * @retval @p arr.size if not found */
// ******************************************************************
template <class S,
          class T,
          class U,
          class V>
int find(ali::ptr_array<S> const& arr,
         U T::*member,
         V const& v)
// ******************************************************************
{
    int i = 0;
    for (; i < arr.size(); ++i)
        if (arr[i]->*member == v)
            break;
    return i;
}

/** @brief Find element in a pointer array (by pointer)
  * @tparam T Type stored in the pointer array
  * @param arr Pointer array to search in
  * @param t Pointer that should match the searched element
  * @return Index of the first matched element
  * @retval @p arr.size if not found */
// ******************************************************************
template <class T>
int find_ptr(ali::auto_ptr_array<T> const& arr,
             T const* t)
// ******************************************************************
{
    int i = 0;
    for (; i < arr.size(); ++i)
        if (arr[i] == t)
            break;
    return i;
}

/** @brief Find element in a pointer array (by value)
  * @tparam T Type stored in the pointer array
  * @tparam U Type of the item to compare
  * @param arr Pointer array to search in
  * @param u Item that should match the searched element
  * @return Index of the first matched element
  * @retval @p arr.size if not found */
// ******************************************************************
template <class T,
          class U>
int find(ali::auto_ptr_array<T> const& arr,
         U const& u)
// ******************************************************************
{
    int i = 0;
    for (; i < arr.size(); ++i)
        if (*arr[i] == u)
            break;
    return i;
}

/** @brief Find element in a pointer array using that element's member value
  * @tparam S Type stored in the pointer array
  * @tparam T Element's member containing class
  * @tparam U Element's member type
  * @tparam V Type of the item to compare
  * @param arr Pointer array to search in
  * @param member Member to compare
  * @param v Item that should match the @p member of the searched element
  * @return Index of the first matched element
  * @retval @p arr.size if not found */
// ******************************************************************
template <class S,
          class T,
          class U,
          class V>
int find(ali::auto_ptr_array<S> const& arr,
         U T::*member,
         V const& v )
// ******************************************************************
{
    int i = 0;
    for (; i < arr.size(); ++i)
        if (arr[i]->*member == v)
            break;
    return i;
}

#endif

template <typename T>
struct format_t;

/** @brief Formatter for @ref ali::array
  * @tparam T Type stored in the array */
template <typename T>
struct format_t<ali::array<T> >
{
    bool _brackets; ///< Whether to print brackets
    format_t<T> _values; ///< Formatter for the array's values

    /** @brief Constructor
      * @param values Formatter for the array's values */
    format_t(const format_t<T> &values = format_t<T>())
        : _brackets(true)
        , _values(values)
    {}

    /** @brief Set whether to print brackets
      * @param brackets Whether to print brackets
      * @return Self */
    format_t& brackets(bool brackets)
    {
        this->_brackets = brackets;
        return *this;
    }

    /** @brief Set formatter for the array's values
      * @param value Formatter for the array's values
      * @return Self */
    format_t& values(const format_t<T> &values)
    {
        this->_values = values;
        return *this;
    }

    /** @brief Helper structure */
    struct helper
    {
        /** @brief Constructor
          * @param array The array to format
          * @param flags The formatting flags */
        helper(const ali::array<T> &array,
               const format_t &flags)
            : array(array)
            , flags(flags)
        {}

        const ali::array<T> &array; ///< The array to format
        const format_t flags; ///< The formatting flags

        /** @brief Format to @c ali::string
          * @return The array formatted to @ref ali::string */
        operator ali::string() const
        {
            ali::string text;
            if (this->flags._brackets) {
                text += '[';
            }
            ali::str::comma comma(",");
            for (typename ali::array<T>::const_iterator it = this->array.begin(),
                    end = this->array.end(); it != end; ++it) {
                text.append(comma());
                text.append(ali::string(this->flags._values(*it)));
            }
            if (this->flags._brackets) {
                text += ']';
            }
            return text;
        }

        /** @brief Format to @c ali::wstring
          * @return The array formatted to @ref ali::wstring */
        operator ali::wstring() const
        {
            ali::wstring text;
            if (this->flags._brackets) {
                text += L'[';
            }
            ali::wstr::comma comma(L",");
            for (typename ali::array<T>::const_iterator it = this->array.begin(),
                    end = this->array.end(); it != end; ++it) {
                text.append(comma());
                text.append(ali::wstring(this->flags._values(*it)));
            }
            if (this->flags._brackets) {
                text += L']';
            }
            return text;
        }
    };

    /** @brief Functor operator
      * @param array Array to format
      * @return The helper structure */
    helper operator ()(const ali::array<T> &array) const
    {
        return helper(array, *this);
    }
};

}   //  namespace ali

#else   //  !ALI_STREAMLINED_ARRAYS

#include "ali_array1.h"

#endif  //  ALI_STREAMLINED_ARRAYS
