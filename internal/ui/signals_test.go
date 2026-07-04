package ui

import (
	"sync"
	"testing"
	"time"
)

func TestSignalEmission(t *testing.T) {
	sig := NewSignal("TestSignal")

	var wg sync.WaitGroup
	receivedVal := 0

	wg.Add(1)
	sig.Connect(func(args ...interface{}) {
		defer wg.Done()
		if len(args) > 0 {
			if val, ok := args[0].(int); ok {
				receivedVal = val
			}
		}
	})

	sig.Emit(42)

	// Wait for the async slot to complete
	done := make(chan struct{})
	go func() {
		wg.Wait()
		close(done)
	}()

	select {
	case <-done:
		if receivedVal != 42 {
			t.Errorf("Expected to receive 42, got %d", receivedVal)
		}
	case <-time.After(time.Second):
		t.Fatal("Timeout waiting for signal emission")
	}
}
