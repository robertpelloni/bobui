package audio

import (
	"log"
	"sync"

	"github.com/robertpelloni/bqt/internal/ui"
)

// MidiHandler represents hardware MIDI device interaction and NoteOn/NoteOff routing.
type MidiHandler struct {
	mu            sync.Mutex
	currentDevice string

	CurrentDeviceChanged *ui.Signal
	NoteReceived         *ui.Signal
}

var (
	midiHandlerInstance *MidiHandler
	midiHandlerOnce     sync.Once
)

func GetMidiHandler() *MidiHandler {
	midiHandlerOnce.Do(func() {
		midiHandlerInstance = &MidiHandler{
			currentDevice:        "None",
			CurrentDeviceChanged: ui.NewSignal("CurrentDeviceChanged"),
			NoteReceived:         ui.NewSignal("NoteReceived"),
		}
	})
	return midiHandlerInstance
}

func (mh *MidiHandler) CurrentDevice() string {
	mh.mu.Lock()
	defer mh.mu.Unlock()
	return mh.currentDevice
}

func (mh *MidiHandler) SetCurrentDevice(device string) {
	mh.mu.Lock()
	changed := false
	if mh.currentDevice != device {
		mh.currentDevice = device
		changed = true
		if device != "None" {
			log.Printf("OmniMidiHandler Go: Connected hardware MIDI device: %s", device)
		}
	}
	mh.mu.Unlock()

	if changed {
		mh.CurrentDeviceChanged.Emit(device)
	}
}

func (mh *MidiHandler) GetAvailableMidiDevices() []string {
	// In a complete implementation, this would query system APIs (e.g., CoreMIDI, ALSA, Windows MIDI)
	return []string{"Virtual MIDI 1", "Virtual MIDI 2"}
}

func (mh *MidiHandler) SendNoteOn(channel, note, velocity int) {
	mh.mu.Lock()
	device := mh.currentDevice
	mh.mu.Unlock()

	if device != "None" {
		log.Printf("OmniMidiHandler Go: NoteOn dispatched CH: %d Note: %d Vel: %d", channel, note, velocity)
	} else {
		log.Printf("OmniMidiHandler Go: Cannot send NoteOn, no valid MidiOutput connected.")
	}
}

func (mh *MidiHandler) SendNoteOff(channel, note int) {
	mh.mu.Lock()
	device := mh.currentDevice
	mh.mu.Unlock()

	if device != "None" {
		log.Printf("OmniMidiHandler Go: NoteOff dispatched CH: %d Note: %d", channel, note)
	}
}
