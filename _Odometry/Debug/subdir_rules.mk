################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
Bump.obj: C:/Users/Bronco/OneDrive\ -\ University\ of\ Pittsburgh/2023\ Spring\ Semester/ECE\ 1188/ECE-1188_Hulk-Final-Race-master/inc/Bump.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"C:/ti/ccs1210/ccs/tools/compiler/ti-cgt-arm_20.2.7.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="C:/Users/Bronco/OneDrive - University of Pittsburgh/2023 Spring Semester/ECE 1188/ECE-1188_Hulk-Final-Race-master/Odometry" --include_path="C:/ti/ccs1210/ccs/tools/compiler/ti-cgt-arm_20.2.7.LTS/include" --include_path="C:/ti/ccs1210/ccs/ccs_base/arm/include" --include_path="C:/ti/ccs1210/ccs/ccs_base/arm/include/CMSIS" --include_path="C:/Users/Bronco/OneDrive - University of Pittsburgh/2023 Spring Semester/ECE 1188/ECE-1188_Hulk-Final-Race-master/inc" --advice:power=all --define=__MSP432P401R__ --define=ccs -g --c99 --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="Bump.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

Clock.obj: C:/Users/Bronco/OneDrive\ -\ University\ of\ Pittsburgh/2023\ Spring\ Semester/ECE\ 1188/ECE-1188_Hulk-Final-Race-master/inc/Clock.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"C:/ti/ccs1210/ccs/tools/compiler/ti-cgt-arm_20.2.7.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="C:/Users/Bronco/OneDrive - University of Pittsburgh/2023 Spring Semester/ECE 1188/ECE-1188_Hulk-Final-Race-master/Odometry" --include_path="C:/ti/ccs1210/ccs/tools/compiler/ti-cgt-arm_20.2.7.LTS/include" --include_path="C:/ti/ccs1210/ccs/ccs_base/arm/include" --include_path="C:/ti/ccs1210/ccs/ccs_base/arm/include/CMSIS" --include_path="C:/Users/Bronco/OneDrive - University of Pittsburgh/2023 Spring Semester/ECE 1188/ECE-1188_Hulk-Final-Race-master/inc" --advice:power=all --define=__MSP432P401R__ --define=ccs -g --c99 --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="Clock.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

LaunchPad.obj: C:/Users/Bronco/OneDrive\ -\ University\ of\ Pittsburgh/2023\ Spring\ Semester/ECE\ 1188/ECE-1188_Hulk-Final-Race-master/inc/LaunchPad.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"C:/ti/ccs1210/ccs/tools/compiler/ti-cgt-arm_20.2.7.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="C:/Users/Bronco/OneDrive - University of Pittsburgh/2023 Spring Semester/ECE 1188/ECE-1188_Hulk-Final-Race-master/Odometry" --include_path="C:/ti/ccs1210/ccs/tools/compiler/ti-cgt-arm_20.2.7.LTS/include" --include_path="C:/ti/ccs1210/ccs/ccs_base/arm/include" --include_path="C:/ti/ccs1210/ccs/ccs_base/arm/include/CMSIS" --include_path="C:/Users/Bronco/OneDrive - University of Pittsburgh/2023 Spring Semester/ECE 1188/ECE-1188_Hulk-Final-Race-master/inc" --advice:power=all --define=__MSP432P401R__ --define=ccs -g --c99 --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="LaunchPad.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

Motor.obj: C:/Users/Bronco/OneDrive\ -\ University\ of\ Pittsburgh/2023\ Spring\ Semester/ECE\ 1188/ECE-1188_Hulk-Final-Race-master/inc/Motor.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"C:/ti/ccs1210/ccs/tools/compiler/ti-cgt-arm_20.2.7.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="C:/Users/Bronco/OneDrive - University of Pittsburgh/2023 Spring Semester/ECE 1188/ECE-1188_Hulk-Final-Race-master/Odometry" --include_path="C:/ti/ccs1210/ccs/tools/compiler/ti-cgt-arm_20.2.7.LTS/include" --include_path="C:/ti/ccs1210/ccs/ccs_base/arm/include" --include_path="C:/ti/ccs1210/ccs/ccs_base/arm/include/CMSIS" --include_path="C:/Users/Bronco/OneDrive - University of Pittsburgh/2023 Spring Semester/ECE 1188/ECE-1188_Hulk-Final-Race-master/inc" --advice:power=all --define=__MSP432P401R__ --define=ccs -g --c99 --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="Motor.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

%.obj: ../%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"C:/ti/ccs1210/ccs/tools/compiler/ti-cgt-arm_20.2.7.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="C:/Users/Bronco/OneDrive - University of Pittsburgh/2023 Spring Semester/ECE 1188/ECE-1188_Hulk-Final-Race-master/Odometry" --include_path="C:/ti/ccs1210/ccs/tools/compiler/ti-cgt-arm_20.2.7.LTS/include" --include_path="C:/ti/ccs1210/ccs/ccs_base/arm/include" --include_path="C:/ti/ccs1210/ccs/ccs_base/arm/include/CMSIS" --include_path="C:/Users/Bronco/OneDrive - University of Pittsburgh/2023 Spring Semester/ECE 1188/ECE-1188_Hulk-Final-Race-master/inc" --advice:power=all --define=__MSP432P401R__ --define=ccs -g --c99 --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

