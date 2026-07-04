package widgets

import (
	"image"
	"math"

	"gioui.org/layout"
	"gioui.org/op"
	giowidget "gioui.org/widget"
	"github.com/robertpelloni/bqt/internal/ui/theme"
)

type ScrollPolicy int

const (
	ScrollAsNeeded ScrollPolicy = iota
	ScrollAlwaysOn
	ScrollAlwaysOff
)

type ScrollBarPlacement int

const (
	ScrollBarOverlay ScrollBarPlacement = iota
	ScrollBarOccupy
)

// ScrollView is a minimal QuickControls2-style scroll container baseline.
// It now supports overlay/occupy scrollbar placement and a lightweight
// two-axis model where the primary axis is list-driven and the secondary axis
// maintains its own normalized viewport state.
type ScrollView struct {
	List               giowidget.List
	VerticalPolicy     ScrollPolicy
	HorizontalPolicy   ScrollPolicy
	VerticalPlacement  ScrollBarPlacement
	HorizontalPlacement ScrollBarPlacement
	BarThickness       int

	VerticalBar        ScrollBar
	HorizontalBar      ScrollBar
	ApproxContentItems int
	LastScrollDelta    float32
	LastCrossDelta     float32
}

func NewScrollView(axis layout.Axis) *ScrollView {
	return &ScrollView{
		List:                giowidget.List{List: layout.List{Axis: axis}},
		VerticalPolicy:      ScrollAsNeeded,
		HorizontalPolicy:    ScrollAlwaysOff,
		VerticalPlacement:   ScrollBarOverlay,
		HorizontalPlacement: ScrollBarOverlay,
		VerticalBar:         ScrollBar{Orientation: ScrollVertical},
		HorizontalBar:       ScrollBar{Orientation: ScrollHorizontal},
	}
}

func (sv *ScrollView) Layout(gtx layout.Context, count int, child func(layout.Context, int) layout.Dimensions) layout.Dimensions {
	return sv.layoutWithTheme(gtx, theme.GetTheme(theme.Cyberpunk), count, child)
}

func (sv *ScrollView) LayoutWithTheme(gtx layout.Context, th theme.Theme, count int, child func(layout.Context, int) layout.Dimensions) layout.Dimensions {
	return sv.layoutWithTheme(gtx, th, count, child)
}

func (sv *ScrollView) SetCrossAxisViewport(position, pageSize float32) {
	bar := sv.secondaryBar()
	bar.SetViewport(position, pageSize)
}

func (sv *ScrollView) layoutWithTheme(gtx layout.Context, th theme.Theme, count int, child func(layout.Context, int) layout.Dimensions) layout.Dimensions {
	sv.ApproxContentItems = count
	if sv.BarThickness <= 0 {
		sv.BarThickness = gtx.Dp(12)
	}

	orig := gtx.Constraints
	listDims := sv.layoutContent(gtx, count, child, 0, 0)
	sv.syncPrimaryFromList(listDims.Size, count)
	sv.syncSecondaryVisibility()

	occupiedW, occupiedH := sv.occupiedSpace()
	if occupiedW > 0 || occupiedH > 0 {
		gtx.Constraints = orig
		listDims = sv.layoutContent(gtx, count, child, occupiedW, occupiedH)
		sv.syncPrimaryFromList(listDims.Size, count)
		sv.syncSecondaryVisibility()
	}

	gtx.Constraints = orig
	fullSize := image.Pt(listDims.Size.X+occupiedW, listDims.Size.Y+occupiedH)
	if fullSize.X < 0 {
		fullSize.X = 0
	}
	if fullSize.Y < 0 {
		fullSize.Y = 0
	}

	contentRect := image.Rect(0, 0, listDims.Size.X, listDims.Size.Y)

	barGtx := gtx
	barGtx.Constraints.Min = fullSize
	barGtx.Constraints.Max = fullSize

	sv.layoutBars(barGtx, th, contentRect, fullSize)
	sv.applyScrollDeltas(count)

	return layout.Dimensions{Size: fullSize, Baseline: listDims.Baseline}
}

func (sv *ScrollView) layoutContent(gtx layout.Context, count int, child func(layout.Context, int) layout.Dimensions, occupyWidth, occupyHeight int) layout.Dimensions {
	max := gtx.Constraints.Max
	min := gtx.Constraints.Min
	max.X = scrollMaxInt(0, max.X-occupyWidth)
	max.Y = scrollMaxInt(0, max.Y-occupyHeight)
	min.X = scrollMaxInt(0, min.X-occupyWidth)
	min.Y = scrollMaxInt(0, min.Y-occupyHeight)
	gtx.Constraints.Max = max
	gtx.Constraints.Min = min
	return sv.List.Layout(gtx, count, child)
}

func (sv *ScrollView) syncPrimaryFromList(size image.Point, count int) {
	bar := sv.primaryBar()
	policy := sv.primaryPolicy()
	majorAxisSize := sv.List.Axis.Convert(size).X
	start, end := scrollRangeFromListPosition(sv.List.Position, count, majorAxisSize)
	scrollable := scrollRangeIsScrollable(start, end)
	bar.Visible = scrollPolicyVisible(policy, scrollable)
	bar.SetViewport(start, end-start)
}

func (sv *ScrollView) syncSecondaryVisibility() {
	bar := sv.secondaryBar()
	policy := sv.secondaryPolicy()
	scrollable := bar.PageSize > 0 && bar.PageSize < 1
	bar.Visible = scrollPolicyVisible(policy, scrollable)
}

