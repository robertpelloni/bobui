package shell

import (
	"fmt"
	"log"
	"sync"
)

// WinUIBridge represents an exploratory parity spike for integrating
// Windows UI (WinUI 3 / App SDK) events into the BQt kernel.
// Like JavaFX, WinUI has its own strict UI thread (DispatcherQueue).
// Events must be marshaled asynchronously over the BQt EventLoop.
type WinUIBridge struct {
	*ShellBridge

	mu        sync.Mutex
	Connected bool
}

// NewWinUIBridge creates a new experimental integration surface for WinUI.
func NewWinUIBridge() *WinUIBridge {
	return &WinUIBridge{
		ShellBridge: NewShellBridge(),
		Connected:   false,
	}
}

// ConnectShell simulates attaching a native WinUI window surface.
func (b *WinUIBridge) ConnectShell(hwnd string) error {
	b.mu.Lock()
	defer b.mu.Unlock()

	if b.Connected {
		return fmt.Errorf("WinUI shell is already connected")
	}

	b.Connected = true
	log.Printf("WinUI shell attached via HWND: %s", hwnd)

	// Acknowledge connection asynchronously on the BQt EventLoop
	b.EventLoop.Post(func() {
		log.Printf("WinUI bridge verified on BQt EventLoop")
	})

	return nil
}

// DisconnectShell safely detaches the WinUI shell.
func (b *WinUIBridge) DisconnectShell() {
	b.mu.Lock()
	defer b.mu.Unlock()
	b.Connected = false
	log.Println("WinUI shell disconnected from BQt kernel.")
}
