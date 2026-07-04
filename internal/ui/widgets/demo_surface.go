package widgets

import (
	"gioui.org/layout"
	"gioui.org/unit"
	"gioui.org/widget"
	"gioui.org/widget/material"
	"github.com/robertpelloni/bqt/internal/ui/theme"
)

type DemoSurface struct {
	WebView                                                   WebView
	btn1, btn2, btn3                                          widget.Clickable
	btn4, btn5, btn6                                          widget.Clickable // Audio, Synth, Event Loop
	btnUndo, btnClipboard, btnTimeMachine, btnSearch, btnMesh widget.Clickable // Backend features
	dialog                                                    Dialog
	popup                                                     Popup
	drawer                                                    Drawer
	menu                                                      Menu
	tooltip                                                   ToolTip
}

func (ds *DemoSurface) Layout(gtx layout.Context, th theme.Theme) layout.Dimensions {
	mth := material.NewTheme()
	mth.Palette.Fg = th.Text
	mth.Palette.Bg = th.Primary

	if ds.btn1.Clicked(gtx) {
		ds.dialog = Dialog{Visible: true, Title: "BQt Dialog", Body: "This is a native dialog."}
	}
	if ds.btn2.Clicked(gtx) {
		ds.popup = Popup{Visible: true, Title: "BQt Popup", Body: "This is a transient popup."}
	}
	if ds.btn3.Clicked(gtx) {
		ds.drawer = Drawer{Visible: true, Title: "Navigation Drawer", WidthDp: 250}
	}

	if ds.btn4.Clicked(gtx) {
		ds.popup = Popup{Visible: true, Title: "Audio Graph", Body: "OmniAudioGraph processing active."}
	}
	if ds.btn5.Clicked(gtx) {
		ds.popup = Popup{Visible: true, Title: "Synthesizer", Body: "OmniSynthesizer node added."}
	}
	if ds.btn6.Clicked(gtx) {
		ds.popup = Popup{Visible: true, Title: "Event Loop", Body: "Unified Event Loop syncing tasks."}
	}
	if ds.btnUndo.Clicked(gtx) {
		ds.popup = Popup{Visible: true, Title: "Undo Stack", Body: "Multi-user history ledger reversed."}
	}
	if ds.btnClipboard.Clicked(gtx) {
		ds.popup = Popup{Visible: true, Title: "Clipboard Sync", Body: "P2P Clipboard shared memory updated."}
	}
	if ds.btnTimeMachine.Clicked(gtx) {
		ds.popup = Popup{Visible: true, Title: "Time Machine", Body: "Ledger commit snapshot captured."}
	}
	if ds.btnSearch.Clicked(gtx) {
		ds.popup = Popup{Visible: true, Title: "Omni Search", Body: "Global mesh query executed."}
	}
	if ds.btnMesh.Clicked(gtx) {
		ds.popup = Popup{Visible: true, Title: "Mesh Network", Body: "Peer-to-peer data sync established."}
	}

	if ds.btn3.Hovered() {
		ds.tooltip = ToolTip{Visible: true, Text: "Click to open drawer"}
		ds.tooltip.X = 150
		ds.tooltip.Y = 150
	} else if ds.btn4.Hovered() {
		ds.tooltip = ToolTip{Visible: true, Text: "Initialize OmniAudioGraph / JUCE Adapter"}
		ds.tooltip.X = 150
		ds.tooltip.Y = 200
	} else if ds.btn5.Hovered() {
		ds.tooltip = ToolTip{Visible: true, Text: "Initialize native Go OmniSynthesizer"}
		ds.tooltip.X = 150
		ds.tooltip.Y = 250
	} else if ds.btn6.Hovered() {
		ds.tooltip = ToolTip{Visible: true, Text: "Monitor BQt Unified Event Loop"}
		ds.tooltip.X = 150
		ds.tooltip.Y = 300
	} else if ds.btnUndo.Hovered() {
		ds.tooltip = ToolTip{Visible: true, Text: "Trigger kernel UndoStack ledger operation"}
		ds.tooltip.X = 150
		ds.tooltip.Y = 350
	} else if ds.btnClipboard.Hovered() {
		ds.tooltip = ToolTip{Visible: true, Text: "Trigger distributed Clipboard synchronization"}
		ds.tooltip.X = 150
		ds.tooltip.Y = 400
	} else if ds.btnTimeMachine.Hovered() {
		ds.tooltip = ToolTip{Visible: true, Text: "Trigger TimeMachine local snapshot commit"}
		ds.tooltip.X = 150
		ds.tooltip.Y = 450
	} else if ds.btnSearch.Hovered() {
		ds.tooltip = ToolTip{Visible: true, Text: "Execute GlobalSearch across Go peer mesh"}
		ds.tooltip.X = 150
		ds.tooltip.Y = 500
	} else if ds.btnMesh.Hovered() {
		ds.tooltip = ToolTip{Visible: true, Text: "Initialize MeshNode telemetry & P2P"}
		ds.tooltip.X = 150
		ds.tooltip.Y = 550
	} else {
		ds.tooltip.Visible = false
	}

	// Init menu items if needed
	if len(ds.menu.Items) == 0 {
		ds.menu.Items = []*MenuItem{
			{Label: "File", Action: "file"},
			{Label: "Edit", Action: "edit"},
		}
	}

	return layout.Stack{}.Layout(gtx,
		layout.Expanded(func(gtx layout.Context) layout.Dimensions {
			return layout.UniformInset(unit.Dp(16)).Layout(gtx, func(gtx layout.Context) layout.Dimensions {
				return layout.Flex{Axis: layout.Vertical, Spacing: layout.SpaceEvenly}.Layout(gtx,
					layout.Rigid(func(gtx layout.Context) layout.Dimensions {
						return layout.Flex{Axis: layout.Horizontal, Spacing: layout.SpaceEvenly}.Layout(gtx,
							layout.Rigid(material.Button(mth, &ds.btn1, "Show Dialog").Layout),
							layout.Rigid(material.Button(mth, &ds.btn2, "Show Popup").Layout),
							layout.Rigid(material.Button(mth, &ds.btn3, "Show Drawer").Layout),
						)
					}),
					layout.Rigid(func(gtx layout.Context) layout.Dimensions {
						return layout.Flex{Axis: layout.Horizontal, Spacing: layout.SpaceEvenly}.Layout(gtx,
							layout.Rigid(material.Button(mth, &ds.btn4, "OmniAudioGraph").Layout),
							layout.Rigid(material.Button(mth, &ds.btn5, "OmniSynthesizer").Layout),
							layout.Rigid(material.Button(mth, &ds.btn6, "Event Loop").Layout),
						)
					}),
					layout.Rigid(func(gtx layout.Context) layout.Dimensions {
						return layout.Flex{Axis: layout.Horizontal, Spacing: layout.SpaceEvenly}.Layout(gtx,
							layout.Rigid(material.Button(mth, &ds.btnUndo, "Undo Stack").Layout),
							layout.Rigid(material.Button(mth, &ds.btnClipboard, "Clipboard Sync").Layout),
							layout.Rigid(material.Button(mth, &ds.btnTimeMachine, "Time Machine").Layout),
						)
					}),
					layout.Rigid(func(gtx layout.Context) layout.Dimensions {
						return layout.Flex{Axis: layout.Horizontal, Spacing: layout.SpaceEvenly}.Layout(gtx,
							layout.Rigid(material.Button(mth, &ds.btnSearch, "Global Search").Layout),
							layout.Rigid(material.Button(mth, &ds.btnMesh, "Mesh Network").Layout),
						)
					}),
				)
			})
		}),
		layout.Expanded(func(gtx layout.Context) layout.Dimensions {
			dims, _ := ds.dialog.Layout(gtx, th)
			return dims
		}),
		layout.Expanded(func(gtx layout.Context) layout.Dimensions {
			return ds.popup.Layout(gtx, th)
		}),
		layout.Expanded(func(gtx layout.Context) layout.Dimensions {
			// dummy child for drawer
			dummyChild := func(gtx layout.Context) layout.Dimensions {
				return material.Body1(mth, "Drawer Content").Layout(gtx)
			}
			return ds.drawer.Layout(gtx, th, dummyChild)
		}),
		layout.Expanded(func(gtx layout.Context) layout.Dimensions {
			dims, _ := ds.menu.Layout(gtx, th)
			return dims
		}),
		layout.Expanded(func(gtx layout.Context) layout.Dimensions {
			return ds.tooltip.Layout(gtx, th)
		}),
	)
}

func NewDemoSurface() *DemoSurface {
	return &DemoSurface{}
}
