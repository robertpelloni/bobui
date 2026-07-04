package widgets

import (
	"encoding/json"
	"fmt"
	"strings"

	"gioui.org/layout"
	"gioui.org/unit"
	"gioui.org/widget"
	"gioui.org/widget/material"
	"github.com/robertpelloni/bqt/internal/ui/theme"
)

// ScriptMessage represents a lightweight page-to-host or host-to-page bridge payload.
type ScriptMessage struct {
	ID      string `json:"id"`
	Channel string `json:"channel"`
	Payload string `json:"payload"`
	Kind    string `json:"kind"` // emit | eval | request | reply | error
}

type JSBridgeHandler func(payload string) (string, error)

// JSBridge provides a compile-safe but now executable bridge model for
// WebEngineQuick-style host/page messaging. Messages can be queued and then
// pumped through local handlers, while still preserving a small BQt-facing API.
type JSBridge struct {
	OnMessage func(msg ScriptMessage)
	Handlers  map[string]JSBridgeHandler

	EvalHandler JSBridgeHandler
	Queue       []ScriptMessage
	History     []ScriptMessage

	nextID                 int
	AutoDispatch           bool
	autoDispatchInitialized bool
}

func (b *JSBridge) ensureState() {
	if b.Handlers == nil {
		b.Handlers = make(map[string]JSBridgeHandler)
	}
	if !b.autoDispatchInitialized {
		b.AutoDispatch = true
		b.autoDispatchInitialized = true
	}
}

func (b *JSBridge) nextMessageID() string {
	b.nextID++
	return fmt.Sprintf("msg-%d", b.nextID)
}

func (b *JSBridge) record(msg ScriptMessage) {
	b.History = append(b.History, msg)
	if b.OnMessage != nil {
		b.OnMessage(msg)
	}
}

func (b *JSBridge) QueueMessage(msg ScriptMessage) ScriptMessage {
	b.ensureState()
	if msg.ID == "" {
		msg.ID = b.nextMessageID()
	}
	b.Queue = append(b.Queue, msg)
	b.record(msg)
	return msg
}

func (b *JSBridge) RegisterHandler(channel string, handler JSBridgeHandler) {
	b.ensureState()
	b.Handlers[channel] = handler
}

func (b *JSBridge) RegisterEvalHandler(handler JSBridgeHandler) {
	b.ensureState()
	b.EvalHandler = handler
}

func (b *JSBridge) Emit(channel, payload string) ScriptMessage {
	return b.QueueMessage(ScriptMessage{Channel: channel, Payload: payload, Kind: "emit"})
}

func (b *JSBridge) Eval(source string) ScriptMessage {
	return b.QueueMessage(ScriptMessage{Channel: "eval", Payload: source, Kind: "eval"})
}

func (b *JSBridge) Request(channel, payload string) ScriptMessage {
	return b.QueueMessage(ScriptMessage{Channel: channel, Payload: payload, Kind: "request"})
}

// Dispatch routes a page->host request into a registered handler and returns a reply/error.
func (b *JSBridge) Dispatch(channel, payload string) ScriptMessage {
	b.ensureState()
	return b.DispatchMessage(ScriptMessage{ID: b.nextMessageID(), Channel: channel, Payload: payload, Kind: "request"})
}

func (b *JSBridge) DispatchMessage(msg ScriptMessage) ScriptMessage {
	b.ensureState()
	if msg.ID == "" {
		msg.ID = b.nextMessageID()
	}

	switch msg.Kind {
	case "emit":
		if h, ok := b.Handlers[msg.Channel]; ok {
			_, _ = h(msg.Payload)
		}
		return ScriptMessage{}
	case "eval":
		if b.EvalHandler == nil {
			return ScriptMessage{ID: msg.ID, Channel: msg.Channel, Payload: "no eval handler registered", Kind: "error"}
		}
		result, err := b.EvalHandler(msg.Payload)
		if err != nil {
			return ScriptMessage{ID: msg.ID, Channel: msg.Channel, Payload: err.Error(), Kind: "error"}
		}
		return ScriptMessage{ID: msg.ID, Channel: msg.Channel, Payload: result, Kind: "reply"}
	case "request":
		h, ok := b.Handlers[msg.Channel]
		if !ok {
			return ScriptMessage{ID: msg.ID, Channel: msg.Channel, Payload: "handler not found", Kind: "error"}
		}
		result, err := h(msg.Payload)
		if err != nil {
			return ScriptMessage{ID: msg.ID, Channel: msg.Channel, Payload: err.Error(), Kind: "error"}
		}
		return ScriptMessage{ID: msg.ID, Channel: msg.Channel, Payload: result, Kind: "reply"}
	case "reply", "error":
		return msg
	default:
		return ScriptMessage{ID: msg.ID, Channel: msg.Channel, Payload: "unsupported message kind", Kind: "error"}
	}
}

