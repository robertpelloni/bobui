package audio

import (
	"sync"

	"github.com/robertpelloni/bqt/internal/ui"
)

// OmniGain is the pure Go implementation of the OmniGain node.
type OmniGain struct {
	mu          sync.Mutex
	volume      float32
	GainChanged *ui.Signal
}

func NewOmniGain() *OmniGain {
	return &OmniGain{
		volume:      1.0, // Default to 100% volume
		GainChanged: ui.NewSignal("GainChanged"),
	}
}

// Name returns the node name for the AudioGraph.
func (og *OmniGain) Name() string {
	return "OmniGain"
}

// SetVolume updates the gain multiplier. Range is typically 0.0 to 1.0 (or higher for boost).
// It emits the GainChanged signal (similar to Q_PROPERTY NOTIFY in C++) if the volume changes.
func (og *OmniGain) SetVolume(vol float32) {
	og.mu.Lock()
	changed := false
	if og.volume != vol {
		og.volume = vol
		changed = true
	}
	og.mu.Unlock()

	if changed {
		og.GainChanged.Emit(vol)
	}
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
