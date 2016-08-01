/*
 *  ali_callback.h
 *  ali Library
 *
 *  Copyright (c) 2010 - 2014 Acrobits s.r.o. All rights reserved.
 */

#pragma once

#include "ali_callback_forward.h"
#include "ali_features.h"

#if !defined(ALI_FEATURE_RVALUE_REF) || !defined(ALI_FEATURE_VARIADIC_TEMPLATES)
#   include "ali_callback_legacy.h"
#else

#include "ali_attribute.h"
#include "ali_auto_ptr.h"
#include "ali_noncopyable.h"
#include "ali_nullptr.h"
#include "ali_utility.h"


namespace ali
{

// ******************************************************************
template <typename return_type,
          typename... param_types>
class callback<return_type(param_types...)>
// ******************************************************************
{
protected:    // Struct
    // ******************************************************************
    class basic_fun
        : public ali::noncopyable
    // ******************************************************************
    {
    public:
        virtual return_type call( param_types... params ) const = 0;
        virtual basic_fun* clone( void* buf, int size ) const = 0;
        virtual basic_fun* move( void* buf, int size ) = 0;
        virtual void destroy( void* buf, int size ) = 0;

    protected:  //  Methods
        basic_fun( void ) {}
        virtual ~basic_fun( void ) {}
    };

    // ******************************************************************
    template <typename object_type,
              typename member_type>
    class member_fun
        : public basic_fun
    // ******************************************************************
    {
    public:
        // ******************************************************************
        static basic_fun* create( void* buf,
                                  int size,
                                  object_type* obj,
                                  member_type fun )
        // ******************************************************************
        {
            return int(sizeof(member_fun)) > size
                ? new member_fun(obj, fun)
                : new (buf) member_fun(obj, fun);
        }

        // ******************************************************************
        virtual return_type call( param_types... params ) const ALI_ATTRIBUTE_OVERRIDE
        // ******************************************************************
        {
            return (_obj->*_fun)(ali::forward<param_types>(params)...);
        }

        // ******************************************************************
        virtual basic_fun* clone( void* buf,
                                  int size ) const ALI_ATTRIBUTE_OVERRIDE
        // ******************************************************************
        {
            return create(buf, size, _obj, _fun);
        }

        // ******************************************************************
        virtual basic_fun* move( void* buf,
                                 int size ) ALI_ATTRIBUTE_OVERRIDE
        // ******************************************************************
        {
            if (int(sizeof(member_fun)) > size)
                return this;

            basic_fun *ret = new (buf) member_fun(_obj, _fun);
            this->~member_fun();
            return ret;
        }

        // ******************************************************************
        virtual void destroy( void* buf, int size ) ALI_ATTRIBUTE_OVERRIDE
        //  For some reason, xcode 4.6.3 complains about the override keyword
        //  (just here... no idea why).
        //	Xcode 5.1.1 doesn't complain.
        // ******************************************************************
        {
            ali_assert((int(sizeof(member_fun)) > size) == (this != buf));

            if ( int(sizeof(member_fun)) > size )
                delete this;
            else
                this->~member_fun();
        }

    protected:    //  Methods
        // ******************************************************************
        member_fun( object_type* obj,
                    member_type fun )
        // ******************************************************************
            : _obj(obj)
            , _fun(fun)
        {}

        virtual ~member_fun( void ) {}

    protected:    //  Data members
        object_type*    _obj;
        member_type     _fun;
    };

    // ******************************************************************
    template <class function_type>
    class other_fun
        : public basic_fun
    // ******************************************************************
    {
    public:
        // ******************************************************************
        static basic_fun* create( void* buf,
                                  int size,
                                  function_type fun )
        // ******************************************************************
        {
            return int(sizeof(other_fun)) > size
                ? new other_fun(ali::move(fun))
                : new (buf) other_fun(ali::move(fun));
        }

        // ******************************************************************
        virtual return_type call( param_types... params ) const ALI_ATTRIBUTE_OVERRIDE
        // ******************************************************************
        {
            return _fun(ali::forward<param_types>(params)...);
        }

        // ******************************************************************
        virtual basic_fun* clone( void* buf,
                                  int size ) const ALI_ATTRIBUTE_OVERRIDE
        // ******************************************************************
        {
            return create(buf, size, _fun);
        }

        // ******************************************************************
        virtual basic_fun* move( void* buf,
                                 int size ) ALI_ATTRIBUTE_OVERRIDE
        // ******************************************************************
        {
            if (int(sizeof(other_fun)) > size)
                return this;

            basic_fun *ret = new (buf) other_fun(ali::move(_fun));
            this->~other_fun();
            return ret;
        }

        // ******************************************************************
        virtual void destroy( void* buf, int size ) ALI_ATTRIBUTE_OVERRIDE
        //  For some reason, xcode 4.6.3 complains about the override keyword
        //  (just here... no idea why).
        //	Xcode 5.1.1 doesn't complain.
        // ******************************************************************
        {
            ali_assert((int(sizeof(other_fun)) > size) == (this != buf));

            if ( int(sizeof(other_fun)) > size )
                delete this;
            else
                this->~other_fun();
        }

    protected:    //  Methods
        // ******************************************************************
        other_fun( function_type fun )
        // ******************************************************************
            : _fun(ali::move(fun))
        {}

        virtual ~other_fun( void ) {}

    protected:    //  Data members
        function_type   _fun;
    };

public:     // Methods
    // ******************************************************************
    callback( ali::nullptr_type const& = nullptr )
    // ******************************************************************
        : _fun(nullptr)
    {}

    // ******************************************************************
    template <typename function_type>
    callback( function_type fun )
    // ******************************************************************
        : _fun(nullptr)
    {
        this->assign(fun);
    }

    // ******************************************************************
    template <typename object_type,
              typename member_type>
    callback( object_type* obj,
              member_type fun )
    // ******************************************************************
        : _fun(nullptr)
    {
        this->assign(obj, fun);
    }

    // ******************************************************************
    callback( callback const& b )
    // ******************************************************************
        : _fun(nullptr)
    {
        this->operator=(b);
    }

    // ******************************************************************
    callback( callback && b )
    // ******************************************************************
        : _fun(nullptr)
    {
        this->operator=(ali::move(b));
    }

    // ******************************************************************
    template <typename other_return_type,
              typename... other_param_types>
    callback( callback<other_return_type(other_param_types...)> const& b )
    // ******************************************************************
        : _fun(nullptr)
    {
        static_assert(sizeof...(param_types) == sizeof...(other_param_types),
                      "ali::callback can be created from another ali::callback only when both have the same number of arguments");
        this->assign(b);
    }

    // ******************************************************************
    ~callback( void )
    // ******************************************************************
    {
        this->reset();
    }

    // ******************************************************************
    callback& operator=( callback const& b )
    // ******************************************************************
    {
        if ( this->_fun != b._fun )
        {
            this->reset();

            if ( b._fun != nullptr )
                this->_fun = b._fun->clone(this->_buf, sizeof(this->_buf));
        }

        return *this;
    }

    // ******************************************************************
    callback& operator=( callback && b )
    // ******************************************************************
    {
        if ( this->_fun != b._fun )
        {
            this->reset();

            if ( b._fun != nullptr )
            {
                this->_fun = b._fun->move(this->_buf, sizeof(this->_buf));
                // Do not reset, _fun was already moved or destroyed
                b._fun = nullptr;
            }
        }

        return *this;
    }

    // ******************************************************************
    template <typename function_type>
    callback& assign( function_type const& fun )
    // ******************************************************************
    {
        this->reset();

        this->_fun = other_fun<function_type>::create(this->_buf,
                                                      sizeof(this->_buf),
                                                      fun);

        return *this;
    }

    // ******************************************************************
    template <typename object_type,
              typename member_type>
    callback& assign( object_type* obj,
                      member_type fun )
    // ******************************************************************
    {
        ali_assert(obj != nullptr);

        this->reset();

        this->_fun = member_fun<object_type, member_type>::create(this->_buf,
                                                                  sizeof(this->_buf),
                                                                  obj, fun);

        return *this;
    }

    // ******************************************************************
    callback& reset( void )
    // ******************************************************************
    {
        if ( this->_fun != nullptr )
        {
            this->_fun->destroy(this->_buf, sizeof(this->_buf));
            this->_fun = nullptr;
        }
        return *this;
    }

    // ******************************************************************
    return_type operator()( param_types... params ) const
    // ******************************************************************
    {
        ali_assert(this->_fun != nullptr);
        return this->_fun->call(ali::forward<param_types>(params)...);
    }

    // ******************************************************************
    bool is_null( void ) const
    // ******************************************************************
    {
        return this->_fun == nullptr;
    }

    // ******************************************************************
    operator safebool( void ) const
    // ******************************************************************
    {
        return safeboolize(!is_null());
    }

    // ******************************************************************
    bool operator!( void ) const
    // ******************************************************************
    {
        return is_null();
    }

protected:    // Data members
        //  Reserve inplace space for a member function plus
        //  'this' pointer and an additional pointer parameter.
    static int const    _inplace_size = 4 * sizeof(void*);

    double              _buf[1 + (_inplace_size - 1) / sizeof(double)];
        //  Using double to ensure conservative buffer alignment.
    basic_fun*          _fun;
};

}

#endif
