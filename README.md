# Autonomous-Car-Prototype

# Aim
Design a self-driving car which operates safely and autonomously without requiring human intervention.

# Overview
A robotic vehicle designed to fulfill the transportation capabilities without any human intervention is called an Autonomous vehicle. To qualify it to be fully autonomous, it should be able to navigate to the desired destination without human inputs, thus capable of sensing the surrounding environment and making decisions based on sensor values. The surrounding
environment consists of the other vehicles on the road, pedestrians walking, road signs, signals, zebra crossings, obstacles, weather etc. Various sensors could be used to get feedback on the sensing and decisions could be made accordingly.

# Relevance of selection
When it comes to Autonomous vehicles all human factors which lead to causing an accident could be prevented. Factors like drinking and driving, reckless driving, driving without a license,driving using their phones or being sick, all could be avoided very easily. Autonomous cars come trained about the driving, they have extremely great driving skills, fuel management capability, speed control and better connectivity when in emergency.The study says that in 89 percent of all accident’s humans are responsible for the accident and only 11 percent of all  accidents have been because of the undesirable weather conditions. OpenCV is a great solution for self-driving cars. OpenCV can be used for implementation of various modules like lane detection using Hough transform, vehicle detection using HAAR cascade, pedestrian detection using HOG descriptors, road sign detection using feature extractors and deep neural networks etc. Interfacing these modules and building a prototype of self-driving car could be achieved with the help of OpenCV.

# Challenges
The most important and difficult challenge is to integrate all the modules and to run parallel so that the response and decision making is real-time. Lane detection would work fine on roads with lanes, but problem would arrive for roads with no lanes. This would cause ambiguity and would require proper handling. The identification of road signs is a big challenge as it requires learning algorithm which is passed through a huge dataset multiple times, so that it could make the decisions real-time and
provide feedback to the other modules. In identification for traffic signal, it is really important to extract the right region of interest and apply the algorithm on just ROI. Selecting ROI is difficult as sizes, orientation and height of traffic lights changes every frame. Integrations with google maps API provides a bigger challenge.


