################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/MEF_acumulado.c \
../source/MEF_habitual.c \
../source/MEF_peatones.c \
../source/MEF_semaforos.c \
../source/SD2_board.c \
../source/cars.c \
../source/key.c \
../source/main.c \
../source/mtb.c \
../source/semihost_hardfault.c 

C_DEPS += \
./source/MEF_acumulado.d \
./source/MEF_habitual.d \
./source/MEF_peatones.d \
./source/MEF_semaforos.d \
./source/SD2_board.d \
./source/cars.d \
./source/key.d \
./source/main.d \
./source/mtb.d \
./source/semihost_hardfault.d 

OBJS += \
./source/MEF_acumulado.o \
./source/MEF_habitual.o \
./source/MEF_peatones.o \
./source/MEF_semaforos.o \
./source/SD2_board.o \
./source/cars.o \
./source/key.o \
./source/main.o \
./source/mtb.o \
./source/semihost_hardfault.o 


# Each subdirectory must supply rules for building sources it contributes
source/%.o: ../source/%.c source/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DCPU_MKL46Z256VLH4_cm0plus -DCPU_MKL46Z256VLH4 -DFSL_RTOS_BM -DSDK_OS_BAREMETAL -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -DSDK_DEBUGCONSOLE=1 -I"C:\nxp\default_workspace\SD2_TP1_v2.0\board" -I"C:\nxp\default_workspace\SD2_TP1_v2.0\source" -I"C:\nxp\default_workspace\SD2_TP1_v2.0" -I"C:\nxp\default_workspace\SD2_TP1_v2.0\drivers" -I"C:\nxp\default_workspace\SD2_TP1_v2.0\CMSIS" -I"C:\nxp\default_workspace\SD2_TP1_v2.0\utilities" -I"C:\nxp\default_workspace\SD2_TP1_v2.0\startup" -O0 -fno-common -g3 -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-source

clean-source:
	-$(RM) ./source/MEF_acumulado.d ./source/MEF_acumulado.o ./source/MEF_habitual.d ./source/MEF_habitual.o ./source/MEF_peatones.d ./source/MEF_peatones.o ./source/MEF_semaforos.d ./source/MEF_semaforos.o ./source/SD2_board.d ./source/SD2_board.o ./source/cars.d ./source/cars.o ./source/key.d ./source/key.o ./source/main.d ./source/main.o ./source/mtb.d ./source/mtb.o ./source/semihost_hardfault.d ./source/semihost_hardfault.o

.PHONY: clean-source

