#include "RhythmicProfile.hpp"

#include <score/serialization/DataStreamVisitor.hpp>
#include <score/serialization/JSONVisitor.hpp>

#include <score/plugins/SerializableHelpers.hpp>
#include <score/model/EntitySerialization.hpp>
#include <score/model/EntityMapSerialization.hpp>


#include <wobjectimpl.h>

//W_OBJECT_IMPL(Acousmoscribe::RhythmicProfile)

template <>
void DataStreamReader::read(const Acousmoscribe::RhythmicProfile& rp)
{
  m_stream << rp.speed << rp.acceleration << rp.variation;
  insertDelimiter();
}

template <>
void DataStreamWriter::write(Acousmoscribe::RhythmicProfile& rp)
{
  m_stream >> rp.speed >> rp.acceleration >> rp.variation;
  checkDelimiter();
}

template <>
void JSONReader::read(const Acousmoscribe::RhythmicProfile& rp)
{
  stream.StartArray();
  stream.Int(rp.speed);
  stream.Int(rp.acceleration);
  stream.Int(rp.variation);
  stream.EndArray();
}

template <>
void JSONWriter::write(Acousmoscribe::RhythmicProfile& rp)
{
  const auto& arr = base.GetArray();
  rp.speed = static_cast<Acousmoscribe::Speed>(arr[0].GetInt());
  rp.acceleration = static_cast<Acousmoscribe::Acceleration>(arr[1].GetInt());
  rp.variation = static_cast<Acousmoscribe::Variation>(arr[2].GetInt());
}
