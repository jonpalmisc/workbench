namespace eval fs {
	proc fetch {url} {
		exec curl -sL $url -o archive.jpa
	}

	proc untar {path dest} {
		exec mkdir $dest
		exec tar xf $path -C $dest --strip-components 1
	}
}
