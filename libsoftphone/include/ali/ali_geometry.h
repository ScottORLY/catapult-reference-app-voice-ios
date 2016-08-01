//
//  ali_geometry.h
//
//  Created by Jiri Kral on 12/11/11.
//  Copyright (c) 2011 Acrobits. All rights reserved.
//

#pragma once

#include "ali_utility.h"

namespace ali
{
    // ******************************************************************
    template<class T>
    struct point
    // ******************************************************************
    {
        point()
        :_x(0),_y(0){}

        point(T x, T y)
        :_x(x),_y(y){}

        template <class TT>
        explicit point(const point<TT> &other)
            :_x(other.x())
            ,_y(other.y())
        {}

        point(const point &other)
            :_x(other._x)
            ,_y(other._y)
        {}

        T x() const {return _x;}
        T y() const {return _y;}

        point transpose() const
        {
            return point(x(), y());
        }

    private:
        T _x;
        T _y;
    };

    // ******************************************************************
    template<class T>
    struct size
    // ******************************************************************
    {
        size()
        :_w(0),_h(0){}

        size(T w, T h)
        :_w(w),_h(h){}

        template <class TT>
        explicit size(const size<TT> &other)
            :_w(other.w())
            ,_h(other.h())
        {}

        size(const size &other)
                :_w(other._w)
                ,_h(other._h)
        {}

        T w() const {return _w;}
        T h() const {return _h;}

        size transpose() const
        {
            return size(h(), w());
        }
        
        void swap(size & b)
        {
            using ali::swap;

            swap(_w,b._w);
            swap(_h,b._h);
        }
        
        friend void swap(size & a, size & b)
        {
            a.swap(b);
        }

    private:
        T _w;
        T _h;
    };

    // ******************************************************************
    template<class T>
    struct rect
    // ******************************************************************
    {
    public:
        rect(){}

        rect(T x, T y, T w, T h)
        :_origin(x,y),_size(w,h){}

        rect(point<T> const& origin, ali::size<T> const& sz)
        :_origin(origin),_size(sz){}

        rect(point<T> const& p1, point<T> const& p2)
        :_origin(p1),_size(p2.x() - p1.x(), p2.y() - p1.y()){}

        template <class TT>
        explicit rect(const rect<TT> &other)
            :_origin(point<T>(other.origin()))
            ,_size(ali::size<T>(other.size()))
        {}

        rect(const rect &other)
            :_origin(other._origin)
            ,_size(other._size)
        {}

        point<T> const& origin() const {return _origin;}

        ali::size<T> const& size() const {return _size;}

        rect transpose() const
        {
            return rect(origin().transpose(), size().transpose());
        }


    private:
        point<T> _origin;
        ali::size<T> _size;
    };
}

