/*
 * Copyright (C) 2014 Open Source Robotics Foundation
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
 *
*/

#include <map>

#include <ros/ros.h>
#include <gazebo/physics/physics.hh>
#include <gazebo/sensors/sensors.hh>
#include <gazebo/math/gzmath.hh>
#include "robocup_agent_plugin/AgentPlugin.hh"
#include "robocup_msgs/GameStateMonitor.h"
#include "robocup_msgs/SendJoints.h"
// #include "robocup_msgs/Say.h"

using namespace gazebo;

GZ_REGISTER_MODEL_PLUGIN(AgentPlugin)

/////////////////////////////////////////////////
AgentPlugin::AgentPlugin()
{
  // Initialize the joint names.
  this->jointNames.push_back("HeadYaw");
  this->jointNames.push_back("HeadPitch");
  this->jointNames.push_back("LHipYawPitch");
  this->jointNames.push_back("LHipRoll");
  this->jointNames.push_back("LHipPitch");
  this->jointNames.push_back("LKneePitch");
  this->jointNames.push_back("LAnklePitch");
  this->jointNames.push_back("LAnkleRoll");
  this->jointNames.push_back("LShoulderPitch");
  this->jointNames.push_back("LShoulderRoll");
  this->jointNames.push_back("LElbowYaw");
  this->jointNames.push_back("LElbowRoll");
  this->jointNames.push_back("RHipYawPitch");
  this->jointNames.push_back("RHipRoll");
  this->jointNames.push_back("RHipPitch");
  this->jointNames.push_back("RKneePitch");
  this->jointNames.push_back("RAnklePitch");
  this->jointNames.push_back("RAnkleRoll");
  this->jointNames.push_back("RShoulderPitch");
  this->jointNames.push_back("RShoulderRoll");
  this->jointNames.push_back("RElbowYaw");
  this->jointNames.push_back("RElbowRoll");
  this->jointNames.push_back("LWristYaw");
  this->jointNames.push_back("RWristYaw");

  // Initialize joint forces.
  for (int i = 0; i < 24; ++i)
    this->jointForces.push_back(0.0);

  this->toAgent["naoH25V40::HeadYaw"]        = "hj1";
  this->toAgent["naoH25V40::HeadPitch"]      = "hj2";
  this->toAgent["naoH25V40::LHipYawPitch"]   = "llj1";
  this->toAgent["naoH25V40::LHipRoll"]       = "llj2";
  this->toAgent["naoH25V40::LHipPitch"]      = "llj3";
  this->toAgent["naoH25V40::LKneePitch"]     = "llj4";
  this->toAgent["naoH25V40::LAnklePitch"]    = "llj5";
  this->toAgent["naoH25V40::LAnkleRoll"]     = "llj6";
  this->toAgent["naoH25V40::LShoulderPitch"] = "laj1";
  this->toAgent["naoH25V40::LShoulderRoll"]  = "laj2";
  this->toAgent["naoH25V40::LElbowYaw"]      = "laj4";
  this->toAgent["naoH25V40::LElbowRoll"]     = "laj3";
  this->toAgent["naoH25V40::LWristYaw"]      = "naoH25V40::LWristYaw";
  this->toAgent["naoH25V40::RHipYawPitch"]   = "rlj1";
  this->toAgent["naoH25V40::RHipRoll"]       = "rlj2";
  this->toAgent["naoH25V40::RHipPitch"]      = "rlj3";
  this->toAgent["naoH25V40::RKneePitch"]     = "rlj4";
  this->toAgent["naoH25V40::RAnklePitch"]    = "rlj5";
  this->toAgent["naoH25V40::RAnkleRoll"]     = "rlj6";
  this->toAgent["naoH25V40::RShoulderPitch"] = "raj1";
  this->toAgent["naoH25V40::RShoulderRoll"]  = "raj2";
  this->toAgent["naoH25V40::RElbowYaw"]      = "raj4";
  this->toAgent["naoH25V40::RElbowRoll"]     = "raj3";
  this->toAgent["naoH25V40::RWristYaw"]      = "naoH25V40::RWristYaw";

  this->toServer["he1"]                  = "naoH25V40::HeadYaw";
  this->toServer["he2"]                  = "naoH25V40::HeadPitch";
  this->toServer["lle1"]                 = "naoH25V40::LHipYawPitch";
  this->toServer["lle2"]                 = "naoH25V40::LHipRoll";
  this->toServer["lle3"]                 = "naoH25V40::LHipPitch";
  this->toServer["lle4"]                 = "naoH25V40::LKneePitch";
  this->toServer["lle5"]                 = "naoH25V40::LAnklePitch";
  this->toServer["lle6"]                 = "naoH25V40::LAnkleRoll";
  this->toServer["lae1"]                 = "naoH25V40::LShoulderPitch";
  this->toServer["lae2"]                 = "naoH25V40::LShoulderRoll";
  this->toServer["lae4"]                 = "naoH25V40::LElbowYaw";
  this->toServer["lae3"]                 = "naoH25V40::LElbowRoll";
  this->toServer["naoH25V40::LWristYaw"] = "naoH25V40::LWristYaw";
  this->toServer["rle1"]                 = "naoH25V40::RHipYawPitch";
  this->toServer["rle2"]                 = "naoH25V40::RHipRoll";
  this->toServer["rle3"]                 = "naoH25V40::RHipPitch";
  this->toServer["rle4"]                 = "naoH25V40::RKneePitch";
  this->toServer["rle5"]                 = "naoH25V40::RAnklePitch";
  this->toServer["rle6"]                 = "naoH25V40::RAnkleRoll";
  this->toServer["rae1"]                 = "naoH25V40::RShoulderPitch";
  this->toServer["rae2"]                 = "naoH25V40::RShoulderRoll";
  this->toServer["rae4"]                 = "naoH25V40::RElbowYaw";
  this->toServer["rae3"]                 = "naoH25V40::RElbowRoll";
  this->toServer["naoH25V40::RWristYaw"] = "naoH25V40::RWristYaw";

  // Start up ROS
  std::string name = "set_joints";
  int argc = 0;
  ros::init(argc, NULL, name);

  gzlog << "RoboCup 3D simulator agent plugin running" << std::endl;

  this->gzNode = transport::NodePtr(new transport::Node());
  this->gzNode->Init();

  this->gameState.time = ros::Time();
  this->gameState.half = 1;
  this->gameState.score_left = 0;
  this->gameState.score_right = 0;
  this->gameState.play_mode = "before_kickoff";

  // ROS Nodehandle.
  this->node.reset(new ros::NodeHandle("~"));
}

