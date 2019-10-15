# STM32F429ZI Display Code

This repository was created by [Schulich Racing](https://www.schulichracing.ca/) for its upcoming SR20 racecar. 
- In order to start editing this repository you must first download [Keil](https://www.keil.com/demo/eval/arm.htm). Keil is the IDE that is required to work on this project.
- Second, you may choose to download [STM32CubeMX](https://www.st.com/en/development-tools/stm32cubemx.html) so that you may edit the configuration of the microcontroller. *This is not inherently required to program the display as the codebase has been built already, but is recommended if you wish the edit the configuration of the microcontroller.*
- To start editing the project, first plug in the STM32F429ZI via USB. Make sure that the port you are connecting to on the micro controller is a mini-USB to a USB port on your computer. Then open Keil and click **Project** on the top toolbar and then click **Open Project**. Then open the **STMdisplay.uvprojx** file located in *\STMdisplay\MDK-ARM\STMdisplay.uvprojx*
- From this point on you should be able to edit this project. To compile your code press **F7** on your keyboard, and to load your code onto the microcontroller press **F8**. Your first compilation may take up to 2 minutes, but after that it should only take a few seconds for each compilation after.

### Here are some links to help you get started:
- [BSP Function Documentation](https://documentation.help/STM32F429I-Discovery-BSP-Driver/group__STM32F429I__DISCOVERY__LCD__Exported__Functions.html)
- [LCD-TFT Display Controller Manual](https://www.st.com/content/ccc/resource/technical/document/application_note/group0/25/ca/f9/b4/ae/fc/4e/1e/DM00287603/files/DM00287603.pdf/jcr:content/translations/en.DM00287603.pdf)
- [LCD Image Converter](https://github.com/riuson/lcd-image-converter)
-  [STemWin](https://www.st.com/en/embedded-software/stemwin.html)
- [Getting started with STemWin library](https://www.st.com/content/ccc/resource/technical/document/application_note/54/c9/95/42/8c/0b/43/69/DM00089670.pdf/files/DM00089670.pdf/jcr:content/translations/en.DM00089670.pdf)
> This project was configured by following this [YouTube tutorial.](https://www.youtube.com/watch?v=u_TVAudWabI)
> Happy coding!
