package widgets

import (
	"gioui.org/layout"
	"gioui.org/unit"
	"gioui.org/widget"
	"gioui.org/widget/material"
	"github.com/robertpelloni/bqt/internal/ui/theme"
)

type TabBar struct {
	Tabs   []string
	Active int

	buttons []widget.Clickable
}

func (t *TabBar) ensureButtons() {
	if len(t.buttons) == len(t.Tabs) {
		return
	}
	t.buttons = make([]widget.Clickable, len(t.Tabs))
	if t.Active >= len(t.Tabs) {
		t.Active = 0
	}
}

func (t *TabBar) ActiveTitle() string {
	if t == nil || len(t.Tabs) == 0 {
		return ""
	}
	if t.Active < 0 || t.Active >= len(t.Tabs) {
		t.Active = 0
	}
	return t.Tabs[t.Active]
}

func (t *TabBar) ClickTab(index int) {
	t.ensureButtons()
	if index >= 0 && index < len(t.buttons) {
		t.buttons[index].Click()
	}
}

func (t *TabBar) Layout(gtx layout.Context, th theme.Theme) layout.Dimensions {
	// --- GO NATIVE DOCKING TAB RENDER PASS ---
	// The baseline now uses real clickable tab buttons so managed windows can
	// participate in actual runtime tab selection instead of static rendering.
	if len(t.Tabs) == 0 {
		return layout.Dimensions{}
	}
	t.ensureButtons()

	mth := material.NewTheme()
	mth.Palette.Fg = th.Text
	mth.Palette.Bg = th.Surface

	children := make([]layout.FlexChild, 0, len(t.Tabs))
	for i, tab := range t.Tabs {
		idx := i
		label := tab
		children = append(children, layout.Rigid(func(gtx layout.Context) layout.Dimensions {
			if t.buttons[idx].Clicked(gtx) {
				t.Active = idx
			}
			btn := material.Button(mth, &t.buttons[idx], label)
			if idx == t.Active {
				btn.Background = th.Primary
				btn.Color = th.Background
			} else {
				btn.Background = th.Surface
				btn.Color = th.Text
			}
			return layout.Inset{Right: unit.Dp(6)}.Layout(gtx, btn.Layout)
		}))
	}
	return layout.Flex{Axis: layout.Horizontal}.Layout(gtx, children...)
}
