# httpsys
A simple multi-threaded http(s) web server developed using http.sys. It uses c++ std::thread for multi-threading, and windows http.sys to setup a web server hook to listen for incoming http(s) calls. To enable https a certificate must be bound to the ip:port using

### netsh http add sslcert ipport=0.0.0.0:8080 certhash=<YOUR CERT THUMBPRINT> appid={00112233-4455-6677-8899-AABBCCDDEEFF}

