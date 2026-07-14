package widgets

import (
	"fmt"
	"strings"
	"time"

	"gioui.org/layout"
	"gioui.org/unit"
	"gioui.org/widget/material"
	"github.com/robertpelloni/bqt/internal/ui/theme"
)

type Calendar struct {
	CurrentDate time.Time
}

func (c *Calendar) Layout(gtx layout.Context, th theme.Theme) layout.Dimensions {
	if c.CurrentDate.IsZero() {
		c.CurrentDate = time.Now()
	}

	mth := material.NewTheme()
	mth.Palette.Fg = th.Text
	mth.Palette.Bg = th.Surface

	now := c.CurrentDate
	first := time.Date(now.Year(), now.Month(), 1, 0, 0, 0, 0, time.Local)
	startDay := int(first.Weekday())
	daysInMonth := time.Date(now.Year(), now.Month()+1, 0, 0, 0, 0, 0, time.Local).Day()

	rows := make([]string, 0, 6)
	day := 1 - startDay
	for r := 0; r < 6; r++ {
		parts := make([]string, 0, 7)
		for col := 0; col < 7; col++ {
			if day > 0 && day <= daysInMonth {
				parts = append(parts, fmt.Sprintf("%2d", day))
			} else {
				parts = append(parts, "  ")
			}
			day++
		}
		rows = append(rows, strings.Join(parts, " "))
	}

	return layout.Flex{Axis: layout.Vertical}.Layout(gtx,
		layout.Rigid(material.H6(mth, fmt.Sprintf("%s %d", now.Month(), now.Year())).Layout),
		layout.Rigid(layout.Spacer{Height: unit.Dp(8)}.Layout),
		layout.Rigid(material.Label(mth, unit.Sp(10), "S  M  T  W  T  F  S").Layout),
		layout.Rigid(layout.Spacer{Height: unit.Dp(6)}.Layout),
		layout.Rigid(material.Label(mth, unit.Sp(10), rows[0]).Layout),
		layout.Rigid(material.Label(mth, unit.Sp(10), rows[1]).Layout),
		layout.Rigid(material.Label(mth, unit.Sp(10), rows[2]).Layout),
		layout.Rigid(material.Label(mth, unit.Sp(10), rows[3]).Layout),
		layout.Rigid(material.Label(mth, unit.Sp(10), rows[4]).Layout),
		layout.Rigid(material.Label(mth, unit.Sp(10), rows[5]).Layout),
	)
}
