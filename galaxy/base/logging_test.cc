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

#include "gtest/gtest.h"

namespace galaxy {

TEST(TestLogging, TestCheck) {
  CHECK_EQ(1000, 1000);
  CHECK_LE(1000, 1000);
  CHECK_LE(1000, 2000);
  CHECK_LT(1000, 2000);
  CHECK_GE(1000, 1000);
  CHECK_GE(2000, 1000);
  CHECK_GT(2000, 1000);
  CHECK_NE(2000, 1000);
}

TEST(TestLogging, TestLog) {
  LOG(WARNING) << "This is a logging @ the warning level";
  LOG(INFO) << "This is a logging @ the info level";
  LOG(DEBUG) << "This is a logging @ the debug level";
  LOG(ERROR) << "This is a logging @ the error level";
}

} // namespace galaxy
