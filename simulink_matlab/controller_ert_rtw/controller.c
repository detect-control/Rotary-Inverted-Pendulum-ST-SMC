/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: controller.c
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

#include "controller.h"
#include <math.h>
#include "rtwtypes.h"
#include "zero_crossing_types.h"
#include "solver_zc.h"
#include "math.h"
#ifndef slZcHadEvent
#define slZcHadEvent(ev, zcsDir)       (((ev) & (zcsDir)) != 0x00 )
#endif

#ifndef slZcUnAliasEvents
#define slZcUnAliasEvents(evL, evR)    ((((slZcHadEvent((evL), (SL_ZCS_EVENT_N2Z)) && slZcHadEvent((evR), (SL_ZCS_EVENT_Z2P))) || (slZcHadEvent((evL), (SL_ZCS_EVENT_P2Z)) && slZcHadEvent((evR), (SL_ZCS_EVENT_Z2N)))) ? (SL_ZCS_EVENT_NUL) : (evR)))
#endif

/* Private macros used by the generated code to access rtModel */
#ifndef rtmIsMajorTimeStep
#define rtmIsMajorTimeStep(rtm)        (((rtm)->Timing.simTimeStep) == MAJOR_TIME_STEP)
#endif

#ifndef rtmIsMinorTimeStep
#define rtmIsMinorTimeStep(rtm)        (((rtm)->Timing.simTimeStep) == MINOR_TIME_STEP)
#endif

#ifndef rtmSetTPtr
#define rtmSetTPtr(rtm, val)           ((rtm)->Timing.t = (val))
#endif

/* Continuous states */
X rtX;

/* Disabled State Vector */
XDis rtXDis;

/* Block signals and states (default storage) */
DW rtDW;

/* Previous zero-crossings (trigger) states */
PrevZCX rtPrevZCX;

/* External inputs (root inport signals with default storage) */
ExtU rtU;

/* External outputs (root outports fed by signals with default storage) */
ExtY rtY;

/* Periodic continuous states */
PeriodicIndX rtPeriodicIndX;
PeriodicRngX rtPeriodicRngX;

/* Real-time model */
static RT_MODEL rtM_;
RT_MODEL *const rtM = &rtM_;

/* private model entry point functions */
extern void controller_derivatives(void);
static ZCEventType rt_ZCFcn(ZCDirection zcDir, ZCSigState *prevZc, real_T
  currValue);
extern real_T rtInf;
extern real_T rtMinusInf;
extern real_T rtNaN;
extern real32_T rtInfF;
extern real32_T rtMinusInfF;
extern real32_T rtNaNF;
static boolean_T rtIsInf(real_T value);
static boolean_T rtIsInfF(real32_T value);
static boolean_T rtIsNaN(real_T value);
static boolean_T rtIsNaNF(real32_T value);
real_T rtNaN = -(real_T)NAN;
real_T rtInf = (real_T)INFINITY;
real_T rtMinusInf = -(real_T)INFINITY;
real32_T rtNaNF = -(real32_T)NAN;
real32_T rtInfF = (real32_T)INFINITY;
real32_T rtMinusInfF = -(real32_T)INFINITY;

