package widgets

import (
	"image/color"
	"sync"

	"gioui.org/f32"
	"gioui.org/layout"
	"gioui.org/op/clip"
	"gioui.org/op/paint"
)

// SynthWidget provides a direct gioui.org rendering hook for the OmniSynthesizer polyphony layer.
// To use, instantiate and wire this in the shell or engine layer and update polyphony/buffer
// through SetActiveVoices and UpdateWaveform to avoid cyclic package dependencies with audio core.
type SynthWidget struct {
	mu           sync.Mutex

	waveformData []float32
	activeVoices int
}

func NewSynthWidget() *SynthWidget {
	return &SynthWidget{
		waveformData: make([]float32, 256),
	}
}

// SetActiveVoices sets the active polyphony count.
func (sw *SynthWidget) SetActiveVoices(poly int) {
	sw.mu.Lock()
	defer sw.mu.Unlock()
	sw.activeVoices = poly
}

// UpdateWaveform allows the audio thread to push the latest buffer to the UI.
func (sw *SynthWidget) UpdateWaveform(buffer []float32) {
	sw.mu.Lock()
	defer sw.mu.Unlock()
	// Copy a slice of the buffer for visualization
	length := len(buffer)
	if length > len(sw.waveformData) {
		length = len(sw.waveformData)
	}
	copy(sw.waveformData, buffer[:length])
}

// Layout renders the synthesizer polyphony state and a live waveform using gioui.org hooks.
func (sw *SynthWidget) Layout(gtx layout.Context) layout.Dimensions {
	sw.mu.Lock()
	defer sw.mu.Unlock()

	// Dimensions
	width := float32(gtx.Constraints.Max.X)
	height := float32(gtx.Constraints.Max.Y)

	// Background
	paint.FillShape(gtx.Ops, color.NRGBA{R: 20, G: 20, B: 20, A: 255}, clip.Rect{Max: gtx.Constraints.Max}.Op())

	// Render waveform
	if len(sw.waveformData) > 0 {
		var path clip.Path
		path.Begin(gtx.Ops)

		stepX := width / float32(len(sw.waveformData)-1)
		centerY := height / 2.0

		for i, sample := range sw.waveformData {
			x := float32(i) * stepX
			// Scale sample (-1.0 to 1.0) to UI height
			y := centerY - (sample * (height / 2.5))

			if i == 0 {
				path.MoveTo(f32.Pt(x, y))
			} else {
				path.LineTo(f32.Pt(x, y))
			}
		}

		// Paint the waveform line
		paint.FillShape(gtx.Ops, color.NRGBA{R: 0, G: 255, B: 128, A: 255}, clip.Stroke{
			Path:  path.End(),
			Width: 2.0,
		}.Op())
	}

	// Ensure op doesn't panic on empty layout bounds
	if gtx.Constraints.Max.X == 0 || gtx.Constraints.Max.Y == 0 {
		return layout.Dimensions{}
	}

	return layout.Dimensions{Size: gtx.Constraints.Max}
}
