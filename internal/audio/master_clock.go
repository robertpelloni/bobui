package audio

import (
	"log"
	"sync"
	"time"
	"github.com/robertpelloni/bqt/internal/ui"
)

type MasterClock struct {
	mu               sync.Mutex
	BPM              float64
	IsRunning        bool
	TickChan         chan int
	stopSignal       chan struct{}

	BpmChanged       *ui.Signal
	IsRunningChanged *ui.Signal
	Beat             *ui.Signal
	SixteenthStep    *ui.Signal
}

var (
	clockInstance *MasterClock
	clockOnce     sync.Once
)

func GetMasterClock() *MasterClock {
	clockOnce.Do(func() {
		clockInstance = &MasterClock{
			BPM:              120.0,
			TickChan:         make(chan int, 100),
			BpmChanged:       ui.NewSignal("BpmChanged"),
			IsRunningChanged: ui.NewSignal("IsRunningChanged"),
			Beat:             ui.NewSignal("Beat"),
			SixteenthStep:    ui.NewSignal("SixteenthStep"),
		}
	})
	return clockInstance
}

func (mc *MasterClock) SetBpm(bpm float64) {
	mc.mu.Lock()
	changed := false
	if mc.BPM != bpm {
		mc.BPM = bpm
		changed = true
	}
	mc.mu.Unlock()

	if changed {
		mc.BpmChanged.Emit(bpm)
	}
}

func (mc *MasterClock) Reset() {
	mc.mu.Lock()
	defer mc.mu.Unlock()
	// Reset timing state if necessary in the future
}

// Start begins the Go-native high-priority timing loop.
func (mc *MasterClock) Start() {
	mc.mu.Lock()
	if mc.IsRunning { mc.mu.Unlock(); return }
	mc.IsRunning = true
	mc.stopSignal = make(chan struct{})
	mc.mu.Unlock()

	mc.IsRunningChanged.Emit(true)

	go func() {
		mc.mu.Lock()
		currentBpm := mc.BPM
		mc.mu.Unlock()

		// Calculate 16th note interval
		interval := time.Duration(float64(time.Minute) / (currentBpm * 4))
		ticker := time.NewTicker(interval)
		defer ticker.Stop()

		step := 0
		beatCount := 0
		log.Printf("OmniClock Go: Temporal Heartbeat started at %f BPM.", currentBpm)

		for {
			select {
			case <-ticker.C:
				select {
				case mc.TickChan <- step:
				default: // Channel full, skip to maintain real-time parity
				}

				// Emit 16th step signal via EventLoop
				mc.SixteenthStep.Emit(step)

				// Emit beat signal every 4 steps (quarter note)
				if step%4 == 0 {
					mc.Beat.Emit(beatCount)
					beatCount++
				}

				step = (step + 1) % 16

				// Dynamic interval update (if BPM changed)
				mc.mu.Lock()
				newBpm := mc.BPM
				mc.mu.Unlock()
				if newBpm != currentBpm {
					currentBpm = newBpm
					interval = time.Duration(float64(time.Minute) / (currentBpm * 4))
					ticker.Reset(interval)
				}

			case <-mc.stopSignal:
				return
			}
		}
	}()
}

func (mc *MasterClock) Stop() {
	mc.mu.Lock()
	if mc.IsRunning {
		close(mc.stopSignal)
		mc.IsRunning = false
		mc.mu.Unlock()
		log.Println("OmniClock Go: Temporal Heartbeat stopped.")
		mc.IsRunningChanged.Emit(false)
	} else {
		mc.mu.Unlock()
	}
}
