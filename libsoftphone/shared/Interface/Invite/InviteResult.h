#pragma once

#include "ali_string.h"
#include "ali_json.h"

namespace Softphone
{
namespace Invite
{
    using ResultCallback = ali::callback<void(ali::string const& message, ali::json::array const&)>;
}
}

