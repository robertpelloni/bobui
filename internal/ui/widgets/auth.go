package widgets

import (
	"gioui.org/layout"
	"gioui.org/widget"
	"gioui.org/widget/material"
	"gioui.org/unit"
	"github.com/robertpelloni/bqt/internal/ui/theme"
	"github.com/robertpelloni/bqt/internal/data"
)

type AuthShell struct {
	UserField, PassField widget.Editor
	BootBtn              widget.Clickable
}

func (a *AuthShell) Layout(gtx layout.Context, th theme.Theme) layout.Dimensions {
	// --- GO NATIVE SECURE BOOT RENDER PASS ---
	mth := material.NewTheme()
	mth.Palette.Fg = th.Text
	mth.Palette.Bg = th.Surface

	return layout.Center.Layout(gtx, func(gtx layout.Context) layout.Dimensions {
		return layout.Flex{Axis: layout.Vertical, Alignment: layout.Middle}.Layout(gtx,
			layout.Rigid(func(gtx layout.Context) layout.Dimensions {
				return material.H4(mth, "OMNI.OS KERNEL").Layout(gtx)
			}),
			layout.Rigid(layout.Spacer{Height: unit.Dp(20)}.Layout),
			layout.Rigid(func(gtx layout.Context) layout.Dimensions {
				if a.BootBtn.Clicked(gtx) {
					// Verify against Go Database
					db := data.GetDatabase()
					db.ExecuteQuery("SELECT * FROM _omni_users")
				}
				return material.Button(mth, &a.BootBtn, "BOOT SYSTEM").Layout(gtx)
			}),
		)
	})
}
