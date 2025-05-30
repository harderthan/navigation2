// Copyright (c) 2018 Intel Corporation
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef NAV2_BEHAVIOR_TREE__COMPUTE_PATH_TO_POSE_ACTION_HPP_
#define NAV2_BEHAVIOR_TREE__COMPUTE_PATH_TO_POSE_ACTION_HPP_

#include <memory>
#include <string>

#include "nav2_msgs/action/compute_path_to_pose.hpp"
#include "nav_msgs/msg/path.h"
#include "nav2_behavior_tree/bt_action_node.hpp"

namespace nav2_behavior_tree
{

class ComputePathToPoseAction : public BtActionNode<nav2_msgs::action::ComputePathToPose>
{
public:
  ComputePathToPoseAction(
    const std::string & action_name,
    const BT::NodeConfiguration & conf)
  : BtActionNode<nav2_msgs::action::ComputePathToPose>(action_name, conf)
  {
  }

  void on_tick() override
  {
    getInput("goal", goal_.pose);
    getInput("planner_id", goal_.planner_id);
  }

  void on_success() override
  {
    setOutput("path", result_.result->path);

    if (first_time_) {
      first_time_ = false;
    } else {
      config().blackboard->set("path_updated", true);
    }
  }

  static BT::PortsList providedPorts()
  {
    return providedBasicPorts({
        BT::OutputPort<nav_msgs::msg::Path>("path", "Path created by ComputePathToPose node"),
        BT::InputPort<geometry_msgs::msg::PoseStamped>("goal", "Destination to plan to"),
        BT::InputPort<std::string>("planner_id", "")
      });
  }

private:
  bool first_time_{true};
};

}  // namespace nav2_behavior_tree

#endif  // NAV2_BEHAVIOR_TREE__COMPUTE_PATH_TO_POSE_ACTION_HPP_
