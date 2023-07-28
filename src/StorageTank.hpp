#ifndef _STORAGE_TANK_
#define _STORAGE_TANK_
#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <thread>
const uint32_t WaterSpecificHeatCapacity = 4200U;
const uint32_t IceSpecificHeatCapacity = 2090U;
const uint32_t EnthalpyOfFusion = 334'000U;
const float AbsoluteZero = -273;
/// @brief water storage tank
class StorageTank final {
public:
  ///
  /// @brief create a water storage tank
  ///
  /// @param volume total init water and ice volume
  /// @param temperature init water temperature
  /// @param iceVolume ice volume
  explicit StorageTank(const float volume, const float temperature, const float iceVolume = 0U)
      : volume_(volume), temperature_(temperature), ice_(iceVolume) {
    if (volume == 0) {
      temperature_ = 0.0;
    }
    if (temperature_ > 100.0) {
      throw std::runtime_error("water temperature cannot above 100 degree");
    }
    if (temperature_ < AbsoluteZero) {
      throw std::runtime_error("water temperature cannot below absolute zero(-273) degree");
    }
    if (volume > static_cast<float>(maxVolume_)) {
      throw std::runtime_error("cannot add water more than the max volume of the tank");
    }
    if (iceVolume != 0 && temperature > 0) {
      throw std::runtime_error("cannot init water storage tank with ice and temperature > 0");
    }
    if (iceVolume > volume) {
      throw std::runtime_error("ice volume cannot greater than total volume");
    }
    if (temperature < 0 && iceVolume != volume) {
      throw std::runtime_error("temperature < 0, then ice volume shoule be total volume");
    }
  }

  /// @brief get current temperature
  float getTemperature() const noexcept {
    return this->temperature_;
  }

  /// @brief get current ice volume
  float getIce() const noexcept {
    return this->ice_;
  }

  /// @brief get current  volume
  float getVolume() const noexcept {
    return this->volume_;
  }

  ///
  /// @brief receive heat from outside, change the temperature, the water could be boiled
  ///
  /// @param heat heat from outside
  void receiveHeat(const uint32_t heat) noexcept {
    float availableHeat = static_cast<float>(heat);
    while (availableHeat > 0) {
      if (temperature_ < 0) {
        float toZeroHeat = IceSpecificHeatCapacity * volume_ * (-temperature_);
        if (availableHeat > toZeroHeat) {
          availableHeat -= toZeroHeat;
          temperature_ = 0;
        } else {
          temperature_ += availableHeat / (static_cast<float>(IceSpecificHeatCapacity) * volume_);
          availableHeat = 0;
        }
        continue;
      }
      if (temperature_ == 0) {
        if (ice_ > 0) {
          const float iceFusionHeat = ice_ * EnthalpyOfFusion;
          if (iceFusionHeat > availableHeat) {
            ice_ = ice_ - static_cast<float>(availableHeat) / EnthalpyOfFusion;
            availableHeat = 0;
          } else {
            ice_ = 0;
            availableHeat -= iceFusionHeat;
          }
          continue;
        }
      }

      if (ice_ == 0 && temperature_ >= 0) {
        float increaseTemperature =
            ((availableHeat / static_cast<float>(WaterSpecificHeatCapacity)) / volume_);
        temperature_ += increaseTemperature;
        availableHeat = 0;
        continue;
      }
    }
    if (temperature_ >= 100.0) {
      std::cout << "warning message: the water is boiled\n";
      temperature_ = 100.0;
    }
  }

  // no move and no copy, every system want to use water storage tank, create one by itself
  StorageTank(const StorageTank &src) = delete;
  StorageTank(StorageTank &&src) = delete;
  StorageTank &operator=(const StorageTank &rhs) = delete;
  StorageTank &operator=(StorageTank &&rhs) = delete;
  ~StorageTank() = default;

private:
  float volume_;              ///< current volume
  float temperature_;         ///< current temperature
  uint32_t maxVolume_ = 100U; ///< max volume is 100 kg
  float ice_ = 0U;            ///< ice volume in this tank
};
#endif