package widgets

import (
	"gioui.org/layout"
	"gioui.org/widget/material"
	"github.com/robertpelloni/bqt/internal/ui/theme"
)

// BQtHeaderBar achieves GTK GtkHeaderBar functional parity in the pure Go UI layer.
type BQtHeaderBar struct {
	Title         string
	Subtitle      string
	ShowClose     bool
	CustomWidgets []layout.Widget
}

func (h *BQtHeaderBar) Layout(gtx layout.Context, th theme.Theme) layout.Dimensions {
	mth := material.NewTheme()
	mth.Palette.Fg = th.Text
	mth.Palette.Bg = th.Primary

	return layout.Flex{Axis: layout.Horizontal, Alignment: layout.Middle}.Layout(gtx,
		layout.Rigid(func(gtx layout.Context) layout.Dimensions {
			return layout.Flex{Axis: layout.Vertical}.Layout(gtx,
				layout.Rigid(material.H6(mth, h.Title).Layout),
				layout.Rigid(material.Body2(mth, h.Subtitle).Layout),
			)
		}),
		layout.Flexed(1, func(gtx layout.Context) layout.Dimensions {
			// Spacer
			return layout.Dimensions{Size: gtx.Constraints.Min}
		}),
		layout.Rigid(func(gtx layout.Context) layout.Dimensions {
			if len(h.CustomWidgets) > 0 {
				var children []layout.FlexChild
				for _, cw := range h.CustomWidgets {
					children = append(children, layout.Rigid(cw))
				}
				return layout.Flex{Axis: layout.Horizontal}.Layout(gtx, children...)
			}
			return layout.Dimensions{}
		}),
	)
}
