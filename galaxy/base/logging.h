/* Copyright 2015 The TensorFlow Authors. All Rights Reserved.
Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at
    http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/

#ifndef __GALAXY_BASE_LOGGING_H__
#define __GALAXY_BASE_LOGGING_H__

#include <limits>
#include <memory>
#include <sstream>

#include "absl/base/log_severity.h"
#include "absl/strings/string_view.h"

#include "macros.h"
#include "types.h"

namespace galaxy {

constexpr int32_t DEBUG = 0;
constexpr int32_t INFO = 1;
constexpr int32_t WARNING = 2;
constexpr int32_t ERROR = 3;
constexpr int32_t FATAL = 4;
constexpr int32_t NUM_SEVERITIES = 5;

class LogMessage : public std::basic_ostringstream<char> {
public:
  LogMessage(const char *fname, int32_t line, int32_t severity);
  ~LogMessage() override;

  // Change the location of the log message.
  LogMessage &AtLocation(const char *fname, int32_t line);

protected:
  void GenerateLogMessage();

private:
  const char *fname_;
  int line_;
  int severity_;
};

// LogMessageFatal ensures the process will exit in failure after
// logging this message.
class LogMessageFatal : public LogMessage {
public:
  LogMessageFatal(const char *file, int line) ATTRIBUTE_COLD;
  ATTRIBUTE_NORETURN ~LogMessageFatal() override;
};

// LogMessageNull supports the DVLOG macro by simply dropping any log messages.
class LogMessageNull : public std::basic_ostringstream<char> {
public:
  LogMessageNull() {}
  ~LogMessageNull() override {}
};

#define _GALAXY_LOG_DEBUG                                                      \
  \ 
  ::galaxy::LogMessage(__FILE__, __LINE__, ::galaxy::DEBUG)
#define _GALAXY_LOG_INFO                                                       \
  ::galaxy::LogMessage(__FILE__, __LINE__, ::galaxy::INFO)
#define _GALAXY_LOG_WARNING                                                    \
  ::galaxy::LogMessage(__FILE__, __LINE__, ::galaxy::WARNING)
#define _GALAXY_LOG_ERROR                                                      \
  ::galaxy::LogMessage(__FILE__, __LINE__, ::galaxy::ERROR)
#define _GALAXY_LOG_FATAL ::galaxy::LogMessageFatal(__FILE__, __LINE__)

#define _GALAXY_LOG_QFATAL _GALAXY_LOG_FATAL

#define LOG(severity) _GALAXY_LOG_##severity

// CHECK dies with a fatal error if condition is not true.  It is *not*
// controlled by NDEBUG, so the check will be executed regardless of
// compilation mode.  Therefore, it is safe to do things like:
//    CHECK(fp->Write(x) == 4)
#define CHECK(condition)                                                       \
  if (PREDICT_FALSE(!(condition)))                                             \
  LOG(FATAL) << "Check failed: " #condition " "

// Function is overloaded for integral types to allow static const
// integrals declared in classes and not defined to be used as arguments to
// CHECK* macros. It's not encouraged though.
template <typename T> inline const T &GetReferenceableValue(const T &t) {
  return t;
}
inline char GetReferenceableValue(char t) { return t; }
inline unsigned char GetReferenceableValue(unsigned char t) { return t; }
inline signed char GetReferenceableValue(signed char t) { return t; }
inline short GetReferenceableValue(short t) { return t; }
inline unsigned short GetReferenceableValue(unsigned short t) { return t; }
inline int GetReferenceableValue(int32_t t) { return t; }
inline unsigned int GetReferenceableValue(unsigned int t) { return t; }
inline long GetReferenceableValue(long t) { return t; }
inline unsigned long GetReferenceableValue(unsigned long t) { return t; }
inline long long GetReferenceableValue(long long t) { return t; }
inline unsigned long long GetReferenceableValue(unsigned long long t) {
  return t;
}

// This formats a value for a failing CHECK_XX statement.  Ordinarily,
// it uses the definition for operator<<, with a few special cases below.
template <typename T>
inline void MakeCheckOpValueString(std::ostream *os, const T &v) {
  (*os) << v;
}

// Overrides for char types provide readable values for unprintable
// characters.
template <> void MakeCheckOpValueString(std::ostream *os, const char &v);
template <> void MakeCheckOpValueString(std::ostream *os, const signed char &v);
template <>
void MakeCheckOpValueString(std::ostream *os, const unsigned char &v);

#if LANG_CXX11
// We need an explicit specialization for std::nullptr_t.
template <>
void MakeCheckOpValueString(std::ostream *os, const std::nullptr_t &p);
#endif

// A container for a string pointer which can be evaluated to a bool -
// true iff the pointer is non-NULL.
struct CheckOpString {
  CheckOpString(string *str) : str_(str) {}
  // No destructor: if str_ is non-NULL, we're about to LOG(FATAL),
  // so there's no point in cleaning up str_.
  operator bool() const { return PREDICT_FALSE(str_ != NULL); }
  string *str_;
};

// Build the error message string. Specify no inlining for code size.
template <typename T1, typename T2>
string *MakeCheckOpString(const T1 &v1, const T2 &v2,
                          const char *exprtext) ATTRIBUTE_NOINLINE;

// A helper class for formatting "expr (V1 vs. V2)" in a CHECK_XX
// statement.  See MakeCheckOpString for sample usage.  Other
// approaches were considered: use of a template method (e.g.,
// base::BuildCheckOpString(exprtext, base::Print<T1>, &v1,
// base::Print<T2>, &v2), however this approach has complications
// related to volatile arguments and function-pointer arguments).
class CheckOpMessageBuilder {
public:
  // Inserts "exprtext" and " (" to the stream.
  explicit CheckOpMessageBuilder(const char *exprtext);
  // Deletes "stream_".
  ~CheckOpMessageBuilder();
  // For inserting the first variable.
  std::ostream *ForVar1() { return stream_; }
  // For inserting the second variable (adds an intermediate " vs. ").
  std::ostream *ForVar2();
  // Get the result (inserts the closing ")").
  string *NewString();

private:
  std::ostringstream *stream_;
};

template <typename T1, typename T2>
string *MakeCheckOpString(const T1 &v1, const T2 &v2, const char *exprtext) {
  CheckOpMessageBuilder comb(exprtext);
  MakeCheckOpValueString(comb.ForVar1(), v1);
  MakeCheckOpValueString(comb.ForVar2(), v2);
  return comb.NewString();
}

// Helper functions for CHECK_OP macro.
// The (int, int) specialization works around the issue that the compiler
// will not instantiate the template version of the function on values of
// unnamed enum type - see comment below.
// The (size_t, int) and (int, size_t) specialization are to handle unsigned
// comparison errors while still being thorough with the comparison.
#define DEFINE_CHECK_OP_IMPL(name, op)                                         \
  template <typename T1, typename T2>                                          \
  inline string *name##Impl(const T1 &v1, const T2 &v2,                        \
                            const char *exprtext) {                            \
    if (PREDICT_TRUE(v1 op v2))                                                \
      return NULL;                                                             \
    else                                                                       \
      return ::galaxy::MakeCheckOpString(v1, v2, exprtext);                    \
  }                                                                            \
  inline string *name##Impl(int v1, int v2, const char *exprtext) {            \
    return name##Impl<int, int>(v1, v2, exprtext);                             \
  }                                                                            \
  inline string *name##Impl(const size_t v1, const int v2,                     \
                            const char *exprtext) {                            \
    if (PREDICT_FALSE(v2 < 0)) {                                               \
      return ::galaxy::MakeCheckOpString(v1, v2, exprtext);                    \
    }                                                                          \
    return name##Impl<size_t, size_t>(v1, v2, exprtext);                       \
  }                                                                            \
  inline string *name##Impl(const int v1, const size_t v2,                     \
                            const char *exprtext) {                            \
    if (PREDICT_FALSE(v2 >= std::numeric_limits<int>::max())) {                \
      return ::galaxy::MakeCheckOpString(v1, v2, exprtext);                    \
    }                                                                          \
    const size_t uval = (size_t)((unsigned)v2);                                \
    return name##Impl<size_t, size_t>(v1, uval, exprtext);                     \
  }

// We use the full name Check_EQ, Check_NE, etc. in case the file including
// base/logging.h provides its own #defines for the simpler names EQ, NE, etc.
// This happens if, for example, those are used as token names in a
// yacc grammar.
DEFINE_CHECK_OP_IMPL(Check_EQ,
                     ==)           // Compilation error with CHECK_EQ(NULL, x)?
DEFINE_CHECK_OP_IMPL(Check_NE, !=) // Use CHECK(x == NULL) instead.
DEFINE_CHECK_OP_IMPL(Check_LE, <=)
DEFINE_CHECK_OP_IMPL(Check_LT, <)
DEFINE_CHECK_OP_IMPL(Check_GE, >=)
DEFINE_CHECK_OP_IMPL(Check_GT, >)
#undef DEFINE_CHECK_OP_IMPL

// In optimized mode, use CheckOpString to hint to compiler that
// the while condition is unlikely.
#define CHECK_OP_LOG(name, op, val1, val2)                                     \
  while (::galaxy::CheckOpString _result = ::galaxy::name##Impl(               \
             ::galaxy::GetReferenceableValue(val1),                            \
             ::galaxy::GetReferenceableValue(val2), #val1 " " #op " " #val2))  \
  ::galaxy::LogMessageFatal(__FILE__, __LINE__) << *(_result.str_)

#define CHECK_OP(name, op, val1, val2) CHECK_OP_LOG(name, op, val1, val2)

// CHECK_EQ/NE/...
#define CHECK_EQ(val1, val2) CHECK_OP(Check_EQ, ==, val1, val2)
#define CHECK_NE(val1, val2) CHECK_OP(Check_NE, !=, val1, val2)
#define CHECK_LE(val1, val2) CHECK_OP(Check_LE, <=, val1, val2)
#define CHECK_LT(val1, val2) CHECK_OP(Check_LT, <, val1, val2)
#define CHECK_GE(val1, val2) CHECK_OP(Check_GE, >=, val1, val2)
#define CHECK_GT(val1, val2) CHECK_OP(Check_GT, >, val1, val2)
#define CHECK_NOTNULL(val)                                                     \
  ::galaxy::CheckNotNull(__FILE__, __LINE__, "'" #val "' Must be non NULL",    \
                         (val))

} // namespace galaxy

#endif // __GALAXY_BASE_LOGGING_H__
