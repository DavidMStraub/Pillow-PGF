#
# The Python Imaging Library.
# $Id$
#
# PGF file handling
#
# PGF is an image file format defined by xeraina, commonly used
# as an alternative to JPEG.  
#
# History:
# 2021-02-08 cs   Created
#
# Copyright (c) 2021 by xeraina and Christoph Stamm.
#
# See the README file for information on usage and redistribution.
#

from . import Image, ImageFile
from ._binary import i32le as i32

MAGIC = b"PGF"

MODES = {
    # PGF mode -> (pil mode, required channels)
    0: ("1", 1),    # needs unpacking to 1 bit per byte
    1: ("L", 1),    # 8 bit grayscale
    2: ("P", 1),    # 8 bit indexed (palette needed)
    3: ("RGB", 3),  # 24 bit true color, BGR->RGB mapping necessary
    4: ("CMYK", 4),
    9: ("LAB", 3),
    17: ("RGBA", 4) # channel mapping necessary
}

def _accept(prefix):
    return prefix[:3] == MAGIC


##
# Image plugin for the PGF format.

class PgfImageFile(ImageFile.ImageFile):

    format = "PGF"
    format_description = "Progressive Graphics File"

    def _open(self):

        # Header
        pre_header = self.fp.read(8)

        if not _accept(pre_header):
            raise SyntaxError("not a PGF file")

        header = self.fp.read(16)

        width = i32(header[:4])
        height = i32(header[4:8])
        bpp = int(header[10])
        pgfmode = int(header[12])

        self._size = (width, height)
        mode, channels = MODES[pgfmode]
        self.mode = mode

        #stride = (bpp * width + 7) / 8
        #buffer_size = stride * height
        #args = (buffer_size, stride)

        fd = self.fp.fileno()
        args = (fd)

        self.tile = [("pgf", (0, 0) + self.size, 0, args)]



Image.register_open(PgfImageFile.format, PgfImageFile, _accept)

Image.register_extension(PgfImageFile.format, ".pgf")
