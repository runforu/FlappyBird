FlappyBird
=======================
How to develop Flappy Bird within five days for game developer beginner.

### Download
Download the latest release from [release page].

### Development
The [Allegro game library] provides basic functions like handling bitmap and font.

#### On Windows
[Microsoft Visual Studio] 2019 or higher with NuGet.

#### On Linux
Install allegro and run `make`.

Note that source dependencies are not currently set in the Makefile, so when developping it is needed to run the following command to ensure the executable is properly updated.
```
make clean && make
```

[GPLv3]: https://www.gnu.org/licenses/gpl-3.0.en.html
[release page]: https://github.com/runforu/FlappyBird/releases
[Allegro game library]: https://liballeg.org/
[Microsoft Visual Studio]: https://visualstudio.microsoft.com/downloads/
