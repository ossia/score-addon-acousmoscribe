#include "ChangeSpectralKey.hpp"

#include <Acousmoscribe/Model/Process.hpp>
#include <Process/TimeValueSerialization.hpp>

#include <score/model/path/PathSerialization.hpp>
#include <score/tools/IdentifierGeneration.hpp>

namespace Acousmoscribe
{

/* Nature */

ChangeSpectralKeyNature::ChangeSpectralKeyNature(
    const Model& model,
    const Id<SpectralKey>& to_change,
    Nature nature)
    : m_model{model}
{
  auto& sKey = model.spectralKey.at(to_change);
  SpectralKeyData data = sKey.spectralKeyData();
  m_before = std::make_pair(sKey.id(), data);
  data.nature = nature;
  m_after = std::make_pair(sKey.id(), data);
}

void ChangeSpectralKeyNature::update(unused_t, unused_t, Nature nature)
{
  m_after.second.nature = nature;
}

void ChangeSpectralKeyNature::undo(const score::DocumentContext& ctx) const
{
  auto& model = m_model.find(ctx);
  auto& sKey = m_before;
  auto& m = model.spectralKey.at(sKey.first);
  m.setNature(sKey.second.nature);
}

void ChangeSpectralKeyNature::redo(const score::DocumentContext& ctx) const
{
  auto& model = m_model.find(ctx);
  auto& sKey = m_after;
  auto& m = model.spectralKey.at(sKey.first);
  m.setNature(sKey.second.nature);
}

void ChangeSpectralKeyNature::serializeImpl(DataStreamInput& s) const
{
  s << m_model << m_before << m_after;
}

void ChangeSpectralKeyNature::deserializeImpl(DataStreamOutput& s)
{
  s >> m_model >> m_before >> m_after;
}

/* Nature2 */

ChangeSpectralKeyNature2::ChangeSpectralKeyNature2(
    const Model& model,
    const Id<SpectralKey>& to_change,
    Nature nature2)
    : m_model{model}
{
  auto& sKey = model.spectralKey.at(to_change);
  SpectralKeyData data = sKey.spectralKeyData();
  m_before = std::make_pair(sKey.id(), data);
  data.nature2 = nature2;
  m_after = std::make_pair(sKey.id(), data);
}

void ChangeSpectralKeyNature2::update(unused_t, unused_t, Nature nature2)
{
  m_after.second.nature2 = nature2;
}

void ChangeSpectralKeyNature2::undo(const score::DocumentContext& ctx) const
{
  auto& model = m_model.find(ctx);
  auto& sKey = m_before;
  auto& m = model.spectralKey.at(sKey.first);
  m.setNature2(sKey.second.nature2);
}

void ChangeSpectralKeyNature2::redo(const score::DocumentContext& ctx) const
{
  auto& model = m_model.find(ctx);
  auto& sKey = m_after;
  auto& m = model.spectralKey.at(sKey.first);
  m.setNature2(sKey.second.nature2);
}

void ChangeSpectralKeyNature2::serializeImpl(DataStreamInput& s) const
{
  s << m_model << m_before << m_after;
}

void ChangeSpectralKeyNature2::deserializeImpl(DataStreamOutput& s)
{
  s >> m_model >> m_before >> m_after;
}


/* Is rich */

ChangeSpectralKeyIsRich::ChangeSpectralKeyIsRich(
    const Model& model,
    const Id<SpectralKey>& to_change,
    bool isRich)
    : m_model{model}
{
  auto& sKey = model.spectralKey.at(to_change);
  SpectralKeyData data = sKey.spectralKeyData();
  m_before = std::make_pair(sKey.id(), data);
  data.isRich = isRich;
  m_after = std::make_pair(sKey.id(), data);
}

void ChangeSpectralKeyIsRich::update(unused_t, unused_t, bool isRich)
{
  m_after.second.isRich = isRich;
}

void ChangeSpectralKeyIsRich::undo(const score::DocumentContext& ctx) const
{
  auto& model = m_model.find(ctx);
  auto& sKey = m_before;
  auto& m = model.spectralKey.at(sKey.first);
  m.setIsRich(sKey.second.isRich);
}

void ChangeSpectralKeyIsRich::redo(const score::DocumentContext& ctx) const
{
  auto& model = m_model.find(ctx);
  auto& sKey = m_after;
  auto& m = model.spectralKey.at(sKey.first);
  m.setIsRich(sKey.second.isRich);
}

void ChangeSpectralKeyIsRich::serializeImpl(DataStreamInput& s) const
{
  s << m_model << m_before << m_after;
}

void ChangeSpectralKeyIsRich::deserializeImpl(DataStreamOutput& s)
{
  s >> m_model >> m_before >> m_after;
}


/* Is rich 2 */

ChangeSpectralKeyIsRich2::ChangeSpectralKeyIsRich2(
    const Model& model,
    const Id<SpectralKey>& to_change,
    bool isRich2)
    : m_model{model}
{
  auto& sKey = model.spectralKey.at(to_change);
  SpectralKeyData data = sKey.spectralKeyData();
  m_before = std::make_pair(sKey.id(), data);
  data.isRich2 = isRich2;
  m_after = std::make_pair(sKey.id(), data);
}

void ChangeSpectralKeyIsRich2::update(unused_t, unused_t, bool isRich2)
{
  m_after.second.isRich2 = isRich2;
}

void ChangeSpectralKeyIsRich2::undo(const score::DocumentContext& ctx) const
{
  auto& model = m_model.find(ctx);
  auto& sKey = m_before;
  auto& m = model.spectralKey.at(sKey.first);
  m.setIsRich2(sKey.second.isRich2);
}

void ChangeSpectralKeyIsRich2::redo(const score::DocumentContext& ctx) const
{
  auto& model = m_model.find(ctx);
  auto& sKey = m_after;
  auto& m = model.spectralKey.at(sKey.first);
  m.setIsRich2(sKey.second.isRich2);
}

void ChangeSpectralKeyIsRich2::serializeImpl(DataStreamInput& s) const
{
  s << m_model << m_before << m_after;
}

void ChangeSpectralKeyIsRich2::deserializeImpl(DataStreamOutput& s)
{
  s >> m_model >> m_before >> m_after;
}


/* Is warped */

ChangeSpectralKeyWarped::ChangeSpectralKeyWarped(
    const Model& model,
    const Id<SpectralKey>& to_change,
    bool isWarped)
    : m_model{model}
{
  auto& sKey = model.spectralKey.at(to_change);
  SpectralKeyData data = sKey.spectralKeyData();
  m_before = std::make_pair(sKey.id(), data);
  data.isWarped = isWarped;
  m_after = std::make_pair(sKey.id(), data);
}

void ChangeSpectralKeyWarped::update(unused_t, unused_t, bool isWarped)
{
  m_after.second.isWarped = isWarped;
}

void ChangeSpectralKeyWarped::undo(const score::DocumentContext& ctx) const
{
  auto& model = m_model.find(ctx);
  auto& sKey = m_before;
  auto& m = model.spectralKey.at(sKey.first);
  m.setIsWarped(sKey.second.isWarped);
}

void ChangeSpectralKeyWarped::redo(const score::DocumentContext& ctx) const
{
  auto& model = m_model.find(ctx);
  auto& sKey = m_after;
  auto& m = model.spectralKey.at(sKey.first);
  m.setIsWarped(sKey.second.isWarped);
}

void ChangeSpectralKeyWarped::serializeImpl(DataStreamInput& s) const
{
  s << m_model << m_before << m_after;
}

void ChangeSpectralKeyWarped::deserializeImpl(DataStreamOutput& s)
{
  s >> m_model >> m_before >> m_after;
}


/* Is warped 2 */

ChangeSpectralKeyWarped2::ChangeSpectralKeyWarped2(
    const Model& model,
    const Id<SpectralKey>& to_change,
    bool isWarped2)
    : m_model{model}
{
  auto& sKey = model.spectralKey.at(to_change);
  SpectralKeyData data = sKey.spectralKeyData();
  m_before = std::make_pair(sKey.id(), data);
  data.isWarped2 = isWarped2;
  m_after = std::make_pair(sKey.id(), data);
}

void ChangeSpectralKeyWarped2::update(unused_t, unused_t, bool isWarped2)
{
  m_after.second.isWarped2 = isWarped2;
}

void ChangeSpectralKeyWarped2::undo(const score::DocumentContext& ctx) const
{
  auto& model = m_model.find(ctx);
  auto& sKey = m_before;
  auto& m = model.spectralKey.at(sKey.first);
  m.setIsWarped2(sKey.second.isWarped2);
}

void ChangeSpectralKeyWarped2::redo(const score::DocumentContext& ctx) const
{
  auto& model = m_model.find(ctx);
  auto& sKey = m_after;
  auto& m = model.spectralKey.at(sKey.first);
  m.setIsWarped2(sKey.second.isWarped2);
}

void ChangeSpectralKeyWarped2::serializeImpl(DataStreamInput& s) const
{
  s << m_model << m_before << m_after;
}

void ChangeSpectralKeyWarped2::deserializeImpl(DataStreamOutput& s)
{
  s >> m_model >> m_before >> m_after;
}


/* Is hybrid */

ChangeSpectralKeyIsHybrid::ChangeSpectralKeyIsHybrid(
    const Model& model,
    const Id<SpectralKey>& to_change,
    bool isHybrid)
    : m_model{model}
{
  auto& sKey = model.spectralKey.at(to_change);
  SpectralKeyData data = sKey.spectralKeyData();
  m_before = std::make_pair(sKey.id(), data);
  data.isHybrid = isHybrid;
  m_after = std::make_pair(sKey.id(), data);
}

void ChangeSpectralKeyIsHybrid::update(unused_t, unused_t, bool isHybrid)
{
  m_after.second.isHybrid = isHybrid;
}

void ChangeSpectralKeyIsHybrid::undo(const score::DocumentContext& ctx) const
{
  auto& model = m_model.find(ctx);
  auto& sKey = m_before;
  auto& m = model.spectralKey.at(sKey.first);
  m.setIsHybrid(sKey.second.isHybrid);
}

void ChangeSpectralKeyIsHybrid::redo(const score::DocumentContext& ctx) const
{
  auto& model = m_model.find(ctx);
  auto& sKey = m_after;
  auto& m = model.spectralKey.at(sKey.first);
  m.setIsHybrid(sKey.second.isHybrid);
}

void ChangeSpectralKeyIsHybrid::serializeImpl(DataStreamInput& s) const
{
  s << m_model << m_before << m_after;
}

void ChangeSpectralKeyIsHybrid::deserializeImpl(DataStreamOutput& s)
{
  s >> m_model >> m_before >> m_after;
}



/* Is hybrid2 */

ChangeSpectralKeyIsHybrid2::ChangeSpectralKeyIsHybrid2(
    const Model& model,
    const Id<SpectralKey>& to_change,
    bool isHybrid2)
    : m_model{model}
{
  auto& sKey = model.spectralKey.at(to_change);
  SpectralKeyData data = sKey.spectralKeyData();
  m_before = std::make_pair(sKey.id(), data);
  data.isHybrid2 = isHybrid2;
  m_after = std::make_pair(sKey.id(), data);
}

void ChangeSpectralKeyIsHybrid2::update(unused_t, unused_t, bool isHybrid2)
{
  m_after.second.isHybrid2 = isHybrid2;
}

void ChangeSpectralKeyIsHybrid2::undo(const score::DocumentContext& ctx) const
{
  auto& model = m_model.find(ctx);
  auto& sKey = m_before;
  auto& m = model.spectralKey.at(sKey.first);
  m.setIsHybrid2(sKey.second.isHybrid2);
}

void ChangeSpectralKeyIsHybrid2::redo(const score::DocumentContext& ctx) const
{
  auto& model = m_model.find(ctx);
  auto& sKey = m_after;
  auto& m = model.spectralKey.at(sKey.first);
  m.setIsHybrid2(sKey.second.isHybrid2);
}

void ChangeSpectralKeyIsHybrid2::serializeImpl(DataStreamInput& s) const
{
  s << m_model << m_before << m_after;
}

void ChangeSpectralKeyIsHybrid2::deserializeImpl(DataStreamOutput& s)
{
  s >> m_model >> m_before >> m_after;
}

}
