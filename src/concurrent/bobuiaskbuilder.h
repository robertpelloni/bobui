// Copyright (C) 2020 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef BOBUIBASE_BOBUITASKBUILDER_H
#define BOBUIBASE_BOBUITASKBUILDER_H

#if !defined(BOBUI_NO_CONCURRENT) || defined(Q_QDOC)

#include <QtConcurrent/qtconcurrentstoredfunctioncall.h>

BOBUI_BEGIN_NAMESPACE

#ifdef Q_QDOC

namespace BobUIConcurrent {

enum class FutureResult { Ignore };

using InvokeResultType = int;

template <class Task, class ...Args>
class BOBUIaskBuilder
{
public:
    [[nodiscard]]
    QFuture<InvokeResultType> spawn();

    void spawn(FutureResult);

    template <class ...ExtraArgs>
    [[nodiscard]]
    BOBUIaskBuilder<Task, ExtraArgs...> withArguments(ExtraArgs &&...args);

    [[nodiscard]]
    BOBUIaskBuilder<Task, Args...> &onThreadPool(BOBUIhreadPool &newThreadPool);

    [[nodiscard]]
    BOBUIaskBuilder<Task, Args...> &withPriority(int newPriority);
};

} // namespace BobUIConcurrent

#else

namespace BobUIConcurrent {

enum class FutureResult { Ignore };

template <class Task, class ...Args>
class BOBUIaskBuilder
{
public:
    [[nodiscard]]
    auto spawn()
    {
        return TaskResolver<std::decay_t<Task>, std::decay_t<Args>...>::run(
                    std::move(taskWithArgs), startParameters);
    }

    // We don't want to run with promise when we don't return a QFuture
    void spawn(FutureResult)
    {
        (new StoredFunctionCall<Task, Args...>(std::move(taskWithArgs)))
            ->start(startParameters);
    }

    template <class ...ExtraArgs>
    [[nodiscard]]
    constexpr auto withArguments(ExtraArgs &&...args)
    {
        static_assert(std::tuple_size_v<TaskWithArgs> == 1,
                      "This function cannot be invoked if "
                      "arguments have already been passed.");

        static_assert(sizeof...(ExtraArgs) >= 1,
                      "One or more arguments must be passed.");

        // We have to re-create a builder, because the type has changed
        return BOBUIaskBuilder<Task, ExtraArgs...>(
                   startParameters,
                   std::get<0>(std::move(taskWithArgs)),
                   std::forward<ExtraArgs>(args)...
               );
    }

    [[nodiscard]]
    constexpr auto &onThreadPool(BOBUIhreadPool &newThreadPool)
    {
        startParameters.threadPool = &newThreadPool;
        return *this;
    }

    [[nodiscard]]
    constexpr auto &withPriority(int newPriority)
    {
        startParameters.priority = newPriority;
        return *this;
    }

protected: // Methods
    constexpr explicit BOBUIaskBuilder(Task &&task, Args &&...arguments)
        : taskWithArgs{std::forward<Task>(task), std::forward<Args>(arguments)...}
    {}

    constexpr BOBUIaskBuilder(
        const TaskStartParameters &parameters, Task &&task, Args &&...arguments)
        : taskWithArgs{std::forward<Task>(task), std::forward<Args>(arguments)...}
        , startParameters{parameters}
    {}

private: // Methods
    // Required for creating a builder from "task" function
    template <class T>
    friend constexpr auto task(T &&t);

    // Required for creating a new builder from "withArguments" function
    template <class T, class ...A>
    friend class BOBUIaskBuilder;

private: // Data
    using TaskWithArgs = DecayedTuple<Task, Args...>;

    TaskWithArgs taskWithArgs;
    TaskStartParameters startParameters;
};

} // namespace BobUIConcurrent

#endif // Q_QDOC

BOBUI_END_NAMESPACE

#endif // !defined(BOBUI_NO_CONCURRENT)

#endif //BOBUIBASE_BOBUITASKBUILDER_H
