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
	quit   chan struct{}
	wakeup chan struct{}
	running bool
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
			quit:   make(chan struct{}),
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
	if el.running {
		el.mu.Unlock()
		return
	}
	el.running = true
	// Reset quit channel to allow restarting
	el.quit = make(chan struct{})
	quit := el.quit
	el.mu.Unlock()

	log.Println("BQt Unified Event Loop starting...")

	for {
		select {
		case <-quit:
			return
		case <-el.wakeup:
			el.mu.Lock()
			tasks := el.queue
			el.queue = make([]func(), 0)
			el.mu.Unlock()

			for _, task := range tasks {
				task()
			}
		}
	}
}

// Stop signals the EventLoop to stop processing and exit the Run loop.
func (el *EventLoop) Stop() {
	el.mu.Lock()
	defer el.mu.Unlock()

	if !el.running {
		return
	}
	el.running = false

	close(el.quit)
}
