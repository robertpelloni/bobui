package ui

import (
	"image"
	"testing"
	"time"

	"gioui.org/app"
	"gioui.org/layout"
	"gioui.org/op"
	"gioui.org/unit"
)

func newTestLayoutContext(size image.Point) layout.Context {
	var ops op.Ops
	fe := app.FrameEvent{
		Now:    time.Now(),
		Size:   size,
		Metric: unit.Metric{PxPerDp: 1, PxPerSp: 1},
		Frame:  func(frame *op.Ops) {},
	}
	return app.NewContext(&ops, fe)
}

func TestEngineInitializeDemoConfiguresExecutableWebViewRuntime(t *testing.T) {
	e := NewEngine()
	if e.wm != nil {
		e.wm.Reset()
	}
	e.initializeDemo()

	if e.demo == nil {
		t.Fatal("expected demo surface to be initialized")
	}
	if e.demo.WebView.Bridge == nil {
		t.Fatal("expected WebView bridge to be initialized")
	}
	if e.demo.WebView.LastEvaluatedJS != "window.bqtReady = true" {
		t.Fatalf("expected LastEvaluatedJS to be initialized, got %q", e.demo.WebView.LastEvaluatedJS)
	}
	if e.demo.WebView.LastEvalResult != "evaluated:window.bqtReady = true" {
		t.Fatalf("expected LastEvalResult to be initialized, got %q", e.demo.WebView.LastEvalResult)
	}
	if e.demo.WebView.Title != "about:bqt runtime" {
		t.Fatalf("expected title to be updated by runtime handler, got %q", e.demo.WebView.Title)
	}
	if e.demo.WebView.QueueDepth() != 0 {
		t.Fatalf("expected empty runtime queue after initialization, got %d", e.demo.WebView.QueueDepth())
	}
	if e.wm == nil {
		t.Fatal("expected window manager to be initialized")
	}
	if len(e.wm.Windows) != 3 {
		t.Fatalf("expected 3 managed runtime windows after initialization, got %d", len(e.wm.Windows))
	}
	if e.wm.Tabs == nil || len(e.wm.Tabs.Tabs) == 0 {
		t.Fatal("expected window manager tabs to be initialized")
	}
	if active := e.wm.ActiveTab(); active != "Kernel" {
		t.Fatalf("expected initial active tab Kernel, got %q", active)
	}
	visible := e.wm.VisibleWindows()
	if len(visible) != 1 {
		t.Fatalf("expected one visible managed window for initial Kernel tab, got %d", len(visible))
	}
	if visible[0].Title != "Kernel Inspector" {
		t.Fatalf("expected Kernel Inspector to be initially visible, got %q", visible[0].Title)
	}
	if e.wm.Tabs == nil || len(e.wm.Tabs.Tabs) == 0 {
		t.Fatal("expected window manager tabs to be initialized")
	}

	reply := e.demo.WebView.Request("ping", "test")
	if reply.Kind != "reply" {
		t.Fatalf("expected reply kind after initialized request handler, got %q", reply.Kind)
	}
	if reply.Payload != "pong:test" {
		t.Fatalf("expected pong:test payload, got %q", reply.Payload)
	}
}

func TestEngineLayoutSupportsLoginAndAuthenticatedRuntimeStates(t *testing.T) {
	e := NewEngine()
	if e.wm != nil {
		e.wm.Reset()
	}
	e.initializeDemo()
	size := image.Pt(1280, 900)

	loginCtx := newTestLayoutContext(size)
	loginDims := e.layout(loginCtx)
	if loginDims.Size.X <= 0 || loginDims.Size.Y <= 0 {
		t.Fatalf("expected login layout to produce non-zero dimensions, got %+v", loginDims.Size)
	}

	e.login.Authenticated = true
	runtimeCtx := newTestLayoutContext(size)
	runtimeDims := e.layout(runtimeCtx)
	if runtimeDims.Size.X <= 0 || runtimeDims.Size.Y <= 0 {
		t.Fatalf("expected runtime layout to produce non-zero dimensions, got %+v", runtimeDims.Size)
	}
	if runtimeDims.Size.X > size.X || runtimeDims.Size.Y > size.Y {
		t.Fatalf("expected runtime layout to fit within frame size %v, got %v", size, runtimeDims.Size)
	}
}
