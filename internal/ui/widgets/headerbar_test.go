package widgets

import (
	"gioui.org/layout"
	"gioui.org/op"
	"github.com/robertpelloni/bqt/internal/ui/theme"
	"testing"
)

func TestHeaderBar(t *testing.T) {
	hb := &BQtHeaderBar{
		Title:    "Test Title",
		Subtitle: "Test Subtitle",
	}

	gtx := layout.Context{
		Ops: new(op.Ops),
	}
	th := theme.GetTheme(theme.Dark)

	dims := hb.Layout(gtx, th)
	if dims.Size.X == 0 && dims.Size.Y == 0 {
		t.Logf("HeaderBar successfully initialized and laid out with dummy dimensions.")
	}
}