/////////////////////////////////////////////////
AgentPlugin::~AgentPlugin()
{
  event::Events::DisconnectWorldUpdateBegin(this->updateConnection);
}

void AgentPlugin::GameStateCb(const robocup_msgs::GameStateMonitor &_msg)
{
  boost::recursive_mutex::scoped_lock lock(this->mutex);
  this->gameState = _msg;
}

/////////////////////////////////////////////////
void AgentPlugin::Load(physics::ModelPtr _model, sdf::ElementPtr _sdf)
{
  // Default camera view angle is 120 degrees.
  this->viewAngle = 120.0;

  this->stateMsgCounter = 0;

  // Perception noise values.
  this->distanceNoiseSigma = 0.0965;
  this->angle1NoiseSigma = 0.1225;
  this->angle2NoiseSigma = 0.1480;

  this->model = _model;
  this->ball = _model->GetWorld()->GetModel("soccer_ball");

  // Get the noise that is applied to the camera's pose.
  this->cameraPoseNoise.pos.x = math::Rand::GetDblUniform(-0.005, 0.005);
  this->cameraPoseNoise.pos.y = math::Rand::GetDblUniform(-0.005, 0.005);
  this->cameraPoseNoise.pos.z = math::Rand::GetDblUniform(-0.005, 0.005);

  // Middle line
  this->lines.push_back(Line(0, -10, 0, 10));

  // ground lines
  this->lines.push_back(Line(15, -10, 15,10));
  this->lines.push_back(Line(-15, -10, -15,10));

  // side lines
  this->lines.push_back(Line(15,10, -15,10));
  this->lines.push_back(Line(15,-10, -15,-10));

  // penalty lines
  this->lines.push_back(Line(13.2,3, 13.2,-3));
  this->lines.push_back(Line(13.2,3, 15,3));
  this->lines.push_back(Line(13.2,-3, 15,-3));
  this->lines.push_back(Line(-13.2,3, -13.2,-3));
  this->lines.push_back(Line(-13.2,3, -15,3));
  this->lines.push_back(Line(-13.2,-3, -15,-3));

  // center circle ring
  this->lines.push_back(Line(2,0, 1.618033989, 1.175570505));
  this->lines.push_back(Line(1.618033989, 1.175570505,
                             0.618033989, 1.902113033));
  this->lines.push_back(Line(0.618033989, 1.902113033,
                             -0.618033989, 1.902113033));
  this->lines.push_back(Line(-0.618033989, 1.902113033,
                             -1.618033989, 1.175570505));
  this->lines.push_back(Line(-1.618033989, 1.175570505, -2, 0));
  this->lines.push_back(Line(-2, 0, -1.618033989, -1.175570505));
  this->lines.push_back(Line(-1.618033989, -1.175570505,
                             -0.618033989, -1.902113033));
  this->lines.push_back(Line(-0.618033989, -1.902113033,
                             0.618033989, -1.902113033));
  this->lines.push_back(Line(0.618033989, -1.902113033,
                             1.618033989, -1.175570505));
  this->lines.push_back(Line(1.618033989, -1.175570505, 2,0));

  if (!this->ball)
  {
    gzerr << "Unable to get the ball model\n";
    return;
  }

  if (_sdf->HasElement("view_angle"))
    this->viewAngle = _sdf->Get<double>("view_angle");

  if (_sdf->HasElement("head_link"))
  {
    this->headLink = this->model->GetLink(_sdf->Get<std::string>("head_link"));
    if (!this->headLink)
    {
      gzerr << "Unable to get head link with name ["
        << _sdf->Get<std::string>("head_link") << "]\n";
    }
  }
  else
  {
    gzerr << "<head_link> not specified\n";
    return;
  }

  // Get all the hinge joints that are not fixed.
  for (physics::Joint_V::const_iterator iter = _model->GetJoints().begin();
       iter != _model->GetJoints().end(); ++iter)
  {
    if (((*iter)->HasType(physics::Base::HINGE_JOINT) ||
         (*iter)->HasType(physics::Base::UNIVERSAL_JOINT)) &&
        (*iter)->GetUpperLimit(0) != (*iter)->GetLowerLimit(0))
    {
      this->joints.push_back(*iter);
    }
  }

  // Get all the imu sensors.
  if (_sdf->HasElement("imu_sensor"))
  {
    sdf::ElementPtr imuElem = _sdf->GetElement("imu_sensor");
    while(imuElem)
    {
      std::string sensorName = _model->GetWorld()->GetName() + "::" +
        _model->GetName() + "::" +
        imuElem->Get<std::string>();
      sensors::SensorPtr sensor =
        sensors::SensorManager::Instance()->GetSensor(sensorName);

      if (!sensor)
        gzerr << "Unable to get sensor with name[" << sensorName << "]\n";
      else
      {
        this->imuSensors.push_back(
            boost::dynamic_pointer_cast<sensors::ImuSensor>(sensor));
      }

      imuElem = imuElem->GetNextElement("imu_sensor");
    }
  }

  // Get all the touch sensors.
  if (_sdf->HasElement("touch_sensor"))
  {
    sdf::ElementPtr touchElem = _sdf->GetElement("touch_sensor");
    while(touchElem)
    {
      std::string sensorName = _model->GetWorld()->GetName() + "::" +
        _model->GetName() + "::" +
        touchElem->Get<std::string>();
      sensors::SensorPtr sensor =
        sensors::SensorManager::Instance()->GetSensor(sensorName);

      if (!sensor)
        gzerr << "Unable to get sensor with name[" << sensorName << "]\n";
      else
      {
        this->touchSensors.push_back(
            boost::dynamic_pointer_cast<sensors::ContactSensor>(sensor));
      }

      touchElem = touchElem->GetNextElement("touch_sensor");
    }
  }

  // Get all the force sensors.
  if (_sdf->HasElement("force_sensor"))
  {
    sdf::ElementPtr forceElem = _sdf->GetElement("force_sensor");
    while(forceElem)
    {
      std::string sensorName = _model->GetWorld()->GetName() + "::" +
        _model->GetName() + "::" +
        forceElem->Get<std::string>();
      sensors::SensorPtr sensor =
        sensors::SensorManager::Instance()->GetSensor(sensorName);

      if (!sensor)
        gzerr << "Unable to get sensor with name[" << sensorName << "]\n";
      else
      {
        this->forceSensors.push_back(
            boost::dynamic_pointer_cast<sensors::ContactSensor>(sensor));
      }

      forceElem = forceElem->GetNextElement("force_sensor");
    }
  }

  // Get the team name.
  if (_sdf->HasElement("team_name"))
    this->teamName = _sdf->Get<std::string>("team_name");

  // Get the uniform number.
  if (_sdf->HasElement("uniform_number"))
    this->uniformNumber = _sdf->Get<std::string>("uniform_number");

  // Make sure the ROS node for Gazebo has already been initialized
  if (!ros::isInitialized())
  {
    ROS_FATAL_STREAM("A ROS node for Gazebo has not been initialized, " <<
        "unable to load plugin. Load the Gazebo system plugin " <<
        "'libgazebo_ros_api_plugin.so' in the gazebo_ros package)");
    return;
  }

  this->node.reset(new ros::NodeHandle(_model->GetName()));
  this->agentStatePub = this->node->advertise<robocup_msgs::AgentState>(
    "state", 1000);

  this->updateConnection = event::Events::ConnectWorldUpdateBegin(
    boost::bind(&AgentPlugin::Update, this, _1));

  this->modelName = _sdf->Get<std::string>("robot_namespace");

  this->model = _model;

  // Subscribe to the game state message.
  this->subscriber =
    this->node->subscribe("/gameController/game_state", 1000,
    &AgentPlugin::GameStateCb, this);

  // Advertise the service for sending new joint commands.
  this->jointCommandsService =
    this->node->advertiseService("/" + this->modelName + "/send_joints",
    &AgentPlugin::SendJoints, this);

  this->readyPub = this->gzNode->Advertise<msgs::Time>("/gameController/ready");

  // Register the callback for the game controller synchronization message.
  this->syncSub = this->gzNode->Subscribe(std::string("/gameController/sync"),
      &AgentPlugin::OnSyncReceived, this);

  /* Info: The agent has to advertise a topic '/<robot_name>/say' and publish
   a message in this topic each the agent wants to "say" something. The
   game controller plugin is subscribed to this topic and will forward the
   message to the rest of the teammates.

   In a similar way, each agent has to subscribe to a topic
   '/<robot_name>/listen', where the gamecontroller will forward the messages
   coming from other robots.*/

   // Debugging.
   // Register the callback for the 'listen' message.
  /* this->listenSub = this->node->subscribe<robocup_msgs::Say>(
    std::string("/" + this->modelName + "/listen"), 1000,
    &AgentPlugin::OnMessageFromRobot, this);*/

  // Debugging
  // Advertise the topic for sending messages to other robots.
  // this->sayPub = this->node->advertise<robocup_msgs::Say>(
  //  std::string("/" + this->modelName + "/say"), 10);
  // robocup_msgs::Say msg;
  //  this->sayPub.publish(msg);
}

