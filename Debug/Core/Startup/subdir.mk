################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../Core/Startup/startup_stm32l151rctx.s 

OBJS += \
./Core/Startup/startup_stm32l151rctx.o 

S_DEPS += \
./Core/Startup/startup_stm32l151rctx.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Startup/%.o: ../Core/Startup/%.s Core/Startup/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m3 -g3 -DDEBUG -c -I"C:/Users/Slayer/Music/New folder/Version1/Drivers/Bluetooth" -I"C:/Users/Slayer/Music/New folder/Version1/Drivers/Clock" -I"C:/Users/Slayer/Music/New folder/Version1/Drivers/Files" -I"C:/Users/Slayer/Music/New folder/Version1/Drivers/FS" -I"C:/Users/Slayer/Music/New folder/Version1/Drivers/FTP" -I"C:/Users/Slayer/Music/New folder/Version1/Drivers/Gps" -I"C:/Users/Slayer/Music/New folder/Version1/Drivers/Memory" -I"C:/Users/Slayer/Music/New folder/Version1/Drivers/MQTT" -I"C:/Users/Slayer/Music/New folder/Version1/Drivers/Sensor" -I"C:/Users/Slayer/Music/New folder/Version1/Drivers/Sim868" -I"C:/Users/Slayer/Music/New folder/Version1/Drivers/Sleep" -I"C:/Users/Slayer/Music/New folder/Version1/Debug/Drivers/Uart" -I"C:/Users/Slayer/Music/New folder/Version1/Drivers/Uart" -I"C:/Users/Slayer/Music/New folder/Version1/Drivers/GSMLOC" -I"C:/Users/Slayer/Music/New folder/Version1/Working" -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@" "$<"

clean: clean-Core-2f-Startup

clean-Core-2f-Startup:
	-$(RM) ./Core/Startup/startup_stm32l151rctx.d ./Core/Startup/startup_stm32l151rctx.o

.PHONY: clean-Core-2f-Startup