// Pump processes queued runtime messages and returns any generated replies/errors.
func (b *JSBridge) Pump() []ScriptMessage {
	b.ensureState()
	if len(b.Queue) == 0 {
		return nil
	}

	queued := append([]ScriptMessage(nil), b.Queue...)
	b.Queue = b.Queue[:0]

	var replies []ScriptMessage
	for _, msg := range queued {
		reply := b.DispatchMessage(msg)
		if reply.Kind == "" {
			continue
		}
		b.record(reply)
		replies = append(replies, reply)
	}
	return replies
}

func (b *JSBridge) QueueDepth() int {
	return len(b.Queue)
}

// WebView is a lightweight WebEngineQuick-style navigation model for the verified Go baseline.
// It is intentionally not a full browser engine; it provides URL/history/title/content semantics
// plus an executable bridge/runtime surface the rest of the framework can build on.
type WebView struct {
	URL     string
	Title   string
	HTML    string
	History []string
	Index   int

	BackBtn    widget.Clickable
	ForwardBtn widget.Clickable
	ReloadBtn  widget.Clickable

	OnNavigate       func(url string)
	OnLoad           func(url string)
	OnTitleChanged   func(title string)
	OnHistoryChanged func(index int, length int)
	OnScriptMessage  func(msg ScriptMessage)

	Bridge *JSBridge

	LastEvaluatedJS string
	LastEvalResult  string
	LastPostedMsg   ScriptMessage
	LastReply       ScriptMessage
}

func (w *WebView) ensureBridge() {
	if w.Bridge == nil {
		w.Bridge = &JSBridge{}
	}
	w.Bridge.ensureState()
	w.Bridge.OnMessage = func(msg ScriptMessage) {
		w.LastPostedMsg = msg
		if w.OnScriptMessage != nil {
			w.OnScriptMessage(msg)
		}
	}
}

func (w *WebView) captureReplies(replies []ScriptMessage) {
	for _, reply := range replies {
		w.LastReply = reply
		w.LastPostedMsg = reply
		if reply.Channel == "eval" && reply.Kind == "reply" {
			w.LastEvalResult = reply.Payload
		}
	}
}

func (w *WebView) maybePumpBridge() []ScriptMessage {
	if w.Bridge == nil || !w.Bridge.AutoDispatch {
		return nil
	}
	return w.PumpRuntime()
}

func (w *WebView) SetAutoDispatch(enabled bool) {
	w.ensureBridge()
	w.Bridge.AutoDispatch = enabled
	w.Bridge.autoDispatchInitialized = true
}

func (w *WebView) RegisterHandler(channel string, handler JSBridgeHandler) {
	w.ensureBridge()
	w.Bridge.RegisterHandler(channel, handler)
}

func (w *WebView) RegisterEvalHandler(handler JSBridgeHandler) {
	w.ensureBridge()
	w.Bridge.RegisterEvalHandler(handler)
}

func (w *WebView) PumpRuntime() []ScriptMessage {
	w.ensureBridge()
	replies := w.Bridge.Pump()
	w.captureReplies(replies)
	return replies
}

func (w *WebView) QueueDepth() int {
	w.ensureBridge()
	return w.Bridge.QueueDepth()
}

// HandleScriptMessage routes an externally supplied script message through the local runtime.
func (w *WebView) HandleScriptMessage(msg ScriptMessage) ScriptMessage {
	w.ensureBridge()
	if msg.Kind == "reply" || msg.Kind == "error" {
		if msg.ID == "" {
			msg.ID = w.Bridge.nextMessageID()
		}
		w.Bridge.record(msg)
		w.captureReplies([]ScriptMessage{msg})
		return msg
	}
	queued := w.Bridge.QueueMessage(msg)
	replies := w.maybePumpBridge()
	for _, reply := range replies {
		if reply.ID == queued.ID {
			return reply
		}
	}
	return ScriptMessage{}
}

func (w *WebView) Navigate(url, html string) {
	w.URL = url
	w.Title = url
	w.HTML = html
	if w.Index < len(w.History)-1 {
		w.History = w.History[:w.Index+1]
	}
	w.History = append(w.History, url)
	w.Index = len(w.History) - 1
	if w.OnNavigate != nil {
		w.OnNavigate(url)
	}
	if w.OnLoad != nil {
		w.OnLoad(url)
	}
	if w.OnTitleChanged != nil {
		w.OnTitleChanged(w.Title)
	}
	if w.OnHistoryChanged != nil {
		w.OnHistoryChanged(w.Index, len(w.History))
	}
}

func (w *WebView) Back() {
	if w.Index > 0 {
		w.Index--
		w.URL = w.History[w.Index]
		w.Title = w.URL
		if w.OnNavigate != nil {
			w.OnNavigate(w.URL)
		}
		if w.OnTitleChanged != nil {
			w.OnTitleChanged(w.Title)
		}
		if w.OnHistoryChanged != nil {
			w.OnHistoryChanged(w.Index, len(w.History))
		}
	}
}

