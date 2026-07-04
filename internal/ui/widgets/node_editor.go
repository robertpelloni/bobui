package widgets

import (
	"image"

	"gioui.org/layout"
	"gioui.org/op/clip"
	"gioui.org/op/paint"
	"gioui.org/widget/material"
	"gioui.org/unit"
	"gioui.org/f32"
	"github.com/robertpelloni/bqt/internal/ui/theme"
)

type Node struct {
	ID    string
	Pos   f32.Point
	Label string
}

type NodeEditor struct {
	Nodes []Node
}

func (n *NodeEditor) Layout(gtx layout.Context, th theme.Theme) layout.Dimensions {
	mth := material.NewTheme()
	mth.Palette.Fg = th.Text
	mth.Palette.Bg = th.Surface

	for _, node := range n.Nodes {
		rect := image.Rect(int(node.Pos.X), int(node.Pos.Y), int(node.Pos.X)+100, int(node.Pos.Y)+50)
		paint.FillShape(gtx.Ops, th.Surface, clip.Rect(rect).Op())
		_ = material.Label(mth, unit.Sp(11), node.Label)
	}
	return layout.Dimensions{Size: gtx.Constraints.Max}
}
