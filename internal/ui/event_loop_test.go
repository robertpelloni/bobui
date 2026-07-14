package ui

import (
	"sync"
	"testing"
	"time"
)

func TestEventLoopSynchronization(t *testing.T) {
	el := GetEventLoop()

	// Start the event loop in a goroutine
	go el.Run()

	var wg sync.WaitGroup
	wg.Add(3)

	var results []string
	var mu sync.Mutex

	// Post tasks simulating events from different sub-frameworks
	el.Post(func() {
		mu.Lock()
		results = append(results, "GTK Event")
		mu.Unlock()
		wg.Done()
	})

	el.Post(func() {
		mu.Lock()
		results = append(results, "JUCE Event")
		mu.Unlock()
		wg.Done()
	})

	el.Post(func() {
		mu.Lock()
		results = append(results, "U++ Event")
		mu.Unlock()
		wg.Done()
	})

	// Wait with timeout to ensure events are processed
	c := make(chan struct{})
	go func() {
		wg.Wait()
		close(c)
	}()

	select {
	case <-c:
		// success
	case <-time.After(1 * time.Second):
		t.Fatalf("EventLoop processing timed out")
	}

	mu.Lock()
	defer mu.Unlock()
	if len(results) != 3 {
		t.Fatalf("Expected 3 events processed, got %d", len(results))
	}
}
