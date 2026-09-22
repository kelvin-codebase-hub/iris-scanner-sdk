# Iris Scanner x64 source

Build from this folder in a Visual Studio Developer PowerShell:

```powershell
msbuild .\IrisScanner.SdkCheck.sln /restore /t:Build /p:Configuration=Release /p:Platform=x64 /p:VendorSdkRoot=.\sdk
```

The package includes the corrected C++/CLI bridge, the C# SDK check project,
the required x64 vendor runtime DLLs, `IKConfig.ini`, vendor headers, and
`demo-src\IKSDKAPI.lib`.

For M10 automatic face/PTZ tracking, keep these settings in `sdk\IKConfig.ini`:

```ini
[PTZControl]
PTZEnable=1
detectTraceNum=30
trackNums=5
```

The bridge initializes both the iris and face processors.
