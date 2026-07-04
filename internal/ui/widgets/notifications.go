package widgets

import (
	"image"
	"sync"
	"time"

	"gioui.org/layout"
	"gioui.org/op/clip"
	"gioui.org/op/paint"
	"gioui.org/unit"
	"gioui.org/widget/material"
	"github.com/robertpelloni/bqt/internal/ui/theme"
)

type Toast struct {
	Title, Msg string
	Type       string
	Expiry     time.Time
}

type NotificationCenter struct {
	mu     sync.Mutex
	toasts []Toast
}

var (
	notifyInstance *NotificationCenter
	notifyOnce     sync.Once
)

func GetNotificationCenter() *NotificationCenter {
	notifyOnce.Do(func() { notifyInstance = &NotificationCenter{} })
	return notifyInstance
}

func (nc *NotificationCenter) ShowToast(title, msg, tType string) {
	nc.mu.Lock()
	defer nc.mu.Unlock()
	nc.toasts = append(nc.toasts, Toast{Title: title, Msg: msg, Type: tType, Expiry: time.Now().Add(3 * time.Second)})
}

func (nc *NotificationCenter) Layout(gtx layout.Context, th theme.Theme) layout.Dimensions {
	nc.mu.Lock()
	defer nc.mu.Unlock()
	active := make([]Toast, 0, len(nc.toasts))
	for _, t := range nc.toasts {
		if time.Now().Before(t.Expiry) {
			active = append(active, t)
		}
	}
	nc.toasts = active
	if len(nc.toasts) == 0 {
		return layout.Dimensions{}
	}
	mth := material.NewTheme()
	mth.Palette.Fg = th.Text
	mth.Palette.Bg = th.Surface
	rect := image.Rect(0, 0, gtx.Dp(unit.Dp(300)), gtx.Dp(unit.Dp(60)))
	paint.FillShape(gtx.Ops, th.Surface, clip.Rect(rect).Op())
	return material.Label(mth, unit.Sp(12), nc.toasts[0].Title).Layout(gtx)
}
