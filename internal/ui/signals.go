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
// It executes slots asynchronously via goroutines to mirror Qt's queued connections
// and prevent blocking the main event loop.
func (s *Signal) Emit(args ...interface{}) {
	s.mu.RLock()
	defer s.mu.RUnlock()

	log.Printf("BQt Signal System: Emitting signal '%s' to %d listener(s)", s.name, len(s.listeners))

	for _, slot := range s.listeners {
		// Execute each slot in a separate goroutine for non-blocking behavior
		go func(fn func(args ...interface{}), a []interface{}) {
			fn(a...)
		}(slot, args)
	}
}
