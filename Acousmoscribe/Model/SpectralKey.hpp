#pragma once
#include <score/model/IdentifiedObject.hpp>

#include <score/selection/Selectable.hpp>

#include <verdigris>
#include <iostream>

namespace Acousmoscribe {

enum Nature
{
    null = 0,
    noise,
    tonic,
    tonic_warped, //TODO implement support for this instead of later bool
    inharmonic,
};
enum Coloration
{
  neutral =0,
  lo,
  med,
  hi,
  lo_med,
  med_hi,
  lo_hi
};

 struct SpectralKeyData
{
    Nature nature{};
    Nature hybrid_nature{};
    Nature hybrid_nature2{};
    Nature nature2{};
    Coloration coloration{};
    Coloration coloration2{};
    bool isHybrid{false};
    bool isHybrid2{false};
    bool isRich{false};
    bool isRich2{false};

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
    Nature hybrid_nature() const noexcept;
    Nature hybrid_nature2() const noexcept;
    Coloration coloration() const noexcept;
    Coloration coloration2() const noexcept;
    bool isHybrid() const noexcept;
    bool isHybrid2() const noexcept;
    bool isRich() const noexcept;
    bool isRich2() const noexcept;
    bool canBeHybrid(Nature n) const noexcept;

    void setNature(Nature nature);
    void setNature2(Nature nature2);
    void setHybridNature(Nature nature);
    void setHybridNature2(Nature nature2);
    void setColoration(Coloration col);
    void setColoration2(Coloration col);
    void setIsHybrid(bool isHybrid);
    void setIsHybrid2(bool isHybrid2);
    void setIsRich(bool isRich);
    void setIsRich2(bool isRich2);

    SpectralKeyData spectralKeyData() const;
    void setData(SpectralKeyData sd);
    void spectralKeyChanged() W_SIGNAL(spectralKeyChanged);
    PROPERTY(SpectralKeyData, spectralKey READ spectralKeyData WRITE setData NOTIFY spectralKeyChanged)
private:
    SpectralKeyData m_impl;
};

}
