package audio

import (
	"sync"
	"testing"
	"time"

	"github.com/robertpelloni/bqt/internal/ui"
)

func TestBiquadFilter_Name(t *testing.T) {
	f := NewBiquadFilter()
	if f.Name() != "OmniFilter" {
		t.Errorf("expected OmniFilter, got %s", f.Name())
	}
}

func TestBiquadFilter_Process(t *testing.T) {
	// Initialize UI EventLoop to handle signals gracefully during SetLowPass
	el := ui.GetEventLoop()
	go el.Run()
	defer el.Stop()

	f := NewBiquadFilter()
	f.SetLowPass(44100.0, 1000.0, 0.707)

	buffer := make([]float32, 256)
	for i := range buffer {
		buffer[i] = 1.0
	}

	f.Process(buffer)

	if buffer[0] == 1.0 {
		t.Errorf("expected buffer to be modified by filter, but got original value")
	}
}

func TestBiquadFilter_SignalEmission(t *testing.T) {
	el := ui.GetEventLoop()
	go el.Run()
	defer el.Stop()

	f := NewBiquadFilter()

	var wg sync.WaitGroup
	wg.Add(3)

	var emittedType int
	var emittedCutoff float64
	var emittedResonance float64

	f.TypeChanged.Connect(func(args ...interface{}) {
		defer wg.Done()
		if len(args) > 0 {
			if v, ok := args[0].(int); ok {
				emittedType = v
			}
		}
	})

	f.CutoffChanged.Connect(func(args ...interface{}) {
		defer wg.Done()
		if len(args) > 0 {
			if v, ok := args[0].(float64); ok {
				emittedCutoff = v
			}
		}
	})

	f.ResonanceChanged.Connect(func(args ...interface{}) {
		defer wg.Done()
		if len(args) > 0 {
			if v, ok := args[0].(float64); ok {
				emittedResonance = v
			}
		}
	})

	f.SetType(HighPass)
	f.SetCutoff(5000.0)
	f.SetResonance(1.5)

	done := make(chan struct{})
	go func() {
		wg.Wait()
		close(done)
	}()

	select {
	case <-done:
		if emittedType != int(HighPass) {
			t.Errorf("Expected TypeChanged %d, got %d", HighPass, emittedType)
		}
		if emittedCutoff != 5000.0 {
			t.Errorf("Expected CutoffChanged %f, got %f", 5000.0, emittedCutoff)
		}
		if emittedResonance != 1.5 {
			t.Errorf("Expected ResonanceChanged %f, got %f", 1.5, emittedResonance)
		}
	case <-time.After(2 * time.Second):
		t.Fatal("Timeout waiting for filter signals via EventLoop")
	}
}
