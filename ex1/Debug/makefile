################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := C:/ti/ccs1281/ccs/tools/compiler/ti-cgt-msp430_21.6.1.LTS

GEN_OPTS__FLAG := 
GEN_CMDS__FLAG := 

ORDERED_OBJS += \
"./Sources/TA0.obj" \
"./Sources/TA1.obj" \
"./Sources/event.obj" \
"./Sources/main.obj" \
"../lnk_msp430fr5729.cmd" \
$(GEN_CMDS__FLAG) \
-llibmpu_init_fr57xx.a \
-llibmath.a \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include Sources/subdir_vars.mk
-include subdir_rules.mk
-include Sources/subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(INO_DEPS)),)
-include $(INO_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(SV7A_DEPS)),)
-include $(SV7A_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(PDE_DEPS)),)
-include $(PDE_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXE_OUTPUTS += \
ex1.out 

EXE_OUTPUTS__QUOTED += \
"ex1.out" 


# All Target
all: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@$(MAKE) --no-print-directory -Onone "ex1.out"

# Tool invocations
ex1.out: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@echo 'Building target: "$@"'
	@echo 'Invoking: MSP430 Linker'
	"C:/ti/ccs1281/ccs/tools/compiler/ti-cgt-msp430_21.6.1.LTS/bin/cl430" -vmspx --code_model=small --data_model=small -O4 --opt_for_speed=0 --use_hw_mpy=F5 --advice:power="all" --define=__MSP430FR5729__ -g --c99 --float_operations_allowed=none --printf_support=minimal --diag_warning=225 --diag_wrap=off --display_error_number --gen_data_subsections=on --enum_type=packed --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 --call_assumptions=3 --auto_inline=100 -z -m"ex1.map" --heap_size=0 --stack_size=80 --cinit_hold_wdt=off -i"C:/ti/ccs1281/ccs/ccs_base/msp430/include" -i"C:/ti/ccs1281/ccs/ccs_base/msp430/lib/5xx_6xx_FRxx" -i"C:/ti/ccs1281/ccs/ccs_base/msp430/lib/FR57xx" -i"C:/ti/ccs1281/ccs/tools/compiler/ti-cgt-msp430_21.6.1.LTS/lib" -i"C:/ti/ccs1281/ccs/tools/compiler/ti-cgt-msp430_21.6.1.LTS/include" --priority --reread_libs --diag_wrap=off --display_error_number --warn_sections --xml_link_info="ex1_linkInfo.xml" --use_hw_mpy=F5 --arg_size=0 --cinit_compression=off --copy_compression=off --rom_model --unused_section_elimination=on --zero_init=off -o "ex1.out" $(ORDERED_OBJS)
	@echo 'Finished building target: "$@"'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(EXE_OUTPUTS__QUOTED)
	-$(RM) "Sources\TA0.obj" "Sources\TA1.obj" "Sources\event.obj" "Sources\main.obj" 
	-$(RM) "Sources\TA0.d" "Sources\TA1.d" "Sources\event.d" "Sources\main.d" 
	-@echo 'Finished clean'
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

