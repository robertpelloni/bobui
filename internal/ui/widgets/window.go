package widgets

import (
	"image"

	"gioui.org/f32"
	"gioui.org/gesture"
	"gioui.org/io/pointer"
	"gioui.org/layout"
	"gioui.org/op"
	"gioui.org/op/clip"
	"gioui.org/op/paint"
	"gioui.org/unit"
	"gioui.org/widget"
	"gioui.org/widget/material"
	"github.com/robertpelloni/bqt/internal/ui/theme"
)

type Window struct {
	ID    string
	Title string
	Tab   string
	Body  string
	Pos   f32.Point
	Size  f32.Point

	MinSize f32.Point
	Closed  bool
	Active  bool

	CloseBtn widget.Clickable
	activate gesture.Click

	moveDrag   gesture.Drag
	resizeDrag gesture.Drag
	moving     bool
	resizing   bool
	lastMove   f32.Point
	lastResize f32.Point

	activationRequested bool
}

func (w *Window) ensureDefaults() {
	if w.MinSize.X <= 0 {
		w.MinSize.X = 180
	}
	if w.MinSize.Y <= 0 {
		w.MinSize.Y = 120
	}
	if w.Size.X < w.MinSize.X {
		w.Size.X = w.MinSize.X
	}
	if w.Size.Y < w.MinSize.Y {
		w.Size.Y = w.MinSize.Y
	}
}

func (w *Window) MoveBy(dx, dy float32) {
	w.Pos.X += dx
	w.Pos.Y += dy
}

func (w *Window) ResizeBy(dw, dh float32) {
	w.ensureDefaults()
	w.Size.X += dw
	w.Size.Y += dh
	if w.Size.X < w.MinSize.X {
		w.Size.X = w.MinSize.X
	}
	if w.Size.Y < w.MinSize.Y {
		w.Size.Y = w.MinSize.Y
	}
}

func (w *Window) Close() {
	w.Closed = true
	w.Active = false
}

func (w *Window) SetActive(active bool) {
	w.Active = active && !w.Closed
}

func (w *Window) RequestActivate() {
	w.activationRequested = true
}

func (w *Window) ConsumeActivationRequest() bool {
	requested := w.activationRequested
	w.activationRequested = false
	return requested
}

func (w *Window) handleDrag(gtx layout.Context, drag *gesture.Drag, active *bool, last *f32.Point, onDelta func(dx, dy float32)) {
	for {
		ev, ok := drag.Update(gtx.Metric, gtx.Source, gesture.Both)
		if !ok {
			break
		}
		switch ev.Kind {
		case pointer.Press:
			*active = true
			*last = ev.Position
		case pointer.Drag:
			if *active {
				onDelta(ev.Position.X-last.X, ev.Position.Y-last.Y)
				*last = ev.Position
			}
		case pointer.Release, pointer.Cancel:
			*active = false
		}
	}
}

func (w *Window) Layout(gtx layout.Context, th theme.Theme) layout.Dimensions {
	if w.Closed {
		return layout.Dimensions{}
	}
	w.ensureDefaults()
	w.handleDrag(gtx, &w.moveDrag, &w.moving, &w.lastMove, w.MoveBy)
	w.handleDrag(gtx, &w.resizeDrag, &w.resizing, &w.lastResize, w.ResizeBy)

	width := int(w.Size.X)
	height := int(w.Size.Y)
	stack := op.Offset(image.Pt(int(w.Pos.X), int(w.Pos.Y))).Push(gtx.Ops)
	defer stack.Pop()

	local := layout.Context{
		Constraints: layout.Exact(image.Pt(width, height)),
		Metric:      gtx.Metric,
		Now:         gtx.Now,
		Locale:      gtx.Locale,
		Values:      gtx.Values,
		Source:      gtx.Source,
		Ops:         gtx.Ops,
	}

	mth := material.NewTheme()
	mth.Palette.Fg = th.Text
	mth.Palette.Bg = th.Surface

	outer := image.Rect(0, 0, width, height)
	titleBar := image.Rect(0, 0, width, 30)
	bodyRect := image.Rect(0, 30, width, height)
	resizeHandle := image.Rect(width-18, height-18, width, height)
	titleColor := th.Primary
	if w.Active {
		titleColor = th.Accent
	}
	paint.FillShape(local.Ops, th.Surface, clip.Rect(outer).Op())
	paint.FillShape(local.Ops, titleColor, clip.Rect(titleBar).Op())
	paint.FillShape(local.Ops, th.Background, clip.Rect(bodyRect).Op())
	paint.FillShape(local.Ops, th.Accent, clip.Rect(resizeHandle).Op())
	if th.Type == theme.Cyberpunk {
		for i := 40; i < height; i += 40 {
			line := image.Rect(0, i, width, i+1)
			paint.FillShape(local.Ops, th.Primary, clip.Rect(line).Op())
		}
	}

	moveScope := clip.Rect(titleBar).Push(local.Ops)
	w.moveDrag.Add(local.Ops)
	moveScope.Pop()

	resizeScope := clip.Rect(resizeHandle).Push(local.Ops)
	w.resizeDrag.Add(local.Ops)
	resizeScope.Pop()

	activateScope := clip.Rect(outer).Push(local.Ops)
	w.activate.Add(local.Ops)
	activateScope.Pop()
	for {
		ev, ok := w.activate.Update(local.Source)
		if !ok {
			break
		}
		if ev.Kind == gesture.KindClick {
			w.activationRequested = true
		}
	}

	if w.CloseBtn.Clicked(local) {
		w.Close()
		return layout.Dimensions{}
	}

	return layout.Inset{Top: unit.Dp(8), Left: unit.Dp(10), Right: unit.Dp(10), Bottom: unit.Dp(8)}.Layout(local, func(gtx layout.Context) layout.Dimensions {
		body := w.Body
		if body == "" {
			body = "Managed window baseline"
		}
		tabInfo := w.Tab
		if tabInfo == "" {
			tabInfo = "shared"
		}
		state := "inactive"
		if w.Active {
			state = "active"
		}
		return layout.Flex{Axis: layout.Vertical}.Layout(gtx,
			layout.Rigid(func(gtx layout.Context) layout.Dimensions {
				return layout.Flex{Axis: layout.Horizontal, Spacing: layout.SpaceBetween, Alignment: layout.Middle}.Layout(gtx,
					layout.Flexed(1, material.Label(mth, unit.Sp(12), w.Title).Layout),
					layout.Rigid(func(gtx layout.Context) layout.Dimensions {
						btn := material.Button(mth, &w.CloseBtn, "×")
						btn.Background = th.Accent
						btn.Color = th.Background
						return btn.Layout(gtx)
					}),
				)
			}),
			layout.Rigid(layout.Spacer{Height: unit.Dp(18)}.Layout),
			layout.Rigid(material.Body2(mth, body).Layout),
			layout.Rigid(layout.Spacer{Height: unit.Dp(8)}.Layout),
			layout.Rigid(material.Caption(mth, "tab="+tabInfo+" id="+w.ID+" state="+state).Layout),
			layout.Rigid(material.Caption(mth, "click to activate | drag title bar | resize bottom-right").Layout),
		)
	})
}