func (sv *ScrollView) occupiedSpace() (width, height int) {
	if sv.VerticalBar.Visible && sv.VerticalPlacement == ScrollBarOccupy {
		width += sv.BarThickness
	}
	if sv.HorizontalBar.Visible && sv.HorizontalPlacement == ScrollBarOccupy {
		height += sv.BarThickness
	}
	return width, height
}

func (sv *ScrollView) layoutBars(gtx layout.Context, th theme.Theme, contentRect image.Rectangle, fullSize image.Point) {
	if sv.VerticalBar.Visible {
		barRect := image.Rect(contentRect.Max.X, 0, fullSize.X, contentRect.Max.Y)
		if sv.VerticalPlacement == ScrollBarOverlay {
			barRect = image.Rect(scrollMaxInt(0, contentRect.Max.X-sv.BarThickness), 0, contentRect.Max.X, contentRect.Max.Y)
		}
		if barRect.Dx() > 0 && barRect.Dy() > 0 {
			barGtx := gtx
			barGtx.Constraints.Min = barRect.Size()
			barGtx.Constraints.Max = barRect.Size()
			stack := op.Offset(barRect.Min).Push(gtx.Ops)
			sv.VerticalBar.Layout(barGtx, th)
			stack.Pop()
		}
	}
	if sv.HorizontalBar.Visible {
		barRect := image.Rect(0, contentRect.Max.Y, contentRect.Max.X, fullSize.Y)
		if sv.HorizontalPlacement == ScrollBarOverlay {
			barRect = image.Rect(0, scrollMaxInt(0, contentRect.Max.Y-sv.BarThickness), contentRect.Max.X, contentRect.Max.Y)
		}
		if barRect.Dx() > 0 && barRect.Dy() > 0 {
			barGtx := gtx
			barGtx.Constraints.Min = barRect.Size()
			barGtx.Constraints.Max = barRect.Size()
			stack := op.Offset(barRect.Min).Push(gtx.Ops)
			sv.HorizontalBar.Layout(barGtx, th)
			stack.Pop()
		}
	}
}

func (sv *ScrollView) applyScrollDeltas(count int) {
	primary := sv.primaryBar()
	sv.LastScrollDelta = primary.ScrollDistance()
	if sv.LastScrollDelta != 0 && count > 0 {
		sv.List.ScrollBy(sv.LastScrollDelta * float32(count))
	}

	secondary := sv.secondaryBar()
	sv.LastCrossDelta = secondary.ScrollDistance()
	if sv.LastCrossDelta != 0 {
		newPos := scrollClamp01(secondary.Position + sv.LastCrossDelta)
		if secondary.PageSize > 0 && secondary.PageSize < 1 {
			maxPos := scrollClamp01(1 - secondary.PageSize)
			if newPos > maxPos {
				newPos = maxPos
			}
		}
		secondary.Position = newPos
	}
}

func (sv *ScrollView) primaryBar() *ScrollBar {
	if sv.List.Axis == layout.Horizontal {
		return &sv.HorizontalBar
	}
	return &sv.VerticalBar
}

func (sv *ScrollView) secondaryBar() *ScrollBar {
	if sv.List.Axis == layout.Horizontal {
		return &sv.VerticalBar
	}
	return &sv.HorizontalBar
}

func (sv *ScrollView) primaryPolicy() ScrollPolicy {
	if sv.List.Axis == layout.Horizontal {
		return sv.HorizontalPolicy
	}
	return sv.VerticalPolicy
}

func (sv *ScrollView) secondaryPolicy() ScrollPolicy {
	if sv.List.Axis == layout.Horizontal {
		return sv.VerticalPolicy
	}
	return sv.HorizontalPolicy
}

func scrollPolicyVisible(policy ScrollPolicy, scrollable bool) bool {
	switch policy {
	case ScrollAlwaysOn:
		return true
	case ScrollAlwaysOff:
		return false
	default:
		return scrollable
	}
}

func scrollRangeIsScrollable(start, end float32) bool {
	return end-start < 1
}

func scrollRangeFromListPosition(pos layout.Position, elements int, majorAxisSize int) (start, end float32) {
	if elements <= 0 || majorAxisSize <= 0 || pos.Length <= 0 {
		return 0, 1
	}
	lengthEstPx := float32(pos.Length)
	elementLenEstPx := lengthEstPx / float32(elements)
	listOffsetF := float32(pos.Offset)
	listOffsetL := float32(pos.OffsetLast)

	viewportStart := scrollClamp01((float32(pos.First)*elementLenEstPx + listOffsetF) / lengthEstPx)
	viewportEnd := scrollClamp01((float32(pos.First+pos.Count)*elementLenEstPx + listOffsetL) / lengthEstPx)
	viewportFraction := viewportEnd - viewportStart
	visiblePx := float32(majorAxisSize)
	visibleFraction := visiblePx / lengthEstPx
	err := visibleFraction - viewportFraction
	adjStart := viewportStart
	adjEnd := viewportEnd
	if viewportFraction < 1 {
		startShare := viewportStart / (1 - viewportFraction)
		endShare := (1 - viewportEnd) / (1 - viewportFraction)
		adjStart -= startShare * err
		adjEnd += endShare * err
	}
	adjStart = scrollClamp01(adjStart)
	adjEnd = scrollClamp01(adjEnd)
	if adjEnd < adjStart {
		adjEnd = adjStart
	}
	if math.Abs(float64(adjEnd-adjStart)) < 0.0001 {
		adjEnd = scrollClamp01(adjStart + 0.1)
	}
	return adjStart, adjEnd
}
