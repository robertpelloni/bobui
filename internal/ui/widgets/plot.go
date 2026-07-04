package widgets

import (
	"image"

	"gioui.org/layout"
	"gioui.org/op/clip"
	"gioui.org/op/paint"
	"github.com/robertpelloni/bqt/internal/ui/theme"
)

type Plot struct {
	Data []float32
}

func (p *Plot) Layout(gtx layout.Context, th theme.Theme) layout.Dimensions {
	if len(p.Data) == 0 {
		return layout.Dimensions{}
	}
	w := gtx.Constraints.Max.X
	h := gtx.Constraints.Max.Y
	paint.FillShape(gtx.Ops, th.Surface, clip.Rect(image.Rect(0, 0, w, h)).Op())
	// Minimal verified baseline: render vertical columns.
	step := 1
	if len(p.Data) > 0 {
		step = max(1, w/len(p.Data))
	}
	for i, v := range p.Data {
		barH := int(v * float32(h))
		rect := image.Rect(i*step, h-barH, i*step+max(1, step-1), h)
		paint.FillShape(gtx.Ops, th.Primary, clip.Rect(rect).Op())
	}
	return layout.Dimensions{Size: image.Pt(w, h)}
}

func max(a, b int) int {
	if a > b {
		return a
	}
	return b
}
