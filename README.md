Kinetic-C Client Library
========================
![](http://travis-ci.org/atomicobject/kinetic-c.png?branch=master)

This repo contains code for producing C Kinetic clients which use the Seagate Kinetic protocol. Code examples/utilities that use the Kinetic C library are included for reference and usage during development.

[Kinetic-C build status](http://travis-ci.org/atomicobject/kinetic-c) is provided via [Travis CI](http://travis-ci.org)

Getting Started
================

    > git clone --recursive https://github.com/atomicobject/kinetic-c.git

    > cd kinetic-c
    > bundle install # Ensures you have all RubyGems needed
    > bundle execute rake # Run all tests and build kinetic-c library and examples

API Documentation
=================
[Kinetic-C API](http://atomicobject.github.io/kinetic-c/) (generated with Doxygen)

Dependencies
============
* [Kinetic Protocol](https://github.com/Seagate/kinetic-protocol)
    * [Kinetic-Protocol v2.0.4](https://github.com/Seagate/kinetic-protocol/releases/tag/2.0.4)
* [Ruby](https://www.ruby-lang.org) (v1.9.3 or higher) scripting language
* [RubyGems](http://rubygems.org) (installed w/ `bundle install)
    * [bundler](http://bundler.io) (v1.3.5 or higher) environment/dependency manager for Ruby projects
    * [rake](https://github.com/jimweirich/rake) (0.9.2.2 or higher) Ruby implementation of Make
    * [require_all](https://github.com/jarmo/require_all)
    * [constructor](https://github.com/atomicobject/constructor)
    * [diy](https://github.com/atomicobject/diy)
    * [ceedling](https://github.com/ThrowTheSwitch/Ceedling) Ruby/Rake-based build/test system for C projects
        * Ceedling also includes the following tools for testing C code
        * [Unity](https://github.com/ThrowTheSwitch/Unity) lightweight assertion framework and test executor for C
        * [CMock](https://github.com/ThrowTheSwitch/CMock) mock/fake generator for C modules using only C header files as input (written in Ruby)
* [CppCheck](http://cppcheck.sourceforge.net/) for static analysis of source code
* [Valgrind](http://valgrind.org/) for validation of memory usage/management
* [Doxygen](https://github.com/doxygen) and [GraphViz](http://www.graphviz.org/) for generating API documentation

Common Developer Tasks
======================

NOTE: Prefix the following commands with `bundle exec` so that they execute in the context of the bundle environment setup via `bundle install`.

* List available Rake tasks w/descriptions
    * `rake -T`
* Run all tests and build the library and examples
    * `rake`
* Build the library
    * `rake release`
* Run all unit/integration tests
    * `rake test:all`
* Analyze code
    * `rake cppcheck`
* Generate documentation
    * `rake doxygen:gen`
* Apply license to source files (skips already licensed files)
    * `rake apply_license`
* Build/install Google Protocol Buffers support for the Kinetic-Protocol
    * `rake proto`
* Enable verbose output (for current Rake run)
    * `rake verbose *<task_A>* *<task_B>*`

Examples
========

The following examples are provided for development reference and as utilities to aid development

In order to execute a given example, execute `kinetic-c` with a given example name and optional arguments (below)


First, `cd` into the `build/release` folder, and then execute the desired command.
If no command is specified, kinetic-c will execute all of the example commands.


**e.g.** - Execute the NoOp operation against default host: `localhost`:

```
cd build/release
kinetic-c noop
```

**e.g.** - Execute the NoOp operation against `my-kinetic-server.com`:

```
cd build/release
kinetic-c noop --host my-kinetic-server.com
```


`noop [--hostname|-h "some_host.com"|123.253.253.23]`

`read_file_blocking` (incomplete!)

`read_file_nonblocking` (incomplete!)

`write_file_blocking` (incomplete!)

`write_file_nonblocking` (incomplete!)

`write_file_blocking_threads` (incomplete!)

`delete_file_blocking` (incomplete!)

`delete_file_nonblocking` (incomplete!)

`instant_secure_erase` (incomplete!)

`kinetic_stat` (incomplete!)

`dump_keyspace` (incomplete!)

`set_acls` (incomplete!)

`set_pin` (incomplete!)

`set_clusterversion` (incomplete!)

`update_firmware` (incomplete!)

`copy_drive` (incomplete!)