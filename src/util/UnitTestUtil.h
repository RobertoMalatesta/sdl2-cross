#ifndef UTIL_UNITTESTUTIL_H
#define UTIL_UNITTESTUTIL_H

#ifdef CXXTEST_RUNNING
#define FRIENDLY_UNIT_TEST(CLASS) friend class Test ## CLASS;
#else
#define FRIENDLY_UNIT_TEST(CLASS)
#endif

#endif  // UTIL_UNITTESTUTIL_H
