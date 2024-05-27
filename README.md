# DiscreteScroll

<img src="https://github.com/emreyolcu/discrete-scroll/blob/icon/icon.png?raw=true" align="right" width="35%">

[![Downloads](https://img.shields.io/github/downloads/emreyolcu/discrete-scroll/total.svg)](https://github.com/emreyolcu/discrete-scroll/releases)

This small utility fixes macOS's unnecessary scroll wheel acceleration.
It runs in the background, allowing you to scroll
a constant number of lines with each notch of the wheel.

### Supported versions

As of May 2024, this application works on macOS versions 10.9–14.0.

### Installation

You may download the binary [here](https://github.com/emreyolcu/discrete-scroll/releases/download/v1.0.0/DiscreteScroll.zip).

It needs to be run each time you boot.
If you want this to be automatic, do the following:

1. On macOS 13.0 and later, go to `System Settings > General > Login Items`;
   otherwise, go to `System Preferences > Users & Groups > Login Items`.
2. Add `DiscreteScroll` to the list.

If you want to undo the effect, do the following:

1. Launch `Activity Monitor`.
2. Search for `DiscreteScroll` and select it.
3. Click the stop button in the upper-left corner and choose Quit.

### Configuration

The default behavior is to scroll 3 lines with each notch of the wheel.
If you want to change this, run the following command,
replacing `LINES` with the number of lines to scroll with each notch.
This number may even be negative, which inverts scrolling direction.

```
defaults write com.emreyolcu.DiscreteScroll lines -int LINES
```

You should restart the application for this setting to take effect.

### Potential problems

Recent versions of macOS have made it difficult to run unsigned binaries.

If you experience issues launching the application, try the following:

- Remove the quarantine attribute by running the command
  `xattr -dr com.apple.quarantine /path/to/DiscreteScroll.app`,
  where the path points to the application bundle.
- Disable Gatekeeper by running the command
  `spctl --add /path/to/DiscreteScroll.app`,
  where the path points to the application bundle.

If on startup the application asks for accessibility permissions
even though you have previously granted it access, try the following:

1. On macOS 13.0 and later, go to `System Settings > Privacy & Security > Accessibility`;
   otherwise, go to `System Preferences > Security & Privacy > Privacy > Accessibility`.
2. Remove `DiscreteScroll` from the list and add it again.
