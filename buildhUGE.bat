:: Assemble the hUGEDriver source into an RGBDS object file
..\..\tools\hUGETracker_1_0b9\rgbasm.exe -o hUGEDriver.obj -i ..\..\tools\hUGETracker_1_0b9\hUGEDriver\ ..\..\tools\hUGETracker_1_0b9\hUGEDriver\hUGEDriver.asm

:: Convert the RGBDS object file into a GBDK object file
..\..\tools\hUGETracker_1_0b9\hUGEDriver\gbdk_example\rgb2sdas.exe hUGEDriver.obj