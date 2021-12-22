# CppGenericWatchdog

* C++ Impelementation to monitor different critical threads of a process.
* If the desired threads dont respond within the configured timeout don't repsond, it is considered as a missed heartbeat.
* After configured numbers of heart beat, it is concluded that the process is in a bad state and should be restarted.
