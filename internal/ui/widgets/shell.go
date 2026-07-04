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

type Shell struct {
	StartMenuVisible bool
	StartBtn         widget.Clickable
}

func (s *Shell) LayoutTaskbar(gtx layout.Context, th theme.Theme) layout.Dimensions {
	mth := material.NewTheme()
	mth.Palette.Fg = th.Text
	mth.Palette.Bg = th.Surface

	height := gtx.Dp(unit.Dp(48))
	rect := image.Rect(0, gtx.Constraints.Max.Y-height, gtx.Constraints.Max.X, gtx.Constraints.Max.Y)
	paint.FillShape(gtx.Ops, th.Surface, clip.Rect(rect).Op())

	if s.StartBtn.Clicked(gtx) {
		s.StartMenuVisible = !s.StartMenuVisible
	}

	return layout.Flex{Axis: layout.Horizontal, Spacing: layout.SpaceBetween, Alignment: layout.Middle}.Layout(gtx,
		layout.Rigid(func(gtx layout.Context) layout.Dimensions {
			return material.Button(mth, &s.StartBtn, "START").Layout(gtx)
		}),
		layout.Rigid(func(gtx layout.Context) layout.Dimensions {
			return material.Label(mth, unit.Sp(10), "DSP: SYNC | P2P: ACTIVE").Layout(gtx)
		}),
	)
}

func (s *Shell) LayoutStartMenu(gtx layout.Context, th theme.Theme) layout.Dimensions {
	if !s.StartMenuVisible {
		return layout.Dimensions{}
	}
	mth := material.NewTheme()
	mth.Palette.Fg = th.Text
	mth.Palette.Bg = th.Surface
	rect := image.Rect(10, 10, 240, 200)
	paint.FillShape(gtx.Ops, th.Surface, clip.Rect(rect).Op())
	return material.Label(mth, unit.Sp(11), "Applications").Layout(gtx)
}
