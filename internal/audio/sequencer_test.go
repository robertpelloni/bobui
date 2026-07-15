package audio

import (
	"testing"
	"time"

	"github.com/robertpelloni/bqt/internal/ui"
)

func TestSequencer_BpmChanged(t *testing.T) {
	ui.GetEventLoop().Stop()
	go ui.GetEventLoop().Run()
	defer ui.GetEventLoop().Stop()

	seq := NewSequencer()
	done := make(chan float64)

	seq.BpmChanged.Connect(func(args ...interface{}) {
		done <- args[0].(float64)
	})

	seq.SetBPM(140.0)

	select {
	case bpm := <-done:
		if bpm != 140.0 {
			t.Errorf("Expected BPM to be 140.0, got %f", bpm)
		}
	case <-time.After(1 * time.Second):
		t.Fatal("Timeout waiting for BpmChanged signal")
	}
}

func TestSequencer_IsPlayingChanged(t *testing.T) {
	ui.GetEventLoop().Stop()
	go ui.GetEventLoop().Run()
	defer ui.GetEventLoop().Stop()

	seq := NewSequencer()
	done := make(chan bool)

	seq.IsPlayingChanged.Connect(func(args ...interface{}) {
		done <- args[0].(bool)
	})

	seq.Play()

	select {
	case playing := <-done:
		if !playing {
			t.Errorf("Expected playing to be true, got false")
		}
	case <-time.After(1 * time.Second):
		t.Fatal("Timeout waiting for IsPlayingChanged signal (Play)")
	}

	seq.Stop()

	select {
	case playing := <-done:
		if playing {
			t.Errorf("Expected playing to be false, got true")
		}
	case <-time.After(1 * time.Second):
		t.Fatal("Timeout waiting for IsPlayingChanged signal (Stop)")
	}
}
