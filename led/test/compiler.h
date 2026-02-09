/*
 * @Author: 轩
 * @Date: 2026-02-09 23:01:58
 * @LastEditTime: 2026-02-09 23:27:09
 * @FilePath: \led\led\test\compiler.h
 */
// compiler.h (或任何全局头文件)
// compiler.h
#ifndef STATIC_ASSERT
#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 201112L)
    #define STATIC_ASSERT(cond, msg) _Static_assert(cond, #msg)
#else
    // 将行号加入类型名，帮助定位
    #define STATIC_ASSERT_JOIN(a, b) a##b
    #define STATIC_ASSERT(cond, msg) \
        typedef char STATIC_ASSERT_JOIN(static_assert_line_, __LINE__) \
            [(cond) ? 1 : -1]
#endif
#endif