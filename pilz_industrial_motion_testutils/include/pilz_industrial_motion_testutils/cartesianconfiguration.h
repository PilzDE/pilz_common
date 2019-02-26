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

#ifndef CARTESIANCONFIGURATION_H
#define CARTESIANCONFIGURATION_H

#include <vector>
#include <sstream>

#include <boost/optional.hpp>

#include <eigen_conversions/eigen_msg.h>
#include <geometry_msgs/Pose.h>
#include <geometry_msgs/PoseStamped.h>
#include <moveit/robot_model/robot_model.h>
#include <moveit/robot_state/robot_state.h>
#include <moveit/robot_state/conversions.h>
#include <moveit/kinematic_constraints/utils.h>

#include "robotconfiguration.h"
#include "jointconfiguration.h"

namespace pilz_industrial_motion_testutils
{

/**
 * @brief Class to define a robot configuration in space
 * with the help of cartesian coordinates.
 */
class CartesianConfiguration : public RobotConfiguration
{
public:
  CartesianConfiguration();

  CartesianConfiguration(const std::string& group_name,
                         const std::string& link_name,
                         const std::vector<double>& config);

  CartesianConfiguration(const std::string& group_name,
                         const std::string& link_name,
                         const std::vector<double>& config,
                         moveit::core::RobotModelConstPtr robot_model);

public:
  virtual moveit_msgs::Constraints toGoalConstraints() const override;
  virtual moveit_msgs::RobotState toMoveitMsgsRobotState() const override;

  void setLinkName(const std::string& link_name);
  const std::string& getLinkName() const;

  const geometry_msgs::Pose &getPose() const;
  geometry_msgs::Pose &getPose();

  void setSeed(const JointConfiguration& config);
  const JointConfiguration& getSeed() const;
  //! @brief States if a seed for the cartesian configuration is set.
  bool hasSeed() const;

  void setPoseTolerance(const double tol);
  const boost::optional<double> getPoseTolerance() const;

  void setAngleTolerance(const double tol);
  const boost::optional<double> getAngleTolerance() const;

  void clearOrientation();

private:
  static geometry_msgs::Pose toPose(const std::vector<double>& pose);
  static geometry_msgs::PoseStamped toStampedPose(const geometry_msgs::Pose& pose);

private:
  std::string link_name_;
  geometry_msgs::Pose pose_;

  //! @brief The dimensions of the sphere associated with the target region
  //! of the position constraint.
  boost::optional<double> tolerance_pose_ {boost::none};

  //! @brief The value to assign to the absolute tolerances of the
  //! orientation constraint.
  boost::optional<double> tolerance_angle_ {boost::none};

  // A vector is used here (although only one value is stored)
  // because it allows to also store the information if a
  // value was set or not. Furthermore, in contrast to smart pointers
  // a vector allows to use the default assignment and copy operators.
  std::vector<JointConfiguration> seed_;

};

std::ostream& operator<<(std::ostream&, const CartesianConfiguration&);

inline void CartesianConfiguration::setLinkName(const std::string& link_name)
{
  link_name_ = link_name;
}

inline const std::string& CartesianConfiguration::getLinkName() const
{
  return link_name_;
}

inline const geometry_msgs::Pose& CartesianConfiguration::getPose() const
{
  return pose_;
}

inline geometry_msgs::Pose & CartesianConfiguration::getPose()
{
  return pose_;
}

inline moveit_msgs::Constraints CartesianConfiguration::toGoalConstraints() const
{
  if (!tolerance_pose_ || !tolerance_angle_)
  {
    return kinematic_constraints::constructGoalConstraints(link_name_, toStampedPose(pose_));
  }
  else
  {
    return kinematic_constraints::constructGoalConstraints(link_name_, toStampedPose(pose_),
                                                           tolerance_pose_.value(), tolerance_angle_.value());
  }
}

inline void CartesianConfiguration::setSeed(const JointConfiguration& config)
{
  seed_.clear();
  seed_.emplace_back(config);
}

inline const JointConfiguration& CartesianConfiguration::getSeed() const
{
  return seed_.front();
}

inline bool CartesianConfiguration::hasSeed() const
{
  return !seed_.empty();
}

inline void CartesianConfiguration::setPoseTolerance(const double tol)
{
  tolerance_pose_ = tol;
}

inline const boost::optional<double> CartesianConfiguration::getPoseTolerance() const
{
  return tolerance_pose_;
}

inline void CartesianConfiguration::setAngleTolerance(const double tol)
{
  tolerance_angle_ = tol;
}

inline const boost::optional<double> CartesianConfiguration::getAngleTolerance() const
{
  return tolerance_angle_;
}

inline void CartesianConfiguration::clearOrientation()
{
  pose_.orientation.x = 0.;
  pose_.orientation.y = 0.;
  pose_.orientation.z = 0.;
  pose_.orientation.w = 0.;
}

}

#endif // CARTESIANCONFIGURATION_H
