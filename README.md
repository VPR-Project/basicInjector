# basicInjector
> [!CAUTION]
> For educational/authorized use only, we do not take any responsibility for the damages that may occur.

> [!WARNING]
> The executable has been detected by a lot of anti-virus solutions because it tries to inject libraries to another executable, whitelisting this file on your anti-virus before injection is recommended.

This is a simple yet powerful DLL injector.

---
### How to use?
+ ```INJECTOR.exe -p 1453 -f "C:\file.dll"``` *(Change 1453 with the target executable's PID)*

### How it works?
+ It gets the target executable by the provided PID and injects the given DLL file with LoadLibraryA method. Built on pure C++.

### Which operating system(s) this application run on?
+ Only Windows 10/11 for now.

### How to load it to VS22?
+ Download the zip archive from the "Code" section in the main page, extract the folder to anywhere, open the .sln file.

### How to build it?
+ If you're on VS22, use "Release" and "x64" options and build it. You can also use CMake.
---

Contributions are more than welcome.

Do not hesitate about using it in any kind of application you might do, just don't forget to add credits.

## License
This project has been licensed with and under the protection of the **GNU General Public License v3.0**.
> Copyright (c) 2025 - VPR Project
