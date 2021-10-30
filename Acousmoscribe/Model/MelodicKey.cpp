
#include "MelodicKey.hpp"

#include <score/serialization/DataStreamVisitor.hpp>
#include <score/serialization/JSONVisitor.hpp>

#include <score/plugins/SerializableHelpers.hpp>
#include <score/model/EntitySerialization.hpp>
#include <score/model/EntityMapSerialization.hpp>


#include <wobjectimpl.h>
W_OBJECT_IMPL(Acousmoscribe::MelodicKey)

namespace Acousmoscribe{


/*********************
 * PARTIE MELODIC KEY
 *********************/

MelodicKey::MelodicKey(const Id<MelodicKey>& id, QObject* parent)
    : IdentifiedObject<MelodicKey>(id, QStringLiteral("MelodicKey"), parent)
    {}


MelodicKey::MelodicKey(const Id<MelodicKey>& id, MelodicKeyData m, QObject* parent)
    : IdentifiedObject<MelodicKey>(id, QStringLiteral("MelodicKey"), parent)
    , m_impl{m}
    {

}

Pitch MelodicKey::pitch() const noexcept {
    return m_impl.pitch;
}

Range MelodicKey::range() const noexcept {
    return m_impl.range;
}

MelodicKeyData MelodicKey::melodicKeyData() const {
    return m_impl;
}

void MelodicKey::setMelodicKeyData(const MelodicKeyData& k)
{
  if(k != m_impl)
  {
    m_impl = k;
    melodicKeyChanged();
  }
}

void MelodicKey::setPitch(Pitch pitch) {
  if(pitch != m_impl.pitch)
  {
    m_impl.pitch = pitch;
    melodicKeyChanged();
  }
}

void MelodicKey::setRange(Range range) {
  if(range != m_impl.range)
  {
    m_impl.range = range;
    melodicKeyChanged();
  }
}


}
