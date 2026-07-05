package shell

import (
	"github.com/robertpelloni/bqt/internal/ui"
)

// ShellBridge provides the integration layer for external UI shells (like bobfilez)
// to attach to the BQt kernel's core execution context.
type ShellBridge struct {
	Engine    *ui.Engine
	EventLoop *ui.EventLoop
}

// NewShellBridge initializes a new bridge for an external shell to consume.
func NewShellBridge() *ShellBridge {
	return &ShellBridge{
		Engine:    ui.NewEngine(),
		EventLoop: ui.GetEventLoop(),
	}
}

// Run executes the core UI engine loop.
func (sb *ShellBridge) Run() error {
	return sb.Engine.Run()
}
