package widgets

import (
	"image"
	"testing"
	"time"

	"gioui.org/app"
	"gioui.org/layout"
	"gioui.org/op"
	"gioui.org/unit"
	"github.com/robertpelloni/bqt/internal/ui/theme"
)

func newWidgetTestLayoutContext(size image.Point) layout.Context {
	var ops op.Ops
	fe := app.FrameEvent{
		Now:    time.Now(),
		Size:   size,
		Metric: unit.Metric{PxPerDp: 1, PxPerSp: 1},
		Frame:  func(frame *op.Ops) {},
	}
	return app.NewContext(&ops, fe)
}

func TestShellTaskbarProgrammaticClickTogglesStartMenu(t *testing.T) {
	shell := &Shell{}
	th := theme.GetTheme(theme.Cyberpunk)
	size := image.Pt(1280, 900)

	gtx := newWidgetTestLayoutContext(size)
	dims := shell.LayoutTaskbar(gtx, th)
	if dims.Size.X <= 0 || dims.Size.Y <= 0 {
		t.Fatalf("expected taskbar layout to produce non-zero dimensions, got %v", dims.Size)
	}
	if shell.StartMenuVisible {
		t.Fatal("expected start menu to begin hidden")
	}

	shell.StartBtn.Click()
	gtx = newWidgetTestLayoutContext(size)
	shell.LayoutTaskbar(gtx, th)
	if !shell.StartMenuVisible {
		t.Fatal("expected start button click to show start menu")
	}

	shell.StartBtn.Click()
	gtx = newWidgetTestLayoutContext(size)
	shell.LayoutTaskbar(gtx, th)
	if shell.StartMenuVisible {
		t.Fatal("expected second start button click to hide start menu")
	}
}

func TestShellStartMenuLayoutRespectsVisibility(t *testing.T) {
	shell := &Shell{}
	th := theme.GetTheme(theme.Cyberpunk)
	size := image.Pt(1280, 900)

	hidden := shell.LayoutStartMenu(newWidgetTestLayoutContext(size), th)
	if hidden.Size.X != 0 || hidden.Size.Y != 0 {
		t.Fatalf("expected hidden start menu to have zero dimensions, got %v", hidden.Size)
	}

	shell.StartMenuVisible = true
	visible := shell.LayoutStartMenu(newWidgetTestLayoutContext(size), th)
	if visible.Size.X <= 0 || visible.Size.Y <= 0 {
		t.Fatalf("expected visible start menu to have non-zero dimensions, got %v", visible.Size)
	}
	if visible.Size.X > size.X || visible.Size.Y > size.Y {
		t.Fatalf("expected start menu to fit within frame size %v, got %v", size, visible.Size)
	}
}
