![](./WinCust/WinCust.ico) 
# WinCust
Screen recording software.

Currently only supports .mp4 format.

![Build](https://github.com/Edinazaza/WinCust/actions/workflows/buld-test.yml/badge.svg)

## Technology
Implemented in C++ using WinAPI and Microsoft Media Foundation.

TestDemoWinCust uses GDI+.

For WinCustInstaller used [Microsoft Visual Studio Installer Projects](https://marketplace.visualstudio.com/items?itemName=VisualStudioClient.MicrosoftVisualStudio2017InstallerProjects)

## Interface 
![image](https://user-images.githubusercontent.com/59415567/165858850-3268243b-5917-4c9d-acfa-93824ee22059.png)

![image](https://user-images.githubusercontent.com/59415567/165858873-c2917a56-e173-4d40-bb0a-c0dadd235ee4.png)

https://user-images.githubusercontent.com/59415567/165862703-00d9b283-66a5-40b0-a727-602d25ab3603.mp4

## Project
To get bitmap of the screen use [FrameCreator](./src/FrameCreator)  

[VideoCreator](./src/VideoCreator) uses the Sink Writer to encode video  

[WinCust](./WinCust) includes GUI([CustLine](./WinCust/CustLine.h), [StatusBar](./WinCust/CustBar.h), [BorderHighlighter](./WinCust/BorderHiglighter.h)) and [controller](./WinCust/CustLineController.h) to set up video creation.

[TestDemoWinCust](./TestDemoWinCust) is used to test FrameCreator library



