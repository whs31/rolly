#pragma once

namespace leaf::pattern
{
  /**
   * \brief Принимающая часть паттерна "Наблюдатель"
   * \details Класс, реализующий интерфейс IObserver, может подписываться на события, сообщаемые объектом IObservable.
   * Метод update вызывается при возникновении события IObservable.
   * \tparam T Аргументы в сигнатуре метода update
   * \version 3.0.0
   * \see IObservable
   */
  template<typename... T>
  class IObserver
  {
    public:
      virtual ~IObserver() = default;
      virtual auto update(T... args) -> void = 0;
  };
}
