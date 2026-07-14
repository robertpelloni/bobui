package widgets

import (
	"gioui.org/layout"
	"github.com/robertpelloni/bqt/internal/ui/theme"
)

type OmniStudio struct {
	Editor    *CodeEditor
	Terminal  *Terminal
	Explorer  *MeshExplorer
	SplitView *SplitView
}

func NewOmniStudio() *OmniStudio {
	return &OmniStudio{
		Editor:    &CodeEditor{},
		Terminal:  &Terminal{},
		Explorer:  &MeshExplorer{},
		SplitView: &SplitView{Ratio: 0.7},
	}
}

func (os *OmniStudio) Layout(gtx layout.Context, th theme.Theme) layout.Dimensions {
	if os.SplitView == nil {
		os.SplitView = &SplitView{Ratio: 0.7}
	}
	if os.Editor == nil {
		os.Editor = &CodeEditor{}
	}
	if os.Terminal == nil {
		os.Terminal = &Terminal{}
	}
	if os.Explorer == nil {
		os.Explorer = &MeshExplorer{}
	}

	return os.SplitView.Layout(gtx,
		func(gtx layout.Context) layout.Dimensions {
			return layout.Flex{Axis: layout.Horizontal}.Layout(gtx,
				layout.Rigid(func(gtx layout.Context) layout.Dimensions {
					return os.Explorer.Layout(gtx, th)
				}),
				layout.Flexed(1, func(gtx layout.Context) layout.Dimensions {
					return os.Editor.Layout(gtx, th)
				}),
			)
		},
		func(gtx layout.Context) layout.Dimensions {
			return os.Terminal.Layout(gtx, th)
		},
	)
}
