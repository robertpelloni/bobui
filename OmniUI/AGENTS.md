# AI Agent Instructions (AGENTS.md)

## Guiding Principles for Future Development
1.  **Completeness**: Any new feature must include C++ implementation, QML registration, Documentation updates, and a Test Case.
2.  **Consistency**: Follow the existing directory structure (`core/include/<module>`, `core/src/<module>`).
3.  **Documentation**: Always update `Manual.md` and `CHANGELOG.md` when adding features.
4.  **Verification**: Do not assume code works. Verify file existence and compilation paths (CMake).

## Versioning Protocol
- **Patch**: Bug fixes (6.0.1).
- **Minor**: New backward-compatible features (6.1.0).
- **Major**: Breaking changes (7.0.0).
- Always update `OmniUI/VERSION` and `OmniUI/CHANGELOG.md` on release.

## Submodule Management
- If adding external libraries (e.g., specific DSP libs), add them as git submodules in `OmniUI/deps/`.
- Document them in `OmniUI/DASHBOARD.md`.
