/**
 * @author Remy Varannes
 * @author Victor Paleologue
 * @author Julien Freche
 * Aldebaran Robotics (c) 2007-2012 All Rights Reserved
 */

#ifndef TIMELINEPRIVATE_H
#define TIMELINEPRIVATE_H

#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <boost/thread/condition_variable.hpp>
#include <boost/thread/recursive_mutex.hpp>
#include <boost/bind.hpp>
#include <alvalue/alvalue.h>
#include <alproxies/almemoryproxy.h>
#include <alproxies/almotionproxy.h>

#include "actuatorcurve.hpp"
#include "asyncexecuter.hpp"

namespace AL
{
  class XmlElement;
}

class TimelinePrivate : public asyncExecuter
{
public:
  enum MotionResourcesHandler
  {
    PASSIVE,
    WAITING,
    AGGRESSIVE,
  };

  TimelinePrivate(boost::shared_ptr<AL::ALBroker> broker);
  virtual ~TimelinePrivate(void);

  /**
   * loadFromXml.
   * load data about a timeline from a tinyXml element.
   * @param elt Pointer to data about the timeline in xml.
   */
  void loadFromXml(boost::shared_ptr<const AL::XmlElement> elt);

  void play(void);
  void pause(void);
  void stop(void);
  void goTo(const int &pFrame);
  int getSize() const;
  int getFPS(void) const;
  void setFPS(const int pFps);

  bool getEnabled() const;
  int getCurrentFrame() const;
  void setCurrentFrame(int pFrame);

  /**
   * setName will also insert an empty frame number in the STM, so Choregraphe
   * does not complaing about not finding a value.
   */
  void setName(const std::string& var);
  std::string getName() const;

private:
  virtual bool update(void);

  void killMotionOrders();
  void updateFrameInSTM(void);

  /**
   * Call this method at each update.
   * Returns true if success, false otherwise.
   * If false is returned, then we should not go to next keyframe...
   */
  bool executeCurveMotionCommand();

  /**
   * Interpolation computed for when we do gotos while stopped.
   */
  bool singleInterpolationCommand(int currentFrame);

  /**
   * executeCurveMotionCommand will call this method build all objects required to go further
   */
  bool prepareInterpolationCommand(int startFrame);

  /**
   * prepareInterpolationCommand will call this method, that will check for resources and then send command if everything is ok.
   */
  bool sendInterpolationCommand(const std::vector<std::string>& names, const AL::ALValue& times, const AL::ALValue& keys);
  void stimulateStoppedOutputs();
  void killTimer();
  void TimerLoop(int interval);

  boost::shared_ptr<AL::ALMemoryProxy>  _memoryProxy;
  boost::shared_ptr<AL::ALMotionProxy>  _motionProxy;
  int                                   _fps;
  std::vector<ActuatorCurve*>           _actuatorCurves;
  bool                                  _enabled;
  int                                   _currentFrame;
  int                                   _startFrame;
  /**
   * When this frame is encountered, exit the timeline.
   */
  int                                   _endFrame;
  int                                   _lastFrame;
  /**
   * The current 'angleInterpolationBezier' command.\n
   * Equals -1 if no interpolation at the moment.
   */
  int                                   _currentDoInterpolationMoveOrderId;
  /**
   * The name of the variable written in the stm to tell
   * choregraphe which frame is currently playing.\n
   */
  std::string                           _name;
  MotionResourcesHandler                _resourcesAcquisition;
  mutable boost::recursive_mutex        _methodMonitor;
};

#endif /* !TIMELINEPRIVATE_H */
