package shell

import (
	"fmt"
	"log"
	"sync"
	"github.com/robertpelloni/bqt/internal/ui"
)

// BobfilezBridge maps the BQt kernel's ShellBridge into specific endpoints
// required by the bobfilez OS shell, effectively binding web/desktop views.
type BobfilezBridge struct {
	*ShellBridge

	// mu protects concurrent state access from the shell.
	mu sync.Mutex

	// Connected indicates if a bobfilez client is actively bound.
	Connected bool
}

// NewBobfilezBridge creates a new integration surface specifically tuned
// for the bobfilez OS shell consumer.
func NewBobfilezBridge() *BobfilezBridge {
	return &BobfilezBridge{
		ShellBridge: NewShellBridge(),
		Connected:   false,
	}
}

// ConnectShell simulates the attachment of a bobfilez desktop or web frontend
// to the BQt kernel.
func (b *BobfilezBridge) ConnectShell(platform string) error {
	b.mu.Lock()
	defer b.mu.Unlock()

	if b.Connected {
		return fmt.Errorf("bobfilez shell is already connected")
	}

	b.Connected = true
	log.Printf("Bobfilez shell attached via platform: %s", platform)

	// Here we could register platform-specific window handles or web sockets.
	// For now, we utilize the BQt EventLoop to acknowledge the connection asynchronously.
	b.EventLoop.Post(func() {
		log.Printf("Bobfilez bridge verified on BQt EventLoop")
	})

	return nil
}

// DisconnectShell safely detaches the bobfilez shell from the kernel.
func (b *BobfilezBridge) DisconnectShell() {
	b.mu.Lock()
	defer b.mu.Unlock()
	b.Connected = false
	log.Println("Bobfilez shell disconnected from BQt kernel.")
}
