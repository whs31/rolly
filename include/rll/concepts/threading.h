#pragma once

#include <type_traits>
#include <chrono>
#include <rll/global/definitions.h>

#if __cplusplus >= 202'002L
#  include <concepts>
#endif

namespace rll {
#if __cplusplus >= 202'002L || defined(RLL_DOC)
  /**
   * @brief Basic lockable concept.
   * @details Describes the minimal characteristics of a lockable object.
   * <ul>
   *   <li>Blocks until a lock can be acquired for the current execution agent (thread, process,
   * task). If an exception is thrown, no lock is acquired.</li> <li>Releases the non-shared lock
   * held by the execution agent. Throws no exceptions.</li>
   * </ul>
   * @tparam T Type to check.
   * @note Available only in C++20 mode.
   * @sa https://en.cppreference.com/w/cpp/named_req/BasicLockable
   * @see lockable
   * @see mutex
   */
  template <typename T>
  concept basic_lockable = requires(T t) {
    { t.lock() };
    { t.unlock() };
  };

  /**
   * @brief Lockable concept.
   * @details The Lockable requirements extends the BasicLockable requirements to include attempted
   * locking.
   *
   * For type to be lockable, it must meet the following requirements:
   * <ul>
   *   <li>Satisfies the BasicLockable requirements</li>
   *   <li>Provide a <code>try_lock</code> method: Attempts to acquire the lock for the current
   * execution agent (thread, process, task) without blocking. If an exception is thrown, no lock is
   * obtained.</li>
   * </ul>
   * @tparam T Type to check.
   * @note Available only in C++20 mode.
   * @sa https://en.cppreference.com/w/cpp/named_req/Lockable
   * @see basic_lockable
   */
  template <typename T>
  concept lockable = basic_lockable<T> and requires(T t) {
    { t.try_lock() } -> std::convertible_to<bool>;
  };

  /**
   * @brief Timed lockable concept.
   * @details Describes the requirements for a lockable object that can be locked for a specified
   * time period.
   *
   * For type to be timed lockable, it must meet the following requirements:
   * <ul>
   *   <li>Satisfies the Lockable requirements</li>
   *   <li>Provide a <code>try_lock_for</code> method: Blocks for the provided duration rel_time or
   * until a lock on m is acquired. Returns true if a lock is acquired, false otherwise.</li>
   * <li>Provide a <code>try_lock_until</code> method: Blocks until the provided time_point or until
   * a lock on m is acquired. Returns true if a lock is acquired, false otherwise.</li>
   * </ul>
   * @tparam T Type to check.
   * @tparam Duration Duration type.
   * @tparam TimePoint Time point type.
   * @note Available only in C++20 mode.
   * @sa https://en.cppreference.com/w/cpp/named_req/TimedLockable
   * @see lockable
   * @see basic_lockable
   */
  template <typename T, typename Duration, typename TimePoint>
  concept timed_lockable = lockable<T>
                       and requires(
                             T t,
                             std::chrono::duration<Duration> rel_time,
                             std::chrono::time_point<TimePoint> abs_time
                       ) {
                             { t.try_lock_for(rel_time) } -> std::convertible_to<bool>;
                             { t.try_lock_until(abs_time) } -> std::convertible_to<bool>;
                           };

  /**
   * @brief Shared lockable concept.
   * @details The SharedLockable requirements describe the minimal characteristics of types
   * that provide shared blocking semantics for execution agents (i.e. threads).
   *
   * For type to be shared lockable, it must meet the following requirements:
   * <ul>
   *   <li>Provide a <code>lock_shared</code> method: Blocks until a lock can be obtained for the
   * current execution agent (thread, process, task). If an exception is thrown, no lock is
   * obtained.</li> <li>Provide an <code>try_lock_shared</code> method: Attempts to obtain a lock
   * for the current execution agent (thread, process, task) without blocking. If an exception is
   * thrown, no lock is obtained. Returns true if a lock is obtained, false otherwise.</li>
   * <li>Provide an <code>unlock_shared</code> method: Releases the shared lock held by the
   * execution agent. Throws no exceptions.</li>
   * </ul>
   * @tparam T Type to check.
   * @note Available only in C++20 mode.
   * @sa https://en.cppreference.com/w/cpp/named_req/SharedLockable
   * @see lockable
   * @see shared_timed_lockable
   * @see timed_lockable
   */
  template <typename T>
  concept shared_lockable = lockable<T> and requires(T t) {
    { t.lock_shared() };
    { t.try_lock_shared() } -> std::convertible_to<bool>;
    { t.unlock_shared() };
  };

