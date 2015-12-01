# getCpuRate_linux
get cpu use rate on linux
by JkLRz 12/01/15

Instructions
Server: listen to clients and collect their CPU userate.
	argv[1]: listen port

Client: send self's CPU rate to server.
	# first keyin a number for indexing that client, then it will start sending CPU userate.
	argv[1]: connect port
	argv[2]: server IP 
	argv[3]: client index
