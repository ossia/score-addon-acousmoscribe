#pragma once
#include <verdigris>
#include "Variation.hpp"
namespace Acousmoscribe{

enum Grain
{
    smooth = 0,
    fine,
    sharp,
    big
};
struct GrainProfile{
  Grain grain{smooth};
  Variation variation{none};
  bool operator==(const GrainProfile& other) const noexcept = default;
};
}
