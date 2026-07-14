package ui

import (
	"image"

	"gioui.org/f32"
	"gioui.org/layout"
	"gioui.org/op"
	"github.com/robertpelloni/bqt/internal/ui/theme"
	"github.com/robertpelloni/bqt/internal/ui/widgets"
)

type WindowManager struct {
	Windows        []*widgets.Window
	Tabs           *widgets.TabBar
	ActiveWindowID string
}

var (
	wmInstance *WindowManager
)

func GetWindowManager() *WindowManager {
	if wmInstance == nil {
		wmInstance = &WindowManager{
			Windows: make([]*widgets.Window, 0),
			Tabs:    &widgets.TabBar{Tabs: []string{"Kernel", "Terminal", "Docs"}},
		}
	}
	return wmInstance
}

func (wm *WindowManager) Reset() {
	wm.Windows = make([]*widgets.Window, 0)
	wm.Tabs = &widgets.TabBar{Tabs: []string{"Kernel", "Terminal", "Docs"}}
	wm.ActiveWindowID = ""
}

func (wm *WindowManager) ActiveTab() string {
	if wm == nil || wm.Tabs == nil {
		return ""
	}
	return wm.Tabs.ActiveTitle()
}

func (wm *WindowManager) ActiveWindow() *widgets.Window {
	if wm == nil || wm.ActiveWindowID == "" {
		return nil
	}
	for _, win := range wm.Windows {
		if win != nil && !win.Closed && win.ID == wm.ActiveWindowID {
			return win
		}
	}
	return nil
}

func (wm *WindowManager) syncActiveWindowState() {
	active := wm.ActiveWindowID
	for _, win := range wm.Windows {
		if win != nil {
			win.SetActive(win.ID == active)
		}
		if win != nil && win.Closed && win.ID == active {
			active = ""
		}
	}
	wm.ActiveWindowID = active
}

func (wm *WindowManager) ensureActiveVisible() {
	visible := wm.VisibleWindows()
	if len(visible) == 0 {
		wm.ActiveWindowID = ""
		wm.syncActiveWindowState()
		return
	}
	for _, win := range visible {
		if win.ID == wm.ActiveWindowID {
			wm.syncActiveWindowState()
			return
		}
	}
	wm.ActiveWindowID = visible[0].ID
	wm.syncActiveWindowState()
}

func (wm *WindowManager) VisibleWindows() []*widgets.Window {
	if wm == nil {
		return nil
	}
	activeTab := wm.ActiveTab()
	visible := make([]*widgets.Window, 0, len(wm.Windows))
	for _, win := range wm.Windows {
		if win == nil || win.Closed {
			continue
		}
		if win.Tab == "" || activeTab == "" || win.Tab == activeTab {
			visible = append(visible, win)
		}
	}
	return visible
}

func (wm *WindowManager) Layout(gtx layout.Context, th theme.Theme) layout.Dimensions {
	if wm == nil {
		return layout.Dimensions{}
	}

	if wm.Tabs != nil {
		stack := op.Offset(image.Pt(16, 16)).Push(gtx.Ops)
		wm.Tabs.Layout(gtx, th)
		stack.Pop()
	}

	wm.ensureActiveVisible()
	var requestedID string
	for _, win := range wm.VisibleWindows() {
		win.Layout(gtx, th)
		if win.ConsumeActivationRequest() {
			requestedID = win.ID
		}
	}
	if requestedID != "" {
		wm.ActivateWindow(requestedID)
	}
	return layout.Dimensions{Size: gtx.Constraints.Max}
}

func (wm *WindowManager) SpawnWindow(id, title string, pos, size f32.Point) {
	wm.Windows = append(wm.Windows, &widgets.Window{ID: id, Title: title, Pos: pos, Size: size})
}

func (wm *WindowManager) SpawnManagedWindow(id, title, tab, body string, pos, size f32.Point) {
	wm.Windows = append(wm.Windows, &widgets.Window{ID: id, Title: title, Tab: tab, Body: body, Pos: pos, Size: size})
}

func (wm *WindowManager) ActivateWindow(id string) bool {
	for i, win := range wm.Windows {
		if win != nil && !win.Closed && win.ID == id {
			wm.ActiveWindowID = id
			if i != len(wm.Windows)-1 {
				wm.Windows = append(append(wm.Windows[:i], wm.Windows[i+1:]...), win)
			}
			wm.syncActiveWindowState()
			return true
		}
	}
	return false
}

func (wm *WindowManager) CloseWindow(id string) bool {
	for _, win := range wm.Windows {
		if win != nil && win.ID == id {
			win.Close()
			if wm.ActiveWindowID == id {
				wm.ActiveWindowID = ""
			}
			wm.ensureActiveVisible()
			return true
		}
	}
	return false
}

func (wm *WindowManager) MoveWindow(id string, dx, dy float32) bool {
	for _, win := range wm.Windows {
		if win != nil && win.ID == id {
			win.MoveBy(dx, dy)
			return true
		}
	}
	return false
}

func (wm *WindowManager) ResizeWindow(id string, dw, dh float32) bool {
	for _, win := range wm.Windows {
		if win != nil && win.ID == id {
			win.ResizeBy(dw, dh)
			return true
		}
	}
	return false
}
