package widgets

import (
	"gioui.org/layout"
	"github.com/robertpelloni/bqt/internal/ui/theme"
)

type DockArea struct {
	Name string
}

func (d *DockArea) Layout(gtx layout.Context, th theme.Theme, child layout.Widget) layout.Dimensions {
	// Native Go GPU-Accelerated Docking
	return child(gtx)
}