/////////////////////////////////////////////////
void AgentPlugin::Init()
{
  physics::JointControllerPtr jc = this->model->GetJointController();

  for (int i = 0; i < 22; ++i)
  {
    // Get the joint.
    physics::JointPtr joint =
      this->model->GetJoint("naoH25V40::" + this->jointNames[i]);
    if (!joint)
    {
      std::cerr << "SendJoints() Joint [" << "naoH25V40::"
                << this->jointNames[i] << "] not found" << std::endl;
      continue;
    }

    common::PID pid;

    /*if (joint->GetScopedName().find("Ankle") != std::string::npos)
      // Set the force for this joint.
      pid.Init(50, 0, 0, 0, 0, 100, -100);
    else if (joint->GetScopedName().find("LShoulder") != std::string::npos)
      // Set the force for this joint.
      pid.Init(0, 0, 0, 0, 0, 100, -100);
    else
      // Set the force for this joint.
      pid.Init(10, 0, 0, 0, 0, 100, -100);*/

    //if (joint->GetScopedName().find("LShoulderRoll") != std::string::npos)
    //{
      // Set the force for this joint.
    pid.Init(25, 0, 0, 0, 0, 100, -100);

    // Apply the PID.
    jc->SetPositionPID(joint->GetScopedName(), pid);

      // Set the target position for the joint.
      // if (!jc->SetPositionTarget(joint->GetScopedName(), 1.0))
        // std::cerr << "PID Target failed\n";
    //}
  }
}

