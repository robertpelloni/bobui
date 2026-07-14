package ui

import (
	"image"
	"testing"

	"gioui.org/f32"
	"github.com/robertpelloni/bqt/internal/ui/theme"
	"github.com/robertpelloni/bqt/internal/ui/widgets"
)

func TestWindowManagerResetSpawnAndLayout(t *testing.T) {
	wm := &WindowManager{}
	wm.Reset()
	if len(wm.Windows) != 0 {
		t.Fatalf("expected reset window manager to start with no windows, got %d", len(wm.Windows))
	}
	if wm.Tabs == nil || len(wm.Tabs.Tabs) == 0 {
		t.Fatal("expected reset window manager to initialize tabs")
	}

	wm.SpawnManagedWindow("win-1", "Inspector", "Kernel", "kernel-body", f32.Pt(10, 20), f32.Pt(200, 120))
	wm.SpawnManagedWindow("win-2", "Docs", "Docs", "docs-body", f32.Pt(30, 40), f32.Pt(240, 140))
	wm.SpawnWindow("win-3", "Shared", f32.Pt(50, 60), f32.Pt(180, 100))
	if len(wm.Windows) != 3 {
		t.Fatalf("expected 3 spawned windows, got %d", len(wm.Windows))
	}
	if wm.Windows[0].Title != "Inspector" || wm.Windows[1].Title != "Docs" || wm.Windows[2].Title != "Shared" {
		t.Fatalf("unexpected window titles: %+v %+v %+v", wm.Windows[0], wm.Windows[1], wm.Windows[2])
	}

	dims := wm.Layout(newTestLayoutContext(image.Pt(1280, 900)), theme.GetTheme(theme.Cyberpunk))
	if dims.Size.X != 1280 || dims.Size.Y != 900 {
		t.Fatalf("expected window manager layout to cover full available space, got %v", dims.Size)
	}
	if active := wm.ActiveTab(); active != "Kernel" {
		t.Fatalf("expected initial active tab Kernel, got %q", active)
	}
	if wm.ActiveWindow() == nil || wm.ActiveWindow().ID != "win-1" {
		t.Fatalf("expected win-1 to become the initial active visible window, got %+v", wm.ActiveWindow())
	}
	visible := wm.VisibleWindows()
	if len(visible) != 2 {
		t.Fatalf("expected Kernel-tab + shared window to be visible, got %d", len(visible))
	}
	if visible[0].ID != "win-1" || visible[1].ID != "win-3" {
		t.Fatalf("unexpected visible windows for Kernel tab: %+v %+v", visible[0], visible[1])
	}

	if !wm.ActivateWindow("win-3") {
		t.Fatal("expected ActivateWindow to succeed for visible shared window")
	}
	if wm.ActiveWindow() == nil || wm.ActiveWindow().ID != "win-3" {
		t.Fatalf("expected win-3 to become active, got %+v", wm.ActiveWindow())
	}
	if wm.Windows[len(wm.Windows)-1].ID != "win-3" {
		t.Fatalf("expected active window to be promoted to front order, got tail id %q", wm.Windows[len(wm.Windows)-1].ID)
	}
	for _, win := range wm.Windows {
		if win != nil && win.ID == "win-1" {
			win.RequestActivate()
		}
	}
	wm.Layout(newTestLayoutContext(image.Pt(1280, 900)), theme.GetTheme(theme.Cyberpunk))
	if wm.ActiveWindow() == nil || wm.ActiveWindow().ID != "win-1" {
		t.Fatalf("expected click-routed activation to promote win-1, got %+v", wm.ActiveWindow())
	}

	wm.Tabs.ClickTab(2)
	gtx := newTestLayoutContext(image.Pt(1280, 900))
	wm.Layout(gtx, theme.GetTheme(theme.Cyberpunk))
	if active := wm.ActiveTab(); active != "Docs" {
		t.Fatalf("expected active tab Docs after click, got %q", active)
	}
	visible = wm.VisibleWindows()
	if len(visible) != 2 {
		t.Fatalf("expected Docs-tab + shared window to be visible, got %d", len(visible))
	}
	if visible[0].ID != "win-2" || visible[1].ID != "win-3" {
		t.Fatalf("unexpected visible windows for Docs tab: %+v %+v", visible[0], visible[1])
	}
	if wm.ActiveWindow() == nil || wm.ActiveWindow().ID != "win-2" {
		t.Fatalf("expected active window to fall back to visible docs window after tab change, got %+v", wm.ActiveWindow())
	}

	if !wm.MoveWindow("win-2", 10, -5) {
		t.Fatal("expected MoveWindow to succeed for existing window")
	}
	var win2 *widgets.Window
	for _, win := range wm.Windows {
		if win != nil && win.ID == "win-2" {
			win2 = win
			break
		}
	}
	if win2 == nil {
		t.Fatal("expected to find win-2 after move")
	}
	if win2.Pos.X != 40 || win2.Pos.Y != 35 {
		t.Fatalf("unexpected moved position for win-2: %v/%v", win2.Pos.X, win2.Pos.Y)
	}
	if !wm.ResizeWindow("win-2", 20, 30) {
		t.Fatal("expected ResizeWindow to succeed for existing window")
	}
	if win2.Size.X != 260 || win2.Size.Y != 170 {
		t.Fatalf("unexpected resized size for win-2: %v/%v", win2.Size.X, win2.Size.Y)
	}
	if !wm.CloseWindow("win-3") {
		t.Fatal("expected CloseWindow to succeed for existing active window")
	}
	if wm.ActiveWindow() == nil || wm.ActiveWindow().ID != "win-2" {
		t.Fatalf("expected active window to fall back to visible docs window, got %+v", wm.ActiveWindow())
	}
	visible = wm.VisibleWindows()
	if len(visible) != 1 || visible[0].ID != "win-2" {
		t.Fatalf("expected only docs window to remain visible after closing shared window, got %+v", visible)
	}
	if wm.MoveWindow("missing", 1, 1) || wm.ResizeWindow("missing", 1, 1) || wm.CloseWindow("missing") || wm.ActivateWindow("missing") {
		t.Fatal("expected move/resize/close/activate to fail for missing window")
	}
}
