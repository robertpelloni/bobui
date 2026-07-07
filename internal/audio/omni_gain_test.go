package audio

import (
	"sync"
	"testing"
	"time"

	"github.com/robertpelloni/bqt/internal/ui"
)

func TestOmniGain(t *testing.T) {
	gain := NewOmniGain()
	if gain.Name() != "OmniGain" {
		t.Errorf("Expected name OmniGain, got %s", gain.Name())
	}

	gain.SetVolume(0.5)

	buffer := make([]float32, 10)
	for i := range buffer {
		buffer[i] = 1.0
	}

	gain.Process(buffer)

	for i, v := range buffer {
		if v != 0.5 {
			t.Errorf("Expected 0.5 at index %d, got %f", i, v)
		}
	}
}

func TestOmniGainSignalEmission(t *testing.T) {
	// Initialize UI EventLoop for signal dispatch
	el := ui.GetEventLoop()
	go el.Run()
	defer el.Stop()

	gain := NewOmniGain()
	var wg sync.WaitGroup
	wg.Add(1)

	var emittedVol float32
	signalReceived := false

	gain.GainChanged.Connect(func(args ...interface{}) {
		defer wg.Done()
		signalReceived = true
		if len(args) > 0 {
			if v, ok := args[0].(float32); ok {
				emittedVol = v
			}
		}
	})

	// Trigger signal emission by changing volume
	gain.SetVolume(0.75)

	// Wait for async dispatch
	done := make(chan struct{})
	go func() {
		wg.Wait()
		close(done)
	}()

	select {
	case <-done:
		if !signalReceived {
			t.Fatal("Expected GainChanged signal to be dispatched")
		}
		if emittedVol != 0.75 {
			t.Errorf("Expected emitted volume to be 0.75, got %f", emittedVol)
		}
	case <-time.After(2 * time.Second):
		t.Fatal("Timeout waiting for GainChanged signal via EventLoop")
	}
}
