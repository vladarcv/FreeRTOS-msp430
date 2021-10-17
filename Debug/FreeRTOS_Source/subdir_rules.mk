################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
FreeRTOS_Source/event_groups.obj: ../FreeRTOS_Source/event_groups.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.6.LTS/bin/cl430" -vmspx --code_model=large --data_model=small --use_hw_mpy=F5 --include_path="C:/ti/ccsv7/ccs_base/msp430/include" --include_path="C:/Users/vlada/workspace_v7/projekatSRV" --include_path="C:/Users/vlada/workspace_v7/projekatSRV/ETF5438A_HAL" --include_path="C:/Users/vlada/workspace_v7/projekatSRV/F5XX_6XX_Core_Lib" --include_path="C:/Users/vlada/workspace_v7/projekatSRV/FreeRTOS_Source/include" --include_path="C:/Users/vlada/workspace_v7/projekatSRV/FreeRTOS_Source/portable/CCS/MSP430X" --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.6.LTS/include" --advice:power=all --define=__MSP430F5438A__ --define=__LARGE_CODE_MODEL__ --define=__SMALL_DATA_MODEL__ -g --printf_support=minimal --diag_warning=225 --diag_wrap=off --display_error_number --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU23 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="FreeRTOS_Source/event_groups.d_raw" --obj_directory="FreeRTOS_Source" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

FreeRTOS_Source/list.obj: ../FreeRTOS_Source/list.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.6.LTS/bin/cl430" -vmspx --code_model=large --data_model=small --use_hw_mpy=F5 --include_path="C:/ti/ccsv7/ccs_base/msp430/include" --include_path="C:/Users/vlada/workspace_v7/projekatSRV" --include_path="C:/Users/vlada/workspace_v7/projekatSRV/ETF5438A_HAL" --include_path="C:/Users/vlada/workspace_v7/projekatSRV/F5XX_6XX_Core_Lib" --include_path="C:/Users/vlada/workspace_v7/projekatSRV/FreeRTOS_Source/include" --include_path="C:/Users/vlada/workspace_v7/projekatSRV/FreeRTOS_Source/portable/CCS/MSP430X" --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.6.LTS/include" --advice:power=all --define=__MSP430F5438A__ --define=__LARGE_CODE_MODEL__ --define=__SMALL_DATA_MODEL__ -g --printf_support=minimal --diag_warning=225 --diag_wrap=off --display_error_number --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU23 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="FreeRTOS_Source/list.d_raw" --obj_directory="FreeRTOS_Source" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

FreeRTOS_Source/queue.obj: ../FreeRTOS_Source/queue.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.6.LTS/bin/cl430" -vmspx --code_model=large --data_model=small --use_hw_mpy=F5 --include_path="C:/ti/ccsv7/ccs_base/msp430/include" --include_path="C:/Users/vlada/workspace_v7/projekatSRV" --include_path="C:/Users/vlada/workspace_v7/projekatSRV/ETF5438A_HAL" --include_path="C:/Users/vlada/workspace_v7/projekatSRV/F5XX_6XX_Core_Lib" --include_path="C:/Users/vlada/workspace_v7/projekatSRV/FreeRTOS_Source/include" --include_path="C:/Users/vlada/workspace_v7/projekatSRV/FreeRTOS_Source/portable/CCS/MSP430X" --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.6.LTS/include" --advice:power=all --define=__MSP430F5438A__ --define=__LARGE_CODE_MODEL__ --define=__SMALL_DATA_MODEL__ -g --printf_support=minimal --diag_warning=225 --diag_wrap=off --display_error_number --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU23 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="FreeRTOS_Source/queue.d_raw" --obj_directory="FreeRTOS_Source" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

FreeRTOS_Source/tasks.obj: ../FreeRTOS_Source/tasks.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.6.LTS/bin/cl430" -vmspx --code_model=large --data_model=small --use_hw_mpy=F5 --include_path="C:/ti/ccsv7/ccs_base/msp430/include" --include_path="C:/Users/vlada/workspace_v7/projekatSRV" --include_path="C:/Users/vlada/workspace_v7/projekatSRV/ETF5438A_HAL" --include_path="C:/Users/vlada/workspace_v7/projekatSRV/F5XX_6XX_Core_Lib" --include_path="C:/Users/vlada/workspace_v7/projekatSRV/FreeRTOS_Source/include" --include_path="C:/Users/vlada/workspace_v7/projekatSRV/FreeRTOS_Source/portable/CCS/MSP430X" --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.6.LTS/include" --advice:power=all --define=__MSP430F5438A__ --define=__LARGE_CODE_MODEL__ --define=__SMALL_DATA_MODEL__ -g --printf_support=minimal --diag_warning=225 --diag_wrap=off --display_error_number --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU23 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="FreeRTOS_Source/tasks.d_raw" --obj_directory="FreeRTOS_Source" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

FreeRTOS_Source/timers.obj: ../FreeRTOS_Source/timers.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.6.LTS/bin/cl430" -vmspx --code_model=large --data_model=small --use_hw_mpy=F5 --include_path="C:/ti/ccsv7/ccs_base/msp430/include" --include_path="C:/Users/vlada/workspace_v7/projekatSRV" --include_path="C:/Users/vlada/workspace_v7/projekatSRV/ETF5438A_HAL" --include_path="C:/Users/vlada/workspace_v7/projekatSRV/F5XX_6XX_Core_Lib" --include_path="C:/Users/vlada/workspace_v7/projekatSRV/FreeRTOS_Source/include" --include_path="C:/Users/vlada/workspace_v7/projekatSRV/FreeRTOS_Source/portable/CCS/MSP430X" --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.6.LTS/include" --advice:power=all --define=__MSP430F5438A__ --define=__LARGE_CODE_MODEL__ --define=__SMALL_DATA_MODEL__ -g --printf_support=minimal --diag_warning=225 --diag_wrap=off --display_error_number --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU23 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="FreeRTOS_Source/timers.d_raw" --obj_directory="FreeRTOS_Source" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


