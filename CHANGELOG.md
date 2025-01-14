# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

## [2.0.2] - 2025-01-14

### Changed

- Update to ChartDirector 7.1

## [2.0.1] - 2023-03-13

### Fixed

- Implement workaround for issue #16
  Applications could crash on invoking the ChartDirector method setOutputOptions due to a bug in the ChartDirector code. As a workaround a trailing semicolon is added to the options string.
  The included ChartDirector library will be updated as soon as an update is available.

## [2.0.0] - 2021-07-21

### Added

- Support for enhanced dynamic tooltips (new in ChartDirector 7)
- Support for DPI awareness (new in ChartDirector 7)
- Support for loading resources (like images and icons) from graphics files (similar to the QT version)

### Changed

- Update to ChartDirector 7.0
- Enhanced drawing of selection box (less flicker)

### Removed

- Support for wxWidgets versions 2.x

## [1.0.0] - 2018-07-31

First public release

[Unreleased]: ../../compare/v2.0.2...HEAD
[2.0.2]: ../../compare/v2.0.1...v2.0.2
[2.0.1]: ../../compare/v2.0.0...v2.0.1
[2.0.0]: ../../compare/v1.0.0...v2.0.0
[1.0.0]:
