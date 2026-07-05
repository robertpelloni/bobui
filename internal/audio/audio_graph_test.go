package audio

import (
	"testing"
)

func TestAudioGraphConnection(t *testing.T) {
	graph := GetAudioGraph()

	synth := NewSynthesizer()
	gain := NewOmniGain()

	// Add nodes to the graph
	graph.AddNode(synth)
	graph.AddNode(gain)

	// Verify nodes were added
	graph.mu.RLock()
	if _, ok := graph.nodes["OmniSynthesizer"]; !ok {
		t.Fatalf("Expected OmniSynthesizer to be in the graph nodes")
	}
	if _, ok := graph.nodes["OmniGain"]; !ok {
		t.Fatalf("Expected OmniGain to be in the graph nodes")
	}
	graph.mu.RUnlock()

	// Test connecting the nodes
	err := graph.Connect("OmniSynthesizer", "OmniGain")
	if err != nil {
		t.Fatalf("Failed to connect nodes: %v", err)
	}

	// Verify connection exists
	graph.mu.RLock()
	destinations, exists := graph.links["OmniSynthesizer"]
	if !exists || len(destinations) == 0 || destinations[0] != "OmniGain" {
		t.Fatalf("Expected link from OmniSynthesizer to OmniGain")
	}
	graph.mu.RUnlock()

	// Test processing an audio block
	// We'll trigger a note to ensure the synthesizer produces output
	synth.NoteOn(1, 60, 100)

	buffer := make([]float32, 128)

	// Initial buffer should be silent
	for _, sample := range buffer {
		if sample != 0.0 {
			t.Fatalf("Expected initial buffer to be silent (0.0)")
		}
	}

	// Set gain to 0.5 to test effect of gain node
	gain.SetVolume(0.5)

	// Process block
	graph.ProcessBlock(buffer)

	// The buffer should no longer be completely silent because the synthesizer rendered a note
	hasAudio := false
	for _, sample := range buffer {
		if sample != 0.0 {
			hasAudio = true
			break
		}
	}

	if !hasAudio {
		t.Fatalf("Expected audio buffer to contain synthesized sine wave data after ProcessBlock")
	}

	// Note off to clear for subsequent tests if GetAudioGraph singleton is reused
	synth.NoteOff(1, 60)
}