/* Detect zero crossings events. */
static ZCEventType rt_ZCFcn(ZCDirection zcDir, ZCSigState *prevZc, real_T
  currValue)
{
  slZcEventType zcsDir;
  slZcEventType tempEv;
  ZCEventType zcEvent = NO_ZCEVENT;    /* assume */

  /* zcEvent matrix */
  static const slZcEventType eventMatrix[4][4] = {
    /*          ZER              POS              NEG              UNK */
    { SL_ZCS_EVENT_NUL, SL_ZCS_EVENT_Z2P, SL_ZCS_EVENT_Z2N, SL_ZCS_EVENT_NUL },/* ZER */

    { SL_ZCS_EVENT_P2Z, SL_ZCS_EVENT_NUL, SL_ZCS_EVENT_P2N, SL_ZCS_EVENT_NUL },/* POS */

    { SL_ZCS_EVENT_N2Z, SL_ZCS_EVENT_N2P, SL_ZCS_EVENT_NUL, SL_ZCS_EVENT_NUL },/* NEG */

    { SL_ZCS_EVENT_NUL, SL_ZCS_EVENT_NUL, SL_ZCS_EVENT_NUL, SL_ZCS_EVENT_NUL }/* UNK */
  };

  /* get prevZcEvent and prevZcSign from prevZc */
  const slZcEventType prevEv = (slZcEventType)(((uint8_T)(*prevZc)) >> 2);
  const slZcSignalSignType prevSign = (slZcSignalSignType)(((uint8_T)(*prevZc))
    & (uint8_T)0x03);

  /* get current zcSignal sign from current zcSignal value */
  const slZcSignalSignType currSign = (slZcSignalSignType)((currValue) > 0.0 ?
    SL_ZCS_SIGN_POS :
    ((currValue) < 0.0 ? SL_ZCS_SIGN_NEG : SL_ZCS_SIGN_ZERO));

  /* get current zcEvent based on prev and current zcSignal value */
  slZcEventType currEv = eventMatrix[prevSign][currSign];

  /* get slZcEventType from ZCDirection */
  switch (zcDir) {
   case ANY_ZERO_CROSSING:
    zcsDir = SL_ZCS_EVENT_ALL;
    break;

   case FALLING_ZERO_CROSSING:
    zcsDir = SL_ZCS_EVENT_ALL_DN;
    break;

   case RISING_ZERO_CROSSING:
    zcsDir = SL_ZCS_EVENT_ALL_UP;
    break;

   default:
    zcsDir = SL_ZCS_EVENT_NUL;
    break;
  }

  /* had event, check if zc happened */
  if (slZcHadEvent(currEv, zcsDir)) {
    currEv = (slZcEventType)(slZcUnAliasEvents(prevEv, currEv));
  } else {
    currEv = SL_ZCS_EVENT_NUL;
  }

  /* Update prevZc */
  tempEv = (slZcEventType)(currEv << 2);/* shift left by 2 bits */
  *prevZc = (ZCSigState)((currSign) | (tempEv));
  if ((currEv & SL_ZCS_EVENT_ALL_DN) != 0) {
    zcEvent = FALLING_ZCEVENT;
  } else if ((currEv & SL_ZCS_EVENT_ALL_UP) != 0) {
    zcEvent = RISING_ZCEVENT;
  } else {
    zcEvent = NO_ZCEVENT;
  }

  return zcEvent;
}                                      /* rt_ZCFcn */

/* State reduction function */
void local_stateReduction(real_T* x, int_T* p, int_T n, real_T* r)
{
  int_T i, j;
  for (i = 0, j = 0; i < n; ++i, ++j) {
    int_T k = p[i];
    real_T lb = r[j++];
    real_T xk = x[k]-lb;
    real_T rk = r[j]-lb;
    int_T q = (int_T) floor(xk/rk);
    if (q) {
      x[k] = xk-q*rk+lb;
    }
  }
}

/*
 * This function updates continuous states using the ODE4 fixed-step
 * solver algorithm
 */
static void rt_ertODEUpdateContinuousStates(RTWSolverInfo *si )
{
  time_T t = rtsiGetT(si);
  time_T tnew = rtsiGetSolverStopTime(si);
  time_T h = rtsiGetStepSize(si);
  real_T *x = rtsiGetContStates(si);
  ODE4_IntgData *id = (ODE4_IntgData *)rtsiGetSolverData(si);
  real_T *y = id->y;
  real_T *f0 = id->f[0];
  real_T *f1 = id->f[1];
  real_T *f2 = id->f[2];
  real_T *f3 = id->f[3];
  real_T temp;
  int_T i;
  int_T nXc = 2;
  rtsiSetSimTimeStep(si,MINOR_TIME_STEP);

  /* Save the state values at time t in y, we'll use x as ynew. */
  (void) memcpy(y, x,
                (uint_T)nXc*sizeof(real_T));

  /* Assumes that rtsiSetT and ModelOutputs are up-to-date */
  /* f0 = f(t,y) */
  rtsiSetdX(si, f0);
  controller_derivatives();

  /* f1 = f(t + (h/2), y + (h/2)*f0) */
  temp = 0.5 * h;
  for (i = 0; i < nXc; i++) {
    x[i] = y[i] + (temp*f0[i]);
  }

  rtsiSetT(si, t + temp);
  rtsiSetdX(si, f1);
  controller_step();
  controller_derivatives();

  /* f2 = f(t + (h/2), y + (h/2)*f1) */
  for (i = 0; i < nXc; i++) {
    x[i] = y[i] + (temp*f1[i]);
  }

  rtsiSetdX(si, f2);
  controller_step();
  controller_derivatives();

  /* f3 = f(t + h, y + h*f2) */
  for (i = 0; i < nXc; i++) {
    x[i] = y[i] + (h*f2[i]);
  }

  rtsiSetT(si, tnew);
  rtsiSetdX(si, f3);
  controller_step();
  controller_derivatives();

  /* tnew = t + h
     ynew = y + (h/6)*(f0 + 2*f1 + 2*f2 + 2*f3) */
  temp = h / 6.0;
  for (i = 0; i < nXc; i++) {
    x[i] = y[i] + temp*(f0[i] + 2.0*f1[i] + 2.0*f2[i] + f3[i]);
  }

  local_stateReduction(rtsiGetContStates(si), rtsiGetPeriodicContStateIndices(si),
                       1,
                       rtsiGetPeriodicContStateRanges(si));
  rtsiSetSimTimeStep(si,MAJOR_TIME_STEP);
}

