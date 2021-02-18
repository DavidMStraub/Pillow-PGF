import io
import re

import pytest

from PIL import Image, PgfImagePlugin, features

from .helper import (
    assert_image_similar,
    assert_image_similar_tofile,
    skip_unless_feature,
)

try:
    from PIL import _pgf

    HAVE_PGF = True
except ImportError:
    HAVE_PGF = False


class TestUnsupportedPgf:
    def test_unsupported(self):
        if HAVE_PGF:
            PgfImagePlugin.SUPPORTED = False

        file_path = "Tests/images/pgf/rgb_lossy.pgf"
        pytest.warns(UserWarning, lambda: pytest.raises(OSError, Image.open, file_path))

        if HAVE_WEBP:
            WebPImagePlugin.SUPPORTED = True


@skip_unless_feature("pgf")
class TestFileWebp:
    def setup_method(self):
        self.rgb_mode = "RGB"

    def test_version(self):
        _webp.PgfDecoderVersion()
        assert re.search(r"PGF", features.version_module("pgf"))

    def test_read_rgb(self):
        """
        Can we read a RGB mode PGF file without error?
        Does it have the bits we expect?
        """

        with Image.open("Tests/images/pgf/lena_lossless.pgf") as image:
            assert image.mode == self.rgb_mode
            assert image.size == (512, 512)
            assert image.format == "PGF"
            image.load()
            image.getdata()

            # generated from hopper.bmp
            assert_image_similar_tofile(image, "Tests/images/hopper.bmp", 0)

