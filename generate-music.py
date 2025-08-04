#!/usr/bin/env python3
"""
MIDI to XM (Extended Module) Converter
Converts MIDI files to XM tracker format using WAV samples for instruments
"""

import struct
import os
import sys
import wave
from pathlib import Path
from typing import List, Dict, Tuple, Optional
import mido
from collections import defaultdict
import numpy as np

# General MIDI instrument names (for mapping samples)
GM_INSTRUMENTS = [
    "Acoustic Grand Piano",
    "Bright Acoustic Piano",
    "Electric Grand Piano",
    "Honky-tonk Piano",
    "Electric Piano 1",
    "Electric Piano 2",
    "Harpsichord",
    "Clavinet",
    "Celesta",
    "Glockenspiel",
    "Music Box",
    "Vibraphone",
    "Marimba",
    "Xylophone",
    "Tubular Bells",
    "Dulcimer",
    "Drawbar Organ",
    "Percussive Organ",
    "Rock Organ",
    "Church Organ",
    "Reed Organ",
    "Accordion",
    "Harmonica",
    "Tango Accordion",
    "Acoustic Guitar (nylon)",
    "Acoustic Guitar (steel)",
    "Electric Guitar (jazz)",
    "Electric Guitar (clean)",
    "Electric Guitar (muted)",
    "Overdriven Guitar",
    "Distortion Guitar",
    "Guitar harmonics",
    "Acoustic Bass",
    "Electric Bass (finger)",
    "Electric Bass (pick)",
    "Fretless Bass",
    "Slap Bass 1",
    "Slap Bass 2",
    "Synth Bass 1",
    "Synth Bass 2",
    "Violin",
    "Viola",
    "Cello",
    "Contrabass",
    "Tremolo Strings",
    "Pizzicato Strings",
    "Orchestral Harp",
    "Timpani",
    "String Ensemble 1",
    "String Ensemble 2",
    "Synth Strings 1",
    "Synth Strings 2",
    "Choir Aahs",
    "Voice Oohs",
    "Synth Voice",
    "Orchestra Hit",
    "Trumpet",
    "Trombone",
    "Tuba",
    "Muted Trumpet",
    "French Horn",
    "Brass Section",
    "Synth Brass 1",
    "Synth Brass 2",
    "Soprano Sax",
    "Alto Sax",
    "Tenor Sax",
    "Baritone Sax",
    "Oboe",
    "English Horn",
    "Bassoon",
    "Clarinet",
    "Piccolo",
    "Flute",
    "Recorder",
    "Pan Flute",
    "Blown Bottle",
    "Shakuhachi",
    "Whistle",
    "Ocarina",
    "Lead 1 (square)",
    "Lead 2 (sawtooth)",
    "Lead 3 (calliope)",
    "Lead 4 (chiff)",
    "Lead 5 (charang)",
    "Lead 6 (voice)",
    "Lead 7 (fifths)",
    "Lead 8 (bass + lead)",
    "Pad 1 (new age)",
    "Pad 2 (warm)",
    "Pad 3 (polysynth)",
    "Pad 4 (choir)",
    "Pad 5 (bowed)",
    "Pad 6 (metallic)",
    "Pad 7 (halo)",
    "Pad 8 (sweep)",
    "FX 1 (rain)",
    "FX 2 (soundtrack)",
    "FX 3 (crystal)",
    "FX 4 (atmosphere)",
    "FX 5 (brightness)",
    "FX 6 (goblins)",
    "FX 7 (echoes)",
    "FX 8 (sci-fi)",
    "Sitar",
    "Banjo",
    "Shamisen",
    "Koto",
    "Kalimba",
    "Bag pipe",
    "Fiddle",
    "Shanai",
    "Tinkle Bell",
    "Agogo",
    "Steel Drums",
    "Woodblock",
    "Taiko Drum",
    "Melodic Tom",
    "Synth Drum",
    "Reverse Cymbal",
    "Guitar Fret Noise",
    "Breath Noise",
    "Seashore",
    "Bird Tweet",
    "Telephone Ring",
    "Helicopter",
    "Applause",
    "Gunshot",
]