/* Model step function */
void controller_step(void)
{
  real_T rtb_MinMax;
  real_T sign_s;
  boolean_T tmp;
  ZCEventType zcEvent;
  if (rtmIsMajorTimeStep(rtM)) {
    /* set solver stop time */
    rtsiSetSolverStopTime(&rtM->solverInfo,((rtM->Timing.clockTick0+1)*
      rtM->Timing.stepSize0));
  }                                    /* end MajorTimeStep */

  /* Update absolute time of base rate at minor time step */
  if (rtmIsMinorTimeStep(rtM)) {
    rtM->Timing.t[0] = rtsiGetT(&rtM->solverInfo);
  }

  tmp = rtmIsMajorTimeStep(rtM);
  if (tmp) {
    /* Fcn: '<S4>/Avoid Divide by Zero' incorporates:
     *  Constant: '<S4>/Time constant'
     *  Gain: '<S4>/Minimum sampling to time constant ratio'
     *  MinMax: '<S4>/MinMax'
     */
    rtDW.AvoidDividebyZero = fmax(10.0 * rtDW.Probe[0], 0.02);

    /* DiscreteIntegrator: '<S8>/Integrator' incorporates:
     *  Inport: '<Root>/theta'
     */
    if (rtDW.Integrator_IC_LOADING != 0) {
      rtDW.Integrator_DSTATE = rtU.theta;
    }

    if (rtDW.Integrator_PrevResetState != 0) {
      rtDW.Integrator_DSTATE = rtU.theta;
    }

    /* DiscreteIntegrator: '<S8>/Integrator' */
    rtDW.Integrator = rtDW.Integrator_DSTATE;
  }

  /* Product: '<S2>/1//T' incorporates:
   *  Inport: '<Root>/theta'
   *  Sum: '<S2>/Sum1'
   */
  rtDW.uT = 1.0 / rtDW.AvoidDividebyZero * (rtU.theta - rtDW.Integrator);

  /* Integrator: '<S1>/Integrator' incorporates:
   *  Inport: '<Root>/theta'
   */
  if (rtsiIsModeUpdateTimeStep(&rtM->solverInfo)) {
    zcEvent = rt_ZCFcn(ANY_ZERO_CROSSING,&rtPrevZCX.Integrator_Reset_ZCE,
                       (rtU.theta));

    /* evaluate zero-crossings */
    if (zcEvent != NO_ZCEVENT) {
      rtX.Integrator_CSTATE = 0.0;
    }
  }

  /* Integrator: '<S1>/Integrator1' */
  /* Limited  Integrator  */
  if (rtX.Integrator1_CSTATE >= 2.0) {
    rtX.Integrator1_CSTATE = 2.0;
  } else if (rtX.Integrator1_CSTATE <= -2.0) {
    rtX.Integrator1_CSTATE = -2.0;
  }

  if (tmp) {
    /* MATLAB Function: '<S1>/smc' incorporates:
     *  Inport: '<Root>/theta'
     *  Integrator: '<S1>/Integrator'
     *  Integrator: '<S1>/Integrator1'
     */
    rtb_MinMax = (8.0 * rtU.theta + rtDW.uT) + 25.0 * rtX.Integrator_CSTATE;
    sign_s = tanh(rtb_MinMax / 0.1);
    rtDW.sign_s = sign_s;
    if (fabs(rtU.theta) >= 0.5) {
      rtb_MinMax = ((rtDW.uT * rtDW.uT * 0.00187395 + 0.23720580000000002 * cos
                     (rtU.theta)) - 0.23720580000000002) * -2.0 * rtDW.uT;
    } else {
      rtb_MinMax = (((-((rtDW.uT * rtDW.uT * -0.0007 + 1.5303600000000002 * sin
                         (rtU.theta)) / 0.02418) - 8.0 * rtDW.uT) - 25.0 *
                     rtU.theta) + (-8.0 * sqrt(fabs(rtb_MinMax)) * sign_s - 6.0 *
        rtX.Integrator1_CSTATE)) * 0.02418;
    }

    /* Saturate: '<S1>/Saturation' incorporates:
     *  MATLAB Function: '<S1>/smc'
     */
    if (rtb_MinMax > 2.0) {
      /* Outport: '<Root>/control' */
      rtY.control = 2.0;
    } else if (rtb_MinMax < -2.0) {
      /* Outport: '<Root>/control' */
      rtY.control = -2.0;
    } else {
      /* Outport: '<Root>/control' */
      rtY.control = rtb_MinMax;
    }

    /* End of Saturate: '<S1>/Saturation' */
  }

  /* Outport: '<Root>/thetaDot' */
  rtY.thetaDot = rtDW.uT;
  if (rtmIsMajorTimeStep(rtM)) {
    if (rtmIsMajorTimeStep(rtM)) {
      /* Update for DiscreteIntegrator: '<S8>/Integrator' */
      rtDW.Integrator_IC_LOADING = 0U;
      rtDW.Integrator_DSTATE += 0.001 * rtDW.uT;
      rtDW.Integrator_PrevResetState = 0;
    }
  }                                    /* end MajorTimeStep */

  if (rtmIsMajorTimeStep(rtM)) {
    rt_ertODEUpdateContinuousStates(&rtM->solverInfo);

    /* Update absolute time for base rate */
    /* The "clockTick0" counts the number of times the code of this task has
     * been executed. The absolute time is the multiplication of "clockTick0"
     * and "Timing.stepSize0". Size of "clockTick0" ensures timer will not
     * overflow during the application lifespan selected.
     */
    ++rtM->Timing.clockTick0;
    rtM->Timing.t[0] = rtsiGetSolverStopTime(&rtM->solverInfo);

    {
      /* Update absolute timer for sample time: [0.001s, 0.0s] */
      /* The "clockTick1" counts the number of times the code of this task has
       * been executed. The resolution of this integer timer is 0.001, which is the step size
       * of the task. Size of "clockTick1" ensures timer will not overflow during the
       * application lifespan selected.
       */
      rtM->Timing.clockTick1++;
    }
  }                                    /* end MajorTimeStep */
}

