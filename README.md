# Synopticon

# Enabling Motive Plugin
- Copy the "OptiTrack" folder from the "MotivePugin" folder into the Unreal Engine plugins folder, normally located at 
"C:\Program Files\Epic Games\5.#\Engine\Plugins". Enable it in the project settings inside Unreal Editor if it is not already enabled.

# How to work with Opentrack
- Input: neuralnet tracker
- Output: UDP over network
- Use the provided config file from the "OpenTrackConfigFile" folder
    * This matches the game data to the raw tracker data and converts to the unreal enigne coordinate system. (The center on stratup option must be unchecked, and the mapping 1 to 1 for X,Y,Z and Roll,Pitch,Yaw). 
