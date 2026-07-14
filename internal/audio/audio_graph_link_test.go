package audio

import (
	"testing"
)

func TestAudioGraph_StartStop(t *testing.T) {
	ag := GetAudioGraph()

	if ag.isRunning {
		t.Errorf("Expected AudioGraph to initially not be running")
	}

	ag.Start()
	if !ag.isRunning {
		t.Errorf("Expected AudioGraph to be running after Start()")
	}

	// Double start should be a no-op
	ag.Start()
	if !ag.isRunning {
		t.Errorf("Expected AudioGraph to remain running")
	}

	ag.Stop()
	if ag.isRunning {
		t.Errorf("Expected AudioGraph to not be running after Stop()")
	}

	// Double stop should be a no-op
	ag.Stop()
	if ag.isRunning {
		t.Errorf("Expected AudioGraph to remain not running")
	}
}
