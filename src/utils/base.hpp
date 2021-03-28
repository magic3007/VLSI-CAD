#pragma once

#include <functional>

#define DISABLE_COPY_AND_ASSIGN(ClassName)                                                         \
    ClassName(const ClassName &) = delete;                                                         \
    ClassName &operator=(const ClassName &) = delete;

#define DEFER_CONCAT_IMPL(x, y) x##y
#define DEFER_CONCAT(x, y)      DEFER_CONCAT_IMPL(x, y)

class DeferredAction {
    DISABLE_COPY_AND_ASSIGN(DeferredAction);

public:
    explicit DeferredAction(std::function<void()> f) : f_(std::move(f)) {}
    ~DeferredAction() { f_(); }

private:
    std::function<void()> f_;
};

#define DEFER(action) DeferredAction DEFER_CONCAT(__deferred__, __LINE__)([&]() -> void action)