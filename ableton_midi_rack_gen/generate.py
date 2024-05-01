#!/usr/bin/env python3

from argparse import ArgumentParser
import csv
import gzip
from copy import deepcopy
from xml.etree import ElementTree


def main():
    parser = ArgumentParser()
    parser.add_argument("mappings", help="note-to-name mapping CSV file")
    parser.add_argument("adg", help="template ADG file use")

    args = parser.parse_args()

    note_map = {}
    with open(args.mappings, newline="") as f:
        reader = csv.reader(f)

        # Skip header row.
        next(reader)

        for row in reader:
            # Skip rows with no note name.
            if not row[1]:
                continue

            note_map[int(row[0])] = row[1]

    # ADG is just Gzip-compressed XML.
    with gzip.open(args.adg, "r") as adg_file:
        adg_xml = adg_file.read()
        adg_tree = ElementTree.fromstring(adg_xml)

    # Find "branch presets" node, a.k.a. the note mapping container.
    if (branch_presets := adg_tree.find(".//BranchPresets")) is None:
        print("Expected `BranchPresets` node in ADG tree!")
        return

    # Use the first branch preset node as the "template" for all of our others
    # rather than hardcoding the structure format at the time of writing.
    template = branch_presets[0]

    # Remove all preset nodes.
    branch_presets.clear()

    branch_id = 0
    for note in note_map:
        branch = deepcopy(template)

        # Gonna just assume these IDs count up from zero.
        branch.attrib["Id"] = str(branch_id)
        branch_id += 1

        # Set note display name.
        if (name := branch.find("Name")) is not None:
            name.attrib["Value"] = note_map[note]

        # Set attributes for actual MIDI range mapping.
        if (min_note := branch.find(".//KeyRange/Min")) is not None:
            min_note.attrib["Value"] = str(note)
        if (max_note := branch.find(".//KeyRange/Max")) is not None:
            max_note.attrib["Value"] = str(note)
        if (min_fade := branch.find(".//KeyRange/CrossfadeMin")) is not None:
            min_fade.attrib["Value"] = str(note)
        if (max_fade := branch.find(".//KeyRange/CrossfadeMax")) is not None:
            max_fade.attrib["Value"] = str(note)

        branch_presets.append(branch)

    with gzip.open(args.adg.replace(".adg", ".out.adg"), "w") as f:
        f.write('<?xml version="1.0" encoding="UTF-8"?>\n'.encode())  # pyright: ignore
        f.write(
            ElementTree.tostring(
                adg_tree, encoding="unicode"
            ).encode()  # pyright: ignore
        )


if __name__ == "__main__":
    main()
