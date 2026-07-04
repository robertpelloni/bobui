package widgets

import (
	"strings"

	"gioui.org/layout"
	"gioui.org/widget/material"
	"gioui.org/unit"
	"github.com/robertpelloni/bqt/internal/ui/theme"
)

type MarkdownView struct {
	Text string
}

func (m *MarkdownView) Layout(gtx layout.Context, th theme.Theme) layout.Dimensions {
	lines := strings.Split(m.Text, "\n")
	list := &layout.List{Axis: layout.Vertical}
	mth := material.NewTheme()
	mth.Palette.Fg = th.Text
	mth.Palette.Bg = th.Surface

	return list.Layout(gtx, len(lines), func(gtx layout.Context, i int) layout.Dimensions {
		line := lines[i]
		size := unit.Sp(14)
		if strings.HasPrefix(line, "## ") {
			size = unit.Sp(20)
			line = strings.TrimPrefix(line, "## ")
		} else if strings.HasPrefix(line, "# ") {
			size = unit.Sp(24)
			line = strings.TrimPrefix(line, "# ")
		}
		lbl := material.Label(mth, size, line)
		lbl.Color = th.Text
		return lbl.Layout(gtx)
	})
}
