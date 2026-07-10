package widgets

import (
	"image"
	"testing"
	"sync"

	"gioui.org/layout"
	"gioui.org/op"
	"gioui.org/op/clip"
	"gioui.org/op/paint"
	"gioui.org/f32"
	"image/color"
)

type DummySynthWidget struct {
	mu           sync.Mutex
	waveformData []float32
	activeVoices int
}

func NewDummySynthWidget() *DummySynthWidget {
	sw := &DummySynthWidget{
		waveformData: make([]float32, 256),
	}
	return sw
}

func (sw *DummySynthWidget) UpdateWaveform(buffer []float32) {
	sw.mu.Lock()
	defer sw.mu.Unlock()
	length := len(buffer)
	if length > len(sw.waveformData) {
		length = len(sw.waveformData)
	}
	copy(sw.waveformData, buffer[:length])
}

func (sw *DummySynthWidget) Layout(gtx layout.Context) layout.Dimensions {
	sw.mu.Lock()
	defer sw.mu.Unlock()

	width := float32(gtx.Constraints.Max.X)
	height := float32(gtx.Constraints.Max.Y)

	paint.FillShape(gtx.Ops, color.NRGBA{R: 20, G: 20, B: 20, A: 255}, clip.Rect{Max: gtx.Constraints.Max}.Op())

	if len(sw.waveformData) > 0 {
		var path clip.Path
		path.Begin(gtx.Ops)

		stepX := width / float32(len(sw.waveformData)-1)
		centerY := height / 2.0

		for i, sample := range sw.waveformData {
			x := float32(i) * stepX
			y := centerY - (sample * (height / 2.5))

			if i == 0 {
				path.MoveTo(f32.Pt(x, y))
			} else {
				path.LineTo(f32.Pt(x, y))
			}
		}

		paint.FillShape(gtx.Ops, color.NRGBA{R: 0, G: 255, B: 128, A: 255}, clip.Stroke{
			Path:  path.End(),
			Width: 2.0,
		}.Op())
	}

	if gtx.Constraints.Max.X == 0 || gtx.Constraints.Max.Y == 0 {
		return layout.Dimensions{}
	}

	return layout.Dimensions{Size: gtx.Constraints.Max}
}

func TestSynthWidgetLayout(t *testing.T) {
	widget := NewDummySynthWidget()

	ops := new(op.Ops)
	gtx := layout.Context{
		Ops: ops,
		Constraints: layout.Constraints{
			Max: image.Pt(800, 600),
		},
	}

	buffer := make([]float32, 256)
	for i := range buffer {
		buffer[i] = 0.5
	}
	widget.UpdateWaveform(buffer)

	dims := widget.Layout(gtx)

	if dims.Size.X != 800 || dims.Size.Y != 600 {
		t.Errorf("Expected dimensions (800, 600), got (%d, %d)", dims.Size.X, dims.Size.Y)
	}
}
