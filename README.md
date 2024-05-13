# Discrete Scroll

[![Downloads](https://img.shields.io/github/downloads/emreyolcu/discrete-scroll/total.svg)](https://github.com/emreyolcu/discrete-scroll/releases)

This small utility fixes macOS's unnecessary scroll acceleration.
It runs in the background
and allows you to scroll 3 lines with each tick of the wheel.

## Supported versions

As of April 2024, this fix works on macOS versions 10.9–14.0.

## Installation

You may download the binary [here](https://github.com/emreyolcu/discrete-scroll/releases/download/v0.1.1u/DiscreteScroll.zip).
It needs to be run each time you boot.
If you want this to be automatic, do the following:

1. On macOS 13.0 and later, go to `System Settings > General > Login Items`;
otherwise, go to `System Preferences > Users & Groups > Login Items`.
2. Add `DiscreteScroll` to the list.

If you want to undo the effect you may launch Activity Monitor,
search for `DiscreteScroll` and force it to quit.

## Potential problems

Recent versions of macOS have made it difficult to run unsigned binaries.
If you experience issues launching the application
or if you can launch it but it has no effect on scrolling, try the following:

- Remove the quarantine attribute by running the command
`xattr -dr com.apple.quarantine /path/to/DiscreteScroll.app`,
where the path points to the application bundle.
- Disable Gatekeeper by running the command
`spctl --add /path/to/DiscreteScroll.app`,
where the path points to the application bundle.
- Grant access to accessibility features:
  1. On macOS 13.0 and later, go to `System Settings > Privacy & Security > Accessibility`;
  otherwise, go to `System Preferences > Security & Privacy > Privacy > Accessibility`.
  2. Add `DiscreteScroll` to the list.
