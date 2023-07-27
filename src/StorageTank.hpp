#ifndef _STORAGE_TANK_
#define _STORAGE_TANK_
#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <thread>
const uint32_t WaterSpecificHeatCapacity = 4200U;
const uint32_t IceSpecificHeatCapacity = 2090U;
const float AbsoluteZero = -273;
/// @brief water storage tank
class StorageTank {
public:
  ///
  /// @brief create a water storage tank
  ///
  /// @param volume init water volume
  /// @param temperature init water temperature
  explicit StorageTank(const uint32_t volume, const float temperature)
      : volume_(volume), temperature_(temperature) {
    if (volume == 0) {
      temperature_ = 0.0;
    }
    if (temperature_ > 100.0) {
      throw std::runtime_error("water temperature cannot above 100 degree");
    }
    if (temperature_ < AbsoluteZero) {
      throw std::runtime_error("water temperature cannot below absolute zero(-273) degree");
    }
    if (volume > maxVolume_) {
      throw std::runtime_error("cannot add water more than the max volume of the tank");
    }
  }

  /// @brief get current temperature
  float getTemperature() const {
    return this->temperature_;
  }

  ///
  /// @brief add water to this tank
  ///
  /// @param volume water volume
  /// @param temperature water temperature
  void addWater(const uint32_t volume, const float temperature) {
    if (volume + volume_ > maxVolume_) {
      throw std::runtime_error("cannot add water more than the max volume of the tank");
    }
    if (temperature < AbsoluteZero) {
      throw std::runtime_error("water temperature cannot below absolute zero(-273) degree");
    }
    if (temperature > 100.0) {
      throw std::runtime_error("water temperature cannot above 100 degree");
    }
    temperature_ = ((volume * temperature) + (volume_ * temperature_)) / (volume_ + volume);
    volume_ += volume;
  }

  ///
  /// @brief receive heat from outside, change the temperature, the water could be boiled
  ///
  /// @param volume water volume
  /// @param temperature water temperature
  void receiveHeat(const uint32_t heat) noexcept {

    // fixme -1 degree to 1 degree
    // fix me consider Enthalpy of fusion 

    temperature_ += (heat /
                     static_cast<float>(this->temperature_ < 0 ? IceSpecificHeatCapacity
                                                               : WaterSpecificHeatCapacity) /
                     volume_);
    if (temperature_ >= 100U) {
      std::cout << "warning message: the water is boiled\n";
      temperature_ = 100.0;
    }
  }

  // no move and no copy, every system want to use water storage tank, create one by itself
  StorageTank(const StorageTank &src) = delete;
  StorageTank(StorageTank &&src) = delete;
  StorageTank &operator=(const StorageTank &rhs) = delete;
  StorageTank &operator=(StorageTank &&rhs) = delete;

private:
  uint32_t volume_;          ///< current volume
  float temperature_;        ///< current temperature
  uint32_t maxVolume_ = 100; ///< max volume is 100 kg
};
#endif