PWM.obj: C:/Users/Bronco/OneDrive\ -\ University\ of\ Pittsburgh/2023\ Spring\ Semester/ECE\ 1188/ECE-1188_Hulk-Final-Race-master/inc/PWM.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"C:/ti/ccs1210/ccs/tools/compiler/ti-cgt-arm_20.2.7.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="C:/Users/Bronco/OneDrive - University of Pittsburgh/2023 Spring Semester/ECE 1188/ECE-1188_Hulk-Final-Race-master/Odometry" --include_path="C:/ti/ccs1210/ccs/tools/compiler/ti-cgt-arm_20.2.7.LTS/include" --include_path="C:/ti/ccs1210/ccs/ccs_base/arm/include" --include_path="C:/ti/ccs1210/ccs/ccs_base/arm/include/CMSIS" --include_path="C:/Users/Bronco/OneDrive - University of Pittsburgh/2023 Spring Semester/ECE 1188/ECE-1188_Hulk-Final-Race-master/inc" --advice:power=all --define=__MSP432P401R__ --define=ccs -g --c99 --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="PWM.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

SSD1306.obj: C:/Users/Bronco/OneDrive\ -\ University\ of\ Pittsburgh/2023\ Spring\ Semester/ECE\ 1188/ECE-1188_Hulk-Final-Race-master/inc/SSD1306.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"C:/ti/ccs1210/ccs/tools/compiler/ti-cgt-arm_20.2.7.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="C:/Users/Bronco/OneDrive - University of Pittsburgh/2023 Spring Semester/ECE 1188/ECE-1188_Hulk-Final-Race-master/Odometry" --include_path="C:/ti/ccs1210/ccs/tools/compiler/ti-cgt-arm_20.2.7.LTS/include" --include_path="C:/ti/ccs1210/ccs/ccs_base/arm/include" --include_path="C:/ti/ccs1210/ccs/ccs_base/arm/include/CMSIS" --include_path="C:/Users/Bronco/OneDrive - University of Pittsburgh/2023 Spring Semester/ECE 1188/ECE-1188_Hulk-Final-Race-master/inc" --advice:power=all --define=__MSP432P401R__ --define=ccs -g --c99 --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="SSD1306.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

TA3InputCapture.obj: C:/Users/Bronco/OneDrive\ -\ University\ of\ Pittsburgh/2023\ Spring\ Semester/ECE\ 1188/ECE-1188_Hulk-Final-Race-master/inc/TA3InputCapture.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"C:/ti/ccs1210/ccs/tools/compiler/ti-cgt-arm_20.2.7.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="C:/Users/Bronco/OneDrive - University of Pittsburgh/2023 Spring Semester/ECE 1188/ECE-1188_Hulk-Final-Race-master/Odometry" --include_path="C:/ti/ccs1210/ccs/tools/compiler/ti-cgt-arm_20.2.7.LTS/include" --include_path="C:/ti/ccs1210/ccs/ccs_base/arm/include" --include_path="C:/ti/ccs1210/ccs/ccs_base/arm/include/CMSIS" --include_path="C:/Users/Bronco/OneDrive - University of Pittsburgh/2023 Spring Semester/ECE 1188/ECE-1188_Hulk-Final-Race-master/inc" --advice:power=all --define=__MSP432P401R__ --define=ccs -g --c99 --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="TA3InputCapture.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

Tachometer.obj: C:/Users/Bronco/OneDrive\ -\ University\ of\ Pittsburgh/2023\ Spring\ Semester/ECE\ 1188/ECE-1188_Hulk-Final-Race-master/inc/Tachometer.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"C:/ti/ccs1210/ccs/tools/compiler/ti-cgt-arm_20.2.7.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="C:/Users/Bronco/OneDrive - University of Pittsburgh/2023 Spring Semester/ECE 1188/ECE-1188_Hulk-Final-Race-master/Odometry" --include_path="C:/ti/ccs1210/ccs/tools/compiler/ti-cgt-arm_20.2.7.LTS/include" --include_path="C:/ti/ccs1210/ccs/ccs_base/arm/include" --include_path="C:/ti/ccs1210/ccs/ccs_base/arm/include/CMSIS" --include_path="C:/Users/Bronco/OneDrive - University of Pittsburgh/2023 Spring Semester/ECE 1188/ECE-1188_Hulk-Final-Race-master/inc" --advice:power=all --define=__MSP432P401R__ --define=ccs -g --c99 --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="Tachometer.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

