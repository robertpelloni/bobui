# Gemini Instructions
Please read `AGENTS.md` for core rules.
As Gemini, focus heavily on multi-threading implications (like `sync.Mutex` deadlocks) when bridging the C++ `OmniMasterClock` paradigms over into Go routines.
