package widgets

import (
	"fmt"
	"image"
	"math"

	"gioui.org/gesture"
	"gioui.org/io/pointer"
	"gioui.org/layout"
	"gioui.org/op/clip"
	"gioui.org/op/paint"
	"gioui.org/unit"
	"gioui.org/widget/material"
	"github.com/robertpelloni/bqt/internal/ui/theme"
)

type SwipeDirection int

const (
	SwipeNone SwipeDirection = iota
	SwipeLeft
	SwipeRight
	SwipeUp
	SwipeDown
)

func (d SwipeDirection) String() string {
	switch d {
	case SwipeLeft:
		return "left"
	case SwipeRight:
		return "right"
	case SwipeUp:
		return "up"
	case SwipeDown:
		return "down"
	default:
		return "none"
	}
}

// TouchArea is a lightweight touch/swipe primitive for the verified Go baseline.
// It recognizes directional swipe gestures and can optionally treat mouse drags
// as touch-like input so the behavior remains explorable on non-touch desktops.
type TouchArea struct {
	Label             string
	AllowMouseAsTouch bool
	MinSwipeDistance  float32
	MinSize           image.Point

	OnSwipe func(direction SwipeDirection, distance float32)
	OnTouch func(active bool)

	drag gesture.Drag

	Active           bool
	CurrentSource    pointer.Source
	LastSource       pointer.Source
	StartX, StartY   float32
	CurrentX, CurrentY float32
	DeltaX, DeltaY   float32
	LastSwipe        SwipeDirection
	LastDistance     float32
}

func (ta *TouchArea) Layout(gtx layout.Context, th theme.Theme) layout.Dimensions {
	if ta.MinSwipeDistance <= 0 {
		ta.MinSwipeDistance = 24
	}
	if ta.MinSize.X <= 0 {
		ta.MinSize.X = gtx.Dp(unit.Dp(220))
	}
	if ta.MinSize.Y <= 0 {
		ta.MinSize.Y = gtx.Dp(unit.Dp(96))
	}

	for {
		e, ok := ta.drag.Update(gtx.Metric, gtx.Source, gesture.Both)
		if !ok {
			break
		}
		if !ta.acceptSource(e.Source) {
			continue
		}
		ta.LastSource = e.Source
		switch e.Kind {
		case pointer.Press:
			ta.Active = true
			ta.CurrentSource = e.Source
			ta.StartX, ta.StartY = e.Position.X, e.Position.Y
			ta.CurrentX, ta.CurrentY = e.Position.X, e.Position.Y
			ta.DeltaX, ta.DeltaY = 0, 0
			if ta.OnTouch != nil {
				ta.OnTouch(true)
			}
		case pointer.Drag:
			ta.Active = true
			ta.CurrentX, ta.CurrentY = e.Position.X, e.Position.Y
			ta.DeltaX = ta.CurrentX - ta.StartX
			ta.DeltaY = ta.CurrentY - ta.StartY
		case pointer.Release:
			ta.CurrentX, ta.CurrentY = e.Position.X, e.Position.Y
			ta.DeltaX = ta.CurrentX - ta.StartX
			ta.DeltaY = ta.CurrentY - ta.StartY
			ta.finishGesture()
		case pointer.Cancel:
			ta.Active = false
			ta.CurrentSource = 0
			ta.DeltaX, ta.DeltaY = 0, 0
			if ta.OnTouch != nil {
				ta.OnTouch(false)
			}
		}
	}

	mth := material.NewTheme()
	mth.Palette.Fg = th.Text
	mth.Palette.Bg = th.Surface

	size := image.Pt(ta.MinSize.X, ta.MinSize.Y)
	if gtx.Constraints.Max.X > 0 && size.X > gtx.Constraints.Max.X {
		size.X = gtx.Constraints.Max.X
	}
	if gtx.Constraints.Max.Y > 0 && size.Y > gtx.Constraints.Max.Y {
		size.Y = gtx.Constraints.Max.Y
	}
	gtx.Constraints.Min = size
	gtx.Constraints.Max = size

	area := image.Rect(0, 0, size.X, size.Y)
	defer clip.Rect(area).Push(gtx.Ops).Pop()
	ta.drag.Add(gtx.Ops)

	bg := th.Surface
	if ta.Active {
		bg = th.Accent
	}
	paint.FillShape(gtx.Ops, bg, clip.Rect(area).Op())

	inset := layout.UniformInset(unit.Dp(10))
	return inset.Layout(gtx, func(gtx layout.Context) layout.Dimensions {
		return layout.Flex{Axis: layout.Vertical}.Layout(gtx,
			layout.Rigid(material.Label(mth, unit.Sp(12), ta.headerText()).Layout),
			layout.Rigid(layout.Spacer{Height: unit.Dp(6)}.Layout),
			layout.Rigid(material.Label(mth, unit.Sp(10), ta.statusText()).Layout),
		)
	})
}

func (ta *TouchArea) acceptSource(src pointer.Source) bool {
	return src == pointer.Touch || (ta.AllowMouseAsTouch && src == pointer.Mouse)
}

func (ta *TouchArea) finishGesture() {
	ta.Active = false
	ta.CurrentSource = 0
	direction, distance := ta.detectSwipe()
	if direction != SwipeNone {
		ta.LastSwipe = direction
		ta.LastDistance = distance
		if ta.OnSwipe != nil {
			ta.OnSwipe(direction, distance)
		}
	}
	if ta.OnTouch != nil {
		ta.OnTouch(false)
	}
}

func (ta *TouchArea) detectSwipe() (SwipeDirection, float32) {
	adx := float32(math.Abs(float64(ta.DeltaX)))
	ady := float32(math.Abs(float64(ta.DeltaY)))
	if adx < ta.MinSwipeDistance && ady < ta.MinSwipeDistance {
		return SwipeNone, 0
	}
	if adx >= ady {
		if ta.DeltaX < 0 {
			return SwipeLeft, adx
		}
		return SwipeRight, adx
	}
	if ta.DeltaY < 0 {
		return SwipeUp, ady
	}
	return SwipeDown, ady
}

func (ta *TouchArea) headerText() string {
	label := ta.Label
	if label == "" {
		label = "Touch Area"
	}
	if ta.AllowMouseAsTouch {
		return label + " (touch + mouse fallback)"
	}
	return label + " (touch only)"
}

func (ta *TouchArea) statusText() string {
	active := "idle"
	if ta.Active {
		active = fmt.Sprintf("active source=%s dx=%.0f dy=%.0f", ta.sourceString(ta.LastSource), ta.DeltaX, ta.DeltaY)
	}
	return fmt.Sprintf("state=%s | lastSwipe=%s %.0fpx | threshold=%.0fpx", active, ta.LastSwipe.String(), ta.LastDistance, ta.MinSwipeDistance)
}

func (ta *TouchArea) sourceString(src pointer.Source) string {
	switch src {
	case pointer.Touch:
		return "touch"
	case pointer.Mouse:
		return "mouse"
	default:
		return "unknown"
	}
}
