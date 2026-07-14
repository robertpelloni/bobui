package widgets

import (
	"image"

	"gioui.org/layout"
	"gioui.org/op/clip"
	"gioui.org/op/paint"
	"gioui.org/unit"
	"github.com/robertpelloni/bqt/internal/ui/theme"
)

type ProgressBar struct {
	Value float32
}

func (p *ProgressBar) Layout(gtx layout.Context, th theme.Theme) layout.Dimensions {
	if p.Value < 0 {
		p.Value = 0
	}
	if p.Value > 1 {
		p.Value = 1
	}
	w := gtx.Constraints.Max.X
	h := gtx.Dp(unit.Dp(10))
	track := image.Rect(0, 0, w, h)
	fill := image.Rect(0, 0, int(float32(w)*p.Value), h)
	paint.FillShape(gtx.Ops, th.Surface, clip.Rect(track).Op())
	paint.FillShape(gtx.Ops, th.Primary, clip.Rect(fill).Op())
	return layout.Dimensions{Size: image.Pt(w, h)}
}

type Dial struct {
	Value float32
}

func (d *Dial) Layout(gtx layout.Context, th theme.Theme) layout.Dimensions {
	size := gtx.Dp(unit.Dp(60))
	rect := image.Rect(0, 0, size, size)
	paint.FillShape(gtx.Ops, th.Surface, clip.Ellipse(rect).Op(gtx.Ops))
	return layout.Dimensions{Size: image.Pt(size, size)}
}

type CheckBox struct {
	Checked bool
	Label   string
}

func (c *CheckBox) Layout(gtx layout.Context, th theme.Theme) layout.Dimensions {
	size := gtx.Dp(unit.Dp(20))
	box := image.Rect(0, 0, size, size)
	col := th.Surface
	if c.Checked {
		col = th.Primary
	}
	paint.FillShape(gtx.Ops, col, clip.Rect(box).Op())
	return layout.Dimensions{Size: image.Pt(size, size)}
}
