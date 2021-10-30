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
    , m_start(s.start)
    , m_duration(s.duration)
    , _grain(s.grain)
    , _dynamicProfile(s.dynamicProfile)
    , _melodicProfile(s.melodicProfile)
    , _rhythmicProfile(s.rhythmicProfile)
    {}

double Sign::start() const noexcept {
  return m_start;
}

double Sign::duration() const noexcept {
  return m_duration;
}

double Sign::end() const noexcept {
  return m_start + m_duration;
}

/*
 * Getter de sign sur les profiles
 */
DynamicProfile Sign::dynamicProfile() const {
    return _dynamicProfile;
}

MelodicProfile Sign::melodicProfile() const {
    return _melodicProfile;
}

RhythmicProfile Sign::rhythmicProfile() const {
    return _rhythmicProfile;
}

Grain Sign::grain() const {
    return _grain;
}

/*
 * Getter de signData
 */
SignData Sign::signData() const {
    return SignData{m_start, m_duration, _grain, _dynamicProfile, _melodicProfile, _rhythmicProfile};
}

/*
 * Liste de tous les setters
 */
void Sign::scale(double s) noexcept
{
  if (s != 1.)
  {
    m_start *= s;
    m_duration *= s;
    signChanged();
  }
}

void Sign::setStart(double s) noexcept
{
  if (m_start != s)
  {
    m_start = s;
    signChanged();
  }
}

void Sign::setDuration(double s) noexcept
{
  if (m_duration != s)
  {
    m_duration = s;
    signChanged();
  }
}

void Sign::setDynamicProfile(DynamicProfile d)  {
    _dynamicProfile = d;
    signChanged();
}

void Sign::setMelodicProfile(MelodicProfile d) {

    _melodicProfile = d;
    signChanged();
}

void Sign::setRhythmicProfile(RhythmicProfile d) {
    _rhythmicProfile = d;
    signChanged();
}

void Sign::setGrain(Grain g)  {
    _grain = g;
    signChanged();
}

void Sign::setData(SignData d)  {
    m_start = d.start;
    m_duration = d.duration;
    _dynamicProfile = d.dynamicProfile;
    _melodicProfile = d.melodicProfile;
    _rhythmicProfile = d.rhythmicProfile;
    _grain = d.grain;
    signChanged();
}

}

/***************************************
 * Partie SERIALISATION DYNAMIC PROFILE
 ***************************************/
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
