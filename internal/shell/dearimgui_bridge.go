package shell

import (
	"fmt"
	"log"
	"sync"
)

// DearImGuiBridge represents an exploratory parity spike for integrating
// DearImGui's immediate mode rendering loop with the BQt kernel.
// Because DearImGui redraws every frame, state must be synchronized
// safely from the BQt EventLoop into the DearImGui render thread.
type DearImGuiBridge struct {
	*ShellBridge

	mu        sync.Mutex
	Connected bool
}

// NewDearImGuiBridge creates a new experimental integration surface for DearImGui.
func NewDearImGuiBridge() *DearImGuiBridge {
	return &DearImGuiBridge{
		ShellBridge: NewShellBridge(),
		Connected:   false,
	}
}

// ConnectShell simulates attaching the DearImGui backend context.
func (b *DearImGuiBridge) ConnectShell(backend string) error {
	b.mu.Lock()
	defer b.mu.Unlock()

	if b.Connected {
		return fmt.Errorf("DearImGui shell is already connected")
	}

	b.Connected = true
	log.Printf("DearImGui shell attached via backend: %s", backend)

	// Acknowledge connection asynchronously on the BQt EventLoop
	b.EventLoop.Post(func() {
		log.Printf("DearImGui bridge verified on BQt EventLoop")
	})

	return nil
}

// DisconnectShell safely detaches the DearImGui shell.
func (b *DearImGuiBridge) DisconnectShell() {
	b.mu.Lock()
	defer b.mu.Unlock()
	b.Connected = false
	log.Println("DearImGui shell disconnected from BQt kernel.")
}
