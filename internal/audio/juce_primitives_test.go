package audio

import "testing"

func TestJUCEGraphAdapter(t *testing.T) {
	adapter := NewJUCEAudioGraphAdapter("main_bus")
	adapter.MapJUCENode("juce_reverb")

	graph := GetAudioGraph()
	graph.mu.RLock()
	defer graph.mu.RUnlock()

	if _, exists := graph.nodes["juce_reverb"]; !exists {
		t.Fatalf("Expected 'juce_reverb' to be mapped into the Go AudioGraph")
	}
}

func TestJUCEMapping(t *testing.T) {
	adapter := NewJUCEAudioGraphAdapter("synth_bus")

	// Map known primitives
	adapter.MapJUCENode("OmniSynthesizer")
	adapter.MapJUCENode("OmniGain")
	// Map unknown primitive
	adapter.MapJUCENode("CustomProcessor")

	graph := GetAudioGraph()
	graph.mu.RLock()
	defer graph.mu.RUnlock()

	// Verify OmniSynthesizer was mapped to native Go Synthesizer
	synthNode, exists := graph.nodes["OmniSynthesizer"]
	if !exists {
		t.Fatalf("Expected 'OmniSynthesizer' to be mapped into the Go AudioGraph")
	}
	if _, ok := synthNode.(*Synthesizer); !ok {
		t.Errorf("Expected 'OmniSynthesizer' to be of type *Synthesizer, got %T", synthNode)
	}

	// Verify OmniGain was mapped to native Go OmniGain
	gainNode, exists := graph.nodes["OmniGain"]
	if !exists {
		t.Fatalf("Expected 'OmniGain' to be mapped into the Go AudioGraph")
	}
	if _, ok := gainNode.(*OmniGain); !ok {
		t.Errorf("Expected 'OmniGain' to be of type *OmniGain, got %T", gainNode)
	}

	// Verify CustomProcessor was mapped to JUCEProxyNode
	proxyNode, exists := graph.nodes["CustomProcessor"]
	if !exists {
		t.Fatalf("Expected 'CustomProcessor' to be mapped into the Go AudioGraph")
	}
	if _, ok := proxyNode.(*JUCEProxyNode); !ok {
		t.Errorf("Expected 'CustomProcessor' to be of type *JUCEProxyNode, got %T", proxyNode)
	}
}
