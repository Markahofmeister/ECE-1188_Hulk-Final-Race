################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
%.obj: ../%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
<<<<<<< HEAD
	"" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="/arm/include" --include_path="C:/Users/marka/Spring-2023/Cyber-Physical_Systems/final-race/ECE-1188_Hullk-Final-Race/inc" --include_path="/arm/include/CMSIS" --include_path="/Applications/ti/ccs1210/ccs/tools/compiler/ti-cgt-arm_20.2.7.LTS/include" --advice:power="all" --define=__MSP432P401R__ --define=ccs -g --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" $(GEN_OPTS__FLAG) "$<"
=======
	"C:/ti/ccs1210/ccs/tools/compiler/ti-cgt-arm_20.2.7.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="C:/ti/ccs1210/ccs/ccs_base/arm/include" --include_path="C:/Users/marka/Spring-2023/Cyber-Physical_Systems/final-race/ECE-1188_Hullk-Final-Race/inc" --include_path="C:/ti/ccs1210/ccs/ccs_base/arm/include/CMSIS" --include_path="C:/Users/Bronco/OneDrive - University of Pittsburgh/2023 Spring Semester/ECE 1188/ECE-1188_Hulk-Final-Race/_Hulk_Master" --include_path="C:/ti/ccs1210/ccs/tools/compiler/ti-cgt-arm_20.2.7.LTS/include" --advice:power="all" --define=__MSP432P401R__ --define=ccs -g --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" $(GEN_OPTS__FLAG) "$<"
>>>>>>> 2cc8f80223a311d818a086390c4ded00cc0a3c5a
	@echo 'Finished building: "$<"'
	@echo ' '


