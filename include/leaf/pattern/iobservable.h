#pragma once

#include <list>
#include <leaf/global.h>
#include <leaf/pattern/iobserver.h>
#include <leaf/utils/rtti.h>

namespace leaf::pattern
{
  using std::list;

  template<typename... T>
  class IObservable
  {
    public:
      auto subscribe(IObserver<T...>* observer) -> void;
      auto unsubscribe(IObserver<T...>* observer) -> void;
      auto notify(T... args) const -> void;

      auto operator+=(IObserver<T...>* observer) -> void;
      auto operator-=(IObserver<T...>* observer) -> void;

    private:
      list<IObserver<T...>*> m_observers;
  };

  template<typename... T>
  auto IObservable<T...>::subscribe(IObserver<T...>* observer) -> void
  {
    this->m_observers.push_back(observer);
    llog::trace("subscriber added, {} total ({})", this->m_observers.size(), utils::type_name<decltype(this)>());
  }

  template<typename... T>
  auto IObservable<T...>::unsubscribe(IObserver<T...>* observer) -> void
  {
    this->m_observers.remove(observer);
    llog::trace("subscriber removed, {} total ({})", this->m_observers.size(), utils::type_name<decltype(this)>());
  }

  template<typename... T>
  auto IObservable<T...>::notify(T... args) const -> void
  {
    llog::trace("notifying {} observers of type {} ({})",
      this->m_observers.size(),
      utils::type_name<IObserver<T...>*>(),
      utils::type_name<decltype(this)>()
    );
    for(const auto observer : this->m_observers)
      observer->update(args...);
  }

  template<typename... T>
  auto IObservable<T...>::operator+=(IObserver<T...>* observer) -> void { this->subscribe(observer); }

  template<typename... T>
  auto IObservable<T...>::operator-=(IObserver<T...>* observer) -> void { this->unsubscribe(observer); }
}

