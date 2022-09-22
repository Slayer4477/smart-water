################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/Uart/uart.c 

OBJS += \
./Drivers/Uart/uart.o 

C_DEPS += \
./Drivers/Uart/uart.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/Uart/%.o: ../Drivers/Uart/%.c Drivers/Uart/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32L151xC -c -I../Core/Inc -I../Drivers/STM32L1xx_HAL_Driver/Inc -I../Drivers/STM32L1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L1xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/pcbde/OneDrive/Desktop/SW1.10_Workspace/Version1/Drivers/Bluetooth" -I"C:/Users/pcbde/OneDrive/Desktop/SW1.10_Workspace/Version1/Drivers/Clock" -I"C:/Users/pcbde/OneDrive/Desktop/SW1.10_Workspace/Version1/Drivers/Files" -I"C:/Users/pcbde/OneDrive/Desktop/SW1.10_Workspace/Version1/Drivers/FS" -I"C:/Users/pcbde/OneDrive/Desktop/SW1.10_Workspace/Version1/Drivers/FTP" -I"C:/Users/pcbde/OneDrive/Desktop/SW1.10_Workspace/Version1/Drivers/Gps" -I"C:/Users/pcbde/OneDrive/Desktop/SW1.10_Workspace/Version1/Drivers/Memory" -I"C:/Users/pcbde/OneDrive/Desktop/SW1.10_Workspace/Version1/Drivers/MQTT" -I"C:/Users/pcbde/OneDrive/Desktop/SW1.10_Workspace/Version1/Drivers/Sensor" -I"C:/Users/pcbde/OneDrive/Desktop/SW1.10_Workspace/Version1/Drivers/Sim868" -I"C:/Users/pcbde/OneDrive/Desktop/SW1.10_Workspace/Version1/Drivers/Sleep" -I"C:/Users/pcbde/OneDrive/Desktop/SW1.10_Workspace/Version1/Debug/Drivers/Uart" -I"C:/Users/pcbde/OneDrive/Desktop/SW1.10_Workspace/Version1/Drivers/Uart" -I"C:/Users/pcbde/OneDrive/Desktop/SW1.10_Workspace/Version1/Drivers/GSMLOC" -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Drivers-2f-Uart

clean-Drivers-2f-Uart:
	-$(RM) ./Drivers/Uart/uart.d ./Drivers/Uart/uart.o

.PHONY: clean-Drivers-2f-Uart

