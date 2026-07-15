package shell

import (
	"log"
	"sync"

	"github.com/robertpelloni/bqt/internal/ui"
)

// ShellLifecycleHook defines a callback executed during shell attachment/detachment.
type ShellLifecycleHook func(platform string)

// ShellBridge provides the unified integration API layer for external UI shells
// (e.g., bobfilez, JavaFX, WinUI) to securely attach to the BQt kernel's core execution context.
type ShellBridge struct {
	Engine    *ui.Engine
	EventLoop *ui.EventLoop

	mu              sync.RWMutex
	onConnected     []ShellLifecycleHook
	onDisconnected  []ShellLifecycleHook
	activePlatforms map[string]bool
}

// NewShellBridge initializes the primary API boundary for external shell consumption.
func NewShellBridge() *ShellBridge {
	return &ShellBridge{
		Engine:          ui.NewEngine(),
		EventLoop:       ui.GetEventLoop(),
		activePlatforms: make(map[string]bool),
		onConnected:     make([]ShellLifecycleHook, 0),
		onDisconnected:  make([]ShellLifecycleHook, 0),
	}
}

// RegisterConnectHook adds a callback to be executed when a shell platform attaches.
func (sb *ShellBridge) RegisterConnectHook(hook ShellLifecycleHook) {
	sb.mu.Lock()
	defer sb.mu.Unlock()
	sb.onConnected = append(sb.onConnected, hook)
}

// RegisterDisconnectHook adds a callback to be executed when a shell platform detaches.
func (sb *ShellBridge) RegisterDisconnectHook(hook ShellLifecycleHook) {
	sb.mu.Lock()
	defer sb.mu.Unlock()
	sb.onDisconnected = append(sb.onDisconnected, hook)
}

// Dispatch marshals an arbitrary event callback to the BQt kernel's unified EventLoop.
// This is the primary mechanism for shells to send input or state changes non-blockingly.
func (sb *ShellBridge) Dispatch(event func()) {
	sb.EventLoop.Post(event)
}

// AttachShell registers a specific platform shell (e.g., "bobfilez-web", "javafx-desktop").
func (sb *ShellBridge) AttachShell(platform string) {
	sb.mu.Lock()
	if sb.activePlatforms[platform] {
		sb.mu.Unlock()
		return
	}
	sb.activePlatforms[platform] = true
	hooks := append([]ShellLifecycleHook(nil), sb.onConnected...)
	sb.mu.Unlock()

	log.Printf("BQt Kernel: Attaching shell platform '%s'", platform)
	for _, hook := range hooks {
		// Execute hooks on the unified event loop to prevent deadlocks
		hookFunc := hook
		plat := platform
		sb.Dispatch(func() { hookFunc(plat) })
	}
}

// DetachShell deregisters a specific platform shell.
func (sb *ShellBridge) DetachShell(platform string) {
	sb.mu.Lock()
	if !sb.activePlatforms[platform] {
		sb.mu.Unlock()
		return
	}
	delete(sb.activePlatforms, platform)
	hooks := append([]ShellLifecycleHook(nil), sb.onDisconnected...)
	sb.mu.Unlock()

	log.Printf("BQt Kernel: Detaching shell platform '%s'", platform)
	for _, hook := range hooks {
		hookFunc := hook
		plat := platform
		sb.Dispatch(func() { hookFunc(plat) })
	}
}

// Run executes the core UI engine loop.
func (sb *ShellBridge) Run() error {
	return sb.Engine.Run()
}
