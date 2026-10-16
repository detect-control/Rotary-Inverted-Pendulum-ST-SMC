/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: controller.h
 *
 * Code generated for Simulink model 'controller'.
 *
 * Model version                  : 3.18
 * Simulink Coder version         : 25.2 (R2025b) 28-Jul-2025
 * C/C++ source code generated on : Fri Oct 16 20:08:58 2026
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex-M
 * Code generation objectives:
 *    1. Execution efficiency
 *    2. RAM efficiency
 * Validation result: Not run
 */

#ifndef controller_h_
#define controller_h_
#ifndef controller_COMMON_INCLUDES_
#define controller_COMMON_INCLUDES_
#include "rtwtypes.h"
#include "rtw_continuous.h"
#include "rtw_solver.h"
#include "math.h"
#endif                                 /* controller_COMMON_INCLUDES_ */

#include <string.h>
#include <math.h>
#include "zero_crossing_types.h"

/* Macros for accessing real-time model data structure */
#ifndef rtmGetErrorStatus
#define rtmGetErrorStatus(rtm)         ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
#define rtmSetErrorStatus(rtm, val)    ((rtm)->errorStatus = (val))
#endif

#ifndef rtmGetStopRequested
#define rtmGetStopRequested(rtm)       ((rtm)->Timing.stopRequestedFlag)
#endif

#ifndef rtmSetStopRequested
#define rtmSetStopRequested(rtm, val)  ((rtm)->Timing.stopRequestedFlag = (val))
#endif

#ifndef rtmGetStopRequestedPtr
#define rtmGetStopRequestedPtr(rtm)    (&((rtm)->Timing.stopRequestedFlag))
#endif

#ifndef rtmGetT
#define rtmGetT(rtm)                   (rtmGetTPtr((rtm))[0])
#endif

#ifndef rtmGetTPtr
#define rtmGetTPtr(rtm)                ((rtm)->Timing.t)
#endif

#ifndef rtmGetTStart
#define rtmGetTStart(rtm)              ((rtm)->Timing.tStart)
#endif

/* Forward declaration for rtModel */
typedef struct tag_RTM RT_MODEL;

/* Block signals and states (default storage) for system '<Root>' */
typedef struct {
  real_T Probe[2];                     /* '<S4>/Probe' */
  real_T AvoidDividebyZero;            /* '<S4>/Avoid Divide by Zero' */
  real_T Integrator;                   /* '<S8>/Integrator' */
  real_T uT;                           /* '<S2>/1//T' */
  real_T sign_s;                       /* '<S1>/smc' */
  real_T Integrator_DSTATE;            /* '<S8>/Integrator' */
  int8_T Integrator_PrevResetState;    /* '<S8>/Integrator' */
  uint8_T Integrator_IC_LOADING;       /* '<S8>/Integrator' */
} DW;

/* Continuous states (default storage) */
typedef struct {
  real_T Integrator_CSTATE;            /* '<S1>/Integrator' */
  real_T Integrator1_CSTATE;           /* '<S1>/Integrator1' */
} X;

/* Periodic continuous state vector (global) */
typedef int_T PeriodicIndX[1];
typedef real_T PeriodicRngX[2];

/* State derivatives (default storage) */
typedef struct {
  real_T Integrator_CSTATE;            /* '<S1>/Integrator' */
  real_T Integrator1_CSTATE;           /* '<S1>/Integrator1' */
} XDot;

/* State disabled  */
typedef struct {
  boolean_T Integrator_CSTATE;         /* '<S1>/Integrator' */
  boolean_T Integrator1_CSTATE;        /* '<S1>/Integrator1' */
} XDis;

/* Zero-crossing (trigger) state */
typedef struct {
  ZCSigState Integrator_Reset_ZCE;     /* '<S1>/Integrator' */
} PrevZCX;

#ifndef ODE4_INTG
#define ODE4_INTG

/* ODE4 Integration Data */
typedef struct {
  real_T *y;                           /* output */
  real_T *f[4];                        /* derivatives */
} ODE4_IntgData;

