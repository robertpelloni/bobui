package widgets

import (
	"gioui.org/layout"
	"gioui.org/unit"
	"gioui.org/widget"
	"gioui.org/widget/material"
	"github.com/robertpelloni/bqt/internal/ui/theme"
)

type CodeEditor struct {
	Editor widget.Editor
}

func (e *CodeEditor) Layout(gtx layout.Context, th theme.Theme) layout.Dimensions {
	mth := material.NewTheme()
	mth.Palette.Fg = th.Text
	mth.Palette.Bg = th.Surface

	ed := material.Editor(mth, &e.Editor, "System Kernel Code...")
	ed.TextSize = unit.Sp(13)
	return layout.Flex{Axis: layout.Vertical}.Layout(gtx,
		layout.Flexed(1, ed.Layout),
	)
}
