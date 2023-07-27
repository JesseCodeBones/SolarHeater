#ifndef _pump_
#define _pump_
#include <memory>
#include <thread>
#include "SolarPanel.hpp"
#include "StorageTank.hpp"
class Pump {
public:
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

  void turnOn() noexcept {

    if (!switch_ && this->storageTank_.getTemperature() < 100.0) {
      switch_ = true;
      auto runFun = [this]() {
        while (this->switch_) {
          this->storageTank_.receiveHeat(this->solarPanel_.getHeat());
          std::cout << "new temperature is :" << this->storageTank_.getTemperature() << std::endl;
          if (this->storageTank_.getTemperature() == 100.0) {
            this->switch_ = false;
            std::cout << "the water is boiled, stop the pump\n";
            break;
          }
        }
      };
      std::thread thread{runFun};
      thread.detach();
    }
  }

  void turnOff() {

    auto turnOffFun = [this]() {
      this->switch_ = false;
    };
    std::thread thread{turnOffFun};
    thread.detach();
  }

private:
  SolarPanel &solarPanel_;   ///< solar panel provide heat
  StorageTank &storageTank_; ///< storage tank for water storage
  bool switch_ = false;
  std::unique_ptr<std::thread> currentRunningThread;
};
#endif