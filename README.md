# Autonomous-Car-Prototype

# Aim -
To design an Autonomous car prototype which could sense the environment variables like the road lanes, vehicles, road signs and traffic signals.

# Functional Requirements
## Lane Detection
Lane Detection forms an integral part of autonomous car as it plays an important role in preventing accidents. The system should detect the white and yellow lanes on the road real-time and mark it on the frame.
The coordinates can then be used to compare the location of the vehicle with the location of the lane. If the vehicle intersects the lane we could warn the system about it.

## Vehicle Detection
The vehicles are the moving objects on the road and most prone to accidents. The system should be able to detect the vehicles in its region of interest and should be able to denote it on the frame by drawing a rectangle.
These locations of the vehicle could be further used to make decisions of keeping considerable distance between those cars.

## Traffic Signal Detection
Traffic Signs provide assistance to driving by giving a feedback of when it is safe to go ahead. The system should be able to detect the traffic signs in the frame which are facing the vehicle.

## Real-Time Processing
Autonomous Cars are critical systems and they should produce results real- time.
The system should be able to run all modules in parallel and detect all the characteristics required and to plot it on the frame. As this system takes an input video, should be able to achieve fps close to the input video.  

## Output Video
The system takes input video as an argument as doesn’t have a live camera. The system should be able to write a video with all the sensing parameters on the frame

## Machine Vision and Machine Learning  Algorithm
The system should be able to use the machine vision and machine learning  algorithm to detect objects so that it’s more accurate and real-time
