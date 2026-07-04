package widgets

import (
	"image"

	"gioui.org/layout"
	"gioui.org/op/clip"
	"gioui.org/op/paint"
	"gioui.org/unit"
	"gioui.org/widget/material"
	"github.com/robertpelloni/bqt/internal/ui/theme"
)

// TableView is a minimal Go-native model/view widget intended to close one of the
// largest remaining parity gaps versus Qt/JavaFX style frameworks.
type TableView struct {
	Columns []string
	Rows    [][]string
}

func (tv *TableView) Layout(gtx layout.Context, th theme.Theme) layout.Dimensions {
	mth := material.NewTheme()
	mth.Palette.Fg = th.Text
	mth.Palette.Bg = th.Surface

	rowHeight := gtx.Dp(unit.Dp(28))
	colCount := len(tv.Columns)
	if colCount == 0 {
		return material.Label(mth, unit.Sp(12), "No columns").Layout(gtx)
	}
	colWidth := tableMax(1, gtx.Constraints.Max.X/colCount)

	// header
	headerRect := image.Rect(0, 0, gtx.Constraints.Max.X, rowHeight)
	paint.FillShape(gtx.Ops, th.Primary, clip.Rect(headerRect).Op())

	for i, col := range tv.Columns {
		x := i * colWidth
		rect := image.Rect(x, 0, x+colWidth, rowHeight)
		defer clip.Rect(rect).Push(gtx.Ops).Pop()
		_ = material.Label(mth, unit.Sp(11), col)
	}

	list := &layout.List{Axis: layout.Vertical}
	return list.Layout(gtx, len(tv.Rows), func(gtx layout.Context, i int) layout.Dimensions {
		rowY := rowHeight * (i + 1)
		bg := th.Surface
		if i%2 == 1 {
			bg = th.Background
		}
		paint.FillShape(gtx.Ops, bg, clip.Rect(image.Rect(0, rowY, gtx.Constraints.Max.X, rowY+rowHeight)).Op())

		for j := 0; j < colCount && j < len(tv.Rows[i]); j++ {
			cellX := j * colWidth
			rect := image.Rect(cellX, rowY, cellX+colWidth, rowY+rowHeight)
			defer clip.Rect(rect).Push(gtx.Ops).Pop()
			_ = material.Label(mth, unit.Sp(10), tv.Rows[i][j])
		}
		return layout.Dimensions{Size: image.Pt(gtx.Constraints.Max.X, rowHeight)}
	})
}

func tableMax(a, b int) int {
	if a > b {
		return a
	}
	return b
}
