package widgets

import (
	"image"
	"testing"

	"gioui.org/layout"
	"gioui.org/op"
)

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
