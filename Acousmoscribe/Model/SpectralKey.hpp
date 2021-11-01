#pragma once
#include <score/model/IdentifiedObject.hpp>

#include <score/selection/Selectable.hpp>

#include <verdigris>
#include <iostream>

namespace Acousmoscribe {

enum Nature
{
    null = 0,
    tonic,
    inharmonic,
    noise,
    tonic_inharmonic,
    noisy_tonic,
    noisy_inharmonic,
};


 struct SpectralKeyData
{
    Nature nature{};
    Nature nature2{};
    bool isHybrid{false};
    bool isHybrid2{false};
    bool isRich{false};
    bool isRich2{false};
    bool isWarped{false};
    bool isWarped2{false};

    bool operator==(const SpectralKeyData& other) const noexcept = default;
};

class SpectralKey final : public IdentifiedObject<SpectralKey>
{
    W_OBJECT(SpectralKey)
    SCORE_SERIALIZE_FRIENDS

public:
    Selectable selection;

    SpectralKey(const Id<SpectralKey>& id, QObject* parent);
    SpectralKey(const Id<SpectralKey>& id, SpectralKeyData n, QObject* parent);
    ~SpectralKey() = default;

    template <typename Impl>
    SpectralKey(Impl&& vis, QObject* parent) : IdentifiedObject{vis, parent}
    {
      vis.writeTo(*this);
    }

    Nature nature() const noexcept;
    Nature nature2() const noexcept;
    bool isHybrid() const noexcept;
    bool isHybrid2() const noexcept;
    bool isRich() const noexcept;
    bool isRich2() const noexcept;
    bool isWarped() const noexcept;
    bool isWarped2() const noexcept;

    void setNature(Nature nature);
    void setNature2(Nature nature2);
    void setIsHybrid(bool isHybrid);
    void setIsHybrid2(bool isHybrid2);
    void setIsRich(bool isRich);
    void setIsRich2(bool isRich2);
    void setIsWarped(bool isWarped);
    void setIsWarped2(bool isWarped2);

    SpectralKeyData spectralKeyData() const;
    void setData(SpectralKeyData sd);
    void spectralKeyChanged() W_SIGNAL(spectralKeyChanged);
    PROPERTY(SpectralKeyData, spectralKey READ spectralKeyData WRITE setData NOTIFY spectralKeyChanged)
private:
    SpectralKeyData m_impl;
};

}
