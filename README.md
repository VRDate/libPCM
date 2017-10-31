**libPCM:**
========
* libPCM is a project for Reading, and Writing Audio/Image Samples.
* libPCM the project includes libPCM the library, and a utility (TrimSilence) for stripping empty samples from audio files (I may eventually add a function to trim empty columsn/rows of pixels as well, but it's not here yet).

License:
=======
libPCM (the project) is released under the terms of the 3 clause [`Revised BSD`](https://tldrlegal.com/license/bsd-3-clause-license-%28revised%29) license.

Here's a tl;dr of my license:

* **Do** If you permanently fork libPCM, change the name to reduce confusion.
* **Do** include in your acknowledgments a link to [libPCM on GitHub](https://www.github.com/BumbleBritches57/libPCM).
* **Don't** plagiarize libPCM.
* **Don't** relicense libPCM.
* **Don't** sue me if something goes wrong. I never have and never will guarantee libPCM works for anything, you figure out if it's right for you.

Compiling:
=========
* libPCM alone won't do a whole lot for you, it's real use is in being a library other libraries rely on.
* The makefile by default builds the library as a static library, for RELEASE, to change this set `BUILDTYPE` to `DEBUG`.
* libPCM is included as a submodule in git, so you don't need to install it if you're using one of my libraries, ModernPNG, ModernFLAC, ModernAVC, etc.
* libPCM includes BitIO as a submodules in git, in order to compile you need to clone the repo with `git clone --recursive https://www.github.com/BumbleBritches57/libPCM`
*  **Microsoft** has decided to not support C11's `_Generic` preprocessor keyword that BitIO relies on, so you can not link libPCM, or anything that relies on it with the MSVC linker. To solve this problem, pester Microsoft to fix their support for the C11 standard.

How To Use libPCM:
===================
In order to use libPCM, you need to include libPCM.h.

libPCM:
-----
* To use libPCM in `main()` call `PCMFileInit`.
* Go read the BitIO README to learn how to set up a `BitInput` and/or `BitOutput`, and a `BitBuffer`.
* Call `IdentifyPCMFile` to figure out what kind of file is in your BitInput.
* Call `ParsePCMMetadata` in order to extract all the internal/external metadata.
* Call `ExtractSamples`/`ExtractPixels` to extract a specified number of audio or image samples.
* Once you've extracted all the samples/pixels from the file, call `IsThereMoreMetadata` to see if there is more data after the sample chunk (this is especially important in AIF files)
* If there is in fact more data after the samples, call `ParsePCMMetadata` again.
