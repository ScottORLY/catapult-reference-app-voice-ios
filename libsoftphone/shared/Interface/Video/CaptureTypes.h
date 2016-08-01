//
//  CaptureTypes.h
//  libsoftphone
//
//  Created by Jiri Kral on 4/20/12.
//  Copyright (c) 2012 Acrobits. All rights reserved.
//

#pragma once

#include "ali_string.h"
#include "ali_geometry.h"

namespace Video
{
    namespace Capture
    {
        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
        struct CameraPosition
        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
        {
            //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
            enum Type
            //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
            {
                Unknown,
                Front,
                Back,
                Virtual
            };
        };

        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
        struct Rotation
        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
        {
            enum Type
            {
                Unknown,
                Rotate_0,
                Rotate_90,
                Rotate_180,
                Rotate_270
            };

            //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
            static int toDegrees(Rotation::Type rot)
            //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
            {
                switch(rot)
                {
                    case Rotate_0:
                        return 0;

                    case Rotate_90:
                        return 90;

                    case Rotate_180:
                        return 180;

                    case Rotate_270:
                        return 270;
                    case Unknown:
                        break;
                }
                ali_assert(false);
                return -1;
            }

            //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
            static Type fromDegrees(int degrees)
            //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
            {
                switch (degrees)
                {
                    case 270:
                        return Rotate_270;

                    case 180:
                        return Rotate_180;

                    case 90:
                        return Rotate_90;

                    case 0:
                        return Rotate_0;

                    default:
                        ali_assert(false);
                        return Unknown;
                }
            }
        };

        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
        struct CameraInfo
        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
        {
            CameraInfo()
                :position(CameraPosition::Unknown)
                ,hasFlash(false)
                ,hasTorch(false)
                ,orientation(Rotation::Rotate_0)
            {}

            ali::string				id;
            ali::string				name;

            CameraPosition::Type	position;
            bool					hasFlash;
            bool					hasTorch;
            Rotation::Type          orientation;

            bool operator ==(CameraInfo const& other) const
            {
                return id == other.id;
            }

            bool operator !=(CameraInfo const& other) const
            {
                return id != other.id;
            }

            bool operator <(CameraInfo const& other) const
            {
                return id < other.id;
            }
        };

        // the transform defines how to get the final rectangle from the native video
        // format. First scale the native rect to mScaledSize, then crop mCropRectangle out of it,
        // finally rotate  by mRotation
        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
        struct Transform
        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
        {
            Transform()
            :rotation(Rotation::Unknown){}
            
            ali::size<float>    nativeResolution;
            ali::size<float>    scaledSize;
            ali::rect<float>    cropRectangle;
            Rotation::Type      rotation;
        };
    }
}


namespace ali
{
    template <class T>
    struct format_t;

    template <>
    struct format_t<Video::Capture::Rotation::Type>
    {
        int operator ()(const Video::Capture::Rotation::Type &type)
        {
            if (type == Video::Capture::Rotation::Unknown)
                return -1;
            return Video::Capture::Rotation::toDegrees(type);
        }
    };
}