class XMInstrument:
    """Represents an XM instrument with samples"""

    def __init__(self, name: str = ""):
        self.name = name[:22].ljust(22, "\0")
        self.samples = []
        self.sample_map = [0] * 96  # Note -> Sample mapping
        self.volume_envelope = []
        self.panning_envelope = []


class XMSample:
    """Represents an XM sample"""

    def __init__(self, data: bytes, name: str = "", rate: int = 44100):
        self.data = data
        self.name = name[:22].ljust(22, "\0")
        self.length = len(data)
        self.loop_start = 0
        self.loop_length = 0
        self.volume = 64
        self.finetune = 0
        self.type = 0  # 0=8bit, 16=16bit
        self.panning = 128
        self.relative_note = 0
        self.rate = rate


class XMPattern:
    """Represents an XM pattern"""

    def __init__(self, rows: int = 64):
        self.rows = rows
        self.data = []  # List of pattern data


class XMModule:
    """XM Module file structure"""

    def __init__(self):
        self.name = "Untitled"
        self.tracker_name = "MIDI2XM Converter"
        self.version = 0x0104  # XM version 1.04
        self.header_size = 276
        self.song_length = 1
        self.restart_pos = 0
        self.channels = 16
        self.patterns = []
        self.instruments = []
        self.speed = 6  # Ticks per row (XM "speed")
        self.bpm = 125  # Beats per minute (XM "tempo")
        self.pattern_order = []
        self.freq_table = 0  # 0=Amiga, 1=Linear

    def write(self, filename: str):
        """Write XM file"""
        with open(filename, "wb") as f:
            # Write header
            f.write(b"Extended Module: ")
            f.write(self.name[:20].encode("latin-1").ljust(20, b"\0"))
            f.write(b"\x1a")
            f.write(self.tracker_name[:20].encode("latin-1").ljust(20, b"\0"))
            f.write(struct.pack("<H", self.version))
            f.write(struct.pack("<I", self.header_size))
            f.write(struct.pack("<H", self.song_length))
            f.write(struct.pack("<H", self.restart_pos))
            f.write(struct.pack("<H", self.channels))
            f.write(struct.pack("<H", len(self.patterns)))
            f.write(struct.pack("<H", len(self.instruments)))
            f.write(struct.pack("<H", self.freq_table))
            f.write(struct.pack("<H", self.speed))
            f.write(struct.pack("<H", self.bpm))

            # Pattern order table
            for i in range(256):
                if i < len(self.pattern_order):
                    f.write(struct.pack("B", self.pattern_order[i]))
                else:
                    f.write(b"\0")

            # Write patterns
            for pattern in self.patterns:
                self._write_pattern(f, pattern)

            # Write instruments
            for instrument in self.instruments:
                self._write_instrument(f, instrument)

    def _write_pattern(self, f, pattern: XMPattern):
        """Write pattern data"""
        # Pattern header
        f.write(struct.pack("<I", 9))  # Header length
        f.write(struct.pack("B", 0))  # Packing type
        f.write(struct.pack("<H", pattern.rows))

        # Calculate packed data size
        packed_data = self._pack_pattern_data(pattern)
        f.write(struct.pack("<H", len(packed_data)))
        f.write(packed_data)

    def _pack_pattern_data(self, pattern: XMPattern) -> bytes:
        """Pack pattern data using XM compression"""
        packed = bytearray()

        for row in pattern.data:
            for channel_data in row:
                if channel_data is None:
                    packed.append(0x80)  # Empty note
                else:
                    note, inst, vol, eff, param = channel_data

                    # Determine what to include
                    flags = 0
                    if note != 0:
                        flags |= 0x01
                    if inst != 0:
                        flags |= 0x02
                    if vol != 0:
                        flags |= 0x04
                    if eff != 0:
                        flags |= 0x08
                    if param != 0:
                        flags |= 0x10

                    if flags != 0x1F:  # Use compression
                        packed.append(flags | 0x80)

                    if note != 0:
                        packed.append(note)
                    if inst != 0:
                        packed.append(inst)
                    if vol != 0:
                        packed.append(vol)
                    if eff != 0:
                        packed.append(eff)
                    if param != 0:
                        packed.append(param)

        return bytes(packed)

    def _write_instrument(self, f, instrument: XMInstrument):
        """Write instrument data"""
        # Calculate instrument size
        inst_size = 263 if instrument.samples else 33

        # Instrument header
        f.write(struct.pack("<I", inst_size))
        f.write(instrument.name.encode("latin-1"))
        f.write(struct.pack("B", 0))  # Type
        f.write(struct.pack("<H", len(instrument.samples)))

        if instrument.samples:
            # Sample header size
            f.write(struct.pack("<I", 40))

            # Sample number for all notes
            for i in range(96):
                f.write(struct.pack("B", instrument.sample_map[i]))

            # Volume envelope (simplified)
            for i in range(48):
                f.write(struct.pack("<H", 0))

            # Panning envelope (simplified)
            for i in range(48):
                f.write(struct.pack("<H", 0))

            # Envelope points
            f.write(struct.pack("B", 0))  # Vol points
            f.write(struct.pack("B", 0))  # Pan points

            # Envelope sustain/loop points
            f.write(struct.pack("B", 0))
            f.write(struct.pack("B", 0))
            f.write(struct.pack("B", 0))
            f.write(struct.pack("B", 0))
            f.write(struct.pack("B", 0))
            f.write(struct.pack("B", 0))

            # Volume/panning/flags
            f.write(struct.pack("B", 0))  # Volume type
            f.write(struct.pack("B", 0))  # Panning type
            f.write(struct.pack("B", 0))  # Vibrato type
            f.write(struct.pack("B", 0))  # Vibrato sweep
            f.write(struct.pack("B", 0))  # Vibrato depth
            f.write(struct.pack("B", 0))  # Vibrato rate
            f.write(struct.pack("<H", 0))  # Volume fadeout
            f.write(struct.pack("<H", 0))  # Reserved

            # Write samples
            for sample in instrument.samples:
                self._write_sample_header(f, sample)

            # Write sample data
            for sample in instrument.samples:
                # Convert to delta values for XM format
                delta_data = self._convert_to_delta(sample.data)
                f.write(delta_data)

    def _write_sample_header(self, f, sample: XMSample):
        """Write sample header"""
        f.write(struct.pack("<I", sample.length))
        f.write(struct.pack("<I", sample.loop_start))
        f.write(struct.pack("<I", sample.loop_length))
        f.write(struct.pack("B", sample.volume))
        f.write(struct.pack("b", sample.finetune))
        f.write(struct.pack("B", sample.type))
        f.write(struct.pack("B", sample.panning))
        f.write(struct.pack("b", sample.relative_note))
        f.write(struct.pack("B", 0))  # Reserved
        f.write(sample.name.encode("latin-1"))

    def _convert_to_delta(self, data: bytes) -> bytes:
        """Convert sample data to delta format"""
        if len(data) == 0:
            return b""

        delta = bytearray()
        prev = 0

        for b in data:
            delta.append((b - prev) & 0xFF)
            prev = b

        return bytes(delta)


