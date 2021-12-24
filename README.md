# CppGenericWatchdog

* C++ Impelementation to monitor different critical threads of a process.
* If the desired threads dont respond within the configured timeout, it is considered as a missed heartbeat.
* After configured numbers of heart beat misses, it is concluded that the thread is in a bad state and necessary action should be taken. This is informed via configured callback function.
