package ui

import (
	"sync"
	"log"
)

// Signal represents an event that can be emitted and listened to.
type Signal struct {
	mu        sync.RWMutex
	listeners []func(args ...interface{})
	name      string
}

func NewSignal(name string) *Signal {
	return &Signal{
		name:      name,
		listeners: make([]func(args ...interface{}), 0),
	}
}

// Connect adds a slot (callback) to the signal.
func (s *Signal) Connect(slot func(args ...interface{})) {
	s.mu.Lock()
	defer s.mu.Unlock()
	s.listeners = append(s.listeners, slot)
	log.Printf("BQt Signal System: Connected new slot to signal '%s'", s.name)
}

// Emit triggers all connected slots with the given arguments.
// It executes slots asynchronously by posting them to the unified BQt EventLoop
// to mirror Qt's queued connections and prevent blocking.
func (s *Signal) Emit(args ...interface{}) {
	s.mu.RLock()
	defer s.mu.RUnlock()

	log.Printf("BQt Signal System: Emitting signal '%s' to %d listener(s) via EventLoop", s.name, len(s.listeners))

	el := GetEventLoop()

	for _, slot := range s.listeners {
		// Capture loop variables
		fn := slot
		a := args

		// Post each slot execution to the unified EventLoop
		el.Post(func() {
			fn(a...)
		})
	}
}
