#pragma once
#include <Acousmoscribe/Commands/CommandFactory.hpp>
#include <Acousmoscribe/Model/MelodicKey.hpp>
#include <score/tools/Unused.hpp>

#include <score/command/PropertyCommand.hpp>
#include <score/model/path/Path.hpp>

PROPERTY_COMMAND_T(
    Acousmoscribe,
    ChangeMelodicKey,
    MelodicKey::p_melodicKey,
    "Change melodic key")
SCORE_COMMAND_DECL_T(Acousmoscribe::ChangeMelodicKey)
