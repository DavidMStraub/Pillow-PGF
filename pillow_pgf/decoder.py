"""PGF file decoder."""

from typing import Tuple

from PIL import Image, ImageFile

import libpgf


class PGFDecoder(ImageFile.PyDecoder):
    """PGF file decoder."""

    def decode(self, buffer) -> Tuple[int, int]:
        """Decode a chunk of the image."""
        (buffer_size, stride) = self.args
        self.set_as_raw(libpgf.decode(self.fd, self.im, buffer_size, stride))
        return -1, 0


Image.register_decoder("PGF", PGFDecoder)
