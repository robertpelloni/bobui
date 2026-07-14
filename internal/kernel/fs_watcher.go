package kernel

import (
	"log"
	"sync"
	"time"
)

// Watcher mirrors QFileSystemWatcher functionality in native Go.
type Watcher struct {
	mu    sync.RWMutex
	files map[string]time.Time
	dirs  map[string]time.Time

	OnChanged chan string
	stop      chan struct{}
}

func NewWatcher() *Watcher {
	return &Watcher{
		files:     make(map[string]time.Time),
		dirs:      make(map[string]time.Time),
		OnChanged: make(chan string, 100),
		stop:      make(chan struct{}),
	}
}

func (w *Watcher) AddPath(path string) {
	w.mu.Lock()
	defer w.mu.Unlock()
	w.files[path] = time.Now()
	log.Printf("BQT WATCHER: Tracking path: %s", path)
}

func (w *Watcher) RemovePath(path string) {
	w.mu.Lock()
	defer w.mu.Unlock()
	delete(w.files, path)
	delete(w.dirs, path)
}

// Start initiates the polling or OS-event loop for changes.
func (w *Watcher) Start(interval time.Duration) {
	go func() {
		ticker := time.NewTicker(interval)
		defer ticker.Stop()
		for {
			select {
			case <-ticker.C:
				w.check()
			case <-w.stop:
				return
			}
		}
	}()
}

func (w *Watcher) Stop() {
	close(w.stop)
}

func (w *Watcher) check() {
	// This is a placeholder for actual OS-level notification (fsnotify).
	// In this baseline, we provide the interface and event routing.
	w.mu.RLock()
	defer w.mu.RUnlock()
	for path := range w.files {
		// Mock change detection for parity demonstration
		select {
		case w.OnChanged <- path:
		default:
		}
	}
}