#endif

/* External inputs (root inport signals with default storage) */
typedef struct {
  real_T theta;                        /* '<Root>/theta' */
} ExtU;

/* External outputs (root outports fed by signals with default storage) */
typedef struct {
  real_T control;                      /* '<Root>/control' */
  real_T thetaDot;                     /* '<Root>/thetaDot' */
} ExtY;

/* Real-time Model Data Structure */
struct tag_RTM {
  const char_T *errorStatus;
  RTWSolverInfo solverInfo;
  X *contStates;
  int_T *periodicContStateIndices;
  real_T *periodicContStateRanges;
  real_T *derivs;
  XDis *contStateDisabled;
  boolean_T zCCacheNeedsReset;
  boolean_T derivCacheNeedsReset;
  boolean_T CTOutputIncnstWithState;
  real_T odeY[2];
  real_T odeF[4][2];
  ODE4_IntgData intgData;

  /*
   * Sizes:
   * The following substructure contains sizes information
   * for many of the model attributes such as inputs, outputs,
   * dwork, sample times, etc.
   */
  struct {
    int_T numContStates;
    int_T numPeriodicContStates;
    int_T numSampTimes;
  } Sizes;

  /*
   * Timing:
   * The following substructure contains information regarding
   * the timing information for the model.
   */
  struct {
    uint32_T clockTick0;
    time_T stepSize0;
    uint32_T clockTick1;
    time_T tStart;
    SimTimeStep simTimeStep;
    boolean_T stopRequestedFlag;
    time_T *t;
    time_T tArray[2];
  } Timing;
};

/* Continuous states (default storage) */
extern X rtX;

/* Disabled states (default storage) */
extern XDis rtXDis;

/* Block signals and states (default storage) */
extern DW rtDW;

/* Zero-crossing (trigger) state */
extern PrevZCX rtPrevZCX;

/* External inputs (root inport signals with default storage) */
extern ExtU rtU;

/* External outputs (root outports fed by signals with default storage) */
extern ExtY rtY;

/* Model entry point functions */
extern void controller_initialize(void);
extern void controller_step(void);

/* Real-time Model object */
extern RT_MODEL *const rtM;

/*-
 * These blocks were eliminated from the model due to optimizations:
 *
 * Block '<S2>/Gain' : Eliminated nontunable gain of 1
 * Block '<S8>/Saturation' : Eliminated Saturate block
 * Block '<S2>/[A,B]' : Eliminated Saturate block
 */

/*-
 * The generated code includes comments that allow you to trace directly
 * back to the appropriate location in the model.  The basic format
 * is <system>/block_name, where system is the system number (uniquely
 * assigned by Simulink) and block_name is the name of the block.
 *
 * Note that this particular code originates from a subsystem build,
 * and has its own system numbers different from the parent model.
 * Refer to the system hierarchy for this subsystem below, and use the
 * MATLAB hilite_system command to trace the generated code back
 * to the parent model.  For example,
 *
 * hilite_system('rip/controller')    - opens subsystem rip/controller
 * hilite_system('rip/controller/Kp') - opens and selects block Kp
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'rip'
 * '<S1>'   : 'rip/controller'
 * '<S2>'   : 'rip/controller/Filtered Derivative (Discrete or Continuous)'
 * '<S3>'   : 'rip/controller/smc'
 * '<S4>'   : 'rip/controller/Filtered Derivative (Discrete or Continuous)/Enable//disable time constant'
 * '<S5>'   : 'rip/controller/Filtered Derivative (Discrete or Continuous)/Initialization'
 * '<S6>'   : 'rip/controller/Filtered Derivative (Discrete or Continuous)/Integrator (Discrete or Continuous)'
 * '<S7>'   : 'rip/controller/Filtered Derivative (Discrete or Continuous)/Initialization/Init_u'
 * '<S8>'   : 'rip/controller/Filtered Derivative (Discrete or Continuous)/Integrator (Discrete or Continuous)/Discrete'
 */
#endif                                 /* controller_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