/////////////////////////////////////////////////
bool AgentPlugin::SendJoints(
  robocup_msgs::SendJoints::Request  &_req,
  robocup_msgs::SendJoints::Response &_res)
{
  std::cout << "SendJoints" << std::endl;

  boost::recursive_mutex::scoped_lock lock(this->mutex);

  boost::array<float, 22> jointValues = _req.joints;

  for (int i = 0; i < 22; ++i)
  {
    // Store the desired force.
    this->jointForces[i] = jointValues[i];
  }

  // Notify the gamecontroller plugin that I'm ready to accept new commands.
  // Hack: the content is not used.
  msgs::Time msg2;
  msg2.set_sec(1);
  msg2.set_nsec(1);
  // Notify the agent plugin that it's time to send state updates.
  this->readyPub->Publish(msg2);

  return true;
}

/////////////////////////////////////////////////
// Debugging
/*void AgentPlugin::OnMessageFromRobot(const robocup_msgs::Say::ConstPtr& _msg)
{
  boost::array<signed char, 20> message = _msg->message;
  std::cout << this->modelName << ": New message received from other robot."
            << std::endl;
}*/

/////////////////////////////////////////////////
void AgentPlugin::OnSyncReceived(ConstTimePtr &_msg)
{
  this->SendState();
}

