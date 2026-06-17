SET PATH=D:\App\Keil\Keil Core\ARM\ARMCC\Bin;C:\Program Files\Common Files\Oracle\Java\javapath;D:\App\VMware\bin;C:\WINDOWS\system32;C:\WINDOWS;C:\WINDOWS\System32\Wbem;C:\WINDOWS\System32\WindowsPowerShell\v1.0;C:\WINDOWS\System32\OpenSSH;C:\Program Files\Microsoft SQL Server\150\Tools\Binn;C:\Program Files\Microsoft SQL Server\Client SDK\ODBC\170\Tools\Binn;C:\Program Files\dotnet;D:\Windows Kits\10\Windows Performance Toolkit;C:\Users\31817\AppData\Local\Microsoft\WindowsApps;D:\App\Visual Studio Code\Microsoft VS Code\bin;C:\Users\31817\.dotnet\tools;C:\Program Files\dotnet\;C:\Program Files\Common Files\Autodesk Shared\;D:\App\Node-js\;C:\ProgramData\chocolatey\bin;D:\App\PuTTY\;D:\App\Git\Git\cmd;C:\Program Files\TortoiseGit\bin;D:\App\WubSCP\WinSCP\;C:\Program Files (x86)\STMicroelectronics\STM32 ST-LINK Utility\ST-LINK Utility;C:\Program Files (x86)\ZeroTier\One\;C:\Users\31817\.cargo\bin;C:\Users\31817\Tools\git\cmd;C:\Users\31817\Tools\node\node-v23.9.0-win-x64;C:\Users\31817\Tools\dotnet;d:\App\Cursor\cursor\resources\app\codeBin;C:\Users\31817\AppData\Local\Programs\Python\Python311-32\Scripts\;C:\Users\31817\AppData\Local\Programs\Python\Python311-32\;C:\Users\31817\AppData\Local\Microsoft\WindowsApps;D:\App\Visual Studio Code\Microsoft VS Code\bin;C:\Users\31817\.dotnet\tools;D:\App\Pycharm\PyCharm 2025.2\bin;D:\App\Clion\CLion 2025.2\bin;C:\Users\31817\.lmstudio\bin;C:\Users\31817\AppData\Roaming\npm;C:\Users\31817\.dotnet\tools;C:\Users\31817\AppData\Local\Programs\Ollama;D:\App\AndroidSdk\Sdk\platform-tools;D:\App\AndroidSdk\Sdk\emulator;C:\Users\31817\AppData\Local\Microsoft\WinGet\Packages\BrechtSanders.WinLibs.POSIX.UCRT_Microsoft.Winget.Source_8wekyb3d8bbwe\mingw64\bin;C:\Users\31817\AppData\Local\GitHubDesktop\bin
SET CPU_TYPE=STM32F103C8
SET CPU_VENDOR=STMicroelectronics
SET UV2_TARGET=Test_Project
SET CPU_CLOCK=0x007A1200
"D:\App\Keil\Keil Core\ARM\ARMCC\Bin\ArmAsm" --Via "test_project\startup_stm32f103xb._ia"
"D:\App\Keil\Keil Core\ARM\ARMCC\Bin\ArmCC" --Via "test_project\main.__i"
"D:\App\Keil\Keil Core\ARM\ARMCC\Bin\ArmCC" --Via "test_project\gpio.__i"
"D:\App\Keil\Keil Core\ARM\ARMCC\Bin\ArmCC" --Via "test_project\stm32f1xx_it.__i"
"D:\App\Keil\Keil Core\ARM\ARMCC\Bin\ArmCC" --Via "test_project\stm32f1xx_hal_msp.__i"
"D:\App\Keil\Keil Core\ARM\ARMCC\Bin\ArmCC" --Via "test_project\stm32f1xx_hal_gpio_ex.__i"
"D:\App\Keil\Keil Core\ARM\ARMCC\Bin\ArmCC" --Via "test_project\stm32f1xx_hal.__i"
"D:\App\Keil\Keil Core\ARM\ARMCC\Bin\ArmCC" --Via "test_project\stm32f1xx_hal_rcc.__i"
"D:\App\Keil\Keil Core\ARM\ARMCC\Bin\ArmCC" --Via "test_project\stm32f1xx_hal_rcc_ex.__i"
"D:\App\Keil\Keil Core\ARM\ARMCC\Bin\ArmCC" --Via "test_project\stm32f1xx_hal_gpio.__i"
"D:\App\Keil\Keil Core\ARM\ARMCC\Bin\ArmCC" --Via "test_project\stm32f1xx_hal_dma.__i"
"D:\App\Keil\Keil Core\ARM\ARMCC\Bin\ArmCC" --Via "test_project\stm32f1xx_hal_cortex.__i"
"D:\App\Keil\Keil Core\ARM\ARMCC\Bin\ArmCC" --Via "test_project\stm32f1xx_hal_pwr.__i"
"D:\App\Keil\Keil Core\ARM\ARMCC\Bin\ArmCC" --Via "test_project\stm32f1xx_hal_flash.__i"
"D:\App\Keil\Keil Core\ARM\ARMCC\Bin\ArmCC" --Via "test_project\stm32f1xx_hal_flash_ex.__i"
"D:\App\Keil\Keil Core\ARM\ARMCC\Bin\ArmCC" --Via "test_project\stm32f1xx_hal_exti.__i"
"D:\App\Keil\Keil Core\ARM\ARMCC\Bin\ArmCC" --Via "test_project\system_stm32f1xx.__i"
"D:\App\Keil\Keil Core\ARM\ARMCC\Bin\ArmLink" --Via "Test_Project\Test_Project.lnp"
"D:\App\Keil\Keil Core\ARM\ARMCC\Bin\fromelf.exe" "Test_Project\Test_Project.axf" --i32combined --output "Test_Project\Test_Project.hex"
