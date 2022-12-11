source core/env.tcl

proc BuildInit {} {
	cd [recipe::tagged_name]
}

proc BuildAutoConfigure {} {
	exec -ignorestderr ./configure --prefix=[env::dev_prefix] >@stdout
}

proc BuildAutoMake {} {
	exec -ignorestderr make all >@stdout
}

proc BuildAutoInstall {} {
	exec -ignorestderr make install >@stdout
}
