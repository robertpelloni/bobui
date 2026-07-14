package audio

import (
	"testing"
)

func TestSynthesizerPolyphony(t *testing.T) {
	synth := NewSynthesizer()

	if synth.Polyphony() != 8 {
		t.Errorf("Expected default polyphony to be 8, got %d", synth.Polyphony())
	}

	synth.SetPolyphony(16)
	if synth.Polyphony() != 16 {
		t.Errorf("Expected polyphony to be 16 after setting, got %d", synth.Polyphony())
	}
}

func TestSynthesizerNoteOnOff(t *testing.T) {
	synth := NewSynthesizer()
	synth.SetPolyphony(2)

	synth.NoteOn(1, 60, 100) // Middle C
	synth.NoteOn(1, 64, 100) // E
	synth.NoteOn(1, 67, 100) // G (should be ignored due to polyphony)

	synth.mu.Lock()
	activeCount := len(synth.activeNotes)
	synth.mu.Unlock()

	if activeCount != 2 {
		t.Errorf("Expected 2 active notes, got %d", activeCount)
	}

	synth.NoteOff(1, 60)

	synth.mu.Lock()
	activeCount = len(synth.activeNotes)
	synth.mu.Unlock()

	if activeCount != 1 {
		t.Errorf("Expected 1 active note after NoteOff, got %d", activeCount)
	}
}

func TestSynthesizerProcessBlock(t *testing.T) {
	synth := NewSynthesizer()
	buffer := make([]float32, 256)

	// Process without any notes, buffer should be empty
	synth.Process(buffer)
	for i, sample := range buffer {
		if sample != 0.0 {
			t.Fatalf("Expected buffer to be empty, but got %f at index %d", sample, i)
		}
	}

	// Process with an active note
	synth.NoteOn(1, 60, 100)
	synth.Process(buffer)

	nonZeroFound := false
	for _, sample := range buffer {
		if sample != 0.0 {
			nonZeroFound = true
			break
		}
	}

	if !nonZeroFound {
		t.Fatalf("Expected buffer to contain non-zero audio data after Process with active note")
	}
}
