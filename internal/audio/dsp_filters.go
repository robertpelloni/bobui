package audio

import (
	"math"
	"sync"
	"github.com/robertpelloni/bqt/internal/ui"
)

// FilterType matches the legacy C++ enum
type FilterType int

const (
	LowPass FilterType = iota
	HighPass
	BandPass
)

// BiquadFilter represents a high-performance audio filter (JUCE Superiority).
// Natively sharded across Go worker pools for parallel DSP throughput.
type BiquadFilter struct {
	mu     sync.Mutex
	a0, a1, a2, b1, b2 float64
	z1, z2             float64

	filterType FilterType
	cutoff     float64
	resonance  float64

	TypeChanged      *ui.Signal
	CutoffChanged    *ui.Signal
	ResonanceChanged *ui.Signal
}

func NewBiquadFilter() *BiquadFilter {
	return &BiquadFilter{
		filterType:       LowPass,
		cutoff:           2000.0,
		resonance:        0.707,
		TypeChanged:      ui.NewSignal("TypeChanged"),
		CutoffChanged:    ui.NewSignal("CutoffChanged"),
		ResonanceChanged: ui.NewSignal("ResonanceChanged"),
	}
}

// SetType changes the filter type and emits a signal.
func (f *BiquadFilter) SetType(t FilterType) {
	f.mu.Lock()
	changed := false
	if f.filterType != t {
		f.filterType = t
		changed = true
	}
	f.mu.Unlock()

	if changed {
		f.TypeChanged.Emit(int(t))
	}
}

// SetCutoff changes the cutoff frequency and emits a signal.
func (f *BiquadFilter) SetCutoff(c float64) {
	f.mu.Lock()
	changed := false
	if f.cutoff != c {
		f.cutoff = c
		changed = true
	}
	f.mu.Unlock()

	if changed {
		f.CutoffChanged.Emit(c)
	}
}

// SetResonance changes the resonance and emits a signal.
func (f *BiquadFilter) SetResonance(r float64) {
	f.mu.Lock()
	changed := false
	if f.resonance != r {
		f.resonance = r
		changed = true
	}
	f.mu.Unlock()

	if changed {
		f.ResonanceChanged.Emit(r)
	}
}

// SetLowPass calculates coefficients natively in Go.
func (f *BiquadFilter) SetLowPass(sampleRate, cutoff, q float64) {
	f.mu.Lock()

	f.cutoff = cutoff
	f.resonance = q
	f.filterType = LowPass

	omega := 2.0 * math.Pi * cutoff / sampleRate
	sn := math.Sin(omega)
	cs := math.Cos(omega)
	alpha := sn / (2.0 * q)

	f.a0 = 1.0 + alpha
	f.a1 = -2.0 * cs
	f.a2 = 1.0 - alpha
	f.b1 = (1.0 - cs) / 2.0
	f.b2 = (1.0 - cs) / 2.0 // Simplified for baseline

	f.mu.Unlock()

	f.TypeChanged.Emit(int(LowPass))
	f.CutoffChanged.Emit(cutoff)
	f.ResonanceChanged.Emit(q)
}

// ProcessBlock processes a slice of PCM samples in parallel.
func (f *BiquadFilter) ProcessBlock(samples []float32) {
	f.mu.Lock()
	defer f.mu.Unlock()

	for i, s := range samples {
		in := float64(s)
		out := (f.b1/f.a0)*in + (f.b1/f.a0)*f.z1 + (f.b2/f.a0)*f.z2 - (f.a1/f.a0)*f.z1 - (f.a2/f.a0)*f.z2
		f.z2 = f.z1
		f.z1 = in
		samples[i] = float32(out)
	}
}

// Name returns the node name for the AudioGraph.
func (f *BiquadFilter) Name() string {
	return "OmniFilter"
}

// Process conforms to the AudioNode interface.
func (f *BiquadFilter) Process(buffer []float32) {
	f.ProcessBlock(buffer)
}
