package audio

import (
	"log"
	"sync"

	"github.com/robertpelloni/bqt/internal/ui"
)

// OmniGain is the pure Go implementation of the OmniGain node.
type OmniGain struct {
	mu     sync.Mutex
	volume float32
}

func NewOmniGain() *OmniGain {
	return &OmniGain{
		volume: 1.0, // Default to 100% volume
	}
}

// Name returns the node name for the AudioGraph.
func (og *OmniGain) Name() string {
	return "OmniGain"
}

// SetVolume updates the gain multiplier. Range is typically 0.0 to 1.0 (or higher for boost).
func (og *OmniGain) SetVolume(vol float32) {
	og.mu.Lock()
	defer og.mu.Unlock()
	og.volume = vol

	// Post event to the unified BQt Event Loop for UI thread synchronization
	ui.GetEventLoop().Post(func() {
		log.Printf("OmniGain Go: Volume updated to %.2f", vol)
	})
}

// Process applies the gain to the audio buffer.
func (og *OmniGain) Process(buffer []float32) {
	og.mu.Lock()
	vol := og.volume
	og.mu.Unlock()

	// Fast path for 1.0 gain
	if vol == 1.0 {
		return
	}

	for i := range buffer {
		buffer[i] *= vol
	}
}
