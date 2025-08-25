// Copyright (C) 2025 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwasmsuspendresumecontrol_p.h"
#include "qstdweb_p.h"

#include <emscripten.h>
#include <emscripten/val.h>
#include <emscripten/bind.h>

using emscripten::val;

/*
    QWasmSuspendResumeControl controls asyncify suspend and resume when handling native events.

    The class supports registering C++ event handlers, and creates a corresponding
    JavaScript event handler which can be passed to addEventListener() or similar
    API:

      auto handler = [](emscripten::val argument){
         // handle event
      };
      uint32_t index = control->registerEventHandler(handler);
      element.call<void>("addEventListener", "eventname", control->jsEventHandlerAt(index));

    The wasm instance suspends itself by calling the suspend() function, which resumes
    and returns whenever there was a native event. Call sendPendingEvents() to send
    the native event and invoke the C++ event handlers.

      // about to suspend
      control->suspend(); // <- instance/app sleeps here
      // was resumed, send event(s)
      control->sendPendingEvents();

    QWasmSuspendResumeControl also supports the case where the wasm instance returns
    control to the browser's event loop (without suspending), and will call the C++
    event handlers directly in that case.
*/

QWasmSuspendResumeControl *QWasmSuspendResumeControl::s_suspendResumeControl = nullptr;

// Setup/constructor function for Module.suspendResumeControl.
// FIXME if assigning to the Module object from C++ is/becomes possible
// then this does not need to be a separate JS function.
void qtSuspendResumeControlClearJs() {
    EM_ASM({
        Module.qtSuspendResumeControl = ({
            resume: null,
            asyncifyEnabled: false, // asyncify 1 or JSPI enabled
            eventHandlers: {},
            pendingEvents: [],
            exclusiveEventHandler: 0,
        });
    });
}

// Suspends the calling thread
EM_ASYNC_JS(void, qtSuspendJs, (), {
    return new Promise(resolve => {
        Module.qtSuspendResumeControl.resume = resolve;
    });
});

// Registers a JS event handler which when called registers its index
// as the "current" event handler, and then resumes the wasm instance.
// The wasm instance will then call the C++ event after it is resumed.
void qtRegisterEventHandlerJs(int index) {
    EM_ASM({

        function createNamedFunction(name, parent, obj) {
            return {
                [name]: function(...args) {
                    return obj.call(parent, args);
                }
            }[name];
        }

        function deepShallowClone(parent, obj, depth) {
            if (obj === null)
                return obj;

            if (typeof obj === 'function') {
                if (obj.name !== "")
                    return createNamedFunction(obj.name, parent, obj);
            }

            if (depth >= 1)
                return obj;

            if (typeof obj !== 'object')
                return obj;

            if (Array.isArray(obj)) {
                const arrCopy = [];
                for (let i = 0; i < obj.length; i++)
                    arrCopy[i] = deepShallowClone(obj, obj[i], depth + 1);

                return arrCopy;
            }

            const objCopy = {};
            for (const key in obj)
                objCopy[key] = deepShallowClone(obj, obj[key], depth + 1);

            return objCopy;
        }

        let index = $0;
        let control = Module.qtSuspendResumeControl;
        let handler = (arg) => {
            // Copy the top level object, alias the rest.
            // functions are copied by creating new forwarding functions.
            arg = deepShallowClone(arg, arg, 0);

            // Add event to event queue
            control.pendingEvents.push({
                index: index,
                arg: arg
            });

            // Handle the event based on instance state and asyncify flag
            if (control.exclusiveEventHandler > 0) {
                // In exclusive mode, resume on exclusive event handler match only
                if (index != control.exclusiveEventHandler)
                    return;

                const resume = control.resume;
                control.resume = null;
                resume();
            } else if (control.resume) {
                // The instance is suspended in processEvents(), resume and process the event
                const resume = control.resume;
                control.resume = null;
                resume();
            } else {
                if (control.asyncifyEnabled) {
                    // The instance is either not suspended or is supended outside of processEvents()
                    // (e.g. on emscripten_sleep()). Currently there is no way to determine
                    // which state the instance is in. Keep the event in the event queue to be
                    // processed on the next processEvents() call.
                    // FIXME: call event handler here if we can determine that the instance
                    // is not suspended.
                } else {
                    // The instance is not suspended, call the handler directly
                    Module.qtSendPendingEvents();
                }
            }
        };
        control.eventHandlers[index] = handler;
    }, index);
}

