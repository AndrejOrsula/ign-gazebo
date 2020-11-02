/*
 * Copyright (C) 2020 Open Source Robotics Foundation
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
#ifndef IGNITION_GAZEBO_SYSTEMS_LOGICAL_AUDIO_SENSOR_PLUGIN_HH_
#define IGNITION_GAZEBO_SYSTEMS_LOGICAL_AUDIO_SENSOR_PLUGIN_HH_

#include <memory>

#include <ignition/gazebo/System.hh>

namespace ignition
{
namespace gazebo
{
// Inline bracket to help doxygen filtering.
inline namespace IGNITION_GAZEBO_VERSION_NAMESPACE {
namespace systems
{
  // Forward Declaration
  class LogicalAudioSensorPluginPrivate;

  /// \brief A plugin for trivial audio detection.
  ///
  /// Each <plugin> tag can only accept one sensor (either a sound source
  /// or microphone).
  /// In order to create multiple audio sources and microphones,
  /// multiple <plugin> tags must be used (one for each sensor).
  /// After each simulation step, microphones check if audio
  /// was detected by any sources in the world.
  /// No audio is actually played to an audio device
  /// such as speakers - this plugin is meant to check if audio
  /// could theoretically be heard at a certain location or not.
  ///
  /// Secifying an audio source via SDF is done as follows:
  ///
  /// <audiosource> A new audiosource in the environment, which has the
  ///   following child elements:
  ///     * <id> The source ID, which must be unique and >= 0.
  ///     * <pose> The pose, expressed as "x y z roll pitch yaw".
  ///       Each pose coordinate must be separated by whitespace.
  ///       The pose is defined relative to the plugin's parent element.
  ///       So, if the plugin is used inside of a <model> tag, then the
  ///       source's <pose> is relative to the model's pose.
  ///       If the plugin is used inside of a <world> tag, then the source's
  ///       <pose> is relative to the world (i.e., <pose> specifies an
  ///       absolute pose).
  ///     * <attenuationfunction> The attenuation function.
  ///       See logical_audio::AttenuationFunction for a list of valid
  ///       attenuation functions, and logical_audio::SetAttenuationFunction
  ///       for how to specify an attenuation function in SDF.
  ///     * <attenuationshape> The attenuation shape.
  ///       See logical_audio::AttenuationShape for a list of valid
  ///       attenuation shapes, and logical_audio::SetAttenuationShape for how
  ///       to specify an attenuation shape in SDF.
  ///     * <innerradius> The inner radius of the attenuation shape.
  ///       This value must be >= 0.0. The volume of the source will be
  ///       <audiosource><volume> at locations that have a distance <= inner
  ///       radius from the source.
  ///     * <falloffdistance> The falloff distance. This value must be greater
  ///       than the value of the source's <innerradius>. This defines the
  ///       distance from the audio source where the volume becomes 0.
  ///     * <volumelevel> The volume level emitted from the source. This must
  ///       be a value between 0.0 and 1.0 (representing 0% to 100%).
  ///     * <playing> Whether the source should play immediately or not.
  ///       Use true to initiate audio immediately, and false otherwise.
  ///     * <playduration> The duration (in seconds) audio is played from the
  ///       source. This value must be >= 0. A value of 0 means that the source
  ///       will play for an infinite amount of time.
  ///
  /// Specifying a microphone via SDF is done as follows:
  ///
  /// <microphone> A new microphone in the environment,
  ///   which has the following child elements:
  ///     * <id> The microphone ID, which must be unique and >= 0.
  ///     * <pose> The pose, expressed as "x y z roll pitch yaw".
  ///       Each pose coordinate must be separated by whitespace.
  ///       The pose is defined relative to the plugin's parent element.
  ///       So, if the plugin is used inside of a <model> tag, then the
  ///       source's <pose> is relative to the model's pose.
  ///       If the plugin is used inside of a <world> tag, then the source's
  ///       <pose> is relative to the world (i.e., <pose> specifies an
  ///       absolute pose).
  ///     * <volumedetectionthreshold> The minimum volume level the microphone
  ///       can detect. This must be a value between 0.0 and 1.0
  ///       (representing 0% to 100%).
  ///
  /// Sources can be started and stopped via ignition service calls.
  /// If a source is successfully created, the following services will be
  /// created for the source (where <id> is the value specified in the source's
  /// <id> tag from the SDF):
  ///     * /play_source_<id>
  ///         * Starts playing the source with the specified ID.
  ///           If the source is already playing, nothing happens.
  ///     * /stop_source_<id>
  ///         * Stops playing the source with the specified ID.
  ///           If the source is already stopped, nothing happens.
  class IGNITION_GAZEBO_VISIBLE LogicalAudioSensorPlugin :
    public System,
    public ISystemConfigure,
    public ISystemPreUpdate,
    public ISystemPostUpdate
  {
    /// \brief Constructor
    public: LogicalAudioSensorPlugin();

    /// \brief Destructor
    public: ~LogicalAudioSensorPlugin() override;

    /// Documentation inherited
    public: void Configure(const Entity &_entity,
                const std::shared_ptr<const sdf::Element> &_sdf,
                EntityComponentManager &_ecm,
                EventManager &_eventMgr) override;

    // Documentation inherited
    public: void PreUpdate(const ignition::gazebo::UpdateInfo &_info,
                ignition::gazebo::EntityComponentManager &_ecm) override;

    /// Documentation inherited
    public: void PostUpdate(const UpdateInfo &_info,
                const EntityComponentManager &_ecm) override;

    /// \brief Private data pointer
    private: std::unique_ptr<LogicalAudioSensorPluginPrivate> dataPtr;
  };
}
}
}
}

#endif