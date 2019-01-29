echo off
@echo %TIME%   Rebuild Device

call clean_device.bat

call build_device.bat
