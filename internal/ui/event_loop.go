package ui

import (
	"log"
	"sync"
)

// EventLoop synchronizes and processes generic tasks and events
// for the unified BQt interface (bridging GTK, JUCE, and U++ components).
type EventLoop struct {
	mu     sync.Mutex
	queue  []func()
	active bool
	wakeup chan struct{}
}

var (
	globalEventLoop *EventLoop
	once            sync.Once
)

// GetEventLoop returns the global BQt unified EventLoop singleton.
func GetEventLoop() *EventLoop {
	once.Do(func() {
		globalEventLoop = &EventLoop{
			queue:  make([]func(), 0),
			wakeup: make(chan struct{}, 1),
		}
	})
	return globalEventLoop
}

// Post queues an event to be processed on the main unified loop thread.
func (el *EventLoop) Post(task func()) {
	el.mu.Lock()
	el.queue = append(el.queue, task)
	el.mu.Unlock()

	// Non-blocking wakeup signal
	select {
	case el.wakeup <- struct{}{}:
	default:
	}
}

// Run blocks and processes events continuously.
// It is intended to run as the core orchestration thread unifying sub-frameworks.
func (el *EventLoop) Run() {
	el.mu.Lock()
	if el.active {
		el.mu.Unlock()
		return
	}
	el.active = true
	el.mu.Unlock()

	log.Println("BQt Unified Event Loop starting...")

	for {
		<-el.wakeup

		el.mu.Lock()
		tasks := el.queue
		el.queue = make([]func(), 0)
		el.mu.Unlock()

		for _, task := range tasks {
			task()
		}
	}
}
