#pragma once
#include <iostream>

//#include <verdigris>

namespace Acousmoscribe{

enum Pitch
{
    very_high = 0,
    high,
    mid_high,
    mid,
    mid_low,
    low,
    very_low
};

enum Variation
{
    none = 0,
    random_variation
};

enum Pace{
    unknow =0,
    lento,
    moderato,
    presto
};

struct MelodicProfile
{
    Pitch pitch{mid};
    Pitch pitchEnd{mid};
    Variation var{none};
    Pace pace{unknow};

    bool operator==(const MelodicProfile& other) const noexcept = default;
};

}
