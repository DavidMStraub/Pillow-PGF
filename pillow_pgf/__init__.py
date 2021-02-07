"""PGF plugin for Pillow."""

from PIL import Image, ImageFile

from . import decoder


MODES = {
    # (photoshop mode, bits) -> (pil mode, required channels)
    (0, 1): ("1", 1),
    (0, 8): ("L", 1),
    (1, 8): ("L", 1),
    (2, 8): ("P", 1),
    (3, 8): ("BGR", 3),
    (4, 8): ("CMYK", 4),
    (7, 8): ("L", 1),
    (8, 8): ("L", 1),
    (9, 8): ("LAB", 3),
    (17, 8): ("RGBA", 4),
}


class PGFImageFile(ImageFile.ImageFile):
    """PGF image plugin."""

    format = "PGF"
    format_description = "Progressive Graphics File"

    def _open(self):
        """Open the file."""

        pre_header = self.fp.read(8)
        header_size = int(pre_header[4:])
        _full_header = self.fp.read(header_size)
        header = _full_header[:16]
        post_header = _full_header[16:]

        width = int(header[:4])
        height = int(header[4:8])
        mode = int(header[12])
        bits_per_channel = int(header[13])

        self._size = (width, height)

        mode, channels = MODES[(mode, bits_per_channel)]

        self.mode = mode

        stride = (mode * width + 7) / 8
        buffer_size = stride * height

        args = (buffer_size, stride)
        self.tile = [("PGF", (0, 0) + self.size, 8 + header_size, args)]


def _validate(prefix):
    return prefix[:3] == b"PGF"


Image.register_open(PGFImageFile.format, PGFImageFile, _validate)
Image.register_extension(PGFImageFile.format, ".pgf")
