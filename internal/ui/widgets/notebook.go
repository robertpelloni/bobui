package widgets

import (
	"gioui.org/layout"
	"gioui.org/widget"
	"gioui.org/widget/material"
	"github.com/robertpelloni/bqt/internal/ui/theme"
)

// BQtNotebook achieves GTK GtkNotebook functional parity in the pure Go UI layer.
type NotebookPage struct {
	Title  string
	Widget layout.Widget
}

type BQtNotebook struct {
	Pages       []NotebookPage
	ActiveIndex int
	tabs        []widget.Clickable
}

func (n *BQtNotebook) AddPage(title string, w layout.Widget) {
	n.Pages = append(n.Pages, NotebookPage{Title: title, Widget: w})
	n.tabs = append(n.tabs, widget.Clickable{})
}

func (n *BQtNotebook) Layout(gtx layout.Context, th theme.Theme) layout.Dimensions {
	if len(n.Pages) == 0 {
		return layout.Dimensions{}
	}

	mth := material.NewTheme()
	mth.Palette.Fg = th.Text
	mth.Palette.Bg = th.Primary

	// Handle tab clicks
	for i := range n.tabs {
		if n.tabs[i].Clicked(gtx) {
			n.ActiveIndex = i
		}
	}

	// Layout Tabs
	return layout.Flex{Axis: layout.Vertical}.Layout(gtx,
		layout.Rigid(func(gtx layout.Context) layout.Dimensions {
			var flexChildren []layout.FlexChild
			for i, page := range n.Pages {
				idx := i // capture
				btn := material.Button(mth, &n.tabs[idx], page.Title)
				if idx != n.ActiveIndex {
					// Make inactive tabs look different
					btn.Background = th.Surface
					btn.Color = th.Text
				}
				flexChildren = append(flexChildren, layout.Rigid(btn.Layout))
			}
			return layout.Flex{Axis: layout.Horizontal}.Layout(gtx, flexChildren...)
		}),
		layout.Flexed(1, func(gtx layout.Context) layout.Dimensions {
			if n.ActiveIndex >= 0 && n.ActiveIndex < len(n.Pages) {
				return n.Pages[n.ActiveIndex].Widget(gtx)
			}
			return layout.Dimensions{}
		}),
	)
}
