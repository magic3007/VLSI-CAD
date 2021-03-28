#pragma once

#include <utility>

#define CLASS_ARG(T, name)                                                                         \
public:                                                                                            \
    inline const T &name() const noexcept { /* NOLINT */                                           \
        return this->name##_;                                                                      \
    }                                                                                              \
    inline T &name() noexcept { /* NOLINT */                                                       \
        return this->name##_;                                                                      \
    }                                                                                              \
                                                                                                   \
private:                                                                                           \
    T name##_ /* NOLINT */

#define SHARED_CLASS_ARG(ClassName, name)                                                          \
public:                                                                                            \
    explicit ClassName(std::shared_ptr<ClassName##Impl> name) : name##_(std::move(name)) {}        \
                                                                                                   \
private:                                                                                           \
    std::shared_ptr<ClassName##Impl> name##_ /* NOLINT */

#define FORWARDED_METHOD(name)                                                                     \
    template<typename... Args>                                                                     \
    decltype(auto) name(Args &&...args) {                                                          \
        return impl_->name(std::forward<Args>(args)...);                                           \
    }

#define MAKE_SHARED_CLASS(name)                                                                    \
    namespace detail {                                                                             \
    template<typename... Args>                                                                     \
    decltype(auto) make##name(Args &&...args) {                                                    \
        return name(std::make_shared<name##Impl>(std::forward<Args>(args)...));                    \
    }                                                                                              \
    }

#define CUSTOM_MAKE_SHARED_CLASS(name, impl)                                                       \
    namespace detail {                                                                             \
    template<typename... Args>                                                                     \
    decltype(auto) make##name(Args &&...args) {                                                    \
        return name(std::make_shared<impl>(std::forward<Args>(args)...));                          \
    }                                                                                              \
    }
