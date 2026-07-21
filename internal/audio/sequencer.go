package audio

import (
	"sync"

	"github.com/robertpelloni/bqt/internal/ui"
)

// Sequencer represents the native Go port of OmniSequencer.
type Sequencer struct {
	mu          sync.Mutex
	isPlaying   bool
	bpm         float64

	BpmChanged        *ui.Signal
	IsPlayingChanged  *ui.Signal
	Beat              *ui.Signal
}

func NewSequencer() *Sequencer {
	return &Sequencer{
		bpm:              120.0,
		BpmChanged:       ui.NewSignal("BpmChanged"),
		IsPlayingChanged: ui.NewSignal("IsPlayingChanged"),
		Beat:             ui.NewSignal("Beat"),
	}
}

func (s *Sequencer) SetBPM(bpm float64) {
	s.mu.Lock()
	if s.bpm == bpm {
		s.mu.Unlock()
		return
	}
	s.bpm = bpm
	s.mu.Unlock()
	s.BpmChanged.Emit(bpm)
}

func (s *Sequencer) Play() {
	s.mu.Lock()
	if s.isPlaying {
		s.mu.Unlock()
		return
	}
	s.isPlaying = true
	s.mu.Unlock()
	s.IsPlayingChanged.Emit(true)
}

func (s *Sequencer) Stop() {
	s.mu.Lock()
	if !s.isPlaying {
		s.mu.Unlock()
		return
	}
	s.isPlaying = false
	s.mu.Unlock()
	s.IsPlayingChanged.Emit(false)
}

func (s *Sequencer) TickBeat() {
	s.Beat.Emit()
}
