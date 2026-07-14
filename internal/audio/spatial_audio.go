package audio

import (
	"log"
	"sync"
	"github.com/robertpelloni/bqt/internal/ui"
)

type SpatialAudio struct {
	mu           sync.Mutex
	screenWidth  float64
	screenHeight float64

	PlayClick        *ui.Signal
	PlayNotification *ui.Signal
}

var (
	spatialInstance *SpatialAudio
	spatialOnce     sync.Once
)

func GetSpatialAudio() *SpatialAudio {
	spatialOnce.Do(func() {
		spatialInstance = &SpatialAudio{
			screenWidth:      1920.0, // Default fallback
			screenHeight:     1080.0,
			PlayClick:        ui.NewSignal("PlaySpatialClick"),
			PlayNotification: ui.NewSignal("PlaySpatialNotification"),
		}
	})
	return spatialInstance
}

func (sa *SpatialAudio) SetScreenDimensions(width, height float64) {
	sa.mu.Lock()
	defer sa.mu.Unlock()
	sa.screenWidth = width
	sa.screenHeight = height
}

func (sa *SpatialAudio) CalculatePan(x, y float64) float32 {
	sa.mu.Lock()
	defer sa.mu.Unlock()

	if sa.screenWidth <= 0 {
		return 0.0
	}
	// Map 0 -> -1.0 (Left), screenWidth -> 1.0 (Right)
	return float32((x / sa.screenWidth) * 2.0 - 1.0)
}

func (sa *SpatialAudio) PlaySpatialClick(deviceId string, x, y float64, volume float64) {
	pan := sa.CalculatePan(x, y)
	log.Printf("OmniSpatialAudio Go: Playing Click for %s at Pan: %f", deviceId, pan)

	// Dispatch via unified EventLoop
	sa.PlayClick.Emit(deviceId, pan, volume)
}

func (sa *SpatialAudio) PlaySpatialNotification(deviceId string, x, y float64, soundPath string) {
	pan := sa.CalculatePan(x, y)
	log.Printf("OmniSpatialAudio Go: Playing Notification %s for %s at Pan: %f", soundPath, deviceId, pan)

	// Dispatch via unified EventLoop
	sa.PlayNotification.Emit(deviceId, soundPath, pan)
}
