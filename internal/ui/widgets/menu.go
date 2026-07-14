package widgets

import (
	"image"

	"gioui.org/layout"
	"gioui.org/op"
	"gioui.org/op/clip"
	"gioui.org/op/paint"
	"gioui.org/unit"
	"gioui.org/widget"
	"gioui.org/widget/material"
	"github.com/robertpelloni/bqt/internal/ui/theme"
	"gioui.org/io/event"
)

type MenuItem struct {
	Label   string
	Action  string
	Click   widget.Clickable
	Hovered bool
}

// Menu is a transient popover list of options.
type Menu struct {
	Visible bool
	Items   []*MenuItem
	X, Y    int
	WidthDp float32
}

func (m *Menu) Layout(gtx layout.Context, th theme.Theme) (layout.Dimensions, int) {
	if !m.Visible || len(m.Items) == 0 {
		return layout.Dimensions{}, -1
	}

	mth := material.NewTheme()
	mth.Palette.Fg = th.Text
	mth.Palette.Bg = th.Surface

    // Global overlay to capture clicks outside the menu to dismiss it
    defer clip.Rect(image.Rect(0, 0, gtx.Constraints.Max.X, gtx.Constraints.Max.Y)).Push(gtx.Ops).Pop()

    // Block clicks outside
    event.Op(gtx.Ops, m)

	if m.WidthDp <= 0 {
		m.WidthDp = 200
	}
	w := gtx.Dp(unit.Dp(m.WidthDp))

    // Fake a layout to get the height
    itemHeight := gtx.Dp(unit.Dp(36))
    h := itemHeight * len(m.Items) + gtx.Dp(16) // 8dp padding top and bottom

    // Clamping to screen bounds
    px := m.X
    py := m.Y
    if px + w > gtx.Constraints.Max.X {
        px = gtx.Constraints.Max.X - w - gtx.Dp(8)
    }
    if py + h > gtx.Constraints.Max.Y {
        py = gtx.Constraints.Max.Y - h - gtx.Dp(8)
    }

    // Draw Menu Surface
    defer op.Offset(image.Pt(px, py)).Push(gtx.Ops).Pop()

    // Block clicks from passing through the menu itself to the dimmer
    defer clip.Rect(image.Rect(0, 0, w, h)).Push(gtx.Ops).Pop()
    event.Op(gtx.Ops, &m.Items) // block events

	paint.FillShape(gtx.Ops, th.Surface, clip.Rect(image.Rect(0, 0, w, h)).Op())

    // Add a border
    paint.FillShape(gtx.Ops, th.Accent, clip.Stroke{
		Path:  clip.Rect(image.Rect(0, 0, w, h)).Path(),
		Width: 1,
	}.Op())

	innerGtx := gtx
	innerGtx.Constraints.Min = image.Point{X: w, Y: h}
	innerGtx.Constraints.Max = image.Point{X: w, Y: h}

    selectedIndex := -1

	layout.UniformInset(unit.Dp(8)).Layout(innerGtx, func(gtx layout.Context) layout.Dimensions {
		list := layout.List{Axis: layout.Vertical}
		return list.Layout(gtx, len(m.Items), func(gtx layout.Context, i int) layout.Dimensions {
            item := m.Items[i]

            if item.Click.Clicked(gtx) {
                selectedIndex = i
                m.Visible = false
            }

            if item.Click.Hovered() {
                item.Hovered = true
            } else {
                item.Hovered = false
            }

            return material.Clickable(gtx, &item.Click, func(gtx layout.Context) layout.Dimensions {
                bg := th.Surface
                if item.Hovered {
                    bg = th.Accent // Use accent color as hover highlight
                }

                paint.FillShape(gtx.Ops, bg, clip.Rect(image.Rect(0, 0, gtx.Constraints.Max.X, gtx.Constraints.Max.Y)).Op())

                return layout.Inset{Top: unit.Dp(8), Bottom: unit.Dp(8), Left: unit.Dp(16), Right: unit.Dp(16)}.Layout(gtx, func(gtx layout.Context) layout.Dimensions {
                    return material.Body1(mth, item.Label).Layout(gtx)
                })
            })
		})
	})

	return layout.Dimensions{Size: gtx.Constraints.Max}, selectedIndex
}
