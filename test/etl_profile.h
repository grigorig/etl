///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2017 jwellbelove

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files(the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions :

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
******************************************************************************/

#ifndef ETL_PROFILE_H_INCLUDED
#define ETL_PROFILE_H_INCLUDED

#define ETL_THROW_EXCEPTIONS
#define ETL_VERBOSE_ERRORS
#define ETL_CHECK_PUSH_POP
#define ETL_ISTRING_REPAIR_ENABLE
#define ETL_IVECTOR_REPAIR_ENABLE
#define ETL_IDEQUE_REPAIR_ENABLE
#define ETL_IN_UNIT_TEST
#define ETL_DEBUG_COUNT

#define ETL_MESSAGE_TIMER_USE_ATOMIC_LOCK
#define ETL_CALLBACK_TIMER_USE_ATOMIC_LOCK

#define ETL_POLYMORPHIC_RANDOM

#define ETL_POLYMORPHIC_BITSET
#define ETL_POLYMORPHIC_DEQUE
#define ETL_POLYMORPHIC_FLAT_MAP
#define ETL_POLYMORPHIC_FLAT_MULTIMAP
#define ETL_POLYMORPHIC_FLAT_SET
#define ETL_POLYMORPHIC_FLAT_MULTISET
#define ETL_POLYMORPHIC_FORWARD_LIST
#define ETL_POLYMORPHIC_LIST
#define ETL_POLYMORPHIC_MAP
#define ETL_POLYMORPHIC_MULTIMAP
#define ETL_POLYMORPHIC_SET
#define ETL_POLYMORPHIC_MULTISET
#define ETL_POLYMORPHIC_QUEUE
#define ETL_POLYMORPHIC_STACK
#define ETL_POLYMORPHIC_REFERENCE_FLAT_MAP
#define ETL_POLYMORPHIC_REFERENCE_FLAT_MULTIMAP
#define ETL_POLYMORPHIC_REFERENCE_FLAT_SET
#define ETL_POLYMORPHIC_REFERENCE_FLAT_MULTISET
#define ETL_POLYMORPHIC_UNORDERED_MAP
#define ETL_POLYMORPHIC_UNORDERED_MULTIMAP
#define ETL_POLYMORPHIC_UNORDERED_SET
#define ETL_POLYMORPHIC_UNORDERED_MULTISET
#define ETL_POLYMORPHIC_STRINGS
#define ETL_POLYMORPHIC_POOL
#define ETL_POLYMORPHIC_VECTOR
#define ETL_POLYMORPHIC_INDIRECT_VECTOR

//#define ETL_POLYMORPHIC_CONTAINERS

//#define ETL_MESSAGES_ARE_VIRTUAL
//#define ETL_POLYMORPHIC_MESSAGES

//#define ETL_NO_STL

//#define ETL_FUNCTION_FORCE_CPP03
//#define ETL_PRIORITY_QUEUE_FORCE_CPP03
//#define ETL_QUEUE_ATOMIC_FORCE_CPP03
//#define ETL_VARIANT_FORCE_CPP03
//#define ETL_VECTOR_FORCE_CPP03
//#define ETL_QUEUE_FORCE_CPP03
//#define ETL_QUEUE_MPMC_MUTEX_FORCE_CPP03
//#define ETL_QUEUE_ISR_FORCE_CPP03
//#define ETL_QUEUE_LOCKED_FORCE_CPP03
//#define ETL_OPTIONAL_FORCE_CPP03
//#define ETL_LARGEST_TYPE_FORCE_CPP03

#ifdef _MSC_VER
  #include "etl/profiles/msvc_x86.h"
#else
  #include "etl/profiles/gcc_windows_x86.h"
#endif

#endif
