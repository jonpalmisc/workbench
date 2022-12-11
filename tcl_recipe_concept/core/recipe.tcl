source core/fs.tcl

namespace eval recipe {
	variable name ""
	variable version ""

	proc tagged_name {} {
		return "$recipe::name-$recipe::version"
	}
}

proc RecipeInit {} {
	exec mkdir -p work
	cd work
}

proc RecipeName {name} {
	set recipe::name $name
}

proc RecipeVersion {version} {
	set recipe::version $version
}

proc FromArchive {url} {
	fs::fetch $url
	fs::untar archive.jpa [recipe::tagged_name]
}
