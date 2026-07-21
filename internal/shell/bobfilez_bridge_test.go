package shell

import (
	"testing"
	"time"

	"github.com/robertpelloni/bqt/internal/ui"
)

func TestBobfilezBridge_ConnectionLifecycle(t *testing.T) {
	// Re-initialize the global event loop to prevent pollution from other tests.
	ui.GetEventLoop().Stop()

	defer ui.GetEventLoop().Stop()

	bridge := NewBobfilezBridge()
	if bridge.Connected {
		t.Fatal("Expected new bridge to be disconnected initially")
	}

	err := bridge.ConnectShell("desktop-x11")
	if err != nil {
		t.Fatalf("Failed to connect shell: %v", err)
	}

	if !bridge.Connected {
		t.Fatal("Expected bridge to be connected after successful ConnectShell")
	}

	err = bridge.ConnectShell("web-wasm")
	if err == nil {
		t.Fatal("Expected error when attempting to connect an already connected shell")
	}

	bridge.DisconnectShell()
	if bridge.Connected {
		t.Fatal("Expected bridge to be disconnected after DisconnectShell")
	}
}

func TestBobfilezBridge_EventLoopIntegration(t *testing.T) {
	// Re-initialize the global event loop to prevent pollution.
	ui.GetEventLoop().Stop()
	go ui.GetEventLoop().Run()
	defer ui.GetEventLoop().Stop()

	bridge := NewBobfilezBridge()

	// Create a channel to verify async execution on the event loop.
	done := make(chan bool)

	err := bridge.ConnectShell("test-platform")
	if err != nil {
		t.Fatalf("Failed to connect: %v", err)
	}

	// Post a custom task through the bridge's event loop reference
	bridge.EventLoop.Post(func() {
		done <- true
	})

	select {
	case <-done:
		// Success
	case <-time.After(1 * time.Second):
		t.Fatal("Timeout waiting for event loop to process task via BobfilezBridge")
	}
}
