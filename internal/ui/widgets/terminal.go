package widgets

import (
	"gioui.org/layout"
	"gioui.org/widget/material"
	"gioui.org/unit"
	"github.com/robertpelloni/bqt/internal/ui/theme"
	"github.com/robertpelloni/bqt/internal/kernel"
)

type Terminal struct {
	Shell  *kernel.Process
	Buffer []string
}

func (t *Terminal) Layout(gtx layout.Context, th theme.Theme) layout.Dimensions {
	mth := material.NewTheme()
	mth.Palette.Fg = th.Text
	mth.Palette.Bg = th.Surface

	select {
	case out := <-t.Shell.Output:
		t.Buffer = append(t.Buffer, out)
		if len(t.Buffer) > 50 { t.Buffer = t.Buffer[1:] }
	default:
	}

	list := &layout.List{Axis: layout.Vertical}
	list.ScrollToEnd = true
	return list.Layout(gtx, len(t.Buffer), func(gtx layout.Context, i int) layout.Dimensions {
		return material.Label(mth, unit.Sp(11), t.Buffer[i]).Layout(gtx)
	})
}
