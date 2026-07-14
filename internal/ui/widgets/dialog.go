package widgets

import (
	"image"

	"gioui.org/layout"
	"gioui.org/op/clip"
	"gioui.org/op/paint"
	"gioui.org/unit"
	"gioui.org/widget"
	"gioui.org/widget/material"
	"github.com/robertpelloni/bqt/internal/ui/theme"
)

// Dialog is a Go-native QuickControls2-style modal surface baseline.
type Dialog struct {
	Visible bool
	Title   string
	Body    string

	Accept widget.Clickable
	Cancel widget.Clickable
}

func (d *Dialog) Layout(gtx layout.Context, th theme.Theme) (layout.Dimensions, string) {
	if !d.Visible {
		return layout.Dimensions{}, ""
	}

	mth := material.NewTheme()
	mth.Palette.Fg = th.Text
	mth.Palette.Bg = th.Surface

	result := ""
	if d.Accept.Clicked(gtx) {
		result = "accept"
		d.Visible = false
	}
	if d.Cancel.Clicked(gtx) {
		result = "cancel"
		d.Visible = false
	}

	w := gtx.Dp(unit.Dp(360))
	h := gtx.Dp(unit.Dp(220))
	rect := image.Rect((gtx.Constraints.Max.X-w)/2, (gtx.Constraints.Max.Y-h)/2, (gtx.Constraints.Max.X+w)/2, (gtx.Constraints.Max.Y+h)/2)

	paint.FillShape(gtx.Ops, th.Background, clip.Rect(image.Rect(0, 0, gtx.Constraints.Max.X, gtx.Constraints.Max.Y)).Op())
	paint.FillShape(gtx.Ops, th.Surface, clip.Rect(rect).Op())

	dims := layout.Center.Layout(gtx, func(gtx layout.Context) layout.Dimensions {
		return layout.Flex{Axis: layout.Vertical}.Layout(gtx,
			layout.Rigid(material.H6(mth, d.Title).Layout),
			layout.Rigid(layout.Spacer{Height: unit.Dp(12)}.Layout),
			layout.Rigid(material.Body1(mth, d.Body).Layout),
			layout.Rigid(layout.Spacer{Height: unit.Dp(16)}.Layout),
			layout.Rigid(func(gtx layout.Context) layout.Dimensions {
				return layout.Flex{Axis: layout.Horizontal}.Layout(gtx,
					layout.Rigid(material.Button(mth, &d.Accept, "OK").Layout),
					layout.Rigid(layout.Spacer{Width: unit.Dp(8)}.Layout),
					layout.Rigid(material.Button(mth, &d.Cancel, "Cancel").Layout),
				)
			}),
		)
	})
	return dims, result
}
