#ifndef _pump_
#define _pump_
#include <memory>
#include <thread>
#include "SolarPanel.hpp"
#include "StorageTank.hpp"
class Pump final {
public:
  ///
  /// @brief create a pump
  ///
  /// @param solarPanel provide heat
  /// @param storageTank receive heat
  Pump(SolarPanel &solarPanel, StorageTank &storageTank) noexcept
      : solarPanel_(solarPanel), storageTank_(storageTank) {
  }
  // no delete constructor anymore
  Pump() = delete;

  // no move and no copy, every system want to use pump, create one by itself
  Pump(const Pump &src) = delete;
  Pump(Pump &&src) = delete;
  Pump &operator=(const Pump &rhs) = delete;
  Pump &operator=(Pump &&rhs) = delete;
  ~Pump() = default;

  ///< @brief turn on the pump, get heat from solar panel, and provide it to storage tank
  void turnOn() noexcept {
    if (!switch_ && this->storageTank_.getTemperature() < 100.0) {
      switch_ = true;
      auto runFun = [this]() noexcept(true) {
        while (this->switch_) {
          this->storageTank_.receiveHeat(this->solarPanel_.getHeat());
          std::cout << "storage tank info: volume(kg) = " << this->storageTank_.getVolume()
                    << ", temperature(°C) = " << this->storageTank_.getTemperature()
                    << ", ice volume(kg) = " << this->storageTank_.getIce() << std::endl;
          if (this->storageTank_.getTemperature() == 100.0) {
            this->switch_ = false;
            std::cout << "the water is boiled, stop the pump\n";
            break;
          }
        }
        std::this_thread::yield();
      };
      std::thread thread{runFun};
      thread.detach();
    }
  }

  ///< @brief turn off the pump
  void turnOff() noexcept {
    auto turnOffFun = [this]() noexcept(true) {
      this->switch_ = false;
    };
    std::thread thread{turnOffFun};
    thread.detach();
  }

private:
  SolarPanel &solarPanel_;                           ///< solar panel provide heat
  StorageTank &storageTank_;                         ///< storage tank for water storage
  bool switch_ = false;                              ///< virtual switch
  std::unique_ptr<std::thread> currentRunningThread; ///< pump thread
};
#endif