% Rotating Inverted Pendulum Control with STA-SMC (Simplified Model)
% Control goal: Stabilize theta to 0 radians
% Controller: Super-Twisting SMC with tanh boundary layer + integral action
% Model: mp*r*thetaddot - mp*g*sin(theta) = u - bp*thetadot^2

clc; clear; close all;

%% System Parameters
params.mp = 0.156;    % Pendulum mass (kg)
params.lp = 0.155;    % Pendulum length / r (m)
params.bp = 0.0007;   % Pendulum damping coefficient (N·m·s/rad)
params.g  = 9.81;     % Gravity (m/s²)

%% Controller Parameters
controller.c1 = 8;    % Sliding surface proportional gain
controller.c2 = 25;   % Sliding surface integral gain
controller.k1 = 8;    % Super-twisting sqrt-term gain
controller.k2 = 6;    % Integral sign-term gain
controller.n  = 0.1; % tanh boundary layer thickness

% saturation limit (N·m)
controller.uMax = 2;

%% Simulation Settings with RK4
h = 0.001;            % Smaller step for accuracy
t = 0:h:10;
nSteps = length(t);
% State: [theta, theta_dot, int_e, intSign]
X = zeros(nSteps, 4);
control = zeros(nSteps, 1);

% Initial state
X(1,:) = [0.3, 0, 0, 0];

%% RK4 Integration Loop
for i = 1:nSteps-1
    current_X = X(i,:)';

    [k1, tau_phi1] = pendulumDynamics(t(i), current_X, params, controller);
    control(i) = tau_phi1;

    [k2, ~] = pendulumDynamics(t(i)+h/2, current_X + h/2*k1, params, controller);
    [k3, ~] = pendulumDynamics(t(i)+h/2, current_X + h/2*k2, params, controller);
    [k4, ~] = pendulumDynamics(t(i)+h,   current_X + h*k3,   params, controller);

    X(i+1,:) = current_X' + (h/6)*(k1 + 2*k2 + 2*k3 + k4)';
end

[~, control(nSteps)] = pendulumDynamics(t(end), X(nSteps,:)', params, controller);

%% Extract States
theta    = X(:,1);
thetaDot = X(:,2);
intE     = X(:,3);

% Sliding surface (for plotting)
s = thetaDot + controller.c1*theta + controller.c2*intE;

%% Plot Results
figure;
subplot(2,2,1);
plot(t, theta, 'b', 'LineWidth', 1.5);
xlabel('Time (s)'); ylabel('\theta (rad)');
title('Pendulum Angle \theta'); grid on;

subplot(2,2,2);
plot(t, thetaDot, 'g', 'LineWidth', 1.5);
xlabel('Time (s)'); ylabel('$\dot{\theta}$ (rad/s)', 'Interpreter','latex');
title('Pendulum Angular Velocity'); grid on;

subplot(2,2,3);
plot(t, control, 'r', 'LineWidth', 1.5);
xlabel('Time (s)'); ylabel('u (N\cdot m)');
title('Control Input'); grid on;

subplot(2,2,4);
plot(t, s, 'm', 'LineWidth', 1.5);
xlabel('Time (s)'); ylabel('s');
title('Sliding Surface'); grid on;

%% Dynamics Function (Simplified Model, FIXED STA-SMC)
function [dXdt, control] = pendulumDynamics(~, X, params, ctrl)
    % Extract states
    theta    = X(1);
    thetaDot = X(2);
    intE     = X(3);
    intSign  = X(4);

    mp = params.mp;
    r  = params.lp;
    g  = params.g;
    bp = params.bp;

    % Sliding surface
    e = theta;
    s = thetaDot + ctrl.c1*e + ctrl.c2*intE;

    % Smooth switching function (replaces hard sign)
    sat_s = tanh(s / ctrl.n);

    % Drift term
    f = (-bp*thetaDot + mp*g*sin(theta)) / (mp*r);

    % Input gain
    g_in = (mp*r);

    % All terms inside g_in scaling for consistency:
    %   equivalent:  -f - c1*thetaDot - c2*e
    %   STA:         -k1*sqrt(|s|)*tanh(s/n) - k2*intSign
    v_eq  = -f - ctrl.c1*thetaDot - ctrl.c2*e;
    v_sta = -ctrl.k1*sqrt(abs(s))*sat_s - ctrl.k2*intSign;

    control = g_in * (v_eq + v_sta);

    % Saturation (actuator limit)
    control = max(min(control, ctrl.uMax), -ctrl.uMax);

    % Angular acceleration
    thetaDDot = f + control/g_in;

    % State derivatives
    dIntE    = theta;
    dIntSign = sat_s;   % use tanh, not sign(s)

    dXdt = [thetaDot; thetaDDot; dIntE; dIntSign];
end