/////////////////////////////////////////////////
void AgentPlugin::Update(const common::UpdateInfo &_info)
{
  /*boost::recursive_mutex::scoped_lock lock(this->mutex);

  // Torque control.
  for (int i = 0; i < 22; ++i)
  {
    // Get the joint.
    physics::JointPtr joint =
      this->model->GetJoint("naoH25V40::" + this->jointNames[i]);
    if (!joint)
    {
      std::cerr << "SendJoints() Joint [" << "naoH25V40::"
                << this->jointNames[i] << "] not found" << std::endl;
      continue;
    }

    // joint->SetForce(0, this->jointForces[i]);
  }*/

  // Position control.
  physics::JointControllerPtr jc = this->model->GetJointController();

  for (int i = 0; i < 22; ++i)
  {
    // Get the joint.
    physics::JointPtr joint =
      this->model->GetJoint("naoH25V40::" + this->jointNames[i]);
    if (!joint)
    {
      std::cerr << "SendJoints() Joint [" << "naoH25V40::" << this->jointNames[i]
                << "] not found" << std::endl;
      continue;
    }

    // Set the target position for the joint.
    if (!jc->SetPositionTarget(joint->GetScopedName(), this->jointForces[i]))
    //if (!jc->SetPositionTarget(joint->GetScopedName(), 0.0))
      std::cerr << "PID Target failed\n";
    //}
  }

  // Velocity control.
  /*physics::JointControllerPtr jc = this->model->GetJointController();

  for (int i = 0; i < 22; ++i)
  {
    // Get the joint.
    physics::JointPtr joint =
      this->model->GetJoint("naoH25V40::" + this->jointNames[i]);
    if (!joint)
    {
      std::cerr << "SendJoints() Joint [" << "naoH25V40::"
                << this->jointNames[i] << "] not found" << std::endl;
      continue;
    }

    // std::cout << this->jointForces[i] << std::endl;

    // Set the target position for the joint.
    if (!jc->SetVelocityTarget(joint->GetScopedName(), this->jointForces[i]))
      std::cerr << "PID Target failed\n";
  }
  //std::cout << "---" << std::endl;*/
}

