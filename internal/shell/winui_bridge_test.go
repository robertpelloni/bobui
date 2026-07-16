package shell

import (
	"testing"
	"time"

	"github.com/robertpelloni/bqt/internal/ui"
)

func TestWinUIBridge_ConnectionLifecycle(t *testing.T) {
	ui.GetEventLoop().Stop()

	defer ui.GetEventLoop().Stop()

	bridge := NewWinUIBridge()
	if bridge.Connected {
		t.Fatal("Expected new bridge to be disconnected initially")
	}

	err := bridge.ConnectShell("0x000000000001004A")
	if err != nil {
		t.Fatalf("Failed to connect shell: %v", err)
	}

	if !bridge.Connected {
		t.Fatal("Expected bridge to be connected after successful ConnectShell")
	}

	err = bridge.ConnectShell("0x000000000001005B")
	if err == nil {
		t.Fatal("Expected error when attempting to connect an already connected shell")
	}

	bridge.DisconnectShell()
	if bridge.Connected {
		t.Fatal("Expected bridge to be disconnected after DisconnectShell")
	}
}

func TestWinUIBridge_EventLoopIntegration(t *testing.T) {
	ui.GetEventLoop().Stop()
	go ui.GetEventLoop().Run()
	defer ui.GetEventLoop().Stop()

	bridge := NewWinUIBridge()
	done := make(chan bool)

	err := bridge.ConnectShell("test-hwnd")
	if err != nil {
		t.Fatalf("Failed to connect: %v", err)
	}

	bridge.EventLoop.Post(func() {
		done <- true
	})

	select {
	case <-done:
	case <-time.After(1 * time.Second):
		t.Fatal("Timeout waiting for event loop to process task via WinUIBridge")
	}
}
