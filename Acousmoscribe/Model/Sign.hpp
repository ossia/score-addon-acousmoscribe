#pragma once
#include <score/model/IdentifiedObject.hpp>
#include <score/selection/Selectable.hpp>

#include <verdigris>
#include <iostream>

#include "Grain.hpp"
//#include "DynamicProfile.hpp"
#include "MelodicProfile.hpp"
#include "RhythmicProfile.hpp"


namespace Acousmoscribe
{

//Dynamic Profile :
struct DynamicProfile {
    float volumeStart{1};
    float volumeEnd{1};
    bool operator==(const DynamicProfile&) const noexcept = default;
};

struct SignData
{
    double start{};
    double duration{};

    Grain grain{smooth};
    DynamicProfile dynamicProfile{};
    MelodicProfile melodicProfile{};
    RhythmicProfile rhythmicProfile{};

    bool operator==(const SignData&) const noexcept = default;
};

class Sign final : public IdentifiedObject<Sign>
{
    W_OBJECT(Sign)
    SCORE_SERIALIZE_FRIENDS

public:
    Selectable selection;

    Sign(const Id<Sign>& id, QObject* parent);
    Sign(const Id<Sign>& id, SignData s, QObject* parent);

    template <typename DeserializerVisitor, enable_if_deserializer<DeserializerVisitor>* = nullptr>
    Sign(DeserializerVisitor&& vis, QObject* parent) : IdentifiedObject<Sign>{vis, parent}
    {
        vis.writeTo(*this);
    }

    double start() const noexcept;
    double duration() const noexcept;
    double end() const noexcept;
    const DynamicProfile& dynamicProfile() const;
    const MelodicProfile& melodicProfile() const;
    const RhythmicProfile& rhythmicProfile() const;
    Grain grain() const;

    void scale(double s) noexcept;
    void setStart(double s) noexcept;
    void setDuration(double s) noexcept;
    void setDynamicProfile(DynamicProfile d);
    void setMelodicProfile(MelodicProfile d);
    void setRhythmicProfile(RhythmicProfile d);
    void setGrain(Grain g);

    void setData(SignData d);
    SignData signData() const;
    void signChanged() W_SIGNAL(signChanged);
    PROPERTY(SignData, signData READ signData WRITE setData NOTIFY signChanged)

private:
    SignData m_impl;
};
}