/* Derivatives for root system: '<Root>' */
void controller_derivatives(void)
{
  XDot *_rtXdot;
  boolean_T lsat;
  boolean_T usat;
  _rtXdot = ((XDot *) rtM->derivs);

  /* Derivatives for Integrator: '<S1>/Integrator' incorporates:
   *  Inport: '<Root>/theta'
   */
  _rtXdot->Integrator_CSTATE = rtU.theta;

  /* Derivatives for Integrator: '<S1>/Integrator1' */
  lsat = (rtX.Integrator1_CSTATE <= -2.0);
  usat = (rtX.Integrator1_CSTATE >= 2.0);
  if (((!lsat) && (!usat)) || (lsat && (rtDW.sign_s > 0.0)) || (usat &&
       (rtDW.sign_s < 0.0))) {
    _rtXdot->Integrator1_CSTATE = rtDW.sign_s;
  } else {
    /* in saturation */
    _rtXdot->Integrator1_CSTATE = 0.0;
  }

  /* End of Derivatives for Integrator: '<S1>/Integrator1' */
}

/* Model initialize function */
void controller_initialize(void)
{
  /* Registration code */
  {
    /* Setup solver object */
    rtsiSetSimTimeStepPtr(&rtM->solverInfo, &rtM->Timing.simTimeStep);
    rtsiSetTPtr(&rtM->solverInfo, &rtmGetTPtr(rtM));
    rtsiSetStepSizePtr(&rtM->solverInfo, &rtM->Timing.stepSize0);
    rtsiSetdXPtr(&rtM->solverInfo, &rtM->derivs);
    rtsiSetContStatesPtr(&rtM->solverInfo, (real_T **) &rtM->contStates);
    rtsiSetNumContStatesPtr(&rtM->solverInfo, &rtM->Sizes.numContStates);
    rtsiSetNumPeriodicContStatesPtr(&rtM->solverInfo,
      &rtM->Sizes.numPeriodicContStates);
    rtsiSetPeriodicContStateIndicesPtr(&rtM->solverInfo,
      &rtM->periodicContStateIndices);
    rtsiSetPeriodicContStateRangesPtr(&rtM->solverInfo,
      &rtM->periodicContStateRanges);
    rtsiSetContStateDisabledPtr(&rtM->solverInfo, (boolean_T**)
      &rtM->contStateDisabled);
    rtsiSetErrorStatusPtr(&rtM->solverInfo, (&rtmGetErrorStatus(rtM)));
    rtsiSetRTModelPtr(&rtM->solverInfo, rtM);
  }

  rtsiSetSimTimeStep(&rtM->solverInfo, MAJOR_TIME_STEP);
  rtsiSetIsMinorTimeStepWithModeChange(&rtM->solverInfo, false);
  rtsiSetIsContModeFrozen(&rtM->solverInfo, false);
  rtM->intgData.y = rtM->odeY;
  rtM->intgData.f[0] = rtM->odeF[0];
  rtM->intgData.f[1] = rtM->odeF[1];
  rtM->intgData.f[2] = rtM->odeF[2];
  rtM->intgData.f[3] = rtM->odeF[3];
  rtM->contStates = ((X *) &rtX);
  rtM->contStateDisabled = ((XDis *) &rtXDis);
  rtM->Timing.tStart = (0.0);
  rtM->periodicContStateIndices = ((int_T*) rtPeriodicIndX);
  rtM->periodicContStateRanges = ((real_T*) rtPeriodicRngX);
  rtsiSetSolverData(&rtM->solverInfo, (void *)&rtM->intgData);
  rtsiSetSolverName(&rtM->solverInfo,"ode4");
  rtmSetTPtr(rtM, &rtM->Timing.tArray[0]);
  rtM->Timing.stepSize0 = 0.001;

  /* Start for Probe: '<S4>/Probe' */
  rtDW.Probe[0] = 0.001;
  rtDW.Probe[1] = 0.0;
  rtPrevZCX.Integrator_Reset_ZCE = UNINITIALIZED_ZCSIG;

  /* InitializeConditions for DiscreteIntegrator: '<S8>/Integrator' */
  rtDW.Integrator_IC_LOADING = 1U;

  /* InitializeConditions for Integrator: '<S1>/Integrator' */
  rtX.Integrator_CSTATE = 0.0;

  /* InitializeConditions for Integrator: '<S1>/Integrator1' */
  rtX.Integrator1_CSTATE = 0.0;

  /* InitializeConditions for root-level periodic continuous states */
  {
    int_T rootPeriodicContStateIndices[1] = { 0 };

    real_T rootPeriodicContStateRanges[2] = { -3.14, 3.14 };

    (void) memcpy((void*)rtPeriodicIndX, rootPeriodicContStateIndices,
                  1*sizeof(int_T));
    (void) memcpy((void*)rtPeriodicRngX, rootPeriodicContStateRanges,
                  2*sizeof(real_T));
  }
}

/* Test if value is infinite */
static boolean_T rtIsInf(real_T value)
{
  return (boolean_T)isinf(value);
}

/* Test if single-precision value is infinite */
static boolean_T rtIsInfF(real32_T value)
{
  return (boolean_T)isinf(value);
}

/* Test if value is not a number */
static boolean_T rtIsNaN(real_T value)
{
  return (boolean_T)(isnan(value) != 0);
}

/* Test if single-precision value is not a number */
static boolean_T rtIsNaNF(real32_T value)
{
  return (boolean_T)(isnan(value) != 0);
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
