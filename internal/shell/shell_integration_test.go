package shell

import (
	"sync"
	"testing"
	"time"

	"github.com/robertpelloni/bqt/internal/ui"
)

func TestShellBridge_LifecycleHooks(t *testing.T) {
	// Setup event loop
	ui.GetEventLoop().Stop()
	go ui.GetEventLoop().Run()
	defer ui.GetEventLoop().Stop()

	bridge := NewShellBridge()

	var wg sync.WaitGroup
	wg.Add(2)

	var attachedPlatform string
	var detachedPlatform string
	var mu sync.Mutex

	bridge.RegisterConnectHook(func(platform string) {
		mu.Lock()
		attachedPlatform = platform
		mu.Unlock()
		wg.Done()
	})

	bridge.RegisterDisconnectHook(func(platform string) {
		mu.Lock()
		detachedPlatform = platform
		mu.Unlock()
		wg.Done()
	})

	// Trigger lifecycle events
	bridge.AttachShell("bobfilez-test")
	bridge.DetachShell("bobfilez-test")

	// Wait for async hooks to run on the event loop
	done := make(chan struct{})
	go func() {
		wg.Wait()
		close(done)
	}()

	select {
	case <-done:
	case <-time.After(2 * time.Second):
		t.Fatal("Timeout waiting for shell lifecycle hooks to execute on EventLoop")
	}

	mu.Lock()
	defer mu.Unlock()
	if attachedPlatform != "bobfilez-test" {
		t.Errorf("Expected connect hook to receive 'bobfilez-test', got '%s'", attachedPlatform)
	}
	if detachedPlatform != "bobfilez-test" {
		t.Errorf("Expected disconnect hook to receive 'bobfilez-test', got '%s'", detachedPlatform)
	}
}

func TestShellBridge_DispatchPassthrough(t *testing.T) {
	// Setup event loop
	ui.GetEventLoop().Stop()
	go ui.GetEventLoop().Run()
	defer ui.GetEventLoop().Stop()

	bridge := NewShellBridge()

	// Simulate a GTK/JUCE/U++ widget event being routed through the shell boundary
	dispatched := false
	done := make(chan struct{})

	// Emulate shell bridging an OS event down to the BQt kernel
	bridge.Dispatch(func() {
		dispatched = true
		close(done)
	})

	select {
	case <-done:
	case <-time.After(1 * time.Second):
		t.Fatal("Timeout waiting for Dispatch to marshal event to EventLoop")
	}

	if !dispatched {
		t.Error("Expected dispatched to be true after EventLoop execution")
	}
}
