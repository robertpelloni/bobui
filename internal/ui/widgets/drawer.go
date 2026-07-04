package widgets

import (
	"image"

	"gioui.org/layout"
	"gioui.org/op/clip"
	"gioui.org/op/paint"
	"gioui.org/unit"
	"gioui.org/widget/material"
	"github.com/robertpelloni/bqt/internal/ui/theme"
)

// Drawer is a Go-native QuickControls2-style side panel baseline.
type Drawer struct {
	Visible bool
	Title   string
	WidthDp float32
}

func (d *Drawer) Layout(gtx layout.Context, th theme.Theme, child layout.Widget) layout.Dimensions {
	if !d.Visible {
		return layout.Dimensions{}
	}
	if d.WidthDp <= 0 {
		d.WidthDp = 280
	}

	mth := material.NewTheme()
	mth.Palette.Fg = th.Text
	mth.Palette.Bg = th.Surface

	w := gtx.Dp(unit.Dp(d.WidthDp))
	rect := image.Rect(0, 0, w, gtx.Constraints.Max.Y)
	paint.FillShape(gtx.Ops, th.Surface, clip.Rect(rect).Op())

	return layout.Inset{Top: unit.Dp(12), Left: unit.Dp(12), Right: unit.Dp(12), Bottom: unit.Dp(12)}.Layout(gtx, func(gtx layout.Context) layout.Dimensions {
		return layout.Flex{Axis: layout.Vertical}.Layout(gtx,
			layout.Rigid(material.H6(mth, d.Title).Layout),
			layout.Rigid(layout.Spacer{Height: unit.Dp(12)}.Layout),
			layout.Flexed(1, child),
		)
	})
}
