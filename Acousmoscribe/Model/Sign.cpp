#include "Sign.hpp"

#include <score/serialization/DataStreamVisitor.hpp>
#include <score/serialization/JSONVisitor.hpp>

#include <score/plugins/SerializableHelpers.hpp>
#include <score/model/EntitySerialization.hpp>
#include <score/model/EntityMapSerialization.hpp>

#include <wobjectimpl.h>
W_OBJECT_IMPL(Acousmoscribe::Sign)

namespace Acousmoscribe
{

/*********************
 * PARTIE SIGN
 *********************/
Sign::Sign(const Id<Sign>& id, QObject* parent)
    : IdentifiedObject<Sign>(id, QStringLiteral("Sign"), parent)
    {}

Sign::Sign(const Id<Sign>& id, SignData s, QObject* parent)
    : IdentifiedObject<Sign>(id, QStringLiteral("Sign"), parent)
    , m_impl{s}
    {}

double Sign::start() const noexcept {
  return m_impl.start;
}

double Sign::duration() const noexcept {
  return m_impl.duration;
}

double Sign::end() const noexcept {
  return m_impl.start + m_impl.duration;
}

/*
 * Getter de sign sur les profiles
 */
const DynamicProfile& Sign::dynamicProfile() const {
    return m_impl.dynamicProfile;
}

const MelodicProfile& Sign::melodicProfile() const {
    return m_impl.melodicProfile;
}

const RhythmicProfile& Sign::rhythmicProfile() const {
    return m_impl.rhythmicProfile;
}

Grain Sign::grain() const {
    return m_impl.grain;
}

/*
 * Getter de signData
 */
SignData Sign::signData() const {
    return m_impl;
}

/*
 * Liste de tous les setters
 */
void Sign::scale(double s) noexcept
{
  if (s != 1.)
  {
    m_impl.start *= s;
    m_impl.duration *= s;
    signChanged();
  }
}

void Sign::setStart(double s) noexcept
{
  if (m_impl.start != s)
  {
    m_impl.start = s;
    signChanged();
  }
}

void Sign::setDuration(double s) noexcept
{
  if (m_impl.duration != s)
  {
    m_impl.duration = s;
    signChanged();
  }
}

void Sign::setDynamicProfile(DynamicProfile d)  {
    m_impl.dynamicProfile = d;
    signChanged();
}

void Sign::setMelodicProfile(MelodicProfile d) {

    m_impl.melodicProfile = d;
    signChanged();
}

void Sign::setRhythmicProfile(RhythmicProfile d) {
    m_impl.rhythmicProfile = d;
    signChanged();
}

void Sign::setGrain(Grain g)  {
    m_impl.grain = g;
    signChanged();
}

void Sign::setData(SignData d)  {
  if(m_impl != d)
  {
    m_impl = d;
    signChanged();
  }
}

}

template <>
void DataStreamReader::read(const Acousmoscribe::DynamicProfile& dp)
{
  m_stream << dp.volumeStart << dp.volumeEnd;
  insertDelimiter();
}

template <>
void DataStreamWriter::write(Acousmoscribe::DynamicProfile& dp)
{
  m_stream >> dp.volumeStart >> dp.volumeEnd;
  checkDelimiter();
}

template <>
void JSONReader::read(const Acousmoscribe::DynamicProfile& dp)
{
  stream.StartArray();
  stream.Double(dp.volumeStart);
  stream.Double(dp.volumeEnd);
  stream.EndArray();
}

template <>
void JSONWriter::write(Acousmoscribe::DynamicProfile& dp)
{
  const auto& arr = base.GetArray();
  dp.volumeStart = arr[0].GetDouble();
  dp.volumeEnd = arr[1].GetDouble();
}

template <>
void DataStreamReader::read(const Acousmoscribe::SignData& sd)
{
  m_stream << sd.start << sd.duration << sd.grain << sd.dynamicProfile << sd.melodicProfile << sd.rhythmicProfile;
  insertDelimiter();
}

template <>
void DataStreamWriter::write(Acousmoscribe::SignData& sd)
{
  m_stream >> sd.start >> sd.duration >> sd.grain >> sd.dynamicProfile >> sd.melodicProfile >> sd.rhythmicProfile;
  checkDelimiter();
}

template <>
void JSONReader::read(const Acousmoscribe::SignData& sd)
{
  stream.StartArray();
  stream.Double(sd.start);
  stream.Double(sd.duration);
  stream.Int(sd.grain);
  JSONReader::read(sd.dynamicProfile);
  JSONReader::read(sd.melodicProfile);
  JSONReader::read(sd.rhythmicProfile);
  stream.EndArray();
}

template <>
void JSONWriter::write(Acousmoscribe::SignData& sd)
{
  const auto& arr = base.GetArray();
  sd.start = arr[0].GetDouble();
  sd.duration = arr[1].GetDouble();
  sd.grain = (Acousmoscribe::Grain) arr[2].GetInt();
  JSONWriter::write(sd.dynamicProfile);
  JSONWriter::write(sd.melodicProfile);
  JSONWriter::write(sd.rhythmicProfile);
}

template <>
void DataStreamReader::read(const Acousmoscribe::Sign& s)
{
  m_stream << s.signData();
  insertDelimiter();
}

template <>
void DataStreamWriter::write(Acousmoscribe::Sign& s)
{
  Acousmoscribe::SignData d;
  m_stream >> d;
  s.setData(d);
  checkDelimiter();
}

template <>
void JSONReader::read(const Acousmoscribe::Sign& s)
{
  stream.Key("Sign");
  stream.StartArray();
  stream.Double(s.m_impl.start);
  stream.Double(s.m_impl.duration);
  stream.Int(s.m_impl.grain);
  JSONReader::read(s.m_impl.dynamicProfile);
  JSONReader::read(s.m_impl.melodicProfile);
  JSONReader::read(s.m_impl.rhythmicProfile);
  stream.EndArray();
}

template <>
void JSONWriter::write(Acousmoscribe::Sign& s)
{
  const auto& arr = obj["Sign"].toArray();
  s.m_impl.start = arr[0].GetDouble();
  s.m_impl.duration = arr[1].GetDouble();
  s.m_impl.grain = (Acousmoscribe::Grain) arr[2].GetInt();
  JSONWriter{JsonValue{arr[3]}}.write(s.m_impl.dynamicProfile);
  JSONWriter{JsonValue{arr[4]}}.write(s.m_impl.melodicProfile);
  JSONWriter{JsonValue{arr[5]}}.write(s.m_impl.rhythmicProfile);
}

