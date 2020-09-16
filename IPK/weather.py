#!/usr/bin/python3
import sys
import socket

#arguments from input(makefile)
api_key = sys.argv[1]
city = sys.argv[2:]
city_name = " ".join(city)

#connection variables
try:
	s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
except socket.error:
	sys.stderr.write("Could not create socket\n")
	exit()

server = "api.openweathermap.org"
port = 80 
get_request = "GET /data/2.5/weather?q=" + city_name +"&APPID=" + api_key + "&units=metric HTTP/1.0\r\nHost: api.openweathermap.org\r\n\r\n"


#connection and sending request
try:
	s.connect((server, port))
except socket.error:
	sys.stderr.write("Could not connect to the server\n")
	s.close()
	exit()

			
try:
	s.sendall(get_request.encode())
except socket.error:
	sys.stderr.write("Could not send the request\n")
	s.close()
	exit()

#buffer
try:
	result = s.recv(4096)
except socket.error:
	sys.stderr.write("Could not recieve data\n")
	s.close()
	exit()


if not result:
	sys.stderr.write("Did not recieve any data\n")
	s.close()
	exit()



#closing 
s.close()

#getting data
decoded_result = result.decode()


status_code = decoded_result[9:12]

#result status codes
if (status_code == "200"):
	#correct data
	data = decoded_result.split("\n\r\n")[1] #data = decoded_result[decoded_result.index("{"):]
	data = eval(data)


	#output
	print(data["name"])
	print(data["weather"][0]["description"])#["description"])
	print("temp: " + str(data["main"]["temp"]) + "C")
	print("humidity: " + str(data["main"]["humidity"]) + "%")
	print("pressure: " + str(data["main"]["pressure"]) + "hPa")
	if "deg" in data["wind"]:
		print("wind-speed: " + str(round((data["wind"]["speed"])* 3.6, 2)) + "km/h")
		print("wind-deg: " + str(data["wind"]["deg"]))
	else:
		print("wind-speed: 0km/h")
		print("wind-deg: -")	

elif(status_code == "204"):
	#no content
	sys.stderr.write("ERROR: " + status_code + " no content\n")
	exit()

elif(status_code == "400"):
	#bad request
	sys.stderr.write("ERROR: " + status_code + " server didnt understand the request\n")
	exit()

elif(status_code == "401"):
	#unauthorized
	sys.stderr.write("ERROR: " + status_code + " unauthorized, probably wrong API key\n")
	exit()

elif(status_code == "403"):
	#forbidden
	sys.stderr.write("ERROR: " + status_code + " forbidden\n")
	exit()

elif(status_code == "404"):
	#not found
	sys.stderr.write("ERROR: " + status_code + " not found\n")
	exit()	

elif(status_code == "408"):
	#timeout
	sys.stderr.write("ERROR: " + status_code + " timeout\n")
	exit()

elif(status_code == "409"):
	#conflict
	sys.stderr.write("ERROR: " + status_code + " conflict\n")
	exit()

elif(status_code == "500"):
	#server error
	sys.stderr.write("ERROR: " + status_code + " server error\n")
	exit()			

else:
	#other error
	sys.stderr.write("ERROR: " + status_code + "\n")
	exit()









