#!/usr/bin/env python3
import csv
import re
import sys
import chardet


def parse_navigate_coords(nav_str):
    """Parse coordinate string like '{2, 6}' into tuple (2, 6)"""
    if not nav_str or nav_str.strip() == "":
        return (0, 0)
    # Remove curly braces and split by comma
    nav_str = nav_str.strip().strip("{}")
    coords = nav_str.split(",")
    if len(coords) == 2:
        try:
            return (int(coords[0].strip()), int(coords[1].strip()))
        except ValueError:
            return (0, 0)
    return (0, 0)


sprites = []


def process_csv(filename):
    conversations = {}
    current_convo = None
    current_entries = []

    with open(filename, "rb") as f:
        raw_data = f.read()
        result = chardet.detect(raw_data)
        encoding = result["encoding"] or "utf-8"

    with open(filename, "r", newline="", encoding=encoding) as csvfile:
        reader = csv.reader(csvfile, delimiter="\t")

        for row in reader:
            # Skip empty rows
            if not any(cell.strip() for cell in row):
                continue

            # Check if this is a conversation name (non-empty first column, rest empty)
            if row[0].strip() and all(not cell.strip() for cell in row[1:]):
                # Save previous conversation if exists
                if current_convo and current_entries:
                    conversations[current_convo] = current_entries
                    current_entries = []
                current_convo = row[0].strip()
                continue

            # Skip header row
            if row[0] == "Name / ID":
                continue

            # Process data row
            if current_convo:
                # Parse fields with defaults
                name_id = int(row[0]) if row[0].strip() else 0
                portrait = row[1].strip() if row[1].strip() else "nullptr"
                emotion = row[2].strip() if row[2].strip() else "EM_DEFAULT"
                action = row[3].strip() if row[3].strip() else "ACT_DEFAULT"
                line1 = row[4].strip() if len(row) > 4 else ""
                line2 = row[5].strip() if len(row) > 5 else ""
                line3 = row[6].strip() if len(row) > 6 else ""
                shake = (
                    "true"
                    if (len(row) > 7 and row[7].strip().lower() == "true")
                    else "false"
                )
                size = (
                    row[8].strip()
                    if (len(row) > 8 and row[8].strip())
                    else "SIZE_DEFAULT"
                )
                speed = (
                    row[9].strip()
                    if (len(row) > 9 and row[9].strip())
                    else "SP_DEFAULT"
                )
                index = row[10].strip() if (len(row) > 10 and row[10].strip()) else "0"
                anim_ptr = (
                    row[11].strip()
                    if (len(row) > 11 and row[11].strip())
                    else "nullptr"
                )
                nav_str = row[12].strip() if len(row) > 12 else ""
                nav_coords = parse_navigate_coords(nav_str)

                if portrait not in sprites:
                    sprites.append(portrait)

                if portrait != "nullptr":
                    portrait = "&sprite_items::" + portrait

                entry = {
                    "id": name_id,
                    "portrait": portrait,
                    "emotion": emotion,
                    "action": action,
                    "line1": line1.replace("\"", '\\"'),
                    "line2": line2.replace("\"", '\\"'),
                    "line3": line3.replace("\"", '\\"'),
                    "shake": shake,
                    "size": size,
                    "speed": speed,
                    "index": index,
                    "anim": anim_ptr,
                    "nav": nav_coords,
                }
                current_entries.append(entry)

    # Save last conversation
    if current_convo and current_entries:
        conversations[current_convo] = current_entries

    return conversations


def generate_header(conversations):
    """Generate the C++ header file content"""
    output = []

    for convo_name, entries in conversations.items():
        output.append(f"constexpr conversation {convo_name} = {{")

        for entry in entries:
            # Convert index to proper format - if it's "0" or empty, use 0, otherwise use the constant name
            index_value = (
                "0" if (entry["index"] == "0" or not entry["index"]) else entry["index"]
            )

            line = f'    {{{entry["id"]}, {entry["portrait"]}, {entry["emotion"]}, {entry["action"]}, '
            line += f'"{entry["line1"]}", "{entry["line2"]}", "{entry["line3"]}", '
            line += f'{entry["shake"]}, {entry["size"]}, {entry["speed"]}, '
            line += f'{index_value}, {entry["anim"]}, {{{entry["nav"][0]}, {entry["nav"][1]}}}}}'
            output.append(line + ",")

        # Add EOL entry
        eol_line = '    {0, nullptr, EM_DEFAULT, ACT_END, "", "", "", false, SIZE_DEFAULT, SP_DEFAULT, 0, nullptr, {0, 0}}'
        output.append(eol_line + "};")
        output.append("")

    return "\n".join(output)


input_file = "dialogue.txt"

template = """// ge_actions.h

#include "ge_text.h"
#include "ge_sprites.h"
#include "ge_maps.h"
#include "ge_character_manager.h"
#include "ge_animations.h"

using namespace bn;

$sprites

$data
int action_listener(map_manager *man, character_manager *ch_man);"""

conversations = process_csv(input_file)
header_content = generate_header(conversations)
final = template.replace("$data", header_content).replace(
    "$sprites",
    "\n".join(
        [
            '#include "bn_sprite_items_' + s.replace("&", "") + '.h"'
            for s in sprites
            if s != "nullptr"
        ]
    ),
)

with open("include/ge_actions.h", "w") as f:
    f.write(final)
