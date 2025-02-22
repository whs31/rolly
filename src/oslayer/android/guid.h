#pragma once

#include <rll/global/platform_definitions.h>

#if defined(RLL_OS_ANDROID) || defined(DOXYGEN)
#  include <thread>
#  include <jni.h>

namespace rll::oslayer::android {
  struct guid_info {
    [[nodiscard]] guid_info from_env(JNIEnv* env) const;
    JNIEnv* env = nullptr;
    jclass uuid_class;
    jmethodID new_guid_method;
    jmethodID most_significant_bits_method;
    jmethodID least_significant_bits_method;
    std::thread::id init_thread_id;
  };

  extern guid_info info;
  void init_jni(JNIEnv* env);
}  // namespace rll::oslayer::android
#endif  // RLL_OS_ANDROID
