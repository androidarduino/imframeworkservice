This is the qml plugin for im framework.

It consists of two parts, the daemon server and the frontend plugin.

The daemon is a class named 'IMFDaemon'. The qml container creates an instance of IMFDaemon at startup, and let it login to available plugins. The daemon will do the login in a separate thread, and send a signal back to indicate it is ready to send and receive messages.

The frontend plugin is created when there is a qml page containing the 'IMF' plugin is launched. The plugin will talk to the IMFDaemon on behalf of the users through connected signal and slots.

