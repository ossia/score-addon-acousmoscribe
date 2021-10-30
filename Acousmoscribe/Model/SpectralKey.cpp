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
    , m_nature(s.nature)
    , m_nature2(s.nature2)
    , m_isHybrid(s.isHybrid)
    , m_isHybrid2(s.isHybrid2)
    , m_isRich(s.isRich)
    , m_isRich2(s.isRich2)
    , m_isWarped(s.isWarped)
    , m_isWarped2(s.isWarped2)
    {}

Nature SpectralKey::nature() const noexcept {
    return m_nature;
}

Nature SpectralKey::nature2() const noexcept {
    return m_nature2;
}

bool SpectralKey::isHybrid() const noexcept {
    return m_isHybrid;
}

bool SpectralKey::isHybrid2() const noexcept {
    return m_isHybrid2;
}

bool SpectralKey::isRich() const noexcept {
    return m_isRich;
}

bool SpectralKey::isRich2() const noexcept {
    return m_isRich2;
}

bool SpectralKey::isWarped() const noexcept {
    return m_isWarped;
}

bool SpectralKey::isWarped2() const noexcept {
    return m_isWarped2;
}

SpectralKeyData SpectralKey::spectralKeyData() const {
    return SpectralKeyData{m_nature, m_nature2, m_isHybrid, m_isHybrid2, m_isRich, m_isRich2, m_isWarped, m_isWarped2};
}

void SpectralKey::setNature(Nature nature){
    m_nature = nature;
}

void SpectralKey::setNature2(Nature nature2){
    m_nature2 = nature2;
}

void SpectralKey::setIsHybrid(bool isHybrid){
    m_isHybrid = isHybrid;
}

void SpectralKey::setIsHybrid2(bool isHybrid2){
    m_isHybrid2 = isHybrid2;
}

void SpectralKey::setIsRich(bool isRich){
    m_isRich = isRich;
}

void SpectralKey::setIsRich2(bool isRich){
    m_isRich2 = isRich;
}

void SpectralKey::setIsWarped(bool isWarped){
    m_isHybrid2 = isWarped;
}

void SpectralKey::setIsWarped2(bool isWarped2){
    m_isHybrid2 = isWarped2;
}

void SpectralKey::setData(SpectralKeyData sd){
    m_nature = sd.nature;
    m_nature2 = sd.nature2;
    m_isHybrid = sd.isHybrid;
    m_isHybrid2 = sd.isHybrid2;
    m_isRich = sd.isRich;
    m_isRich2 = sd.isRich2;
    m_isWarped = sd.isWarped;
    m_isWarped2 = sd.isWarped2;
}

}
