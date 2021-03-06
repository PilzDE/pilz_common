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

#include <pilz_testutils/service_client_mock.h>
#include <std_srvs/Trigger.h>

namespace pilz_testutils
{
TEST(SeviceClientMockTest, defaultBehaviour)
{
  ServiceClientMockFactory<std::string> srv_client_mock_factory;

  auto service = srv_client_mock_factory.create("service_name", false);  // <- Whatever the second param does
  std::string resp{ "response" };
  EXPECT_CALL(srv_client_mock_factory, call_named("service_name", resp));
  service.call(resp);
}

TEST(SeviceClientMockTest, negationOnService)
{
  ServiceClientMockFactory<std::string> srv_client_mock_factory;
  auto service = srv_client_mock_factory.create("service_name", false);  // <- Whatever the second param does

  EXPECT_CALL(srv_client_mock_factory, handle_invalid_named("service_name")).WillOnce(::testing::Return(false));
  EXPECT_FALSE(!service);
}

}  // namespace pilz_testutils

int main(int argc, char* argv[])
{
  testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}
