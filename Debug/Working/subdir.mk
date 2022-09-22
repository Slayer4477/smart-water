################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Working/working.c 

OBJS += \
./Working/working.o 

C_DEPS += \
./Working/working.d 


# Each subdirectory must supply rules for building sources it contributes
Working/%.o: ../Working/%.c Working/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L151xC -c -I../Core/Inc -I../Drivers/STM32L1xx_HAL_Driver/Inc -I../Drivers/STM32L1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L1xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Slayer/Music/New folder/Version1/Drivers/Bluetooth" -I"C:/Users/Slayer/Music/New folder/Version1/Drivers/Clock" -I"C:/Users/Slayer/Music/New folder/Version1/Drivers/Files" -I"C:/Users/Slayer/Music/New folder/Version1/Drivers/FS" -I"C:/Users/Slayer/Music/New folder/Version1/Drivers/FTP" -I"C:/Users/Slayer/Music/New folder/Version1/Drivers/Gps" -I"C:/Users/Slayer/Music/New folder/Version1/Drivers/Memory" -I"C:/Users/Slayer/Music/New folder/Version1/Drivers/MQTT" -I"C:/Users/Slayer/Music/New folder/Version1/Drivers/Sensor" -I"C:/Users/Slayer/Music/New folder/Version1/Drivers/Sim868" -I"C:/Users/Slayer/Music/New folder/Version1/Drivers/Sleep" -I"C:/Users/Slayer/Music/New folder/Version1/Debug/Drivers/Uart" -I"C:/Users/Slayer/Music/New folder/Version1/Drivers/Uart" -I"C:/Users/Slayer/Music/New folder/Version1/Drivers/GSMLOC" -I"C:/Users/Slayer/Music/New folder/Version1/Working" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Working

clean-Working:
	-$(RM) ./Working/working.d ./Working/working.o

.PHONY: clean-Working

