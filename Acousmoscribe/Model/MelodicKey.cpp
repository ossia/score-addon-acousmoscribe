
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

template <>
void DataStreamReader::read(const Acousmoscribe::MelodicKeyData& mkd)
{
  m_stream << mkd.pitch << mkd.range;
  insertDelimiter();
}

template <>
void DataStreamWriter::write(Acousmoscribe::MelodicKeyData& mkd)
{
  m_stream >> mkd.pitch >> mkd.range;
  checkDelimiter();
}

template <>
void JSONReader::read(const Acousmoscribe::MelodicKeyData& mkd)
{
  stream.StartArray();
  stream.Int(mkd.pitch);
  stream.Int(mkd.range);
  stream.EndArray();
}

template <>
void JSONWriter::write(Acousmoscribe::MelodicKeyData& mkd)
{
  const auto& arr = base.GetArray();
  mkd.pitch = static_cast<Acousmoscribe::Pitch>(arr[0].GetInt());
  mkd.range = static_cast<Acousmoscribe::Range>(arr[1].GetInt());
}

template <>
void DataStreamReader::read(const Acousmoscribe::MelodicKey& mk)
{
  m_stream << mk.melodicKeyData();
  insertDelimiter();
}

template <>
void DataStreamWriter::write(Acousmoscribe::MelodicKey& mk)
{
  Acousmoscribe::MelodicKeyData mkd;
  m_stream >> mkd;
  mk.setMelodicKeyData(mkd);
  checkDelimiter();
}

template <>
void JSONReader::read(const Acousmoscribe::MelodicKey& mk)
{
  stream.Key("MelodicKey");
  stream.StartArray();
  stream.Int(mk.pitch());
  stream.Int(mk.range());
  stream.EndArray();
}

template <>
void JSONWriter::write(Acousmoscribe::MelodicKey& mk)
{
  const auto& arr = obj["MelodicKey"].toArray();
  mk.setPitch(static_cast<Acousmoscribe::Pitch>(arr[0].GetInt()));
  mk.setRange(static_cast<Acousmoscribe::Range>(arr[1].GetInt()));
}
