###Demo
```ruby
#!/usr/bin/env ruby
require './hadope'
require 'benchmark'

puts "Problem, mapping several numerical operations to a large Array"

Hadope::set_device Hadope::CPU
Hadope::Logger.get.quiet_mode

ruby_input = (1..10_000_000).to_a
input = (1..10_000_000).to_a

ruby = Benchmark.realtime {
  last = ruby_input
    .map { |i| i + 1 }
    .map { |i| i + 1 }
    .map { |i| i * 2 }
    .map { |i| i / 2 }
    .map { |i| i - 1 }
    .map { |i| i - 1 }.last
  puts "Ruby says result is #{last}"
}

opencl = Benchmark.realtime {
  last = input[Int]
    .map(i:'i + 1')
    .map(i:'i + 1')
    .map(i:'i * 2')
    .map(i:'i / 2')
    .map(i:'i - 1')
    .map(i:'i - 1')[Fixnum].last
  puts "OpenCL says result is #{last}"
}

puts "Ruby: #{ruby.inspect}, OpenCL: #{opencl.inspect}"

=begin
λ  HaDope git:(master) ✗ ./Benchmark.rb
Problem, mapping several numerical operations to a large Array
Ruby says result is 10000000
OpenCL says result is 10000000
Ruby: 4.992017, OpenCL: 0.318413
=end
```

###RSpec -f d
```
Hadope
  Showcasing features
    all features fit together
    returns the correct result
  Top level namespace
    is defined
  Devices
    defines a Device superclass
    defines a CPU device
  Tasks
    defines a TaskQueue
    defines a Task superclass
    defines a Map task

Hadope::CPU
  is a singleton if accessed using ::get
  can succesfully load an integer dataset
  can succesfully retrieve an integer dataset
  allows loading and retrieving via square-bracket syntax

Hadope::Device
  is abstract
  defines transferring an integer array to device memory
  defines retrieving an integer array from device memory
  creates a task queue when initialized
  allows the output to be retrieved by 'casting' to a Ruby type
  #map
    is defined
    causes a task to be queued

Hadope::Logger
  is a singleton if accessed using ::get
  can be set to loud mode
  can be set to quiet mode

Hadope::Map
  can be created with no statements
  can be created with a single statement
  can be created with multiple statements
  #fuse!
    should create a pipelining variable when one is needed
    should not create a pipelining variable when one is not needed

Hadope::Task
  is abstract
  can have a single statement added
  can have multiple statements added

Hadope::TaskQueue
  initializes with an empty queue of tasks
  #simplify!
    can be called when there are no tasks
    should leave the queued tasks alone when they are not combinable
    can be called when there is only one task
    with consecutive map tasks
      will perform map fusion

Array
  has monkey-patched index-access syntax to shortcut device loading
  doesn't allow classes without a conversion method defined to be cast
  still allows the classic behaviour of index-access

Fixnum
  defines how to convert from the equivalent C type (Int)

Int
  defines how to convert from the Ruby Fixnum type to C Ints

Top 3 slowest examples (0.02891 seconds, 68.7% of total time):
  Hadope Showcasing features all features fit together
    0.01423 seconds ./spec/hadope_spec.rb:5
  Hadope Showcasing features returns the correct result
    0.01226 seconds ./spec/hadope_spec.rb:12
  Hadope::TaskQueue#simplify! with consecutive map tasks will perform map fusion
    0.00243 seconds ./spec/lib/tasks/taskqueue_spec.rb:42

Top 3 slowest example groups:
  Hadope
    0.00338 seconds average (0.02704 seconds / 8 examples) ./spec/hadope_spec.rb:3
  Hadope::TaskQueue
    0.00107 seconds average (0.00533 seconds / 5 examples) ./spec/lib/tasks/taskqueue_spec.rb:5
  Hadope::Map
    0.00102 seconds average (0.00512 seconds / 5 examples) ./spec/lib/tasks/map_spec.rb:5

Finished in 0.0462 seconds
40 examples, 0 failures
Coverage report generated for RSpec to /Users/cronin/Dev/Ruby/HaDope/coverage. 300 / 300 LOC (100.0%) covered.
```
