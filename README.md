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

**macOS / Linux**
```bash
mkdir build
cmake -B build -S .
cmake --build build
./build/main_app
```

**Windows**
```bash
mkdir build
cmake -B build -S .
cmake --build build --config Release
./build/Release/main_app.exe
```

## IDE

1.  Open the project's root folder in a CMake-compatible IDE (e.g., CLion, VS Code with CMake Tools, Visual Studio).
2.  Select `main_app` as the target.
3.  Click the "Run" button.
