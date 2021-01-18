"""PGF file decoder."""

from typing import Tuple

from PIL import Image, ImageFile

import pgf_decode


class PGFDecoder(ImageFile.PyDecoder):
    """PGF file decoder."""

    def decode(self, buffer) -> Tuple[int, int]:
        """Decode a chunk of the image."""
        (pre_header, header, post_header) = self.args
        self.set_as_raw(pgf_decode.decode(buffer, pre_header, header, post_header))
        return -1, 0


Image.register_decoder("PGF", PGFDecoder)
