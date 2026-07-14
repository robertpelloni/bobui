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

// ToolTip is a lightweight hover/help surface baseline.
type ToolTip struct {
	Visible bool
	Text    string
	X, Y    int
}

func (t *ToolTip) Layout(gtx layout.Context, th theme.Theme) layout.Dimensions {
	if !t.Visible || t.Text == "" {
		return layout.Dimensions{}
	}

	mth := material.NewTheme()
	mth.Palette.Fg = th.Text
	mth.Palette.Bg = th.Surface

	w := gtx.Dp(unit.Dp(220))
	h := gtx.Dp(unit.Dp(48))
	rect := image.Rect(t.X, t.Y, t.X+w, t.Y+h)
	paint.FillShape(gtx.Ops, th.Surface, clip.Rect(rect).Op())

	return layout.Inset{Top: unit.Dp(float32(t.Y) + 6), Left: unit.Dp(float32(t.X) + 8)}.Layout(gtx, func(gtx layout.Context) layout.Dimensions {
		lbl := material.Body2(mth, t.Text)
		return lbl.Layout(gtx)
	})
}