QWasmSuspendResumeControl::QWasmSuspendResumeControl()
{
#if QT_CONFIG(thread)
    Q_ASSERT(emscripten_is_main_runtime_thread());
#endif
    qtSuspendResumeControlClearJs();
    suspendResumeControlJs().set("asyncifyEnabled", qstdweb::haveAsyncify());
    QWasmSuspendResumeControl::s_suspendResumeControl = this;
}

QWasmSuspendResumeControl::~QWasmSuspendResumeControl()
{
    qtSuspendResumeControlClearJs();
    QWasmSuspendResumeControl::s_suspendResumeControl = nullptr;
}

QWasmSuspendResumeControl *QWasmSuspendResumeControl::get()
{
    Q_ASSERT_X(s_suspendResumeControl, "QWasmSuspendResumeControl", "Must create a QWasmSuspendResumeControl instance first");
    return s_suspendResumeControl;
}

// Registers a C++ event handler.
uint32_t QWasmSuspendResumeControl::registerEventHandler(std::function<void(val)> handler)
{
    static uint32_t i = 0;
    ++i;
    m_eventHandlers.emplace(i, std::move(handler));
    qtRegisterEventHandlerJs(i);
    return i;
}

// Removes a C++ event handler
void QWasmSuspendResumeControl::removeEventHandler(uint32_t index)
{
    m_eventHandlers.erase(index);
    suspendResumeControlJs()["eventHandlers"].set(index, val::null());
}

// Returns the JS event handler for the given index
val QWasmSuspendResumeControl::jsEventHandlerAt(uint32_t index)
{
    return suspendResumeControlJs()["eventHandlers"][index];
}

emscripten::val QWasmSuspendResumeControl::suspendResumeControlJs()
{
    return val::module_property("qtSuspendResumeControl");
}

// Suspends the calling thread.
void QWasmSuspendResumeControl::suspend()
{
    qtSuspendJs();
}

void QWasmSuspendResumeControl::suspendExclusive(uint32_t eventHandlerIndex)
{
    suspendResumeControlJs().set("exclusiveEventHandler", eventHandlerIndex);
    qtSuspendJs();
}

// Sends any pending events. Returns the number of sent events.
int QWasmSuspendResumeControl::sendPendingEvents()
{
#if QT_CONFIG(thread)
    Q_ASSERT(emscripten_is_main_runtime_thread());
#endif
    emscripten::val control = suspendResumeControlJs();
    emscripten::val pendingEvents = control["pendingEvents"];

    if (control["exclusiveEventHandler"].as<int>() > 0)
        return sendPendingExclusiveEvent();

    if (pendingEvents["length"].as<int>() == 0)
        return 0;

    int count = 0;
    while (pendingEvents["length"].as<int>() > 0) { // Make sure it is reentrant
        // Grab one event (handler and arg), and call it
        emscripten::val event = pendingEvents.call<val>("shift");
        auto it = m_eventHandlers.find(event["index"].as<int>());
        Q_ASSERT(it != m_eventHandlers.end());
        it->second(event["arg"]);
        ++count;
    }
    return count;
}

// Sends the pending exclusive event, and resets the "exclusive" state
int QWasmSuspendResumeControl::sendPendingExclusiveEvent()
{
    emscripten::val control = suspendResumeControlJs();
    int exclusiveHandlerIndex = control["exclusiveEventHandler"].as<int>();
    control.set("exclusiveEventHandler", 0);
    emscripten::val event = control["pendingEvents"].call<val>("pop");
    int eventHandlerIndex = event["index"].as<int>();
    Q_ASSERT(exclusiveHandlerIndex == eventHandlerIndex);
    auto it = m_eventHandlers.find(eventHandlerIndex);
    Q_ASSERT(it != m_eventHandlers.end());
    it->second(event["arg"]);
    return 1;
}