/////////////////////////////////////////////////
void AgentPlugin::SendState()
{
  boost::recursive_mutex::scoped_lock lock(this->mutex);

  robocup_msgs::AgentState msg;

  // Output joint state data
  for (std::vector<physics::JointPtr>::iterator iter = this->joints.begin();
       iter != this->joints.end(); ++iter)
  {
    // Ignore the wrists.
    if ((*iter)->GetName().find("Wrist") == std::string::npos)
    {
      //msg.joint_name.push_back((*iter)->GetName());
      msg.joint_name.push_back(this->toAgent[(*iter)->GetName()]);
      msg.joint_angle_1.push_back((*iter)->GetAngle(0).Degree());
      if ((*iter)->HasType(physics::Base::UNIVERSAL_JOINT))
        msg.joint_angle_2.push_back((*iter)->GetAngle(1).Degree());
      else
        msg.joint_angle_2.push_back(0);
    }
  }

  // Output IMU data
  for (std::vector<sensors::ImuSensorPtr>::iterator iter =
       this->imuSensors.begin(); iter != this->imuSensors.end(); ++iter)
  {
    math::Vector3 angular = (*iter)->GetAngularVelocity();
    math::Vector3 linear = (*iter)->GetLinearAcceleration();

    msg.gyro_name.push_back((*iter)->GetName());
    msg.gyro_frame.push_back((*iter)->GetParentName());

    geometry_msgs::Vector3 angMsg, linMsg;

    angMsg.x = angular.x;
    angMsg.y = angular.y;
    angMsg.z = angular.z;

    linMsg.x = linear.x;
    linMsg.y = linear.y;
    linMsg.z = linear.z;

    msg.gyro_angular.push_back(angMsg);
    msg.gyro_linear.push_back(linMsg);
  }

  // Output touch sensor data
  for (std::vector<sensors::ContactSensorPtr>::iterator iter =
       this->touchSensors.begin(); iter != this->touchSensors.end(); ++iter)
  {
    msg.touch_name.push_back((*iter)->GetName());
    msg.touch_frame.push_back((*iter)->GetParentName());
    msg.touch_val.push_back((*iter)->GetCollisionCount() > 0);
  }

  // Output force sensor data
  for (std::vector<sensors::ContactSensorPtr>::iterator iter =
       this->forceSensors.begin(); iter != this->forceSensors.end(); ++iter)
  {
    msg.force_name.push_back((*iter)->GetName());
    msg.force_frame.push_back((*iter)->GetParentName());

    geometry_msgs::Vector3 origin, value;
    origin.x = origin.y = origin.z = 0;
    value.x = value.y = value.z = 0;

    std::map<std::string, physics::Contact> contacts = (*iter)->GetContacts(
        (*iter)->GetCollisionName(0));

    for (std::map<std::string, physics::Contact>::iterator citer =
         contacts.begin(); citer != contacts.end(); ++citer)
    {
      for (int i = 0; i < citer->second.count; ++i)
      {
        origin.x += citer->second.positions[i].x;
        origin.y += citer->second.positions[i].y;
        origin.z += citer->second.positions[i].z;

        if (citer->second.collision1->GetModel()->GetName() ==
            this->model->GetName())
        {
          value.x += citer->second.wrench[i].body1Force.x;
          value.y += citer->second.wrench[i].body1Force.y;
          value.z += citer->second.wrench[i].body1Force.z;
        }
        else
        {
          value.x += citer->second.wrench[i].body2Force.x;
          value.y += citer->second.wrench[i].body2Force.y;
          value.z += citer->second.wrench[i].body2Force.z;
        }
      }
      origin.x /= citer->second.count;
      origin.y /= citer->second.count;
      origin.z /= citer->second.count;
    }

    msg.force_origin.push_back(origin);
    msg.force_val.push_back(value);
  }

  std::map<std::string, math::Vector3> landmarks;

  landmarks["F1L"] = math::Vector3(-15, 10, 0);
  landmarks["F1R"] = math::Vector3(15, 10, 0);
  landmarks["F2R"] = math::Vector3(15, -10, 0);
  landmarks["F2L"] = math::Vector3(-15, -10, 0);

  landmarks["G1L"] = math::Vector3(-15, 1.05, 0.8);
  landmarks["G1R"] = math::Vector3(15, 1.05, 0.8);
  landmarks["G2L"] = math::Vector3(-15, -1.05, 0.8);
  landmarks["G2R"] = math::Vector3(15, -1.05, 0.8);
  landmarks["B"] = this->ball->GetWorldPose().pos;

  // \todo: Make this the camera pose
  math::Pose cameraPose = this->headLink->GetWorldPose() +
    this->cameraPoseNoise;
  math::Angle cameraYaw = cameraPose.rot.GetAsEuler().z;
  math::Angle cameraPitch = cameraPose.rot.GetAsEuler().y;

  // Check the landmarks
  for (std::map<std::string, math::Vector3>::iterator iter =
       landmarks.begin(); iter != landmarks.end(); ++iter)
  {
    double dx = iter->second.x - cameraPose.pos.x;
    double dy = iter->second.y - cameraPose.pos.y;
    double dz = iter->second.z - cameraPose.pos.z;

    double distance = math::Rand::GetDblNormal(0,
          this->distanceNoiseSigma) + cameraPose.pos.Distance(iter->second);
    double angle1 = GZ_RTOD(atan2(dy, dx)) - cameraYaw.Degree();
    double angle2 = GZ_RTOD(atan2(dz, distance)) - cameraPitch.Degree();

    // Both horizontal and vertical angles must be less than the
    // maximum view angle.
    if (fabs(angle1) < this->viewAngle*0.5 &&
        fabs(angle2) < this->viewAngle*0.5)
    {
      robocup_msgs::Landmark landmark;
      landmark.name = iter->first;
      landmark.bearing.distance = distance;
      landmark.bearing.angle1 = math::Rand::GetDblNormal(0,
          this->angle1NoiseSigma) + angle1;
      landmark.bearing.angle2 = math::Rand::GetDblNormal(0,
          this->angle2NoiseSigma) + angle2;
      msg.landmarks.push_back(landmark);

      /*
      std::cout << iter->first
        << "D[" << dx << " " << dy << " " << dz << "] "
        << "A1[" << GZ_RTOD(atan2(dy, dx)) << "] "
        << "A2[" << GZ_RTOD(atan2(dz, landmark.distance)) << "] "
        << "Y[" << cameraYaw.Degree() << "] "
        << "P[" << cameraPitch.Degree() << "] "
        << "A1R[" << landmark.angle1 << "] "
        << "A2R[" << landmark.angle2 << "]\n";
        */
    }
  }

  this->SendLines(msg);

  // Copy the game state.
  msg.game_state = this->gameState;

  msg.sim_time = ros::Time(this->model->GetWorld()->GetSimTime().Double());
  /*std::cout << "Gazebo time:" <<  this->model->GetWorld()->GetSimTime() << std::endl;
  std::cout << "Gazebo time (double):" <<  this->model->GetWorld()->GetSimTime().Double() << std::endl;
  std::cout << "ROS time:" << msg.sim_time << std::endl;*/

  this->agentStatePub.publish(msg);
}