class MidiToXMConverter:
    """Main converter class"""

    def __init__(self, sample_dir: str, rows_per_beat: int = 4):
        self.sample_dir = Path(sample_dir)
        self.sample_map = {}  # MIDI program -> WAV file mapping
        self.rows_per_beat = rows_per_beat  # Resolution: 4=16th notes, 8=32nd notes
        self.load_samples()

    def load_samples(self):
        """Load WAV samples from directory and map to MIDI instruments"""
        wav_files = list(self.sample_dir.glob("*.wav"))

        # Try to map samples to instruments by name matching
        for i, instrument_name in enumerate(GM_INSTRUMENTS):
            # Look for matching WAV files
            for wav_file in wav_files:
                wav_name = wav_file.stem.lower()
                inst_name_lower = instrument_name.lower()

                # Check for various naming conventions
                if (
                    wav_name == inst_name_lower
                    or wav_name == str(i)
                    or wav_name == f"{i:02d}"
                    or wav_name == f"{i:03d}"
                    or inst_name_lower.replace(" ", "_") == wav_name
                    or inst_name_lower.replace(" ", "-") == wav_name
                ):
                    self.sample_map[i] = wav_file
                    break

        print(f"Loaded {len(self.sample_map)} instrument samples")

    def convert(self, midi_file: str, output_file: str):
        """Convert MIDI file to XM format"""
        # Load MIDI file
        mid = mido.MidiFile(midi_file)

        # Create XM module
        xm = XMModule()
        xm.name = Path(midi_file).stem

        # Convert MIDI to patterns
        patterns, instruments_used, tempo_bpm = self._convert_midi_to_patterns(mid)

        # Set tempo/speed from MIDI
        xm.speed, xm.bpm = tempo_bpm

        # Create instruments
        for prog in sorted(instruments_used):
            if prog in self.sample_map:
                instrument = self._create_instrument(prog)
                xm.instruments.append(instrument)
            else:
                # Create empty instrument
                instrument = XMInstrument(f"Instrument {prog}")
                xm.instruments.append(instrument)

        # Add patterns to module
        xm.patterns = patterns
        xm.pattern_order = list(range(len(patterns)))
        xm.song_length = len(patterns)

        # Write XM file
        xm.write(output_file)
        print(f"Converted {midi_file} to {output_file}")
        print(f"XM Speed: {xm.speed}, BPM: {xm.bpm}")

    def _convert_midi_to_patterns(
        self, mid: mido.MidiFile
    ) -> Tuple[List[XMPattern], set, Tuple[int, int]]:
        """Convert MIDI to XM patterns"""
        # Extract tempo from MIDI
        midi_tempo = 500000  # Default 120 BPM in microseconds per quarter note

        # Find tempo changes
        for track in mid.tracks:
            for msg in track:
                if msg.type == "set_tempo":
                    midi_tempo = msg.tempo
                    break

        # Convert MIDI tempo to XM BPM
        # MIDI: microseconds per quarter note
        # XM BPM: 2.5 * beats_per_second
        midi_bpm = 60000000 / midi_tempo

        # XM timing calculation
        # We want to match the MIDI playback speed
        # XM uses ticks_per_row (speed) and BPM
        # Common XM speeds are 3, 4, 6
        xm_speed = 3  # Lower value = faster playback

        # Calculate XM BPM to match MIDI timing
        # XM plays at: rows_per_minute = BPM * 2.5 / speed
        # We want: rows_per_minute = midi_bpm * rows_per_beat
        rows_per_minute = midi_bpm * self.rows_per_beat
        xm_bpm = int(rows_per_minute * xm_speed / 2.5)

        # Clamp BPM to valid range (32-255)
        xm_bpm = max(32, min(255, xm_bpm))

        # If BPM is maxed out and playback is still too slow, reduce speed
        if xm_bpm >= 255 and xm_speed > 1:
            xm_speed = max(1, xm_speed - 1)
            xm_bpm = int(rows_per_minute * xm_speed / 2.5)
            xm_bpm = max(32, min(255, xm_bpm))

        print(f"MIDI BPM: {midi_bpm:.1f} -> XM Speed: {xm_speed}, BPM: {xm_bpm}")

        # Calculate ticks per row for MIDI conversion
        ticks_per_beat = mid.ticks_per_beat
        ticks_per_row = ticks_per_beat // self.rows_per_beat

        # Track state
        current_notes = {}  # channel -> (note, velocity)
        current_programs = {}  # channel -> program
        instruments_used = set()

        # Convert all MIDI events to absolute time
        events = []
        for track in mid.tracks:
            track_time = 0
            for msg in track:
                track_time += msg.time
                # Only include channel messages and tempo changes
                if not msg.is_meta or msg.type == "set_tempo":
                    events.append((track_time, msg))

        # Sort by time
        events.sort(key=lambda x: x[0])

        # Group events by row
        rows_data = defaultdict(lambda: [None] * 16)  # row -> channel data

        for abs_time, msg in events:
            row = abs_time // ticks_per_row

            if msg.type == "program_change":
                current_programs[msg.channel] = msg.program
                instruments_used.add(msg.program)

            elif msg.type == "note_on" and msg.velocity > 0:
                current_notes[msg.channel] = (msg.note, msg.velocity)

                # Add note to pattern
                xm_note = self._midi_note_to_xm(msg.note)
                xm_inst = self._get_instrument_number(
                    current_programs.get(msg.channel, 0)
                )
                xm_vol = 0x10 + (msg.velocity >> 1)  # Volume column

                rows_data[row][msg.channel] = (xm_note, xm_inst, xm_vol, 0, 0)

            elif msg.type == "note_off" or (
                msg.type == "note_on" and msg.velocity == 0
            ):
                if msg.channel in current_notes:
                    # Add note off
                    rows_data[row][msg.channel] = (97, 0, 0, 0, 0)  # Note off
                    del current_notes[msg.channel]

        # Create patterns
        patterns = []
        pattern_data = []
        rows_per_pattern = 64

        max_row = max(rows_data.keys()) if rows_data else 0

        for row in range(max_row + 1):
            pattern_data.append(rows_data[row])

            if (row + 1) % rows_per_pattern == 0:
                pattern = XMPattern(rows_per_pattern)
                pattern.data = pattern_data
                patterns.append(pattern)
                pattern_data = []

        # Add remaining rows
        if pattern_data:
            pattern = XMPattern(len(pattern_data))
            pattern.data = pattern_data
            patterns.append(pattern)

        return patterns, instruments_used, (xm_speed, xm_bpm)

    def _midi_note_to_xm(self, midi_note: int) -> int:
        """Convert MIDI note to XM note"""
        # XM notes: 1-96 (C-0 to B-7), 97=off
        # MIDI notes: 0-127 (C-1 to G9)
        # Map MIDI C3 (60) to XM C-4 (49)
        xm_note = midi_note - 11
        return max(1, min(96, xm_note))

    def _get_instrument_number(self, program: int) -> int:
        """Get XM instrument number for MIDI program"""
        # Simple 1:1 mapping for now
        return program + 1

    def _create_instrument(self, program: int) -> XMInstrument:
        """Create XM instrument from WAV sample"""
        instrument = XMInstrument(GM_INSTRUMENTS[program])

        # Load WAV file
        wav_file = self.sample_map[program]
        with wave.open(str(wav_file), "rb") as wav:
            # Read WAV data
            frames = wav.readframes(wav.getnframes())
            sample_rate = wav.getframerate()
            channels = wav.getnchannels()
            sample_width = wav.getsampwidth()

            # Convert to mono 8-bit for XM
            if sample_width == 2:
                # 16-bit to 8-bit
                data = np.frombuffer(frames, dtype=np.int16)
                if channels == 2:
                    # Stereo to mono
                    data = data.reshape(-1, 2).mean(axis=1)
                data = (data // 256 + 128).astype(np.uint8)
            else:
                data = np.frombuffer(frames, dtype=np.uint8)
                if channels == 2:
                    data = data.reshape(-1, 2).mean(axis=1).astype(np.uint8)

            # Create sample
            sample = XMSample(data.tobytes(), wav_file.stem, sample_rate)
            instrument.samples.append(sample)

            # Map all notes to this sample
            instrument.sample_map = [0] * 96

        return instrument


def main():
    if len(sys.argv) < 4:
        print(
            "Usage: midi2xm.py <midi_file> <samples_directory> <output_xm> [rows_per_beat]"
        )
        print("Example: midi2xm.py song.mid ./samples/ song.xm")
        print("         midi2xm.py song.mid ./samples/ song.xm 8")
        print("")
        print("rows_per_beat: 4 = 16th notes (default), 8 = 32nd notes")
        print("Use higher values for more precise timing but larger files")
        sys.exit(1)

    midi_file = sys.argv[1]
    samples_dir = sys.argv[2]
    output_file = sys.argv[3]
    rows_per_beat = int(sys.argv[4]) if len(sys.argv) > 4 else 4

    if not os.path.exists(midi_file):
        print(f"Error: MIDI file '{midi_file}' not found")
        sys.exit(1)

    if not os.path.isdir(samples_dir):
        print(f"Error: Samples directory '{samples_dir}' not found")
        sys.exit(1)

    # Create converter and convert
    converter = MidiToXMConverter(samples_dir, rows_per_beat)
    converter.convert(midi_file, output_file)


if __name__ == "__main__":
    main()
