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

Nature SpectralKey::hybrid_nature() const noexcept {
    return m_impl.hybrid_nature;
}

Nature SpectralKey::hybrid_nature2() const noexcept {
    return m_impl.hybrid_nature2;
}

Coloration SpectralKey::coloration() const noexcept{
  return m_impl.coloration;
}

Coloration SpectralKey::coloration2() const noexcept{
  return m_impl.coloration2;
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
bool SpectralKey::canBeHybrid(Nature n) const noexcept {
  return n == Nature::tonic || n == Nature::inharmonic;
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

void SpectralKey::setHybridNature(Nature nature){
    m_impl.hybrid_nature = nature;
}

void SpectralKey::setHybridNature2(Nature nature2){
    m_impl.hybrid_nature2 = nature2;
}

void SpectralKey::setColoration(Coloration col){
  m_impl.coloration = col;
}

void SpectralKey::setColoration2(Coloration col){
  m_impl.coloration2 = col;
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
  m_stream << skd.nature << skd.nature2 <<skd.hybrid_nature<<skd.hybrid_nature2<< skd.coloration << skd.coloration2 << skd.isHybrid << skd.isHybrid2 << skd.isRich << skd.isRich2;
  insertDelimiter();
}

template <>
void DataStreamWriter::write(Acousmoscribe::SpectralKeyData& skd)
{
  m_stream >> skd.nature >> skd.nature2 >> skd.hybrid_nature >> skd.hybrid_nature2>>skd.coloration>>skd.coloration2>> skd.isHybrid >> skd.isHybrid2 >> skd.isRich >> skd.isRich2;
  checkDelimiter();
}

template <>
void JSONReader::read(const Acousmoscribe::SpectralKeyData& skd)
{
  stream.StartArray();
  stream.Int(skd.nature);
  stream.Int(skd.nature2);
  stream.Int(skd.hybrid_nature);
  stream.Int(skd.hybrid_nature2);
  stream.Int(skd.coloration);
  stream.Int(skd.coloration2);
  stream.Bool(skd.isHybrid);
  stream.Bool(skd.isHybrid2);
  stream.Bool(skd.isRich);
  stream.Bool(skd.isRich2);
  stream.EndArray();
}

template <>
void JSONWriter::write(Acousmoscribe::SpectralKeyData& skd)
{
  const auto& arr = base.GetArray();
  skd.nature = static_cast<Acousmoscribe::Nature>(arr[0].GetInt());
  skd.nature2 = static_cast<Acousmoscribe::Nature>(arr[1].GetInt());
  skd.hybrid_nature = static_cast<Acousmoscribe::Nature>(arr[2].GetInt());
  skd.hybrid_nature2 = static_cast<Acousmoscribe::Nature>(arr[3].GetInt());
  skd.coloration = static_cast<Acousmoscribe::Coloration>(arr[4].GetInt());
  skd.coloration2 = static_cast<Acousmoscribe::Coloration>(arr[5].GetInt());
  skd.isHybrid = static_cast<Acousmoscribe::Nature>(arr[6].GetBool());
  skd.isHybrid2 = static_cast<Acousmoscribe::Nature>(arr[7].GetBool());
  skd.isRich = static_cast<Acousmoscribe::Nature>(arr[8].GetBool());
  skd.isRich2 = static_cast<Acousmoscribe::Nature>(arr[9].GetBool());
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
  stream.Int(sk.hybrid_nature());
  stream.Int(sk.hybrid_nature2());
  stream.Int(sk.coloration());
  stream.Int(sk.coloration2());
  stream.Bool(sk.isHybrid());
  stream.Bool(sk.isHybrid2());
  stream.Bool(sk.isRich());
  stream.Bool(sk.isRich2());
  stream.EndArray();
}

template <>
void JSONWriter::write(Acousmoscribe::SpectralKey& sk)
{
  const auto& arr = obj["SpectralKey"].toArray();
  Acousmoscribe::Nature n;
  Acousmoscribe::Coloration c;
  switch (arr[0].GetInt())
  {
  case 1: n = Acousmoscribe::noise; break;
  case 2: n = Acousmoscribe::tonic; break;
  case 3: n = Acousmoscribe::inharmonic; break;
  case 4: n = Acousmoscribe::tonic_warped; break;
  default: n = Acousmoscribe::null;
    break;
  }
  sk.setNature(n);

  switch (arr[1].GetInt())
  {
  case 1: n = Acousmoscribe::noise; break;
  case 2: n = Acousmoscribe::tonic; break;
  case 3: n = Acousmoscribe::inharmonic; break;
  case 4: n = Acousmoscribe::tonic_warped; break;
  default: n = Acousmoscribe::null;
    break;
  }
  sk.setNature2(n);
  switch(arr[2].GetInt())
  {
  case 1: n = Acousmoscribe::noise; break;
  case 2: n = Acousmoscribe::tonic; break;
  default: n = Acousmoscribe::null; break;
  }
  sk.setHybridNature(n);

  switch(arr[3].GetInt())
  {
  case 1: n = Acousmoscribe::noise; break;
  case 2: n = Acousmoscribe::tonic; break;
  default: n = Acousmoscribe::null; break;
  }
  sk.setHybridNature2(n);

  switch(arr[4].GetInt())
  {
    case 1: c = Acousmoscribe::lo; break;
    case 2: c = Acousmoscribe::med; break;
    case 3: c = Acousmoscribe::hi; break;
    case 4: c = Acousmoscribe::lo_med; break;
    case 5: c = Acousmoscribe::med_hi; break;
    case 6: c = Acousmoscribe::lo_hi; break;
    default: c = Acousmoscribe::neutral; break;
  }
  sk.setColoration(c);
  switch(arr[5].GetInt())
  {
    case 1: c = Acousmoscribe::lo; break;
    case 2: c = Acousmoscribe::med; break;
    case 3: c = Acousmoscribe::hi; break;
    case 4: c = Acousmoscribe::lo_med; break;
    case 5: c = Acousmoscribe::med_hi; break;
    case 6: c = Acousmoscribe::lo_hi; break;
    default: c = Acousmoscribe::neutral; break;
  }
  sk.setColoration2(c);

  sk.setIsHybrid(arr[6].GetBool());
  sk.setIsHybrid2(arr[7].GetBool());
  sk.setIsRich(arr[8].GetBool());
  sk.setIsRich2(arr[9].GetBool());
}
