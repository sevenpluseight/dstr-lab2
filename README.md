# Contribution

| Role                       | Name           | TP Number |
| :------------------------- | :------------- |:----------|
| Patient Admission Clerk    | Lai Wei Heng   | TP081018  |
| Medical Supply Manager     | Hew Pik Rou    | TP071195  |
| Emergency Department Officer | Low Wei Ling   | TP080089  |
| Ambulance Dispatcher       | Elysha Sophia binti Ridzuan | TP071162  |

---
# Links

1. [Full Presentation Link - Google Drive](https://drive.google.com/drive/folders/1INx0JHEj6UZ7IICKHKAYslDxfDiHYpWW?usp=drive_link)
2. [GitHub](https://github.com/sevenpluseight/dstr-lab2)

---

# User Credentials

| Role                         | Username | Password |
| :--------------------------- | :------- | :------- |
| PatientAdmissionClerk        | pac1     | pac1     |
| MedicalSupplyManager         | msm1     | msm1     |
| EmergencyDepartmentOfficer   | eod1     | eod1     |
| AmbulanceDispatcher          | ab1      | ab1      |

---

# How to Build and Run

Ensure you have CMake and a C++ compiler installed.

## macOS / Linux

```bash
rm -rf build # Clean previous build (if any)
mkdir build
cmake -B build -S .
cmake --build build
./build/main_app
```

## Windows

```bash
rm -rf build # Clean previous build (if any)
mkdir build
cmake -B build -S .
cmake --build build --config Release
.\build\Release\main_app.exe
.\build\main_app.exe # Run this if .\build\Release\main_app.exe doesn't work
```

**Note for Windows users:** The path to the executable might differ based on your CMake generator. If the above command fails, look for `main_app.exe` inside the `build` directory. It might be in `build\main_app.exe` or `build\Debug\main_app.exe`.

### Why this might happen

The exact location of the compiled application (`main_app.exe`) can vary depending on the tools you have installed on your Windows machine. CMake uses a "generator" to create the project files for a specific build system (like Visual Studio or Ninja).

Some generators create configuration-specific folders (e.g., `build\Release` or `build\Debug`), while others place the executable directly in the build folder (e.g., `build\`).
