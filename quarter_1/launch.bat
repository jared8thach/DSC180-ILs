@echo off
setlocal enableextensions
setlocal enabledelayedexpansion
%WINDIR%\System32\chcp.com 437 > NUL 2>&1

rem //-------------------------------------------------------------------------
rem // Check if script is ran as Administrator (required).
rem //-------------------------------------------------------------------------
%WINDIR%\System32\net.exe session 1>NUL 2>NUL || (ECHO This script requires elevated rights. & EXIT /b 1)

rem //-------------------------------------------------------------------------
rem // Changing directory to default C:\_xlsdk\run\windows directory.
rem //-------------------------------------------------------------------------
cd C:\_xlsdk\run\windows

rem //-------------------------------------------------------------------------
rem // Configurations.
rem //-------------------------------------------------------------------------

rem //-------------------------------------------------------------------------
rem // Debug & execution settings.
rem //-------------------------------------------------------------------------
set DEBUG=YES
set EXECUTE=YES
set /A COMMANDS_COUNT=0
set TIME_OUT_IN_S=1

rem //-------------------------------------------------------------------------
rem // Folders settings.
rem //-------------------------------------------------------------------------
set ROOT_FOLDER=%cd%
set BIN_FOLDER=%ROOT_FOLDER%\Release\64
set OUTPUTS_FOLDER=%ROOT_FOLDER%\outputs

rem //-------------------------------------------------------------------------
rem // Key settings.
rem //-------------------------------------------------------------------------
set PAUSE_TIME_IN_MS=1000
set RESUME_DELAY_IN_MS=30000
set DELAYED_DCTL_IN_MS=90000
set TIME_DATA=no
set KEY_DATA=no
set PERFORMANCE_DATA=no
set IN_CYCLE_PERFORMANCE_DATA=no
set ADDRESS_IPV4=127.0.0.1
set PORT=49260
set XLOADER_LOCK=yes
set OUTPUT_FILE_NAME=group3_collected_data

rem //-------------------------------------------------------------------------
rem // Common logger library's settings.
rem //-------------------------------------------------------------------------
set LOGGER_MODELER_OPTIONS=deferred_logger_stop=yes
set LOGGER_LIBRARY_001=ll='%BIN_FOLDER%\sql_logger.dll','db_differential_elaspsed_time=yes db_wal=yes db_wal_autocheckpoint=0 db_cache=yes db_cache_size=auto db_synchronous=off db_journal_mode=off db_locking_mode=exclusive delayed_dctl=summarize dctl_process_delay=%DELAYED_DCTL_IN_MS% do_not_signal_delayed_dctl=yes'

rem //-------------------------------------------------------------------------
rem // Payloads' settings.
rem //-------------------------------------------------------------------------
set INPUT_LIBRARY_001=il='%BIN_FOLDER%\mouse_input.dll'
set INPUT_LIBRARY_002=il='%BIN_FOLDER%\user_waiting.dll'
set INPUT_LIBRARY_003=il='%BIN_FOLDER%\foreground_window.dll'

rem // set INPUT_LIBRARY_004=il='%BIN_FOLDER%\desktop_mapper.dll'
rem // set INPUT_LIBRARY_005=il='%BIN_FOLDER%\intel_process_watcher_input.dll'

rem  @@@@   @@@@@   @@   @@@@@   @@@@@
rem @    @    @    @  @  @    @    @
rem @         @   @    @ @    @    @
rem  @@@@     @   @    @ @@@@@     @
rem      @    @   @@@@@@ @  @      @
rem      @    @   @    @ @   @     @
rem @@@@@     @   @    @ @    @    @
rem //-------------------------------------------------------------------------
rem // Start the collector.
rem //-------------------------------------------------------------------------
:START

set COMMAND="%BIN_FOLDER%\esrv.exe"^
 --start^
 --output_folder "%OUTPUTS_FOLDER%"^
 --time_in_ms^
 --pause %PAUSE_TIME_IN_MS%^
 --no_pl^
 --end_on_error^
 --priority_boost^
 --kernel_priority_boost^
 --shutdown_priority_boost^
 --do_not_use_system_error_logs^
 --do_not_generate_dump_files^
 --resume_delay %RESUME_DELAY_IN_MS%^
 --library "%BIN_FOLDER%\intel_modeler.dll"^
 --device_options ^"^
 time=%TIME_DATA%^
 generate_key_file=%KEY_DATA%^
 performance=%PERFORMANCE_DATA%^
 in_cycle_performance=%IN_CYCLE_PERFORMANCE_DATA%^
 output=%OUTPUT_FILE_NAME%^
 output_folder='%OUTPUTS_FOLDER%'^
 lock_xls=%XLOADER_LOCK%^
 deferred_logger_stop=yes^
 do_not_signal_flush=yes^
 %LOGGER_LIBRARY_001%^
 %INPUT_LIBRARY_001%^
 %INPUT_LIBRARY_002%^
 %INPUT_LIBRARY_003%^
 ^"^
 --address %ADDRESS_IPV4%^
 --port %PORT%
set /A COMMANDS_COUNT=COMMANDS_COUNT+1
echo COMMAND[%COMMANDS_COUNT%][%DATE% - %TIME%]=[%COMMAND%]
if %DEBUG% == YES echo COMMAND[%COMMANDS_COUNT%]=[%COMMAND%]
if %EXECUTE% == YES (
	%COMMAND%
)

goto END

rem  @@@@   @@@@@  @@@@  @@@@@
rem @    @    @   @    @ @    @
rem @         @   @    @ @    @
rem  @@@@     @   @    @ @@@@@
rem      @    @   @    @ @
rem      @    @   @    @ @
rem @@@@@     @    @@@@  @
rem //-------------------------------------------------------------------------
rem // Stop the collector.
rem //-------------------------------------------------------------------------
:STOP