/////////////////////////////////////////////////
void AgentPlugin::SendLines(robocup_msgs::AgentState &_msg)
{
  // \todo: Make this the camera pose
  math::Pose cameraPose = this->headLink->GetWorldPose() +
    this->cameraPoseNoise;
  math::Angle cameraYaw = cameraPose.rot.GetAsEuler().z;
  math::Angle cameraPitch = cameraPose.rot.GetAsEuler().y;

  double angle1 = cameraYaw.Radian() + GZ_DTOR(this->viewAngle/2.0);
  double angle2 = cameraYaw.Radian() - GZ_DTOR(this->viewAngle/2.0);

  Triangle triangle(math::Vector2d(cameraPose.pos.x, cameraPose.pos.y),
                    math::Vector2d(100 * cos(angle1), 100 * sin(angle1)),
                    math::Vector2d(100 * cos(angle2), 100 * sin(angle2)));

  for (std::vector<Line>::iterator iter = this->lines.begin();
       iter != this->lines.end(); ++iter)
  {
    math::Vector2d pts[2];
    if (triangle.Intersects(*iter, pts[0], pts[1]))
    {
      robocup_msgs::Line line;

      for (int i = 0; i < 2; ++i)
      {
        double dx = pts[i].x - cameraPose.pos.x;
        double dy = pts[i].y - cameraPose.pos.y;
        double dz = 0 - cameraPose.pos.z;

        double distance = math::Rand::GetDblNormal(0,
            this->distanceNoiseSigma) + cameraPose.pos.Distance(
              math::Vector3(pts[i].x, pts[i].y, 0));
        angle1 = GZ_RTOD(atan2(dy, dx)) - cameraYaw.Degree();
        angle2 = GZ_RTOD(atan2(dz, distance)) - cameraPitch.Degree();

        robocup_msgs::Bearing bearing;

        bearing.distance = distance;
        bearing.angle1 = math::Rand::GetDblNormal(0,
              this->angle1NoiseSigma) + angle1;
        bearing.angle2 = math::Rand::GetDblNormal(0,
            this->angle1NoiseSigma) + angle2;

        line.bearings.push_back(bearing);
      }

      _msg.lines.push_back(line);
    }
  }
}
