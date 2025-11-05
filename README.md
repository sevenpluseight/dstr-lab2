# Branch Overview

## ⚠️ Important  
Do **NOT** push directly to the `main` or `dev` branches.  
All changes should be committed to your personal branch first then merged via pull request.

| Branch | Belongs to                  | Description                                                                  |
|:-------|:----------------------------|:-----------------------------------------------------------------------------|
| `main` | -                           | Stable branch containing the final and reviewed code                         |
| `dev`  | -                           | Development branch used for integration and testing before merging to `main` |
| `sp`   | Elysha Sophia binti Ridzuan | Use `git checkout sp` to switch                                              |
| `pr`   | Hew Pik Rou                 | Use `git checkout pr` to switch                                              |
| `wh`   | Lai Wei Heng                | Use `git checkout wh` to switch                                              |
| `wl`   | Low Wei Ling                | Use `git checkout wl` to switch                                              |

## Additional Notes
- ✅ Push your commits often — this prevents bugs, merge conflicts and compilation issues later.
- 🔄 Pull frequently to stay up-to-date with others’ changes.
- ⚙️ If you have uncommitted changes but need to pull:
```bash
git stash
git pull <branch> # Usually from the main branch
git stash apply 
```

---

# User Credentials

| Role                         | Username | Password |
| :--------------------------- | :------- | :------- |
| PatientAdmissionClerk        | pac1     | pac1     |
| PatientAdmissionClerk        | pac2     | pac2     |
| MedicalSupplyManager         | msm1     | msm1     |
| MedicalSupplyManager         | msm2     | msm2     |
| EmergencyDepartmentOfficer   | eod1     | eod1     |
| EmergencyDepartmentOfficer   | eod2     | eod2     |
| AmbulanceDispatcher          | ab1      | ab1      |
| AmbulanceDispatcher          | ab2      | ab2      |

---

# How to Build and Run

## Command-Line

Before building, ensure you have CMake and a C++ compiler (like GCC for Linux/macOS or MSVC for Windows) installed and configured.

**macOS / Linux**
```bash
rm -rf build # Clean previous build (if any)
mkdir build
cmake -B build -S .
cmake --build build
./build/main_app
```

**Note:** The `data` directory and its necessary CSV files will be automatically created/copied into the `build/data` directory during the build process.

**Windows**
```bash
rmdir /s /q build # Clean previous build (if any)
mkdir build
cmake -B build -S .
cmake --build build --config Release
.\build\Release\main_app.exe
.\build\main_app.exe # Run this if the command above doesn't work
```

**Note for Windows users:** The path to the executable might differ based on your CMake generator. If the above command fails, look for `main_app.exe` inside the `build` directory. It might be in `build\main_app.exe` or `build\Debug\main_app.exe`.

### Why this might happen

The exact location of the compiled application (`main_app.exe`) can vary depending on the tools you have installed on your Windows machine. CMake uses a "generator" to create the project files for a specific build system (like Visual Studio or Ninja).

Some generators create configuration-specific folders (e.g., `build\Release` or `build\Debug`), while others place the executable directly in the build folder (e.g., `build\`). The updated instructions and note are designed to help you locate the executable regardless of the generator used.

## IDE

1.  Open the project's root folder in a CMake-compatible IDE (e.g., CLion, VS Code with CMake Tools, Visual Studio).
2.  Select `main_app` as the target.
3.  Click the "Run" button.
