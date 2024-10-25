#include "guid.h"

#ifdef ROLLY_OS_ANDROID
namespace rolly::oslayer::android
{
  guid_info info;

  guid_info guid_info::from_env(JNIEnv* env) const {
    auto i = guid_info();
    i.env = env;
    auto local_uuid_class = env->FindClass("java/util/UUID");
    i.uuid_class = (jclass)env->NewGlobalRef(local_uuid_class);
    env->DeleteLocalRef(local_uuid_class);
    i.new_guid_method = env->GetStaticMethodID(i.uuid_class, "randomUUID", "()Ljava/util/UUID;");
    i.most_significant_bits_method = env->GetMethodID(i.uuid_class, "getMostSignificantBits", "()J");
    i.least_significant_bits_method = env->GetMethodID(i.uuid_class, "getLeastSignificantBits", "()J");
    i.init_thread_id = std::this_thread::get_id();
    return i;
  }

  void init_jni(JNIEnv* env) {
    info = guid_info::from_env(env);
  }
} // namespace rolly::oslayer::android
#endif // ROLLY_OS_ANDROID