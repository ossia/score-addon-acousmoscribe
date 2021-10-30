#ifndef RHYTHM_DEFINE
#define RHYTHM_DEFINE

#include <iostream>

#include <verdigris>

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
    bool isRandom{false};
    bool operator==(const RhythmicProfile&) const noexcept = default;
};

}
#endif
