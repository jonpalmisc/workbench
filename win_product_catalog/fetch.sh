#!/bin/sh

CATALOG="catalog.cab"

curl -sLo "$CATALOG" 'https://go.microsoft.com/fwlink?linkid=2156292'
cabextract "$CATALOG" 1>/dev/null
rm "$CATALOG"
