#! /bin/sh

PWM_CHIP_PATH=/sys/class/pwm/pwmchip0

echo 0 > ${PWM_CHIP_PATH}/export
echo 2 > ${PWM_CHIP_PATH}/export

echo 17500000 > ${PWM_CHIP_PATH}/pwm0/period
echo 1500000 > ${PWM_CHIP_PATH}/pwm0/duty_cycle
echo 1 > ${PWM_CHIP_PATH}/pwm0/enable

echo 17500000 > ${PWM_CHIP_PATH}/pwm2/period
echo 1500000 > ${PWM_CHIP_PATH}/pwm2/duty_cycle
echo 1 > ${PWM_CHIP_PATH}/pwm2/enable