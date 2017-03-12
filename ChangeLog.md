# Change Log
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](http://keepachangelog.com/)
and this project adheres to [Semantic Versioning](http://semver.org/).

## [1.0.0] - 2017-03-07
### Added
- Colorized output enabled via '-c' flag.
- '-v' option to print nu_unit version.
- Indentation for test-level output
- Type-comparison macros that print the inputs and values on error
  - eg: nu_check_int_eq, nu_check_int_gt, etc
- This ChangeLog

## [0.0.1] - 2013-07-02
### Added
- Original nu_unit code
- Basic `nu_check` and `nu_assert` macros
- Excluded type-comparison macros, colorization

[1.0.0]: https://github.com/EvanKuhn/nu_unit/compare/v0.0.1...v1.0.0
[0.0.1]: https://github.com/EvanKuhn/nu_unit/releases/tag/v0.0.1
