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

#include "logging.h"

// TODO(b/142492876): Avoid depending on absl internal.
#include "absl/base/internal/cycleclock.h"
#include "absl/base/internal/sysinfo.h"

#include "macros.h"

#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <string>
#include <unordered_map>

namespace tensorflow {

namespace {

bool EmitThreadIdFromEnv() {
  const char* env_var_val = getenv("LOG_THREAD_ID");
  return env_var_val == nullptr
             ? false
             : ParseInteger(env_var_val, strlen(env_var_val)) != 0;
}

}  // namespace


LogMessage::LogMessage(const char* fname, int line, int severity)
    : fname_(fname), line_(line), severity_(severity) {}

LogMessage& LogMessage::AtLocation(const char* fname, int line) {
  fname_ = fname;
  line_ = line;
  return *this;
}

LogMessage::~LogMessage() {
  // Read the min log level once during the first call to logging.
  static int64 min_log_level = MinLogLevelFromEnv();
  if (severity_ >= min_log_level) {
    GenerateLogMessage();
  }
}


void LogMessage::GenerateLogMessage() {
  static bool log_thread_id = EmitThreadIdFromEnv();
  uint64 now_micros = EnvTime::NowMicros();
  time_t now_seconds = static_cast<time_t>(now_micros / 1000000);
  int32 micros_remainder = static_cast<int32>(now_micros % 1000000);
  const size_t time_buffer_size = 30;
  char time_buffer[time_buffer_size];
  strftime(time_buffer, time_buffer_size, "%Y-%m-%d %H:%M:%S",
           localtime(&now_seconds));
  const size_t tid_buffer_size = 10;
  char tid_buffer[tid_buffer_size] = "";
  if (log_thread_id) {
    snprintf(tid_buffer, sizeof(tid_buffer), " %7u",
             absl::base_internal::GetTID());
  }
  // TODO(jeff,sanjay): Replace this with something that logs through the env.
  fprintf(stderr, "%s.%06d: %c%s %s:%d] %s\n", time_buffer, micros_remainder,
          "IWEF"[severity_], tid_buffer, fname_, line_, str().c_str());
}

LogMessageFatal::LogMessageFatal(const char* file, int line)
    : LogMessage(file, line, FATAL) {}
LogMessageFatal::~LogMessageFatal() {
  // abort() ensures we don't return (we promised we would not via
  // ATTRIBUTE_NORETURN).
  GenerateLogMessage();
  abort();
}

template <>
void MakeCheckOpValueString(std::ostream* os, const char& v) {
  if (v >= 32 && v <= 126) {
    (*os) << "'" << v << "'";
  } else {
    (*os) << "char value " << static_cast<short>(v);
  }
}

template <>
void MakeCheckOpValueString(std::ostream* os, const signed char& v) {
  if (v >= 32 && v <= 126) {
    (*os) << "'" << v << "'";
  } else {
    (*os) << "signed char value " << static_cast<short>(v);
  }
}

template <>
void MakeCheckOpValueString(std::ostream* os, const unsigned char& v) {
  if (v >= 32 && v <= 126) {
    (*os) << "'" << v << "'";
  } else {
    (*os) << "unsigned char value " << static_cast<unsigned short>(v);
  }
}

#if LANG_CXX11
template <>
void MakeCheckOpValueString(std::ostream* os, const std::nullptr_t& p) {
  (*os) << "nullptr";
}
#endif

CheckOpMessageBuilder::CheckOpMessageBuilder(const char* exprtext)
    : stream_(new std::ostringstream) {
  *stream_ << "Check failed: " << exprtext << " (";
}

CheckOpMessageBuilder::~CheckOpMessageBuilder() { delete stream_; }

std::ostream* CheckOpMessageBuilder::ForVar2() {
  *stream_ << " vs. ";
  return stream_;
}

string* CheckOpMessageBuilder::NewString() {
  *stream_ << ")";
  return new string(stream_->str());
}

}  // namespace galaxy
