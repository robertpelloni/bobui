package widgets

import (
	"image"

	"gioui.org/io/pointer"
	"gioui.org/layout"
	"gioui.org/op"
	"gioui.org/op/clip"
	"gioui.org/op/paint"
	giowidget "gioui.org/widget"
	"github.com/robertpelloni/bqt/internal/ui/theme"
)

type ScrollOrientation int

const (
	ScrollVertical ScrollOrientation = iota
	ScrollHorizontal
)

// ScrollBar is a lightweight QuickControls2-style scrollbar baseline.
// It now includes interactive click/drag behavior while preserving a small,
// compile-safe BQt surface.
type ScrollBar struct {
	Orientation ScrollOrientation
	Position    float32 // 0..1
	PageSize    float32 // 0..1 visible fraction
	Visible     bool

	state        giowidget.Scrollbar
	Hovered      bool
	Dragging     bool
	TrackHovered bool
	LastDelta    float32
}

func (sb *ScrollBar) SetViewport(position, pageSize float32) {
	sb.Position = scrollClamp01(position)
	if pageSize <= 0 {
		pageSize = 0.2
	}
	sb.PageSize = scrollClamp01(pageSize)
	if sb.PageSize == 0 {
		sb.PageSize = 0.2
	}
}

func (sb *ScrollBar) ScrollDistance() float32 {
	return sb.LastDelta
}

func (sb *ScrollBar) Layout(gtx layout.Context, th theme.Theme) layout.Dimensions {
	if !sb.Visible {
		sb.LastDelta = 0
		sb.Hovered = false
		sb.TrackHovered = false
		sb.Dragging = false
		return layout.Dimensions{}
	}

	sb.SetViewport(sb.Position, sb.PageSize)
	axis := layout.Vertical
	if sb.Orientation == ScrollHorizontal {
		axis = layout.Horizontal
	}
	viewportStart := sb.Position
	viewportEnd := scrollClamp01(sb.Position + sb.PageSize)
	if viewportEnd < viewportStart {
		viewportEnd = viewportStart
	}

	sb.state.Update(gtx, axis, viewportStart, viewportEnd)
	sb.LastDelta = sb.state.ScrollDistance()
	sb.Dragging = sb.state.Dragging()
	sb.TrackHovered = sb.state.TrackHovered()
	sb.Hovered = sb.TrackHovered || sb.state.IndicatorHovered()

	track := image.Rect(0, 0, gtx.Constraints.Max.X, gtx.Constraints.Max.Y)
	if track.Dx() <= 0 || track.Dy() <= 0 {
		return layout.Dimensions{}
	}

	defer clip.Rect(track).Push(gtx.Ops).Pop()
	sb.state.AddDrag(gtx.Ops)
	defer pointer.PassOp{}.Push(gtx.Ops).Pop()
	sb.state.AddTrack(gtx.Ops)

	trackColor := th.Surface
	if sb.TrackHovered {
		trackColor = th.Background
	}
	paint.FillShape(gtx.Ops, trackColor, clip.Rect(track).Op())

	thumb := sb.thumbRect(track)
	thumbColor := th.Primary
	if sb.Hovered {
		thumbColor = th.Accent
	}
	defer op.Offset(thumb.Min).Push(gtx.Ops).Pop()
	thumbLocal := image.Rect(0, 0, thumb.Dx(), thumb.Dy())
	paint.FillShape(gtx.Ops, thumbColor, clip.Rect(thumbLocal).Op())
	defer clip.Rect(thumbLocal).Push(gtx.Ops).Pop()
	sb.state.AddIndicator(gtx.Ops)

	return layout.Dimensions{Size: track.Size()}
}

func (sb *ScrollBar) thumbRect(track image.Rectangle) image.Rectangle {
	if sb.Orientation == ScrollVertical {
		h := track.Dy()
		thumbH := scrollMaxInt(8, int(float32(h)*sb.PageSize))
		maxY := scrollMaxInt(0, h-thumbH)
		y := int(float32(maxY) * sb.Position)
		return image.Rect(0, y, track.Dx(), y+thumbH)
	}
	w := track.Dx()
	thumbW := scrollMaxInt(8, int(float32(w)*sb.PageSize))
	maxX := scrollMaxInt(0, w-thumbW)
	x := int(float32(maxX) * sb.Position)
	return image.Rect(x, 0, x+thumbW, track.Dy())
}

func scrollClamp01(v float32) float32 {
	if v < 0 {
		return 0
	}
	if v > 1 {
		return 1
	}
	return v
}

func scrollMaxInt(a, b int) int {
	if a > b {
		return a
	}
	return b
}