  /**
   * @brief Shared timed lockable concept.
   * @details The SharedTimedLockable requirements describe the characteristics of types that
   * provide timed shared blocking semantics for execution agents (threads, processes, tasks).
   *
   * For type to be shared timed lockable, it must meet the following requirements:
   * <ul>
   *   <li>Satisfies the SharedLockable requirements</li>
   *   <li>Provide a <code>try_lock_shared_for</code> method: Blocks for the provided duration
   * rel_time or until a lock on m is acquired. Returns true if a lock is acquired, false
   * otherwise.</li> <li>Provide a <code>try_lock_shared_until</code> method: Blocks until the
   * provided time_point or until a lock on m is acquired. Returns true if a lock is acquired, false
   * otherwise.</li>
   * </ul>
   * @tparam T Type to check.
   * @tparam Duration Duration type.
   * @tparam TimePoint Time point type.
   * @note Available only in C++20 mode.
   * @sa https://en.cppreference.com/w/cpp/named_req/SharedTimedLockable
   * @see shared_lockable
   * @see timed_lockable
   * @see lockable
   */
  template <typename T, typename Duration, typename TimePoint>
  concept shared_timed_lockable = timed_lockable<T, Duration, TimePoint>
                              and requires(
                                    T t,
                                    std::chrono::duration<Duration> rel_time,
                                    std::chrono::time_point<TimePoint> abs_time
                              ) {
                                    {
                                      t.try_lock_shared_for(rel_time)
                                    } -> std::convertible_to<bool>;
                                    {
                                      t.try_lock_shared_until(abs_time)
                                    } -> std::convertible_to<bool>;
                                  };

  /**
   * @brief Mutex concept.
   * @details The Mutex requirements extends the Lockable requirements to include inter-thread
   * synchronization.
   *
   * For type to be mutex, it must meet the following requirements:
   * <ul>
   *   <li>Satisfies the Lockable requirements</li>
   *   <li>Be default constructible</li>
   *   <li>Be destructible</li>
   *   <li>Be not copyable</li>
   *   <li>Be not movable</li>
   * </ul>
   * @tparam T Type to check.
   * @note Available only in C++20 mode.
   * @sa https://en.cppreference.com/w/cpp/named_req/Mutex
   * @see shared_mutex
   * @see shared_timed_mutex
   * @see timed_mutex
   * @see lockable
   */
  template <typename T>
  concept mutex = lockable<T> and std::default_initializable<T> and std::destructible<T>
              and not std::copy_constructible<T> and not std::move_constructible<T>;

  /**
   * @brief Timed mutex concept.
   * @details The TimedMutex requirements extends the TimedLockable requirements to include
   * inter-thread synchronization.
   *
   * For type to be timed mutex, it must meet the following requirements:
   * <ul>
   *   <li>Satisfies the TimedLockable requirements</li>
   *   <li>Satisfies the Mutex requirements</li>
   * </ul>
   * @tparam T Type to check.
   * @tparam Duration Duration type.
   * @tparam TimePoint Time point type.
   * @note Available only in C++20 mode.
   * @sa https://en.cppreference.com/w/cpp/named_req/TimedMutex
   * @see shared_timed_mutex
   * @see mutex
   * @see shared_mutex
   */
  template <typename T, typename Duration, typename TimePoint>
  concept timed_mutex = timed_lockable<T, Duration, TimePoint> and mutex<T>;

  /**
   * @brief Shared mutex concept.
   * @details The SharedMutex requirements extends the SharedLockable requirements to include
   * inter-thread synchronization.
   *
   * For type to be shared mutex, it must meet the following requirements:
   * <ul>
   *   <li>Satisfies the SharedLockable requirements</li>
   *   <li>Satisfies the Mutex requirements</li>
   * </ul>
   * @tparam T Type to check.
   * @note Available only in C++20 mode.
   * @sa https://en.cppreference.com/w/cpp/named_req/SharedMutex
   * @see shared_timed_mutex
   * @see timed_mutex
   * @see mutex
   */
  template <typename T>
  concept shared_mutex = shared_lockable<T> and mutex<T>;

  /**
   * @brief Shared timed mutex concept.
   * @details The SharedTimedMutex requirements extends the SharedTimedLockable requirements to
   * include inter-thread synchronization.
   *
   * For type to be shared timed mutex, it must meet the following requirements:
   * <ul>
   *   <li>Satisfies the SharedTimedLockable requirements</li>
   *   <li>Satisfies the Mutex requirements</li>
   * </ul>
   * @tparam T Type to check.
   * @tparam Duration Duration type.
   * @tparam TimePoint Time point type.
   * @note Available only in C++20 mode.
   * @sa https://en.cppreference.com/w/cpp/named_req/SharedTimedMutex
   * @see shared_mutex
   * @see timed_mutex
   * @see mutex
   */
  template <typename T, typename Duration, typename TimePoint>
  concept shared_timed_mutex = shared_timed_lockable<T, Duration, TimePoint> and mutex<T>;
#endif
}  // namespace rll
