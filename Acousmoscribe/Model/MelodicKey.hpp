#pragma once
#include <score/model/IdentifiedObject.hpp>

#include <score/selection/Selectable.hpp>

#include <verdigris>
#include <iostream>

#include "MelodicProfile.hpp"

//#include <verdigris>

namespace Acousmoscribe {

enum Range{
  weak = 0,
  normal,
  strong
};
struct MelodicKeyData{
  Pitch pitch{};
  Range range{};

  bool operator==(const MelodicKeyData&) const noexcept = default;
};

class MelodicKey final
    : public IdentifiedObject<MelodicKey>
{
  W_OBJECT(MelodicKey)
  SCORE_SERIALIZE_FRIENDS
public:
  Selectable selection;

  MelodicKey(const Id<MelodicKey>& id, QObject* parent);
  MelodicKey(const Id<MelodicKey>& id, MelodicKeyData n, QObject* parent);

  template <typename Impl>
  MelodicKey(Impl&& vis, QObject* parent) : IdentifiedObject{vis, parent}
  {
    vis.writeTo(*this);
  }

  Pitch pitch() const noexcept;
  Range range() const noexcept;

  void setPitch(Pitch pitch);
  void setRange(Range range);

  MelodicKeyData melodicKeyData() const;
  void setMelodicKeyData(const MelodicKeyData& k);
  void melodicKeyChanged() W_SIGNAL(melodicKeyChanged);
  PROPERTY(MelodicKeyData, melodicKey READ melodicKeyData WRITE setMelodicKeyData NOTIFY melodicKeyChanged)
private:
  MelodicKeyData m_impl{};
};

}
