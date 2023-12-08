################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/LCD/images.c \
../Drivers/LCD/lcd.c 

C_DEPS += \
./Drivers/LCD/images.d \
./Drivers/LCD/lcd.d 

OBJS += \
./Drivers/LCD/images.o \
./Drivers/LCD/lcd.o 


# Each subdirectory must supply rules for building sources it contributes
Drivers/LCD/%.o Drivers/LCD/%.su Drivers/LCD/%.cyclo: ../Drivers/LCD/%.c Drivers/LCD/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L053xx -c -I"C:/Users/Wjpaw/STM32CubeIDE/embedded_workspace/CommProject/Drivers/LCD" -I../Core/Inc -I"C:/Users/Wjpaw/STM32CubeIDE/embedded_workspace/CommProject/Drivers/UGUI" -I../Drivers/STM32L0xx_HAL_Driver/Inc -I../Drivers/STM32L0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L0xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Drivers-2f-LCD

clean-Drivers-2f-LCD:
	-$(RM) ./Drivers/LCD/images.cyclo ./Drivers/LCD/images.d ./Drivers/LCD/images.o ./Drivers/LCD/images.su ./Drivers/LCD/lcd.cyclo ./Drivers/LCD/lcd.d ./Drivers/LCD/lcd.o ./Drivers/LCD/lcd.su

.PHONY: clean-Drivers-2f-LCD

