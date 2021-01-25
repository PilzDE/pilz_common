/*
 * Copyright (c) 2021 Pilz GmbH & Co. KG
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <future>
#include <chrono>

#include <gmock/gmock.h>

#include <pilz_testutils/async_test.h>
#include <pilz_testutils/logger_mock.h>

namespace pilz_testutils
{
class LoggerMockTest : public testing::Test, public testing::AsyncTest
{
public:
  MOCK_METHOD0(myMethod, void());
};

TEST_F(LoggerMockTest, defaultBehaviour)
{
  pilz_testutils::LoggerMock ros_log_mock;

  EXPECT_LOG(*ros_log_mock, WARN, "Your warning text").WillOnce(ACTION_OPEN_BARRIER_VOID("logger_called_event"));
  std::async(std::launch::async, []() { ROS_WARN("Your warning text"); });

  BARRIER("logger_called_event");  // Wait till log message is received
}

}  // namespace pilz_testutils

int main(int argc, char* argv[])
{
  testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}