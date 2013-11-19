#ifndef TOOLS_CPP11_DEFINES_H
#define TOOLS_CPP11_DEFINES_H

// gcc supports constexpr since 4.6
#if defined(__GNUG__) && !defined(__clang__)
    #if (__GNUC__ >= 4 && __GNUC_MINOR__ >= 6)
        #define CONSTEXPR constexpr
    #else
        #define CONSTEXPR
    #endif
#endif

// clang supports constexpr since 3.1
#if defined(__clang__)
    #if (__clang_major__ >= 3 && __clang_minor__ >= 1)
        #define CONSTEXPR constexpr
    #else
        #define CONSTEXPR
    #endif
#endif

// visual c++ 2013 still no support for constexpr
#ifdef _MSC_VER
	#if (_MSC_FULL_VER <= 180021114 )	 // Nov 2013 CTP partially support for constexpr, but fails with internal error
		#define CONSTEXPR
	#else
		#define CONSTEXPR constexpr
	#endif
#endif

#ifndef CONSTEXPR
    #define CONSTEXPR
#endif

#endif  // TOOLS_CPP11_DEFINES_H
