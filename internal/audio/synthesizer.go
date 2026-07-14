package audio

import (
	"log"
	"math"
	"sync"

	"github.com/robertpelloni/bqt/internal/ui"
)

// Synthesizer is the Go-native implementation of OmniSynthesizer.
// It conforms to the AudioNode interface and manages polyphony and basic synthesis.
type Voice struct {
	velocity float32
	phase    float64
}

type Synthesizer struct {
	mu          sync.Mutex
	polyphony   int
	activeNotes map[int]*Voice
	sampleRate  float64
}

// NewSynthesizer creates a new native Go synthesizer.
func NewSynthesizer() *Synthesizer {
	return &Synthesizer{
		polyphony:   8,
		activeNotes: make(map[int]*Voice),
		sampleRate:  44100.0,
	}
}

// Name returns the node name for the AudioGraph.
func (s *Synthesizer) Name() string {
	return "OmniSynthesizer"
}

// Polyphony returns the maximum number of concurrent voices.
func (s *Synthesizer) Polyphony() int {
	s.mu.Lock()
	defer s.mu.Unlock()
	return s.polyphony
}

// SetPolyphony sets the maximum number of concurrent voices.
func (s *Synthesizer) SetPolyphony(voices int) {
	s.mu.Lock()
	defer s.mu.Unlock()
	if s.polyphony != voices {
		s.polyphony = voices
		log.Printf("OmniSynthesizer Go: Polyphony set to %d", voices)
	}
}

// NoteOn registers a new active note.
func (s *Synthesizer) NoteOn(channel, note, velocity int) {
	s.mu.Lock()
	defer s.mu.Unlock()

	if len(s.activeNotes) >= s.polyphony {
		// Ignore if max polyphony reached (no voice stealing implemented yet)
		log.Printf("OmniSynthesizer Go: Voice limit reached, ignoring NoteON")
		return
	}

	s.activeNotes[note] = &Voice{velocity: float32(velocity) / 127.0, phase: 0.0}

	// Post event to the unified BQt Event Loop for UI thread synchronization
	ui.GetEventLoop().Post(func() {
		log.Printf("OmniSynthesizer Go: Native Synthesis NoteON -> CH: %d NOTE: %d", channel, note)
	})
}

// NoteOff removes an active note.
func (s *Synthesizer) NoteOff(channel, note int) {
	s.mu.Lock()
	defer s.mu.Unlock()

	delete(s.activeNotes, note)

	// Post event to the unified BQt Event Loop for UI thread synchronization
	ui.GetEventLoop().Post(func() {
		log.Printf("OmniSynthesizer Go: Native Synthesis NoteOFF -> CH: %d NOTE: %d", channel, note)
	})
}

// Process generates the audio buffer for all active voices and adds it to the output.
func (s *Synthesizer) Process(buffer []float32) {
	s.mu.Lock()
	defer s.mu.Unlock()

	if len(s.activeNotes) == 0 {
		return // Nothing to synthesize
	}

	for i := 0; i < len(buffer); i++ {
		var sample float32 = 0.0

		// Render active notes as sine waves
		for note, voice := range s.activeNotes {
			// Midi note to frequency
			freq := 440.0 * math.Pow(2.0, float64(note-69)/12.0)
			sample += voice.velocity * float32(math.Sin(2.0*math.Pi*voice.phase))
			voice.phase += freq / s.sampleRate
			if voice.phase > 1.0 {
				voice.phase -= 1.0
			}
		}

		// Add to buffer (mix with existing audio)
		buffer[i] += sample

	}
}
