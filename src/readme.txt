This project generates two packages:

1. An instant messaging service daemon (actually an application too), running at the background, launched by the first call of the dll
2. The protocols are implemented as service plugins, to be launched by the daemon mentioned above
3. A set of dll, headers for programmers to make use of the service, fully binary compatible among versions
4. The dll is a part of the client program, it connects to the service daemon through local socket to get notifications and send requests


A message include several layers, and carry the following meta information:

1. Between the two bottom level protocols: appId, from, threadId, timeStamp, ver...
2. Between the protocol and the server: appId, from, to, threadId, timeStamp, ver... (server add "to" by identifying protocol information)
3. Between the server and the client: from, to, threadId, timeStamp, ver... (server dispatch message according to "appId")


User can be identified by an universal ID in the following format:

	server:port/somebody@host

	Examples: 
			www.msn.com:0/somebody@hotmail.com			irc.freenode.org:6667/somebody@freenode.com		gtalk.google.com:4242/somebody@gmail.com
			www.ftp.com:21/somebody@gmail.com			www.http.com:80/somebody@abc.com				www.ssh.com:22/whatever@ssh.com
			pop_smtp.somemail.com:110/mail@abc.com		docs.google.com:0/abc@gmail.com					evernote.com:0/abc@evernote.com	

End user usage:

    imf["abc@abc.com"]<<"test message";//test message will be send to corresponding app on the other side

    IMContact& liu=imf["abc@abc.com"];
    liu<<"<msg app=testapp1>test message</msg>";

    IMContact& facebook=imf["facebook"];//get my facebook account if available
    Msg msg("<msg type=picture album=abc1 location=home ></msg>");
    msg<<QPicture("picture1.jpg");
    facebook<<msg;

    imf["weather"]<<"what is the weather for Beijing tomorrow?";
