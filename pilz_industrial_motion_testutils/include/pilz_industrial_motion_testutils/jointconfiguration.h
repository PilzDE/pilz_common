/*
 * Copyright (c) 2019 Pilz GmbH & Co. KG
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

#ifndef JOINTCONFIGURATION_H
#define JOINTCONFIGURATION_H

#include <string>
#include <vector>

#include <sensor_msgs/JointState.h>
#include <moveit/robot_state/robot_state.h>
#include <moveit/robot_state/conversions.h>

#include "robotconfiguration.h"

namespace pilz_industrial_motion_testutils
{

class JointConfiguration : public RobotConfiguration
{
public:
  JointConfiguration();

  JointConfiguration(const std::string& group_name,
                     const std::vector<double>& config);

  JointConfiguration(const std::string& group_name,
                     const std::vector<double>& config,
                     moveit::core::RobotModelConstPtr robot_model);


public:
  JointConfiguration& setJointPrefix(const std::string& joint_prefix);

  moveit_msgs::Constraints toGoalConstraints() const override;
  moveit_msgs::RobotState toMoveitMsgsRobotState() const override;

  sensor_msgs::JointState toSensorMsg() const;

private:
  moveit_msgs::RobotState toMoveitMsgsRobotStateWithoutModel() const;
  moveit_msgs::RobotState toMoveitMsgsRobotStateWithModel() const;

  moveit_msgs::Constraints toGoalConstraintsWithoutModel() const;
  moveit_msgs::Constraints toGoalConstraintsWithModel() const;

private:
  static std::string createJointName(const std::string& joint_prefix,
                                     const size_t& joint_number);

private:
  //! Joint positions
  std::vector<double> joints_;
  std::string joint_prefix_ {};
};

inline JointConfiguration& JointConfiguration::setJointPrefix(const std::string& joint_prefix)
{
  joint_prefix_ = joint_prefix;
  return *this;
}

inline std::string JointConfiguration::createJointName(const std::string& joint_prefix,
                                                       const size_t& joint_number)
{
  return joint_prefix + std::to_string(joint_number);
}

inline moveit_msgs::Constraints JointConfiguration::toGoalConstraints() const
{
  return robot_model_? toGoalConstraintsWithModel() :
                       toGoalConstraintsWithoutModel();
}

inline moveit_msgs::RobotState JointConfiguration::toMoveitMsgsRobotState() const
{
  return robot_model_? toMoveitMsgsRobotStateWithModel() :
                       toMoveitMsgsRobotStateWithoutModel();
}

}

#endif // JOINTCONFIGURATION_H
