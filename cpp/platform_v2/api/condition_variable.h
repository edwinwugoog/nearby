#ifndef PLATFORM_V2_API_CONDITION_VARIABLE_H_
#define PLATFORM_V2_API_CONDITION_VARIABLE_H_

#include "platform_v2/base/exception.h"
#include "absl/time/clock.h"

namespace location {
namespace nearby {
namespace api {

// The ConditionVariable class is a synchronization primitive that can be used
// to block a thread, or multiple threads at the same time, until another thread
// both modifies a shared variable (the condition), and notifies the
// ConditionVariable.
class ConditionVariable {
 public:
  virtual ~ConditionVariable() {}

  // Notifies all the waiters that condition state has changed.
  virtual void Notify() = 0;

  // Waits indefinitely for Notify to be called.
  // May return prematurely in case of interrupt, if supported by platform.
  // Returns kSuccess, or kInterrupted on interrupt.
  virtual Exception Wait() = 0;

  // Waits while timeout has not expired for Notify to be called.
  // May return prematurely in case of interrupt, if supported by platform.
  // Returns kSuccess, or kInterrupted on interrupt.
  virtual Exception Wait(absl::Duration timeout) = 0;
};

}  // namespace api
}  // namespace nearby
}  // namespace location

#endif  // PLATFORM_V2_API_CONDITION_VARIABLE_H_
