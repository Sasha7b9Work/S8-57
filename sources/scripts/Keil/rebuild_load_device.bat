@echo off

call clean_device.bat

call build_device.bat

call load_device.bat

@echo %TIME%   Complete.