package widgets

import (
	"gioui.org/layout"
	"gioui.org/widget"
	"gioui.org/widget/material"
	"gioui.org/unit"
	"github.com/robertpelloni/bqt/internal/ui/theme"
)

type RewindSlider struct {
	Slider widget.Float
	Active bool
}

func (r *RewindSlider) Layout(gtx layout.Context, th theme.Theme) layout.Dimensions {
	if !r.Active { return layout.Dimensions{} }

	mth := material.NewTheme()
	mth.Palette.Fg = th.Text
	mth.Palette.Bg = th.Surface

	return layout.S.Layout(gtx, func(gtx layout.Context) layout.Dimensions {
		return layout.Flex{Axis: layout.Vertical}.Layout(gtx,
			layout.Rigid(func(gtx layout.Context) layout.Dimensions {
				lbl := material.Label(mth, unit.Sp(10), "TEMPORAL TIMELINE")
				lbl.Color = th.Primary
				return lbl.Layout(gtx)
			}),
			layout.Rigid(func(gtx layout.Context) layout.Dimensions {
				slider := material.Slider(mth, &r.Slider)
				slider.Color = th.Primary
				return slider.Layout(gtx)
			}),
		)
	})
}
