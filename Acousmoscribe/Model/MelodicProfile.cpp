#include "MelodicProfile.hpp"

#include <score/serialization/DataStreamVisitor.hpp>
#include <score/serialization/JSONVisitor.hpp>

template <>
void DataStreamReader::read(const Acousmoscribe::MelodicProfile& mp)
{
  m_stream << mp.pitch << mp.pitchEnd << mp.var;
  insertDelimiter();
}

template <>
void DataStreamWriter::write(Acousmoscribe::MelodicProfile& mp)
{
  m_stream >> mp.pitch >> mp.pitchEnd >> mp.var;
  checkDelimiter();
}

template <>
void JSONReader::read(const Acousmoscribe::MelodicProfile& mp){
    stream.StartArray();
    stream.Int(mp.pitch);
    stream.Int(mp.pitchEnd);
    stream.Int(mp.var);
    stream.EndArray();
}

template <>
void JSONWriter::write(Acousmoscribe::MelodicProfile& mp)
{
  const auto& arr = base.GetArray();
  mp.pitch = static_cast<Acousmoscribe::Pitch>(arr[0].GetInt());
  mp.pitchEnd = static_cast<Acousmoscribe::Pitch>(arr[1].GetInt());
  mp.var = static_cast<Acousmoscribe::Variation>(arr[2].GetInt());
}
