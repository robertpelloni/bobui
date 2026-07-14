package widgets

import (
	"gioui.org/layout"
	"gioui.org/widget/material"
	"gioui.org/unit"
	"github.com/robertpelloni/bqt/internal/ui/theme"
)

type FileEntry struct {
	Name   string
	Path   string
	PeerID string // Empty for local
}

type MeshExplorer struct {
	Files []FileEntry
}

func (me *MeshExplorer) Layout(gtx layout.Context, th theme.Theme) layout.Dimensions {
	// --- MESH EXPLORER RENDER PASS ---
	// High-performance unified file listing natively in Go.
	
	mth := material.NewTheme()
	mth.Palette.Fg = th.Text
	mth.Palette.Bg = th.Surface

	list := &layout.List{Axis: layout.Vertical}
	return list.Layout(gtx, len(me.Files), func(gtx layout.Context, i int) layout.Dimensions {
		f := me.Files[i]
		label := f.Name
		if f.PeerID != "" {
			label = "[REMOTE] " + label
		}
		
		return material.Label(mth, unit.Sp(11), label).Layout(gtx)
	})
}
