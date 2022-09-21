#include "GrainProfile.hpp"

#include <score/serialization/DataStreamVisitor.hpp>
#include <score/serialization/JSONVisitor.hpp>

template <>
void DataStreamReader::read(const Acousmoscribe::GrainProfile& gp)
{
  m_stream << gp.grain << gp.variation;
  insertDelimiter();
}

template <>
void DataStreamWriter::write(Acousmoscribe::GrainProfile& gp)
{
  m_stream >> gp.grain >> gp.variation;
  checkDelimiter();
}

template <>
void JSONReader::read(const Acousmoscribe::GrainProfile& gp){
    stream.StartArray();
    stream.Int(gp.grain);
    stream.Int(gp.variation);
    stream.EndArray();
}

template <>
void JSONWriter::write(Acousmoscribe::GrainProfile& gp)
{
  const auto& arr = base.GetArray();
  gp.variation = static_cast<Acousmoscribe::Grain>(arr[0].GetInt());
  gp.variation = static_cast<Acousmoscribe::Variation>(arr[1].GetInt());
}
