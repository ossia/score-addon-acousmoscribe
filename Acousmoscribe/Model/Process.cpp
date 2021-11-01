#include "Process.hpp"

#include <Acousmoscribe/Commands/AddKey.hpp>
#include <Acousmoscribe/Model/MelodicKey.hpp>
#include <Acousmoscribe/Model/SpectralKey.hpp>
#include <score/plugins/SerializableHelpers.hpp>
#include <score/model/EntitySerialization.hpp>
#include <score/model/EntityMapSerialization.hpp>

#include <score/serialization/DataStreamVisitor.hpp>
#include <score/serialization/JSONVisitor.hpp>

#include <Process/Dataflow/Port.hpp>

#include <wobjectimpl.h>

W_OBJECT_IMPL(Acousmoscribe::Model)
namespace Acousmoscribe
{

Model::Model(
    const TimeVal& duration, const Id<Process::ProcessModel>& id,
    QObject* parent)
    : Process::ProcessModel{duration, id, "AcousmoscribeProcess", parent}
{
  metadata().setInstanceName(*this);


  // Default MelodicKey
  MelodicKeyData mkData = {mid, weak};

  melodicKeys.add(new MelodicKey{Id<MelodicKey>{0}, mkData , this});

  // Default SpectralKey
  SpectralKeyData skData;
  skData.nature = tonic;
  skData.nature2 = null;

  spectralKey.add(new SpectralKey{Id<SpectralKey>{0}, skData , this});
}

Model::~Model()
{
}

QString Model::prettyName() const noexcept
{
  return tr("Acousmoscribe Process");
}

void Model::setDurationAndScale(const TimeVal& newDuration) noexcept
{
  signsChanged();
  setDuration(newDuration);
}

void Model::setDurationAndGrow(const TimeVal& newDuration) noexcept
{
  if(duration() == newDuration)
    return;
  if(newDuration == TimeVal::zero())
    return;
  auto ratio = double(duration().impl) / newDuration.impl;

  for (auto& sgn : signs)
    sgn.scale(ratio);

  signsChanged();
  setDuration(newDuration);
}

void Model::setDurationAndShrink(const TimeVal& newDuration) noexcept
{
   if(duration() == newDuration)
    return;
  if(newDuration == TimeVal::zero())
    return;

  auto ratio = double(duration().impl) / newDuration.impl;
  auto inv_ratio = newDuration.impl / double(duration().impl);

  std::vector<Id<Sign>> toErase;
  for (Sign& n : signs)
  {
    if (n.end() >= inv_ratio)
    {
      toErase.push_back(n.id());
    }
    else
    {
      n.scale(ratio);
    }
  }

  for (auto& sign : toErase)
  {
    signs.remove(sign);
  }
  signsChanged();
  setDuration(newDuration);
}
}


// PARTIE SERIALIZATION

/***********************************
 * SERIALISATION SIGN AVEC LE MODEL
 ***********************************/
template <>
void DataStreamReader::read(const Acousmoscribe::Model& proc)
{
  m_stream << (int32_t)proc.signs.size();

  for (const auto& sign : proc.signs)
  {
    readFrom(sign);
  }

  m_stream << (int32_t)proc.spectralKey.size();

  for (const auto& speck : proc.spectralKey)
  {
    readFrom(speck);
  }

  m_stream << (int32_t)proc.melodicKeys.size();

  for (const auto& melok : proc.melodicKeys)
  {
    readFrom(melok);
  }

  insertDelimiter();
}

template <>
void DataStreamWriter::write(Acousmoscribe::Model& proc)
{
  int32_t sign_count;
  m_stream >> sign_count;

  for(; sign_count-- > 0;)
    proc.signs.add(new Acousmoscribe::Sign{*this, &proc});

  int32_t speck_count;
  m_stream >> speck_count;

  for(; speck_count-- >0;)
    proc.spectralKey.add(new Acousmoscribe::SpectralKey{*this, &proc});

  int32_t melok_count;
  m_stream >> melok_count;

  for(; melok_count-- >0;)
    proc.melodicKeys.add(new Acousmoscribe::MelodicKey{*this, &proc});

  checkDelimiter();
}

template <>
void JSONReader::read(const Acousmoscribe::Model& proc)
{
  obj["Signs"] = proc.signs;
  obj["MelodicKeys"] = proc.melodicKeys;
  obj["SpectralKeys"] = proc.spectralKey;
}

template <>
void JSONWriter::write(Acousmoscribe::Model& proc)
{
  for(const auto& json_vref : obj["MelodicKeys"].toArray()){
    JSONObject::Deserializer deserializer{json_vref};
    proc.melodicKeys.add(new Acousmoscribe::MelodicKey{deserializer, &proc});
  }
  for(const auto& json_vref : obj["SpectralKeys"].toArray()){
    JSONObject::Deserializer deserializer{json_vref};
    proc.spectralKey.add(new Acousmoscribe::SpectralKey{deserializer, &proc});
  }
  for(const auto& json_vref : obj["Signs"].toArray()){
    JSONObject::Deserializer deserializer{json_vref};
    proc.signs.add(new Acousmoscribe::Sign{deserializer, &proc});
  }
}



