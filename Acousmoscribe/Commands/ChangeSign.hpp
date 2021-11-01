#pragma once
#include "CommandFactory.hpp"
#include <Acousmoscribe/Model/Sign.hpp>

#include <score/command/PropertyCommand.hpp>
#include <score/model/path/Path.hpp>

PROPERTY_COMMAND_T(
    Acousmoscribe,
    ChangeSign,
    Sign::p_signData,
    "Change sign")
SCORE_COMMAND_DECL_T(Acousmoscribe::ChangeSign)

