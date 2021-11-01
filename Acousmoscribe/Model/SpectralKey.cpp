#include "SpectralKey.hpp"

#include <score/plugins/SerializableHelpers.hpp>
#include <score/model/EntitySerialization.hpp>
#include <score/model/EntityMapSerialization.hpp>

#include <score/serialization/DataStreamVisitor.hpp>
#include <score/serialization/JSONVisitor.hpp>

#include <Process/Dataflow/Port.hpp>

#include <wobjectimpl.h>
W_OBJECT_IMPL(Acousmoscribe::SpectralKey)

namespace Acousmoscribe
{

/**********************
 * PARTIE SPECTRAL KEY
 **********************/
SpectralKey::SpectralKey(const Id<SpectralKey>& id, QObject* parent)
    : IdentifiedObject<SpectralKey>(id, QStringLiteral("SpectralKey"), parent)
    {}


SpectralKey::SpectralKey(const Id<SpectralKey>& id, SpectralKeyData s, QObject* parent)
    : IdentifiedObject<SpectralKey>(id, QStringLiteral("SpectralKey"), parent)
    , m_impl{s}
    {}

Nature SpectralKey::nature() const noexcept {
    return m_impl.nature;
}

Nature SpectralKey::nature2() const noexcept {
    return m_impl.nature2;
}

bool SpectralKey::isHybrid() const noexcept {
    return m_impl.isHybrid;
}

bool SpectralKey::isHybrid2() const noexcept {
    return m_impl.isHybrid2;
}

bool SpectralKey::isRich() const noexcept {
    return m_impl.isRich;
}

bool SpectralKey::isRich2() const noexcept {
    return m_impl.isRich2;
}

bool SpectralKey::isWarped() const noexcept {
    return m_impl.isWarped;
}

bool SpectralKey::isWarped2() const noexcept {
    return m_impl.isWarped2;
}

SpectralKeyData SpectralKey::spectralKeyData() const {
    return m_impl;
}

void SpectralKey::setNature(Nature nature){
    m_impl.nature = nature;
}

void SpectralKey::setNature2(Nature nature2){
    m_impl.nature2 = nature2;
}

void SpectralKey::setIsHybrid(bool isHybrid){
    m_impl.isHybrid = isHybrid;
}

void SpectralKey::setIsHybrid2(bool isHybrid2){
    m_impl.isHybrid2 = isHybrid2;
}

void SpectralKey::setIsRich(bool isRich){
    m_impl.isRich = isRich;
}

void SpectralKey::setIsRich2(bool isRich){
    m_impl.isRich2 = isRich;
}

void SpectralKey::setIsWarped(bool isWarped){
    m_impl.isHybrid2 = isWarped;
}

void SpectralKey::setIsWarped2(bool isWarped2){
    m_impl.isHybrid2 = isWarped2;
}

void SpectralKey::setData(SpectralKeyData sd){
  if(m_impl != sd)
  {
    m_impl = sd;
   spectralKeyChanged();
  }

}
}

template <>
void DataStreamReader::read(const Acousmoscribe::SpectralKeyData& skd)
{
  m_stream << skd.nature << skd.nature2 << skd.isHybrid << skd.isHybrid2 << skd.isRich << skd.isRich2 << skd.isWarped << skd.isWarped2;
  insertDelimiter();
}

template <>
void DataStreamWriter::write(Acousmoscribe::SpectralKeyData& skd)
{
  m_stream >> skd.nature >> skd.nature2 >> skd.isHybrid >> skd.isHybrid2 >> skd.isRich >> skd.isRich2 >> skd.isWarped >> skd.isWarped2;
  checkDelimiter();
}

template <>
void JSONReader::read(const Acousmoscribe::SpectralKeyData& skd)
{
  stream.StartArray();
  stream.Int(skd.nature);
  stream.Int(skd.nature2);
  stream.Bool(skd.isHybrid);
  stream.Bool(skd.isHybrid2);
  stream.Bool(skd.isRich);
  stream.Bool(skd.isRich2);
  stream.Bool(skd.isWarped);
  stream.Bool(skd.isWarped2);
  stream.EndArray();
}

template <>
void JSONWriter::write(Acousmoscribe::SpectralKeyData& skd)
{
  const auto& arr = base.GetArray();
  skd.nature = static_cast<Acousmoscribe::Nature>(arr[0].GetInt());
  skd.nature2 = static_cast<Acousmoscribe::Nature>(arr[1].GetInt());
  skd.isHybrid = static_cast<Acousmoscribe::Nature>(arr[2].GetBool());
  skd.isHybrid2 = static_cast<Acousmoscribe::Nature>(arr[3].GetBool());
  skd.isRich = static_cast<Acousmoscribe::Nature>(arr[4].GetBool());
  skd.isRich2 = static_cast<Acousmoscribe::Nature>(arr[5].GetBool());
  skd.isWarped = static_cast<Acousmoscribe::Nature>(arr[6].GetBool());
  skd.isWarped2 = static_cast<Acousmoscribe::Nature>(arr[7].GetBool());
}

/****************************
 * SERIALISATION SPECTRAL KEY
 ****************************/

template <>
void DataStreamReader::read(const Acousmoscribe::SpectralKey& sk)
{
  m_stream << sk.spectralKeyData();
  insertDelimiter();
}

template <>
void DataStreamWriter::write(Acousmoscribe::SpectralKey& sk)
{
    Acousmoscribe::SpectralKeyData sd;
    m_stream >> sd;
    sk.setData(sd);
    checkDelimiter();
}

template <>
void JSONReader::read(const Acousmoscribe::SpectralKey& sk)
{
  stream.Key("SpectralKey");
  stream.StartArray();
  stream.Int(sk.nature());
  stream.Int(sk.nature2());
  stream.Bool(sk.isHybrid());
  stream.Bool(sk.isHybrid2());
  stream.Bool(sk.isRich());
  stream.Bool(sk.isRich2());
  stream.Bool(sk.isWarped());
  stream.Bool(sk.isWarped2());
  stream.EndArray();
}

template <>
void JSONWriter::write(Acousmoscribe::SpectralKey& sk)
{
  const auto& arr = obj["SpectralKey"].toArray();
  Acousmoscribe::Nature n;
  switch (arr[0].GetInt())
  {
  case 1: n = Acousmoscribe::tonic; break;
  case 2: n = Acousmoscribe::inharmonic; break;
  case 3: n = Acousmoscribe::noise; break;
  default: n = Acousmoscribe::null;
    break;
  }
  sk.setNature(n);

  switch (arr[1].GetInt())
  {
  case 1: n = Acousmoscribe::tonic; break;
  case 2: n = Acousmoscribe::inharmonic; break;
  case 3: n = Acousmoscribe::noise; break;
  default: n = Acousmoscribe::null;
    break;
  }
  sk.setNature2(n);
  sk.setIsHybrid(arr[2].GetBool());
  sk.setIsHybrid2(arr[3].GetBool());
  sk.setIsRich(arr[4].GetBool());
  sk.setIsRich2(arr[5].GetBool());
  sk.setIsWarped(arr[6].GetBool());
  sk.setIsWarped2(arr[7].GetBool());
}
