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
EM_JS(void, qtSuspendResumeControlClearJs, (), {
    Module.qtSuspendResumeControl = {
        resume: null,
        eventHandlers: {},
        pendingEvents: [],
    };
});

// Suspends the calling thread
EM_ASYNC_JS(void, qtSuspendJs, (), {
    return new Promise(resolve => {
        Module.qtSuspendResumeControl.resume = resolve;
    });
});

// Registers a JS event handler which when called registers its index
// as the "current" event handler, and then resumes the wasm instance.
// The wasm instance will then call the C++ event after it is resumed.
EM_JS(void, qtRegisterEventHandlerJs, (int index), {
    let control = Module.qtSuspendResumeControl;
    let handler = (arg) => {
        control.pendingEvents.push({
            index: index,
            arg: arg
        });
        if (control.resume) {
            const resume = control.resume;
            control.resume = null;
            resume();
        } else {
            Module.qtSendPendingEvents(); // not suspended, call the handler directly
        }
    };
    control.eventHandlers[index] = handler;
});

QWasmSuspendResumeControl::QWasmSuspendResumeControl()
{
#if QT_CONFIG(thread)
    Q_ASSERT(emscripten_is_main_runtime_thread());
#endif
    qtSuspendResumeControlClearJs();
    Q_ASSERT(!QWasmSuspendResumeControl::s_suspendResumeControl);
    QWasmSuspendResumeControl::s_suspendResumeControl = this;
}

QWasmSuspendResumeControl::~QWasmSuspendResumeControl()
{
    qtSuspendResumeControlClearJs();
    Q_ASSERT(QWasmSuspendResumeControl::s_suspendResumeControl);
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

// Sends any pending events. Returns true if an event was sent, false otherwise.
bool QWasmSuspendResumeControl::sendPendingEvents()
{
#if QT_CONFIG(thread)
    Q_ASSERT(emscripten_is_main_runtime_thread());
#endif
    emscripten::val pendingEvents = suspendResumeControlJs()["pendingEvents"];
    int count = pendingEvents["length"].as<int>();
    if (count == 0)
        return false;
    while (count-- > 0) {
        // Grab one event (handler and arg), and call it
        emscripten::val event = pendingEvents.call<val>("shift");
        auto it = m_eventHandlers.find(event["index"].as<int>());
        Q_ASSERT(it != m_eventHandlers.end());
        it->second(event["arg"]);
    }
    return true;
}

void qtSendPendingEvents()
{
    if (QWasmSuspendResumeControl::s_suspendResumeControl)
        QWasmSuspendResumeControl::s_suspendResumeControl->sendPendingEvents();
}

EMSCRIPTEN_BINDINGS(qtSuspendResumeControl) {
    emscripten::function("qtSendPendingEvents", qtSendPendingEvents QT_WASM_EMSCRIPTEN_ASYNC);
}
