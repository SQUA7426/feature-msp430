################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
Sources/%.obj: ../Sources/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccs1281/ccs/tools/compiler/ti-cgt-msp430_21.6.1.LTS/bin/cl430" -vmspx --code_model=large --data_model=small -O4 --opt_for_speed=0 --use_hw_mpy=F5 --include_path="C:/ti/ccs1281/ccs/ccs_base/msp430/include" --include_path="C:/Users/Timothy/Documents/Micro/feature-msp430/ex1" --include_path="C:/ti/ccs1281/ccs/tools/compiler/ti-cgt-msp430_21.6.1.LTS/include" --advice:power="all" --define=__MSP430FR5729__ -g --c99 --float_operations_allowed=none --printf_support=minimal --diag_warning=225 --diag_wrap=off --display_error_number --gen_data_subsections=on --enum_type=packed --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 --call_assumptions=3 --auto_inline=100 --preproc_with_compile --preproc_dependency="Sources/$(basename $(<F)).d_raw" --obj_directory="Sources" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