TimerA1.obj: C:/Users/Bronco/OneDrive\ -\ University\ of\ Pittsburgh/2023\ Spring\ Semester/ECE\ 1188/ECE-1188_Hulk-Final-Race-master/inc/TimerA1.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"C:/ti/ccs1210/ccs/tools/compiler/ti-cgt-arm_20.2.7.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="C:/Users/Bronco/OneDrive - University of Pittsburgh/2023 Spring Semester/ECE 1188/ECE-1188_Hulk-Final-Race-master/Odometry" --include_path="C:/ti/ccs1210/ccs/tools/compiler/ti-cgt-arm_20.2.7.LTS/include" --include_path="C:/ti/ccs1210/ccs/ccs_base/arm/include" --include_path="C:/ti/ccs1210/ccs/ccs_base/arm/include/CMSIS" --include_path="C:/Users/Bronco/OneDrive - University of Pittsburgh/2023 Spring Semester/ECE 1188/ECE-1188_Hulk-Final-Race-master/inc" --advice:power=all --define=__MSP432P401R__ --define=ccs -g --c99 --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="TimerA1.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

UART0.obj: C:/Users/Bronco/OneDrive\ -\ University\ of\ Pittsburgh/2023\ Spring\ Semester/ECE\ 1188/ECE-1188_Hulk-Final-Race-master/inc/UART0.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"C:/ti/ccs1210/ccs/tools/compiler/ti-cgt-arm_20.2.7.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="C:/Users/Bronco/OneDrive - University of Pittsburgh/2023 Spring Semester/ECE 1188/ECE-1188_Hulk-Final-Race-master/Odometry" --include_path="C:/ti/ccs1210/ccs/tools/compiler/ti-cgt-arm_20.2.7.LTS/include" --include_path="C:/ti/ccs1210/ccs/ccs_base/arm/include" --include_path="C:/ti/ccs1210/ccs/ccs_base/arm/include/CMSIS" --include_path="C:/Users/Bronco/OneDrive - University of Pittsburgh/2023 Spring Semester/ECE 1188/ECE-1188_Hulk-Final-Race-master/inc" --advice:power=all --define=__MSP432P401R__ --define=ccs -g --c99 --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="UART0.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

blinker.obj: C:/Users/Bronco/OneDrive\ -\ University\ of\ Pittsburgh/2023\ Spring\ Semester/ECE\ 1188/ECE-1188_Hulk-Final-Race-master/inc/blinker.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"C:/ti/ccs1210/ccs/tools/compiler/ti-cgt-arm_20.2.7.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="C:/Users/Bronco/OneDrive - University of Pittsburgh/2023 Spring Semester/ECE 1188/ECE-1188_Hulk-Final-Race-master/Odometry" --include_path="C:/ti/ccs1210/ccs/tools/compiler/ti-cgt-arm_20.2.7.LTS/include" --include_path="C:/ti/ccs1210/ccs/ccs_base/arm/include" --include_path="C:/ti/ccs1210/ccs/ccs_base/arm/include/CMSIS" --include_path="C:/Users/Bronco/OneDrive - University of Pittsburgh/2023 Spring Semester/ECE 1188/ECE-1188_Hulk-Final-Race-master/inc" --advice:power=all --define=__MSP432P401R__ --define=ccs -g --c99 --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="blinker.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

fixed.obj: C:/Users/Bronco/OneDrive\ -\ University\ of\ Pittsburgh/2023\ Spring\ Semester/ECE\ 1188/ECE-1188_Hulk-Final-Race-master/inc/fixed.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"C:/ti/ccs1210/ccs/tools/compiler/ti-cgt-arm_20.2.7.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="C:/Users/Bronco/OneDrive - University of Pittsburgh/2023 Spring Semester/ECE 1188/ECE-1188_Hulk-Final-Race-master/Odometry" --include_path="C:/ti/ccs1210/ccs/tools/compiler/ti-cgt-arm_20.2.7.LTS/include" --include_path="C:/ti/ccs1210/ccs/ccs_base/arm/include" --include_path="C:/ti/ccs1210/ccs/ccs_base/arm/include/CMSIS" --include_path="C:/Users/Bronco/OneDrive - University of Pittsburgh/2023 Spring Semester/ECE 1188/ECE-1188_Hulk-Final-Race-master/inc" --advice:power=all --define=__MSP432P401R__ --define=ccs -g --c99 --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="fixed.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

odometry.obj: C:/Users/Bronco/OneDrive\ -\ University\ of\ Pittsburgh/2023\ Spring\ Semester/ECE\ 1188/ECE-1188_Hulk-Final-Race-master/inc/odometry.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"C:/ti/ccs1210/ccs/tools/compiler/ti-cgt-arm_20.2.7.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="C:/Users/Bronco/OneDrive - University of Pittsburgh/2023 Spring Semester/ECE 1188/ECE-1188_Hulk-Final-Race-master/Odometry" --include_path="C:/ti/ccs1210/ccs/tools/compiler/ti-cgt-arm_20.2.7.LTS/include" --include_path="C:/ti/ccs1210/ccs/ccs_base/arm/include" --include_path="C:/ti/ccs1210/ccs/ccs_base/arm/include/CMSIS" --include_path="C:/Users/Bronco/OneDrive - University of Pittsburgh/2023 Spring Semester/ECE 1188/ECE-1188_Hulk-Final-Race-master/inc" --advice:power=all --define=__MSP432P401R__ --define=ccs -g --c99 --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="odometry.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