void qtSendPendingEvents()
{
    if (QWasmSuspendResumeControl::s_suspendResumeControl)
        QWasmSuspendResumeControl::s_suspendResumeControl->sendPendingEvents();
}

EMSCRIPTEN_BINDINGS(qtSuspendResumeControl) {
    emscripten::function("qtSendPendingEvents", qtSendPendingEvents QT_WASM_EMSCRIPTEN_ASYNC);
}

//
// The EventCallback class registers a callback function for an event on an html element.
//
QWasmEventHandler::QWasmEventHandler(emscripten::val element, const std::string &name, std::function<void(emscripten::val)> handler)
:m_element(element)
,m_name(name)
{
    QWasmSuspendResumeControl *suspendResume = QWasmSuspendResumeControl::get();
    Q_ASSERT(suspendResume); // must construct the event dispatcher or platform integration first
    m_eventHandlerIndex = suspendResume->registerEventHandler(std::move(handler));
    m_element.call<void>("addEventListener", m_name, suspendResume->jsEventHandlerAt(m_eventHandlerIndex));
}

QWasmEventHandler::~QWasmEventHandler()
{
    // Do nothing if this instance is default-constructed, or was moved from.
    if (m_element.isUndefined())
        return;

    QWasmSuspendResumeControl *suspendResume = QWasmSuspendResumeControl::get();
    Q_ASSERT(suspendResume);
    m_element.call<void>("removeEventListener", m_name, suspendResume->jsEventHandlerAt(m_eventHandlerIndex));
    suspendResume->removeEventHandler(m_eventHandlerIndex);
}

QWasmEventHandler::QWasmEventHandler(QWasmEventHandler&& other) noexcept
:m_element(std::move(other.m_element))
,m_name(std::move(other.m_name))
,m_eventHandlerIndex(other.m_eventHandlerIndex)
{
    other.m_element = emscripten::val();
    other.m_name = emscripten::val();
    other.m_eventHandlerIndex = 0;
}

QWasmEventHandler& QWasmEventHandler::operator=(QWasmEventHandler&& other) noexcept
{
    m_element = std::move(other.m_element);
    other.m_element = emscripten::val();
    m_name = std::move(other.m_name);
    other.m_name = emscripten::val();
    m_eventHandlerIndex = other.m_eventHandlerIndex;
    other.m_eventHandlerIndex = 0;
    return *this;
}

//
// The QWasmTimer class creates a native single-shot timer. The event handler is provided in the
// constructor and can be reused: each call setTimeout() sets a new timeout, though with the
// limitiation that there can be only one timeout at a time. (Setting a new timer clears the
// previous one).
//
QWasmTimer::QWasmTimer(QWasmSuspendResumeControl *suspendResume, std::function<void()> handler)
    :m_suspendResume(suspendResume)
{
    auto wrapper = [handler = std::move(handler), this](val argument) {
        Q_UNUSED(argument); // no argument for timers
        if (!m_timerId)
            return; // timer was cancelled
        m_timerId = 0;
        handler();
    };

    m_handlerIndex = m_suspendResume->registerEventHandler(std::move(wrapper));
}

QWasmTimer::~QWasmTimer()
{
    clearTimeout();
    m_suspendResume->removeEventHandler(m_handlerIndex);
}

void QWasmTimer::setTimeout(std::chrono::milliseconds timeout)
{
    if (hasTimeout())
        clearTimeout();
    val jsHandler = QWasmSuspendResumeControl::get()->jsEventHandlerAt(m_handlerIndex);
    using ArgType = double; // emscripten::val::call() does not support int64_t
    ArgType timoutValue = static_cast<ArgType>(timeout.count());
    ArgType timerId = val::global("window").call<ArgType>("setTimeout", jsHandler, timoutValue);
    m_timerId = static_cast<int64_t>(std::round(timerId));
}

bool QWasmTimer::hasTimeout()
{
    return m_timerId > 0;
}

void QWasmTimer::clearTimeout()
{
    val::global("window").call<void>("clearTimeout", double(m_timerId));
    m_timerId = 0;
}
