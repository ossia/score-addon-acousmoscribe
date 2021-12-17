#pragma once
#include <QColor>
#include <array>

namespace Acousmoscribe
{

constexpr const auto baseColor = qRgba(20, 20, 100, 255);
constexpr const auto drawColor = qRgba(250, 250, 255, 255);
constexpr const auto focusColor = qRgba(120, 120, 200, 255);
constexpr const auto selectColor = qRgba(180, 255, 255, 255);
constexpr const auto fillColor = Qt::transparent;

inline
std::array<float, 4> baseItemRect(double width, double height) noexcept
{
  const double h_margin = 0.2;
  float x0 = 0.f;
  float y0 = 5.0f;
  float w = width;
  float h = height * (1. - h_margin);
  return std::array<float, 4>{x0, y0, w, h};
}

inline
std::array<float, 4> buttonItemRect(double width, double height) noexcept
{
  const double h_margin = 0.2;
  float x0 = 0.f;
  float y0 = 5.0f + height * (1. - h_margin);
  float w = width;
  float h = h_margin * height - 10.;
  return std::array<float, 4>{x0, y0, w, h};
}
}
