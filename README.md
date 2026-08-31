# MuteNet

MuteNet is a cross-platform, high-performance asynchronous network library built in C++. It provides unified socket abstractions and asynchronous I/O multiplexing interfaces for multiple operating systems (Windows, Linux, macOS/BSD).

---

## 📂 Project Structure

The project has the following directory structure:

1. **`MuteNet/` (Core Library)**
   - Unified interface for sockets (`ISocket.h`, `Socket.h`) with platform-specific implementations:
     - Windows: `WinSocketImpl` (using IOCP)
     - Linux: `LinuxSocketImpl` / `UnixSocketImpl`
   - Asynchronous I/O Dispatcher interface (`IIODispatcher.h`, `IODispatcher.h`):
     - Windows completion ports: `WinIODispatcherImpl`
     - Linux I/O multiplexing: `EpollIODispatcherImpl`
     - macOS/BSD I/O multiplexing: `KQueueIODispatcherImpl`
   - Asynchronous requests matching platforms: `ASyncAcceptRequest`, `ASyncRecvRequest`, `ASyncSendRequest`, `ASyncIORequest`.
   - Utility and Crash reporter: `MemoryResource` allocator, custom Stream Buffers, and `MiniDump` crash dump collector powered by `breakpad`.
   - Dependency management: standard `vcpkg.json` targeting Microsoft `vcpkg` package manager.

2. **`ModuleTest/` (Unit Tests)**
   - Googletest module tests verifying the functionality of:
     - Memory allocators (`AllocatorTest`)
     - I/O dispatchers (`DispatcherTest`)
     - MiniDump crash handlers (`MiniDumpTest`)
     - Network connections and sockets (`NetworkTest`, `SocketTest`, `StreamBufferTest`)
   - Configured with `packages.config` managed via NuGet.

3. **`tools/` (Utility Scripts)**
   - PowerShell scripts such as `migrate_files.ps1` for merging / migrating target codebase files.

4. **`MuteNet.slnx`**
   - The modern Visual Studio 2022 solution file referencing both `MuteNet` library and `ModuleTest` projects.

---

## 🚀 Git Configuration (Branch `26`)

The repository files on branch `26` have been filtered to ensure only clean source/project files are pushed.

### 1. Pushed Files (Tracked)
- All C++ files (`.cpp`, `.h`)
- Solution and project files (`.slnx`, `.vcxproj`, `.vcxproj.filters`)
- Dependency rules & manifests (`vcpkg.json`, `packages.config`)
- Tool scripts and markdown documentations (`migrate_files.ps1`, `TOOLS_README_MIGRATE.md`, `README.md`)

### 2. Gitignored Files (Untracked)
The following directories and files are excluded in `.gitignore` to avoid bloated build artifacts and local developer lockups:
- **Build Artifacts & Outputs**: `x64/`, `Debug/`, `Release/`, and compilation-generated files (`*.obj`, `*.pdb`, `*.ilk`, `*.recipe`, `*.log`, `*.tlog`, `*.exe`, `*.dll`).
- **VS Settings & Cache**: `.vs/` workspace folder, `*.user` (locally tailored build properties), and `*.suo` / `*.db` / `*.opendb` files.
- **Dependency Packages**: `packages/` (automatically restored via NuGet) and `vcpkg_installed/` (locally compiled dependencies).
- **Zipped Backups**: `ModuleTest.zip` (original project zip).

---

## 🛠️ Build and Dependencies

### Prerequisites
- **Visual Studio 2022** or newer (recommending the Modern Solution `.slnx` extension enabled).
- **vcpkg** (Microsoft C++ Library Manager) integrated with Visual Studio:
  - Required package: `breakpad` (used for minidumps).
- **Googletest** (installed automatically via NuGet package manager restore).
