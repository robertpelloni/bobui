#!/bin/bash

# Ensure VERSION.md exists and is updated
if [ ! -f VERSION.md ]; then
  echo "v1.1.66" > VERSION.md
else
  # simple version bump for the patch version if it matches v1.1.x
  current_version=$(cat VERSION.md)
  if [[ $current_version =~ v1\.1\.([0-9]+) ]]; then
    next_patch=$((BASH_REMATCH[1] + 1))
    echo "v1.1.${next_patch}" > VERSION.md
  fi
fi

# Append to CHANGELOG.md
cat << 'EOC' >> CHANGELOG.md

## [Unreleased]
### Added
- Added event dispatch to `OmniFilter` in native Go (`TypeChanged`, `CutoffChanged`, `ResonanceChanged`), mapping them directly to the `EventLoop` for non-blocking execution.

### Changed
- Fully deprecated the legacy C++ `OmniFilter` stubs, cleaning up references in `CMakeLists.txt` files and QML registration.
EOC

# Append to DEPLOY.md
cat << 'EOD' >> DEPLOY.md

### Framework Status
- The native Go path (`internal/audio`) is now stable and includes asynchronous signal/slot dispatch and full DSP node parity for components like `OmniGain` and `OmniFilter`. The C++ legacy build is considered unstable and should only be used when strictly necessary.
EOD

# Ensure README reflects the latest state
sed -i 's/OmniFilter/BiquadFilter (OmniFilter natively ported)/g' README.md || true
