**Furuta Pendulum — Sliding Mode Controller on STM32**  
Full-stack implementation of a (rotary inverted) pendulum control system: from MATLAB/Simulink model design, through automatic C code generation, to embedded deployment on an STM32 Nucleo-H753ZI via STM32CubeIDE.  
![](data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAnEAAAACCAYAAAA3pIp+AAAABmJLR0QA/wD/AP+gvaeTAAAACXBIWXMAAA7EAAAOxAGVKw4bAAAAMUlEQVR4nO3WAQkAIBAEsBPMYs4PZhMDWMAA5njYUmxU1UqyAwBAF2cmeZE4AIBO7gentgXapSWpbgAAAABJRU5ErkJggg==)  
**Overview**  
The Furuta pendulum is an underactuated mechanical system consisting of a driven horizontal arm and a freely-swinging vertical pendulum. This project implements a **Sliding Mode Controller (SMC)** designed in MATLAB/Simulink (staSmc model), auto-generated as C code using Simulink Coder (R2025a), and deployed to an STM32 microcontroller using Bare-metal C on STM32CubeIDE.  
![](data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAnEAAAACCAYAAAA3pIp+AAAABmJLR0QA/wD/AP+gvaeTAAAACXBIWXMAAA7EAAAOxAGVKw4bAAAANUlEQVR4nO3OQQmAABRAsSeYxKS/kJkED6bwYAVvImwJtszMVu0BAPAXx1rd1fn1BACA164HHDwF+DpPyKwAAAAASUVORK5CYII=)  
**Repository Structure**  
FURUTA/  
 ├── simulink_matlab/           # MATLAB/Simulink source files  
 │   ├── rip.slx                # Simulink model (staSmc — SMC controller)  
 │   └── furuta_pendulum_control.m  # MATLAB script (parameters, tuning)  
 │  
 └── stm32CubeIde/            # Embedded project (STM32)  
     ├── startup/startup_stm32h753zitx.s        # Board: nucleo_h753zi, framework: Bare-metal C 
     └── src/  
         ├── main.cpp           # Encoder reading, control loop, motor driver  
         ├── controller.c       # Simulink-generated SMC implementation  
         ├── controller.h       # Simulink-generated controller interface  
         ├── rtwtypes.h         # Simulink Coder RTW type definitions  
         ├── rtw_continuous.h   # RTW continuous-time support 
         ├── rtw_solver.h       # RTW solver definitions 
         └── etc
   
![](data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAnEAAAACCAYAAAA3pIp+AAAABmJLR0QA/wD/AP+gvaeTAAAACXBIWXMAAA7EAAAOxAGVKw4bAAAANUlEQVR4nO3OsQ1AABRAwSdRaPXGMOCv7WkPK+hEcjfBLTNzVFcAAPzFvVZbdX49AQDgtf0BSpoDXv5TGXgAAAAASUVORK5CYII=)  
**Hardware**  

| **Component** | **Details** |   
| Microcontroller | STM32 Nucleo-H753ZI |   
| Motor driver | BTS7960 (RPWM, LPWM, REN, LEN) |   
| Pendulum encoder | YT3806, 2400 counts/rev |      
   
**Pin mapping:**  
 
| **Signal** |  **Pin** |  
| Pendulum encoder A (YT3806) | PA6 |          
| Pendulum encoder B (YT3806) | PA7 |         
| BTS RPWM | PB6 |         
| BTS LPWM | PB7 |         
| BTS REN | PB10 |         
| BTS LEN | PB11 |            

![](data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAnEAAAACCAYAAAA3pIp+AAAABmJLR0QA/wD/AP+gvaeTAAAACXBIWXMAAA7EAAAOxAGVKw4bAAAANUlEQVR4nO3OsQ1AABRAwSdRaPXGMOCv7WkPK+hEcjfBLTNzVFcAAPzFvVZbdX49AQDgtf0BSpoDXv5TGXgAAAAASUVORK5CYII=)   

**Software Stack**  
- **MATLAB R2025a / Simulink** — controller design and simulation  
- **Simulink Coder  (R2025a)** — automatic C code generation (ERT target, ARM cortexM)  
- **STM32CubeIDE**— build system and firmware upload  
- **Framework** — Bare-Metal C  
![](data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAnEAAAACCAYAAAA3pIp+AAAABmJLR0QA/wD/AP+gvaeTAAAACXBIWXMAAA7EAAAOxAGVKw4bAAAANUlEQVR4nO3OMQ2AABAAsSNBCUrfD6LYGNDAgAU2QtIq6DIzW7UHAMBfHGt1V+fXEwAAXrseHDAF/orRG+cAAAAASUVORK5CYII=)  
**Regenerating Controller Code**  
1. Open simulink_matlab/rip.slx in MATLAB R2025a.  
2. Tune SMC parameters in furuta_pendulum_control.m and the controller.c for implementation.  
3. Run **Build Model** (Ctrl+B) — Simulink Coder generates controller.c and controller.h.  
![](data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAnEAAAACCAYAAAA3pIp+AAAABmJLR0QA/wD/AP+gvaeTAAAACXBIWXMAAA7EAAAOxAGVKw4bAAAANUlEQVR4nO3OQQ2AQBAAsSE5CbzRujLwhwQMYIEfIWkVdJuZozoDAOAvrlWtav96AgDAa/cDEXQEKquakOYAAAAASUVORK5CYII=)  
**License**  
MIT License — see [LICENSE.](LICENSE "LICENSE")  
The auto-generated files (controller.c, controller.h, rtwtypes.h, rtw_continuous.h, rtw_solver.h) carry an **Academic License** as required by MathWorks Simulink Coder and are intended for educational and research use only.  
