package kernel

import (
	"runtime"
	"testing"
	"time"
)

func TestProcessBasic(t *testing.T) {
	p := NewProcess()
	if runtime.GOOS == "windows" {
		p.Program = "cmd.exe"
		p.Arguments = []string{"/c", "echo hello"}
	} else {
		p.Program = "echo"
		p.Arguments = []string{"hello"}
	}

	if err := p.Start(); err != nil {
		t.Fatalf("failed to start process: %v", err)
	}

	if p.State() != Running && p.State() != Starting {
		t.Fatalf("expected process to be running or starting, got %v", p.State())
	}

	select {
	case out := <-p.Output:
		if out != "hello" {
			t.Fatalf("expected output 'hello', got %q", out)
		}
	case <-time.After(2 * time.Second):
		t.Fatal("timed out waiting for output")
	}

	select {
	case err := <-p.Done:
		if err != nil {
			t.Fatalf("process finished with error: %v", err)
		}
	case <-time.After(2 * time.Second):
		t.Fatal("timed out waiting for process to finish")
	}

	if p.State() != NotRunning {
		t.Fatalf("expected process to be not running after finish, got %v", p.State())
	}
}

func TestProcessEnvironmentAndDir(t *testing.T) {
	p := NewProcess()
	if runtime.GOOS == "windows" {
		p.Program = "cmd.exe"
		p.Arguments = []string{"/c", "echo %BQT_TEST_VAR%"}
	} else {
		p.Program = "sh"
		p.Arguments = []string{"-c", "echo $BQT_TEST_VAR"}
	}
	p.Environment = []string{"BQT_TEST_VAR=passed"}

	if err := p.Start(); err != nil {
		t.Fatalf("failed to start process: %v", err)
	}

	select {
	case out := <-p.Output:
		if out != "passed" {
			t.Fatalf("expected environment var output 'passed', got %q", out)
		}
	case <-time.After(2 * time.Second):
		t.Fatal("timed out waiting for output")
	}
}
