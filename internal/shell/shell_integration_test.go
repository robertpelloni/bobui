package shell

import (
	"testing"
)

func TestShellBridge(t *testing.T) {
	bridge := NewShellBridge()

	if bridge == nil {
		t.Fatalf("Expected NewShellBridge to return a valid instance, got nil")
	}

	if bridge.Engine == nil {
		t.Errorf("Expected ShellBridge to successfully initialize and expose ui.Engine, got nil")
	}

	if bridge.EventLoop == nil {
		t.Errorf("Expected ShellBridge to successfully initialize and expose ui.EventLoop, got nil")
	}
}
