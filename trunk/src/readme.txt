This project generates two packages:
1. A instant messaging service daemon, running at the background, launched by the first call of the dll.
2. A set of dll, headers for programmers to make use of the service
3. The dll is a part of the client program, it connects to the service daemon through local socket to get notifications and send requests.
