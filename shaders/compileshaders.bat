echo Compilando shaders

C:/VulkanSDK/1.4.341.1/Bin/slangc.exe shaderbase.slang -target spirv -profile spirv_1_4 -emit-spirv-directly -fvk-use-entrypoint-name -entry vertMain -entry fragMain -entry outlineVertMain -entry outlineFragMain -o generated/slang.spv
@echo off
echo Shaders Compilados
pause