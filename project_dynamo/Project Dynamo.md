# How to build

- The following direction should work for VS Code
- Open the folder "project dynamo" as a project in PlatformIO
- Flash the .cpp program onto the ESP with the PlatformIO upload tool
- Right-click the website-folder and perform the NPM build task "build"
- Flash the website data to SPIFFS storage with "pio run -t uploadfs"