func (w *WebView) Forward() {
	if w.Index >= 0 && w.Index < len(w.History)-1 {
		w.Index++
		w.URL = w.History[w.Index]
		w.Title = w.URL
		if w.OnNavigate != nil {
			w.OnNavigate(w.URL)
		}
		if w.OnTitleChanged != nil {
			w.OnTitleChanged(w.Title)
		}
		if w.OnHistoryChanged != nil {
			w.OnHistoryChanged(w.Index, len(w.History))
		}
	}
}

// EvalJS is a host-to-page bridge request that now executes through the local runtime.
func (w *WebView) EvalJS(source string) {
	w.ensureBridge()
	w.LastEvaluatedJS = source
	w.Bridge.Eval(source)
	w.maybePumpBridge()
}

// PostMessage is a page-to-host bridge message that now flows through the local runtime queue.
func (w *WebView) PostMessage(channel, payload string) {
	w.ensureBridge()
	w.Bridge.Emit(channel, payload)
	w.maybePumpBridge()
}

// Request sends a request and routes it through the local runtime bridge, preserving correlation ids.
func (w *WebView) Request(channel, payload string) ScriptMessage {
	w.ensureBridge()
	request := w.Bridge.Request(channel, payload)
	replies := w.maybePumpBridge()
	for _, reply := range replies {
		if reply.ID == request.ID {
			return reply
		}
	}
	return ScriptMessage{ID: request.ID, Channel: channel, Payload: "pending runtime reply", Kind: "request"}
}

// BridgeContractJSON exposes the bridge contract in a tool-friendly format.
func (w *WebView) BridgeContractJSON() string {
	contract := map[string]any{
		"supports": []string{"navigate", "history", "titleChanged", "load", "scriptMessage", "evalJS", "postMessage", "request", "reply", "error", "runtimeQueue", "pumpRuntime", "handleScriptMessage", "registerEvalHandler", "autoDispatch"},
		"message": map[string]string{
			"id":      "optional request/reply correlation id",
			"channel": "logical route name",
			"payload": "string payload",
			"kind":    "emit|eval|request|reply|error",
		},
	}
	data, _ := json.Marshal(contract)
	return string(data)
}

func (w *WebView) runtimeSummary() string {
	w.ensureBridge()
	lastReply := "none"
	if w.LastReply.Kind != "" {
		lastReply = fmt.Sprintf("%s:%s", w.LastReply.Kind, w.LastReply.Channel)
	}
	lastEval := w.LastEvalResult
	if lastEval == "" {
		lastEval = "none"
	}
	return fmt.Sprintf("queue=%d auto=%v lastReply=%s lastEval=%s", w.QueueDepth(), w.Bridge.AutoDispatch, lastReply, lastEval)
}

func (w *WebView) Layout(gtx layout.Context, th theme.Theme) layout.Dimensions {
	w.ensureBridge()

	mth := material.NewTheme()
	mth.Palette.Fg = th.Text
	mth.Palette.Bg = th.Surface

	if w.BackBtn.Clicked(gtx) {
		w.Back()
	}
	if w.ForwardBtn.Clicked(gtx) {
		w.Forward()
	}
	if w.ReloadBtn.Clicked(gtx) {
		if w.OnLoad != nil {
			w.OnLoad(w.URL)
		}
	}

	lines := strings.Split(w.HTML, "<br>")
	list := layout.List{Axis: layout.Vertical}

	return layout.Flex{Axis: layout.Vertical}.Layout(gtx,
		layout.Rigid(func(gtx layout.Context) layout.Dimensions {
			return layout.Flex{Axis: layout.Horizontal}.Layout(gtx,
				layout.Rigid(material.Button(mth, &w.BackBtn, "<").Layout),
				layout.Rigid(material.Button(mth, &w.ForwardBtn, ">").Layout),
				layout.Rigid(material.Button(mth, &w.ReloadBtn, "Reload").Layout),
				layout.Flexed(1, func(gtx layout.Context) layout.Dimensions {
					return material.Label(mth, unit.Sp(10), w.URL).Layout(gtx)
				}),
			)
		}),
		layout.Rigid(func(gtx layout.Context) layout.Dimensions {
			return material.H6(mth, w.Title).Layout(gtx)
		}),
		layout.Rigid(func(gtx layout.Context) layout.Dimensions {
			return material.Caption(mth, w.runtimeSummary()).Layout(gtx)
		}),
		layout.Rigid(func(gtx layout.Context) layout.Dimensions {
			return material.Caption(mth, w.BridgeContractJSON()).Layout(gtx)
		}),
		layout.Flexed(1, func(gtx layout.Context) layout.Dimensions {
			return list.Layout(gtx, len(lines), func(gtx layout.Context, i int) layout.Dimensions {
				return material.Label(mth, unit.Sp(11), lines[i]).Layout(gtx)
			})
		}),
	)
}
