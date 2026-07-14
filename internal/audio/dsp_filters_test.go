package audio

import (
	"testing"
)

func TestBiquadFilter_Name(t *testing.T) {
	f := NewBiquadFilter()
	if f.Name() != "OmniFilter" {
		t.Errorf("expected OmniFilter, got %s", f.Name())
	}
}

func TestBiquadFilter_Process(t *testing.T) {
	f := NewBiquadFilter()
	f.SetLowPass(44100.0, 1000.0, 0.707)

	buffer := make([]float32, 256)
	// Just fill with some dummy data to ensure no panics
	for i := range buffer {
		buffer[i] = 1.0
	}

	f.Process(buffer)

	// Ensure the filter modifies the buffer
	if buffer[0] == 1.0 {
		t.Errorf("expected buffer to be modified by filter, but got original value")
	}
}
