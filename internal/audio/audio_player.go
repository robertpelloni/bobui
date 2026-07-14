package audio

import (
	"log"
	"sync"

	"github.com/robertpelloni/bqt/internal/ui"
)

type AudioPlayer struct {
	mu        sync.Mutex
	source    string
	isPlaying bool

	SourceChanged    *ui.Signal
	IsPlayingChanged *ui.Signal
}

var (
	playerInstance *AudioPlayer
	playerOnce     sync.Once
)

func GetAudioPlayer() *AudioPlayer {
	playerOnce.Do(func() {
		playerInstance = &AudioPlayer{
			SourceChanged:    ui.NewSignal("SourceChanged"),
			IsPlayingChanged: ui.NewSignal("IsPlayingChanged"),
		}
	})
	return playerInstance
}

func (ap *AudioPlayer) Source() string {
	ap.mu.Lock()
	defer ap.mu.Unlock()
	return ap.source
}

func (ap *AudioPlayer) SetSource(path string) {
	ap.mu.Lock()
	changed := false
	if ap.source != path {
		ap.source = path
		changed = true
		log.Printf("OmniAudioPlayer Go: Loaded audio source natively -> %s", path)
	}
	ap.mu.Unlock()

	if changed {
		ap.SourceChanged.Emit(path)
	}
}

func (ap *AudioPlayer) IsPlaying() bool {
	ap.mu.Lock()
	defer ap.mu.Unlock()
	return ap.isPlaying
}

func (ap *AudioPlayer) Play() {
	ap.mu.Lock()
	if !ap.isPlaying {
		ap.isPlaying = true
		ap.mu.Unlock()
		log.Println("OmniAudioPlayer Go: Streaming audio natively.")
		ap.IsPlayingChanged.Emit(true)
	} else {
		ap.mu.Unlock()
	}
}

func (ap *AudioPlayer) Stop() {
	ap.mu.Lock()
	if ap.isPlaying {
		ap.isPlaying = false
		ap.mu.Unlock()
		log.Println("OmniAudioPlayer Go: Audio stream stopped.")
		ap.IsPlayingChanged.Emit(false)
	} else {
		ap.mu.Unlock()
	}
}

func (ap *AudioPlayer) Process(buffer []float32) {
	ap.mu.Lock()
	defer ap.mu.Unlock()
	if !ap.isPlaying {
		return
	}
	// Processing logic goes here in future when reading from file
}

func (ap *AudioPlayer) Name() string {
	return "OmniAudioPlayer"
}
