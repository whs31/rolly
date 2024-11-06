#pragma once

#include <rolly/global/platform_definitions.h>

#ifdef ROLLY_OS_ANDROID
#  include <thread>
#  include <jni.h>

namespace rolly::oslayer::android {
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
}  // namespace rolly::oslayer::android
#endif  // ROLLY_OS_ANDROID
