#ifndef RHYTHM_DEFINE
#define RHYTHM_DEFINE

#include <iostream>

#include <verdigris>
#include "Variation.hpp"
namespace Acousmoscribe{
enum Speed
{
    continuous = 0, //no rhythm
    slow,
    medium,
    fast
};

enum Acceleration
{
    constant =0,
    accelerating,
    decelerating
};

struct RhythmicProfile
{
    Speed speed{continuous};
    Acceleration acceleration{constant};
    Variation variation{none};

    bool operator==(const RhythmicProfile&) const noexcept = default;
};

}
#endif
