# Client/server remote shell
A command line shell that can parse and execute a limited set of commands on the server which are sent by the client. Uses socket programming, multiplexed I/O, and multithreading. Written In C. 

# Running the server
Compile the server file, streamread.c using the following command
```
gcc streamread.c -o "server"
```
Now you can start the server using this command:
```
./server
```

# Running the client
Compile the client file, streamwrite.c using the following command
```
gcc streamwrite.c -o "client"
```
Now you can connect with the server using this command:
```
./client <SERVER_IP>
```
Replace <SERVER_IP> with the IP of the server
