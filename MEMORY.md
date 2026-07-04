# OmniUI / BobQ Internal Memory & Codebase Observations

=======
## High-Confidence Findings
- `bqt` should be treated as the **framework/kernel**, not the full shell/desktop product. The shell belongs to `bobfilez`.
- The repository now contains **both** a large C++/BobUI/OmniUI codebase and a large experimental Go port.
- The actively maintained Go framework path under `internal/` is currently build-verified with `go test ./internal/...` and `go build -buildvcs=false .`, even though the broader repository still contains exploratory and partially migrated areas.
- The C++ tree contains many exploratory components; some are stubs or partial implementations intended as architectural placeholders.
- Documentation drift has improved, but it still needs active maintenance to stay aligned with the verified state.

## Verified Technical Problems
- The legacy C++ tree is still burdened by a large rename-artifact explosion that mixes likely canonical files with generated/support files, tests, docs/assets, and duplicate renamed copies.
- Some non-Go areas of the repository remain exploratory and should not be treated as uniformly build-verified.
- Some docs still describe `bqt` (formerly `bobui`) as a full OS shell instead of a framework kernel consumed by shells like `bobfilez`.
- `VERSION.md` and `CHANGELOG.md` required repeated normalization after prior uncontrolled bumps.

## Design Preferences Going Forward
- Treat previous large-scale additions as **experimental until compiled**.
- Prefer fixing compile/runtime integrity over adding new speculative layers.
- Preserve interesting ideas, but move them to `IDEAS.md` unless or until implemented and verified.
- Keep `bobui` focused on framework parity and multiplayer foundations.

## Immediate Priorities
1. Stabilize Go package graph.
2. Normalize docs to actual scope.
3. Verify build/test status incrementally.
4. Implement missing framework features only after baseline integrity is restored.

