package widgets

import (
	"gioui.org/layout"
	"gioui.org/widget/material"
	"gioui.org/unit"
	"github.com/robertpelloni/bqt/internal/ui/theme"
	"github.com/robertpelloni/bqt/internal/kernel"
)

type AdminShell struct {
	UserList []string
}

func (a *AdminShell) Layout(gtx layout.Context, th theme.Theme) layout.Dimensions {
	// --- GO NATIVE ADMIN RENDER PASS ---
	// Real-time peer permission management natively in Go.
	
	mth := material.NewTheme()
	mth.Palette.Fg = th.Text
	mth.Palette.Bg = th.Surface

	pm := kernel.GetPermissionManager()

	list := &layout.List{Axis: layout.Vertical}
	return list.Layout(gtx, len(a.UserList), func(gtx layout.Context, i int) layout.Dimensions {
		user := a.UserList[i]
		
		return layout.Flex{Axis: layout.Horizontal}.Layout(gtx,
			layout.Flexed(1, material.Label(mth, unit.Sp(12), user).Layout),
			layout.Rigid(func(gtx layout.Context) layout.Dimensions {
				// Lock Toggle Simulation
				isLocked := !pm.CanInteract("sys-mouse-0", "win_1")
				lbl := "UNLOCKED"
				if isLocked { lbl = "LOCKED" }
				return material.Label(mth, unit.Sp(10), lbl).Layout(gtx)
			}),
		)
	})
}
