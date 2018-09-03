# Glove_Controlled_Car-GCC
An Arduino based project which used the following components -
1. Arduino Nano micro controller
2. nrf24L01 radio module
3. mpu6050 gyro module
4. l293d motor driver

There are two circuits in the project.

	GLOVE (Sender) -
		Arduino Nano
		MPU-6050
		NRF24l01
	CAR (Receiver) -
		Arduino Nano
		L293d
		NRF24l01

Both the codes are available in the Repository. Just download the code and upload it into the arduino.
The GLOVE sends the commands to CAR based on the gyro readings. 
You just have to tilt your hand in the direction of movement and the car starts moving in that direction.
