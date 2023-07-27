#ifndef _SOLAR_PANEL_
#define _SOLAR_PANEL_
#include <chrono>
#include <cstdint>
#include <thread>
///
/// @brief solar heat provider
///
class SolarPanel {
public:
  ///
  /// @brief provide heat every second
  ///
  /// @return the heater generated every second
  uint32_t getHeat() const noexcept {
    std::this_thread::sleep_for(std::chrono::seconds(1U));
    return heat_;
  }

  ///
  /// @brief set heat every second
  ///
  /// @param newHeat new heat per second
  void setHeatPerSecond(const uint32_t newHeat)noexcept{
    this->heat_ = newHeat;
  }

  ///
  /// @brief provide heat every second
  ///
  /// @param heat heat energy per second
  SolarPanel(uint32_t heat) noexcept : heat_(heat) {
  }

  // no move and no copy, every system want to use heater, create one by itself
  SolarPanel(const SolarPanel &src) = delete;
  SolarPanel(SolarPanel &&src) = delete;
  SolarPanel &operator=(const SolarPanel &rhs) = delete;
  SolarPanel &operator=(SolarPanel &&rhs) = delete;

private:
  uint32_t heat_; ///< heat energy per second
};
#endif