package audio

import (
	"sync"
	"github.com/robertpelloni/bqt/internal/ui"
)

// Oscilloscope natively ports the legacy C++ OmniOscilloscope into Go.
// It acts as an observation node within the AudioGraph, capturing real-time
// DSP frames for visual rendering without blocking the audio thread.
type Oscilloscope struct {
	mu sync.Mutex

	// buffer holds the most recent audio frame snapshot.
	buffer []float32
	// bufferSize defines the length of the capture window.
	bufferSize int

	// DataReady is emitted asynchronously on the UI EventLoop when a new
	// buffer frame is fully captured and ready for UI rendering.
	DataReady *ui.Signal
}

// NewOscilloscope initializes a new Oscilloscope node with a specific buffer size.
func NewOscilloscope(size int) *Oscilloscope {
	return &Oscilloscope{
		buffer:     make([]float32, size),
		bufferSize: size,
		DataReady:  ui.NewSignal("oscilloscopeDataReady"),
	}
}

// ProcessAudio ingests an audio frame. In a real DSP pipeline, this is called
// by the AudioGraph on the high-priority audio thread.
func (o *Oscilloscope) ProcessAudio(input []float32) {
	o.mu.Lock()

	// Copy input data up to the buffer size to prevent out of bounds.
	copyLen := len(input)
	if copyLen > o.bufferSize {
		copyLen = o.bufferSize
	}

	// Create a fast snapshot copy so the audio thread can continue
	// while the UI thread eventually reads it.
	copy(o.buffer, input[:copyLen])

	// Zero pad if the input is smaller than the buffer
	for i := copyLen; i < o.bufferSize; i++ {
		o.buffer[i] = 0.0
	}

	o.mu.Unlock()

	// Asynchronously notify listeners (like a UI widget) that new data is ready.
	o.DataReady.Emit()
}

// GetBufferSnapshot safely retrieves a copy of the current oscilloscope buffer.
// UI widgets should call this when handling the DataReady signal.
func (o *Oscilloscope) GetBufferSnapshot() []float32 {
	o.mu.Lock()
	defer o.mu.Unlock()

	snapshot := make([]float32, o.bufferSize)
	copy(snapshot, o.buffer)
	return snapshot
}
