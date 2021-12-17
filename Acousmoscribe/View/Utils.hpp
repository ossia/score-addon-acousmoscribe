#pragma once
#include <QColor>
#include <QTextOption>
#include <QFont>
#include <array>

namespace Acousmoscribe
{

constexpr const auto baseColor = qRgba(20, 20, 100, 255);
constexpr const auto drawColor = qRgba(250, 250, 255, 255);
constexpr const auto focusColor = qRgba(60, 180, 255, 255);
constexpr const auto selectColor = qRgba(0, 127, 255, 255);
constexpr const auto fillColor = Qt::transparent;


inline const auto& buttonTextOptions()
{
  static const auto opt = [] {
    QTextOption opt(Qt::AlignCenter);
    opt.setWrapMode(QTextOption::NoWrap);
    return opt;
  }();
  return opt;
}

inline
QFont buttonFont(double button_w, double button_h)
{
  double w_of_a_button = std::max(5., button_w - 5);
  double h_of_a_button = std::max(5., button_h - 5);

  double factor = std::min(w_of_a_button, h_of_a_button);

  return QFont("Sans", 0.3 * factor, QFont::Bold);
}

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
  float y0 = 7.0f + height * (1. - h_margin);
  float w = width;
  float h = h_margin * height - 10.;
  return std::array<float, 4>{x0, y0, w, h};
}
}
