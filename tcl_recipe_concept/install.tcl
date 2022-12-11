#!/usr/bin/env tclsh

source core/build.tcl
source core/recipe.tcl

if { $argc != 1 } {
	puts "usage: install <recipe>"
} else {
	source [lindex $argv 0]
}
