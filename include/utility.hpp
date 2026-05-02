#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <string>

namespace Game {
inline constexpr unsigned int WINDOW_WIDTH = 800;
inline constexpr unsigned int WINDOW_HEIGHT = 600;
inline constexpr float PLAYER_SPEED = 200.0f;
inline constexpr float PLAYER_RADIUS = 30.0f;

std::string getInfo();
} // namespace Game

#endif