package widgets

import (
	"gioui.org/layout"
	"gioui.org/op"
	"github.com/robertpelloni/bqt/internal/ui/theme"
	"testing"
)

func TestNotebook(t *testing.T) {
	notebook := &BQtNotebook{}

	page1Layout := func(gtx layout.Context) layout.Dimensions {
		return layout.Dimensions{}
	}
	notebook.AddPage("Page 1", page1Layout)

	page2Layout := func(gtx layout.Context) layout.Dimensions {
		return layout.Dimensions{}
	}
	notebook.AddPage("Page 2", page2Layout)

	if len(notebook.Pages) != 2 {
		t.Fatalf("Expected 2 pages, got %d", len(notebook.Pages))
	}

	gtx := layout.Context{
		Ops: new(op.Ops),
	}
	th := theme.GetTheme(theme.Dark)

	dims := notebook.Layout(gtx, th)
	if dims.Size.X == 0 && dims.Size.Y == 0 {
		t.Logf("Notebook successfully initialized and laid out with dummy dimensions.")
	}
}
