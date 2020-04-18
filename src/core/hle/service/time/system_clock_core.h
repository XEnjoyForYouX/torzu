// Copyright 2019 yuzu emulator team
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

#pragma once

#include "common/common_types.h"
#include "core/hle/service/time/clock_types.h"

namespace Core {
class System;
}

namespace Service::Time::Clock {

class SteadyClockCore;
class SystemClockContextUpdateCallback;

// Parts of this implementation were based on Ryujinx (https://github.com/Ryujinx/Ryujinx/pull/783).
// This code was released under public domain.

class SystemClockCore {
public:
    explicit SystemClockCore(SteadyClockCore& steady_clock_core);
    virtual ~SystemClockCore();

    SteadyClockCore& GetSteadyClockCore() const {
        return steady_clock_core;
    }

    ResultCode GetCurrentTime(Core::System& system, s64& posix_time) const;

    ResultCode SetCurrentTime(Core::System& system, s64 posix_time);

    virtual ResultCode GetClockContext([[maybe_unused]] Core::System& system,
                                       SystemClockContext& value) const {
        value = context;
        return RESULT_SUCCESS;
    }

    virtual ResultCode SetClockContext(const SystemClockContext& value) {
        context = value;
        return RESULT_SUCCESS;
    }

    virtual ResultCode Flush(const SystemClockContext& context);

    void SetUpdateCallbackInstance(std::shared_ptr<SystemClockContextUpdateCallback> callback) {
        system_clock_context_update_callback = std::move(callback);
    }

    ResultCode SetSystemClockContext(const SystemClockContext& context);

    bool IsInitialized() const {
        return is_initialized;
    }

    void MarkAsInitialized() {
        is_initialized = true;
    }

    bool IsClockSetup(Core::System& system) const;

private:
    SteadyClockCore& steady_clock_core;
    SystemClockContext context{};
    bool is_initialized{};
    std::shared_ptr<SystemClockContextUpdateCallback> system_clock_context_update_callback;
};

} // namespace Service::Time::Clock
