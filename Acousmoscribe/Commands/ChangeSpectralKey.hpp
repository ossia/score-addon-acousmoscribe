#pragma once
#include "CommandFactory.hpp"
#include <Acousmoscribe/Model/SpectralKey.hpp>

#include <score/command/PropertyCommand.hpp>
#include <score/model/path/Path.hpp>

PROPERTY_COMMAND_T(
    Acousmoscribe,
    ChangeSpectralKey,
    SpectralKey::p_spectralKey,
    "Change spectral key")
SCORE_COMMAND_DECL_T(Acousmoscribe::ChangeSpectralKey)


