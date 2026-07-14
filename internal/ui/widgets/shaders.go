package widgets

import (
	"image"
	"image/color"
	"math"

	"gioui.org/layout"
	"gioui.org/op/paint"
	"github.com/robertpelloni/bqt/internal/ui/theme"
)

type ShaderEffect struct {
	Type string
	Time float64
}

func (s *ShaderEffect) Layout(gtx layout.Context, th theme.Theme) layout.Dimensions {
	w, h := max(1, gtx.Constraints.Max.X/4), max(1, gtx.Constraints.Max.Y/4)
	img := image.NewRGBA(image.Rect(0, 0, w, h))

	for y := 0; y < h; y++ {
		for x := 0; x < w; x++ {
			if s.Type == "plasma" {
				val := math.Sin(float64(x)/10.0+s.Time) + math.Cos(float64(y)/10.0+s.Time)
				cVal := uint8((val + 2.0) / 4.0 * 255)
				img.Set(x, y, color.NRGBA{R: 0, G: cVal, B: 255, A: 255})
			} else {
				if x%20 == 0 || y%20 == 0 {
					img.Set(x, y, th.Primary)
				}
			}
		}
	}

	paint.NewImageOp(img).Add(gtx.Ops)
	paint.PaintOp{}.Add(gtx.Ops)
	return layout.Dimensions{Size: gtx.Constraints.Max}
}
