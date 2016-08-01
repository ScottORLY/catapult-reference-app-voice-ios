//
//  ali_simd_detect.h
//  asmtest
//
//  Created by Jiri Kral on 21/12/14.
//  Copyright (c) 2014 Jiri Kral. All rights reserved.
//

#pragma once

#define ALI_SIMD_GENERIC    1
#define ALI_SIMD_ARMV7      2
#define ALI_SIMD_ARM64      3

// defines ALI_SIMD macro to one of the values above

#if __ARM_NEON__
#   if __arm64__
#       define ALI_SIMD ALI_SIMD_ARM64
#   else
#       define ALI_SIMD ALI_SIMD_ARMV7
#   endif
#else
#   define ALI_SIMD ALI_SIMD_GENERIC
#endif
