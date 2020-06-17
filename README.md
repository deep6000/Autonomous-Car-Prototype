# Autonomous-Car-Prototype

# Aim -
To design an Autonomous car prototype which could sense the environment variables like the road lanes, vehicles, road signs and traffic signals.

# Introduction-
-	An Autonomous car is defined as a robotic vehicle, which without any human interaction could fulfil all transportation capabilities.

‐	The autonomous car has a camera installed in the front side of the car. The camera is installed in such a way that it captures the lanes, vehicles, pedestrians, traffic signals, and road signs.

‐	This camera can be used to capture frames, and environmental variables like the lane, vehicles and traffic signs can be detected.

‐	A human driver senses all these parameters and then take a decision accordingly on what could be the next step.

‐	This is what an autonomous car does. It senses the variable, analyse it’s value and then take decision accordingly.

‐	An autonomous car consists of various sensors integrated with camera to give cameras inputs it cannot calculate or takes a long time to calculate. 

‐	Sensors like objection detector to detect objects very close or lightness sensor to give light values could be very important parameters for camera calibration.

‐	The prototype takes an input video as an argument and starts processing it. 

‐	All the processed frames are written into an output video.

‐	The autonomous car consists of the 4 modules, namely, Lane detection module, vehicle detection module, traffic sign detection module, and pedestrian detection module.

‐	All these modules are integrated together on a Jetson Nano using multi-threading and scheduling to achieve desirable frame rate.

‐	The lane detection module detects the lane, vehicle detection module detects the vehicle, traffic signal detection module detects the signal and pedestrian module detects the pedestrians

‐	This prototype is limited to detection only and does not involve processing of that data.

‐	Also the prototype limits itself with one jetson nano. With resources being limited the frame per second cannot be achieved to exactly 30 FPS but is achieved as close as possible.

‐	Adding more modules will definitely decrease the processing speed thus affecting the real time capabilities of the system. 


 
