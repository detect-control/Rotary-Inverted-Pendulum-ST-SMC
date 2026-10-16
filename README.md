**Furuta Pendulum — Sliding Mode Controller on STM32**  
Full-stack implementation of a Furuta (rotary inverted) pendulum control system: from MATLAB/Simulink model design, through automatic C code generation, to embedded deployment on an STM32 Nucleo-H753ZI via PlatformIO.  
![](data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAnEAAAACCAYAAAA3pIp+AAAABmJLR0QA/wD/AP+gvaeTAAAACXBIWXMAAA7EAAAOxAGVKw4bAAAAMUlEQVR4nO3WAQkAIBAEsBPMYs4PZhMDWMAA5njYUmxU1UqyAwBAF2cmeZE4AIBO7gentgXapSWpbgAAAABJRU5ErkJggg==)  
**Overview**  
The Furuta pendulum is an underactuated mechanical system consisting of a driven horizontal arm and a freely-swinging vertical pendulum. This project implements a **Sliding Mode Controller (SMC)** designed in MATLAB/Simulink (staSmc model), auto-generated as C code using Simulink Coder (R2024b), and deployed to an STM32 microcontroller using the Arduino framework via PlatformIO.  
![](data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAnEAAAACCAYAAAA3pIp+AAAABmJLR0QA/wD/AP+gvaeTAAAACXBIWXMAAA7EAAAOxAGVKw4bAAAANUlEQVR4nO3OQQmAABRAsSeYxKS/kJkED6bwYAVvImwJtszMVu0BAPAXx1rd1fn1BACA164HHDwF+DpPyKwAAAAASUVORK5CYII=)  
**Repository Structure**  
FURUTA/  
 ├── simulink_matlab/           # MATLAB/Simulink source files  
 │   ├── rip.slx                # Simulink model (staSmc — SMC controller)  
 │   └── furuta_pendulum_control.m  # MATLAB script (parameters, tuning)  
 │  
 └── platformio_rip/            # PlatformIO embedded project (STM32)  
     ├── platformio.ini         # Board: nucleo_h753zi, framework: arduino  
     └── src/  
         ├── main.cpp           # Encoder reading, control loop, motor driver  
         ├── controller.c       # Simulink-generated SMC implementation  
         ├── controller.h       # Simulink-generated controller interface  
         ├── rtwtypes.h         # Simulink Coder RTW type definitions  
         ├── rtw_continuous.h   # RTW continuous-time support  
         └── rtw_solver.h       # RTW solver definitions  
   
![](data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAnEAAAACCAYAAAA3pIp+AAAABmJLR0QA/wD/AP+gvaeTAAAACXBIWXMAAA7EAAAOxAGVKw4bAAAANUlEQVR4nO3OsQ1AABRAwSdRaPXGMOCv7WkPK+hEcjfBLTNzVFcAAPzFvVZbdX49AQDgtf0BSpoDXv5TGXgAAAAASUVORK5CYII=)  
**Hardware**  
| | |  
|-|-|  
| **Component** | **Details** |   
| Microcontroller | STM32 Nucleo-H753ZI |   
| Motor driver | BTS7960 (RPWM, LPWM, REN, LEN) |   
| Pendulum encoder | YT3806, 2400 counts/rev (pins 7, 10) |   
| Serial baud rate | 115200 |   
   
**Pin mapping:**  
| | |  
|-|-|  
| **Signal** | **Pin** |   
| Motor encoder A | 2 |   
| Motor encoder B | 3 |   
| Pendulum encoder A (YT3806) | 7 |   
| Pendulum encoder B (YT3806) | 10 |   
| BTS RPWM | 5 |   
| BTS LPWM | 6 |   
| BTS REN | 9 |   
| BTS LEN | 8 |   
   
![](data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAnEAAAACCAYAAAA3pIp+AAAABmJLR0QA/wD/AP+gvaeTAAAACXBIWXMAAA7EAAAOxAGVKw4bAAAANUlEQVR4nO3OMQ2AABAAsSPBCj5fFyM6mJHAjAU2QtIq6DIzW7UHAMBfnGt1V8fXEwAAXrsexOEF35f1aEgAAAAASUVORK5CYII=)  
**Software Stack**  
- **MATLAB R2024b / Simulink** — controller design and simulation  
- **Simulink Coder  (R2025a)** — automatic C code generation (ERT target, ARM cortexM)  
- **STM32CubeIDE **— build system and firmware upload  
- **Framework** — Bare-Metal C  
![](data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAnEAAAACCAYAAAA3pIp+AAAABmJLR0QA/wD/AP+gvaeTAAAACXBIWXMAAA7EAAAOxAGVKw4bAAAANUlEQVR4nO3OMQ2AABAAsSNBCUrfD6LYGNDAgAU2QtIq6DIzW7UHAMBfHGt1V+fXEwAAXrseHDAF/orRG+cAAAAASUVORK5CYII=)  
**Regenerating Controller Code**  
1. Open simulink_matlab/rip.slx in MATLAB R2024b.  
2. Tune SMC parameters in furuta_pendulum_control.m.  
3. Run **Build Model** (Ctrl+B) — Simulink Coder generates controller.c and controller.h.  
4. Copy generated files into platformio_rip/src/.  
![](data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAnEAAAACCAYAAAA3pIp+AAAABmJLR0QA/wD/AP+gvaeTAAAACXBIWXMAAA7EAAAOxAGVKw4bAAAANUlEQVR4nO3OQQ2AQBAAsSE5CbzRujLwhwQMYIEfIWkVdJuZozoDAOAvrlWtav96AgDAa/cDEXQEKquakOYAAAAASUVORK5CYII=)  
**License**  
MIT License — see [LICENSE.](LICENSE "LICENSE")  
The auto-generated files (controller.c, controller.h, rtwtypes.h, rtw_continuous.h, rtw_solver.h) carry an **Academic License** as required by MathWorks Simulink Coder and are intended for educational and research use only.  