## 2026-04-04 BTK Comparison Findings
- The active Go `bobui` framework track is much smaller than `BTK` and should be treated as an experimental, fast-iteration framework lane rather than a mature native framework replacement today.
- `BTK` is substantially stronger in native framework breadth, module depth, packaging, and staged compatibility/migration strategy.
- `bobui` remains stronger in explicit framework-vs-shell separation, collaboration framing, and lightweight modern bridge-contract experimentation.
- `BTK` has already started formalizing BobUI-inspired ownership semantics in stronger C++ types such as `BtkInputOwner`, `BtkFocusToken`, and `BtkInputArbitrator`.
- The right strategic relationship is complementary, not competitive: `BTK` as native framework mainline, `bobui` as idea incubator and parity/abstraction lab.
- `bobui` should keep prioritizing verified runtime semantics, especially around `ScrollBar`/`ScrollView` interaction and `WebView` bridge execution, instead of broad but shallow surface expansion.
- 2026-04-05: The Go `ScrollBar`/`ScrollView` baseline now has compile-verified interaction coupling by reusing Gio's internal scrollbar gesture machinery underneath a BobUI-owned widget surface.
- `DemoSurface` now exercises the scrolling baseline with sample content and live status text, which gives future runtime work a visible integration target.
- 2026-04-05: The Go baseline now includes a `TouchArea` primitive with directional swipe recognition and optional mouse-as-touch fallback for development on non-touch systems.
- The touch/swipe addition is intentionally small and framework-owned; it is a foundation for later multi-touch and gesture-composition work, not a claim of full gesture-stack parity.
- 2026-04-05: The Go `WebView` bridge now supports a small executable runtime model with queueing, pumping, eval handlers, correlated request/reply behavior, and explicit external message handling.
- The most useful near-term pattern remains: use Gio internals for proven interaction mechanics while preserving BobUI-owned framework abstractions and honest scope boundaries.
- 2026-04-05: `ScrollView` now supports overlay vs occupy placement and a lightweight secondary-axis viewport model, improving realism without pretending to have full 2D virtualization.
- 2026-04-05: `Engine.Run()` now uses a real Gio window/frame loop, rendering `LoginView`, `DemoSurface`, and `Shell` in a live runtime path instead of only safe initialization.
- 2026-04-05: `internal/ui/widgets/webview_test.go` now provides automated coverage for WebView bridge queueing, reply correlation, eval handling, error propagation, and external message routing.
- 2026-04-05: `internal/ui/engine_test.go` now provides automated validation for live runtime composition using synthetic Gio frame contexts, covering both login and authenticated runtime states.
- 2026-04-05: `internal/ui/widgets/webview_test.go` now also covers navigation/history semantics including callback firing, back/forward behavior, and forward-history truncation.
- 2026-04-05: `internal/ui/widgets/shell_test.go` now provides synthetic frame-context coverage for taskbar start-button toggling and start-menu visibility/layout behavior.
- 2026-04-05: `internal/kernel/managers_test.go`, `internal/data/data_test.go`, and `internal/net/mesh_node_test.go` now provide the first focused automated coverage for Go kernel/data/net support packages.
- 2026-04-05: `internal/kernel/services_test.go` now adds deterministic coverage for `UndoStack`, `Clipboard`, and `MergeKernel`, extending verification into higher-value kernel services.
- 2026-04-05: The Go `WindowManager` is now integrated into the live runtime path, with seeded managed windows and deterministic tests covering initialization, reset/spawn behavior, and layout dimensions.
- 2026-04-05: Managed windows now have clickable tabs, tab-aware visibility filtering, and per-window body content, making runtime composition more meaningful without claiming full docking complexity.
- 2026-04-05: Managed windows now also have a first direct manipulation baseline through close/move/resize helpers plus deterministic manager/window tests covering that behavior.
- 2026-04-05: Managed windows now have an explicit activation baseline with active-window tracking, fallback behavior on visibility changes, ordering promotion, and active visual state.
- 2026-04-05: Direct managed-window activation requests now route through `WindowManager.Layout(...)`, making activation more realistic while keeping manager state authoritative.
- 2026-04-05: The correct path to side-by-side C++ and Go compilation is a staged compatibility-first migration, not a global `qt` -> `bobui` text replacement.
- The huge untracked C++ rename tree should be treated as a separate classification/cleanup project before any broad migration bucket is attempted.
- 2026-04-05: A first-pass census of the rename explosion shows the largest pressure in `tests/`, `src/corelib/`, and `cmake/`, which reinforces that tests/assets/docs are poor early targets and that `cmake/` package/export compatibility is the safest first real C++ migration bucket.
- 2026-04-05: The first actual C++ migration work should improve identity/discovery through additive `cmake/` package/export compatibility surfaces before touching deep runtime identifiers.
- 2026-04-05: The first additive C++ compatibility implementation now exists in `cmake/BobUICompatibilityHelpers.cmake`, `cmake/BobUI6Config.cmake`, and `cmake/BobUIConfig.cmake`, which forward BobUI package discovery to the canonical Qt package surface and create BobUI-namespaced compatibility component targets.
- 2026-04-05: Script-mode validation via `cmake -P cmake/tests/bobui_compatibility_helpers_mapping.cmake` is a safe way to test this bucket before attempting riskier install/export wiring.
- 2026-04-05: `cmake/QtBaseGlobalTargets.cmake` is the clean hook point for publishing additive BobUI package directories (`BobUI6/`, `BobUI/`) alongside the canonical `Qt6/` package directory.
- 2026-04-05: An end-to-end fake-prefix smoke test via `cmake -P cmake/tests/bobui_package_forwarding_smoke.cmake` now validates that `find_package(BobUI6 ...)` and `find_package(BobUI ...)` can succeed against a realistic package-layout model.
- 2026-04-05: Extracting BobUI package publication into `cmake/QtBobUIHelpers.cmake` made the export-wiring path independently testable under a real configure flow.
- 2026-04-05: `cmake -P cmake/tests/bobui_export_publication_configure_smoke.cmake` now validates that the BobUI publication helper emits expected config/helper/version files into both build-style and install-style package directories.
- 2026-04-05: The first direct BobUI module-package slice now exists for `Core` and `Widgets` via `BobUI6Core`/`BobUICore` and `BobUI6Widgets`/`BobUIWidgets` config shims.
- 2026-04-05: `QtModuleHelpers.cmake` can now publish BobUI module-package compat files safely because missing BobUI module configs are treated as intentional no-ops.
- 2026-04-05: The next direct BobUI module-package slice now exists for `Gui` and `Network` via `BobUI6Gui`/`BobUIGui` and `BobUI6Network`/`BobUINetwork` config shims.
- 2026-04-05: The existing forwarding/publication smoke tests scale to additional modules well, making them a good guardrail for continued package-surface migration.
- 2026-04-05: A direct top-level qtbase configure attempt in this environment fails before project logic due to missing visible C/C++ compilers, so native validation is currently gated by toolchain visibility rather than by the additive BobUI package work.
- 2026-04-05: `cmake/tests/bobui_qtbase_native_configure_preflight.cmake` now encodes that reality as a repeatable preflight step that cleanly skips when no compiler is visible in `PATH`.
- 2026-04-05: The next direct BobUI module-package slice now exists for `Sql` and `Xml` via `BobUI6Sql`/`BobUISql` and `BobUI6Xml`/`BobUIXml` config shims.
- 2026-04-05: Another direct BobUI module-package slice now exists for `OpenGL` and `OpenGLWidgets` via `BobUI6OpenGL`/`BobUIOpenGL` and `BobUI6OpenGLWidgets`/`BobUIOpenGLWidgets` config shims.
- 2026-04-05: Another direct BobUI module-package slice now exists for `DBus` and `PrintSupport` via `BobUI6DBus`/`BobUIDBus` and `BobUI6PrintSupport`/`BobUIPrintSupport` config shims.
- 2026-04-05: Another direct BobUI module-package slice now exists for `Concurrent` and `Test` via `BobUI6Concurrent`/`BobUIConcurrent` and `BobUI6Test`/`BobUITest` config shims.
- 2026-04-05: The BobUI umbrella package smoke test now validates a broader supported multi-module component matrix in one `find_package(BobUI6 ...)` / `find_package(BobUI ...)` call, which is closer to realistic downstream usage than the previous minimal two-component check.
- 2026-04-05: `cmake/BobUISupportedComponents.cmake` is now the explicit source of truth for the currently supported additive BobUI CMake component matrix.
- 2026-04-05: `cmake/tests/bobui_supported_component_manifest_consistency.cmake` now checks that the manifest, shim files, and umbrella/publication smoke coverage remain aligned.
- 2026-04-05: `docs/ai/design/2026-04-05-bobui-cmake-supported-component-status.md` now provides a human-readable status report for the same matrix.
- 2026-04-05: `cmake/tests/bobui_supported_component_report_consistency.cmake` now checks that the human-readable report stays aligned with the manifest-backed component inventory.
- 2026-04-05: `cmake/bobui_generate_supported_component_report.cmake` now generates the supported component status report from the manifest, making the checked-in report generator-backed instead of purely hand-maintained.
- 2026-04-05: `cmake/tests/bobui_full_compatibility_validation.cmake` now provides a single repeatable command for the full BobUI CMake compatibility stack, which is a better default validation gate than a hand-maintained checklist of individual scripts.
- 2026-04-05: `.github/workflows/go_wasm.yml` now includes a `validate-framework` job that runs the exact verified Go baseline plus the consolidated BobUI compatibility command.
- 2026-04-05: `BOBUI_SKIP_NATIVE_CONFIGURE=ON` is now the CI-friendly way to keep the compatibility gate green while native compiler/toolchain provisioning remains a separate environment concern.
>>>>>>> 677b0f352ad2c50efba02126daac7b26465b876d
