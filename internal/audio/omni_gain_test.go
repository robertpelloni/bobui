package audio

import "testing"

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
