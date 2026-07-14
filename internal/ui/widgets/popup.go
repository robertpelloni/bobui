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

// Popup is a Go-native QuickControls2-style transient surface.
type Popup struct {
	Visible bool
	Title   string
	Body    string
}

func (p *Popup) Layout(gtx layout.Context, th theme.Theme) layout.Dimensions {
	if !p.Visible {
		return layout.Dimensions{}
	}

	mth := material.NewTheme()
	mth.Palette.Fg = th.Text
	mth.Palette.Bg = th.Surface

	w := gtx.Dp(unit.Dp(320))
	h := gtx.Dp(unit.Dp(180))
	rect := image.Rect((gtx.Constraints.Max.X-w)/2, (gtx.Constraints.Max.Y-h)/2, (gtx.Constraints.Max.X+w)/2, (gtx.Constraints.Max.Y+h)/2)

	// dim background feel
	paint.FillShape(gtx.Ops, th.Background, clip.Rect(image.Rect(0, 0, gtx.Constraints.Max.X, gtx.Constraints.Max.Y)).Op())
	paint.FillShape(gtx.Ops, th.Surface, clip.Rect(rect).Op())

	return layout.Center.Layout(gtx, func(gtx layout.Context) layout.Dimensions {
		return layout.Flex{Axis: layout.Vertical}.Layout(gtx,
			layout.Rigid(material.H6(mth, p.Title).Layout),
			layout.Rigid(layout.Spacer{Height: unit.Dp(12)}.Layout),
			layout.Rigid(material.Body1(mth, p.Body).Layout),
		)
	})
}
